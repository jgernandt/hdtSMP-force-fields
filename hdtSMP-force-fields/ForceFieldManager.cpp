#include "pch.h"

#include "Config.h"
#include "ForceFieldManager.h"
#include "relocation.h"
#include "Timer.h"

#include "Bomb.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Sphere.h"
#include "Vortex.h"

#include "NiStringsExtraData.h"

#define LOG_TIMER

jg::ForceFieldManager::ForceFieldManager() :
	m_workerPool(g_config.geti(Config::THREADS))
{
	m_dataName = "HDT-SMP Force Field";

	m_factories[Skyrim::BSFixedString("CYLINDRICAL")] = std::make_unique<CylinderFactory>();
	m_factories[Skyrim::BSFixedString("PLANAR_BOX")] = std::make_unique<BoxFactory>();
	m_factories[Skyrim::BSFixedString("PLANAR_CYL")] = std::make_unique<RodFactory>();
	m_factories[Skyrim::BSFixedString("PLANAR_SPH")] = std::make_unique<BallFactory>();
	m_factories[Skyrim::BSFixedString("SPHERICAL")] = std::make_unique<SphereFactory>();
	m_factories[Skyrim::BSFixedString("VORTEX")] = std::make_unique<VortexFactory>();

	//Bomb is unfinished and janky, leave it out for now
	//m_factories[Skyrim::BSFixedString("BOMB")] = std::make_unique<BombFactory>();

	_VMESSAGE("Field types:");
	gLog.Indent();
	for (auto&& item : m_factories) {
		_VMESSAGE("%s", item.first.c_str());
	}
	gLog.Outdent();
}

void jg::ForceFieldManager::onEvent(const hdt::PreStepEvent& e)
{
#ifdef LOG_TIMER
	static std::size_t fields = 0;
	static int objects = 0;
	static int steps = 0;
	static long long time = 0;
	static int multiCount = 0;

	Timer<long long, std::micro> timer;
#endif

	static float lastSingle = 1.0f;
	static float lastMulti = 1.0f;

	{
		//Update and make a working copy of the active fields, then release lock immediately
		std::shared_lock<decltype(m_mutex)> lock(m_mutex);

		for (auto&& field : m_fields) {
			if (field->update(e.timeStep)) {
				m_active.push_back(field);
			}
		}
	}

	Timer<float, std::micro> newTimer;
	
	if (!m_active.empty() && e.objects.size() != 0) {
		//We dynamically select the fastest threading policy, based on previous performance.
		if (m_workerPool && lastMulti && lastSingle && randf() < lastSingle / (lastMulti + lastSingle)) {
			m_workerPool.release(&m_active, &e.objects);
			m_workerPool.wait();

			//Some arbitrary power to push towards the faster method
			lastMulti = std::pow(newTimer.elapsed(), 3.0f);

			//If any one frame takes extremely long, we may become permanently locked out of that method.
			//Decaying the time for the unused method guards against this.
			//This factor should force reevaluation within seconds.
			lastSingle *= 0.99f;

#ifdef LOG_TIMER
			multiCount++;
#endif
		}
		else {
			for (int i = 0; i < e.objects.size(); i++) {
				btRigidBody* body = btRigidBody::upcast(e.objects[i]);
				if (body && !body->isStaticOrKinematicObject()) {
					for (auto&& field : m_active) {
						field->actOn(*body);
					}
				}
			}
			lastSingle = std::pow(newTimer.elapsed(), 3.0f);

			lastMulti *= 0.99f;
		}
	}
	
#ifdef LOG_TIMER
	if (g_config.getb(Config::LOG_STATS)) {
		fields += m_active.size();
		objects += e.objects.size();
		time += timer.elapsed();
		steps++;

		if (steps == 120) {
			_MESSAGE("Active fields: %d\tCollision objects: %d\tFraction multithreaded: %.2f\tUpdate time: %d microseconds", 
				fields / 120, objects / 120, (float)multiCount / 120, time / 120);
			fields = 0;
			objects = 0;
			steps = 0;
			time = 0;
			multiCount = 0;
		}
	}
#endif

	m_active.clear();
}

void jg::ForceFieldManager::onDetach(Skyrim::NiAVObject* model, void* owner)
{
	//assert(model);
	//_DMESSAGE("Detaching %s", model->m_name ? model->m_name : "");

	if (auto entry = findEntry(owner); entry != m_fieldMap.end()) {
		std::lock_guard<decltype(m_mutex)> lock(m_mutex);
		if (entry->second) {
			auto field = std::find_if(m_fields.begin(), m_fields.end(), 
				[=](const std::shared_ptr<IForceField>& ptr) { return ptr.get() == entry->second; });
			//We don't need this check unless our data has been corrupted. Should be impossible right now.
			assert(field != m_fields.end());
			m_fields.erase(field);
		}
		m_fieldMap.erase(entry);
	}
}

jg::ForceFieldManager::ObjectMap::const_iterator jg::ForceFieldManager::findEntry(void* object) const
{
	if (object) {
		std::shared_lock<decltype(m_mutex)> lock(m_mutex);
		return m_fieldMap.find(object);
	}
	return m_fieldMap.end();
}

jg::IForceFieldFactory* jg::ForceFieldManager::getFactory(Skyrim::NiStringsExtraData* data) const
{
	assert(data);

	IForceFieldFactory* result = nullptr;

	if (!data->empty()) {
		if (auto it = m_factories.find(Skyrim::BSFixedString(*data->begin())); it != m_factories.end()) {
			result = it->second.get();
			//_DMESSAGE("Field type: %s (%p)", data->m_data[0], data->m_data[0]);
		}
		else {
			//_DMESSAGE("Unknown field type");
		}
	}
	else {
		//_DMESSAGE("Field data is missing a type string");
	}

	return result;
}

Skyrim::NiStringsExtraData* jg::ForceFieldManager::getFieldData(Skyrim::NiAVObject* model) const
{
	assert(model);

	if (auto data = model->findExtraData(m_dataName)) {
		if (is_ni_type<Skyrim::NiStringsExtraData>(*data)) {
			return static_cast<Skyrim::NiStringsExtraData*>(data);
		}
	}

	return nullptr;
}

jg::ParamMap jg::ForceFieldManager::getFieldParameters(Skyrim::NiStringsExtraData* data) const
{
	assert(data && !data->empty());

	ParamMap result;

	auto it = data->begin();
	for (++it; it != data->end(); ++it) {
		if (*it) {
			result.insert(parseParamStr(*it));
		}
	}
	return result;
}

bool jg::ForceFieldManager::hasEntry(void* object) const
{
	if (object) {
		std::shared_lock<decltype(m_mutex)> lock(m_mutex);
		return m_fieldMap.contains(object);
	}
	return false;
}

void jg::ForceFieldManager::insertField(void* owner, std::unique_ptr<IForceField>&& field)
{
	if (field && owner) {
		std::lock_guard<decltype(m_mutex)> lock(m_mutex);

		m_fieldMap.insert({ owner, field.get() });
		m_fields.push_back(std::move(field));
	}
}

jg::ParamMap::value_type jg::ForceFieldManager::parseParamStr(const char* str) const
{
	assert(str);

	//split each string into key:value pairs at an '=', if found
	if (auto eq = std::strchr(str, '='); eq && eq != str) {
		char buf[64];
		if (uintptr_t diff = eq - str; diff < sizeof(buf)) {
			if (!strncpy_s(buf, str, diff)) {
				buf[diff] = '\0';

				ParamMap::value_type result{ buf, ParamVal() };

				switch (buf[0]) {
				case 'b':
					result.second.b = static_cast<bool>(std::atoi(eq + 1));
					//_DMESSAGE("%s: %s", result.first.c_str(), result.second.b ? "true" : "false");
					return result;
				case 'f':
					result.second.f = static_cast<float>(std::atof(eq + 1));
					//_DMESSAGE("%s: %f", result.first.c_str(), result.second.f);
					return result;
				case 'i':
					result.second.i = std::atoi(eq + 1);
					//_DMESSAGE("%s: %d", result.first.c_str(), result.second.i);
					return result;
				case 'q':
				{
					const char* next = eq + 1;
					char* end;
					for (int i = 0; i < 4; i++) {
						float f = std::strtof(next, &end);
						if (end != next) {
							result.second.v[i] = f;
							next = end;
						}
						else {
							return { Skyrim::BSFixedString(), ParamVal() };
						}
					}
					return result;
				}
				case 'v':
				{
					const char* next = eq + 1;
					char* end;
					for (int i = 0; i < 3; i++) {
						float f = std::strtof(next, &end);
						if (end != next) {
							result.second.v[i] = f;
							next = end;
						}
						else {
							//_DMESSAGE("%s: invalid vector", result.first.c_str());
							return { Skyrim::BSFixedString(), ParamVal() };
						}
					}
					//_DMESSAGE("%s: (%f, %f, %f)", result.first.c_str(), 
					//	result.second.v[0], result.second.v[1], result.second.v[2]);
					return result;
				}
				default:
					//_DMESSAGE("%s: NULL", result.first.c_str());
					;
				}
			}
		}
	}
	return { Skyrim::BSFixedString(), ParamVal()};
}

void jg::ForceFieldManager::WorkerPool::barrierComplete() noexcept
{
	try {
		//Really stupid way of accessing the semaphore, but what to do?
		//The completion function must be nothrow_invocable with no arguments.
		//This rules out non-static member functions and std::function.
		//It must also have an explicit type, which rules out lambda and bind expressions.
		//We are left with static member functions and free functions.
		ForceFieldManager::get().m_workerPool.m_stopSignal.release();
	}
	//counting_semaphore::release can throw std::system_error. 
	//ForceFieldManager::get could hypothetically throw from the ctor of ForceFieldManager,
	//but this should not be the first call to it.
	catch (const std::exception& e) {
		_ERROR("ForceFieldManager::WorkerPool::barrierComplete error: %s", e.what());
	}
}

jg::ForceFieldManager::WorkerPool::WorkerPool(int n) :
	m_workers(std::min(n > 1 ? n : 0, 128)),
	m_barrier(std::min(n > 1 ? n : 0, 128), &barrierComplete)
{
	for (auto&& worker : m_workers) {
		worker = std::thread(&WorkerPool::work, this);
	}
	_VMESSAGE("%d worker threads started.", m_workers.size());
}

jg::ForceFieldManager::WorkerPool::~WorkerPool()
{
	release(nullptr, nullptr);
	for (auto&& worker : m_workers) {
		worker.join();
	}
}

void jg::ForceFieldManager::WorkerPool::wait()
{
	m_stopSignal.acquire();
	m_bodies = nullptr;
	m_fields = nullptr;
}

void jg::ForceFieldManager::WorkerPool::release(const Fields* fields, const Bodies* bodies)
{
	if (bodies && fields) {
		m_bodies = bodies;
		m_fields = fields;
		m_jobCounter.store(bodies->size(), std::memory_order::relaxed);
		m_startSignal.release(m_workers.size());
	}
}

void jg::ForceFieldManager::WorkerPool::work()
{
	while (true) {
		m_startSignal.acquire();

		if (!m_bodies || !m_fields) {
			break;
		}

		try {
			int i = m_jobCounter.fetch_sub(1, std::memory_order_relaxed) - 1;
			while (i >= 0) {
				btRigidBody* body = btRigidBody::upcast(m_bodies->at(i));
				if (body && !body->isStaticOrKinematicObject()) {
					for (auto&& field : *m_fields) {
						field->actOn(*body);
					}
				}
				i = m_jobCounter.fetch_sub(1, std::memory_order_relaxed) - 1;
			}
		}
		catch (const std::exception& e) {
			_ERROR("ForceFieldManager::WorkerPool::work error: %s", e.what());
		}

		m_barrier.arrive_and_wait();
	}
}

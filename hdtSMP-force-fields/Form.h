#pragma once
#include "bs_types.h"
#include "get_member.h"

namespace Skyrim
{
	class Form
	{
	public:
		constexpr static FormType FORM_TYPE = FormType::NONE;

	public:
		Form();
		virtual ~Form();

		virtual void BaseFormComponent_01();
		virtual void BaseFormComponent_02();
		virtual void BaseFormComponent_03();
		virtual void Form_04();
		virtual void Form_05();
		virtual void Form_06();
		virtual void Form_07();
		virtual void Form_08();
		virtual void Form_09();
		virtual void Form_0a();
		virtual void Form_0b();
		virtual void Form_0c();
		virtual void Form_0d();
		virtual void Form_0e();
		virtual void Form_0f();
		virtual void Form_10();
		virtual void Form_11();
		virtual void Form_12();
		virtual void Form_13();
		virtual void Form_14();
		virtual FormType getFormType();
		virtual void Form_16();
		virtual void Form_17();
		virtual void Form_18();
		virtual void Form_19();
		virtual void Form_1a();
		virtual void Form_1b();
		virtual void Form_1c();
		virtual void Form_1d();
		virtual void Form_1e();
		virtual void Form_1f();
		virtual void Form_20();
		virtual void Form_21();
		virtual void Form_22();
		virtual void Form_23();
		virtual void Form_24();
		virtual void Form_25();
		virtual void Form_26();
		virtual void Form_27();
		virtual void Form_28();
		virtual void Form_29();
		virtual void Form_2a();
		virtual void Form_2b();
		virtual void Form_2c();
		virtual void Form_2d();
		virtual void Form_2e();
		virtual void Form_2f();
		virtual void Form_30();
		virtual void Form_31();
		virtual void Form_32();
		virtual void Form_33();
		virtual void Form_34();
		virtual void Form_35();
		virtual void Form_36();
		virtual void Form_37();
		virtual void Form_38();
		virtual void Form_39();
		virtual void Form_3a();

		FormID getFormID() const { return get_member<FormID>(this, offsetof_formID); }

	private:
		constexpr static std::size_t offsetof_formID = 0x10;

		char data[0x18];
	};
	static_assert(sizeof(Form) == 0x20);
}

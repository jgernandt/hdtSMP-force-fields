#include "pch.h"
#include "Projectile.h"

var_offset Skyrim::Projectile::offsetof_speedMult(sizeof_ObjectReference, 0xf4, "offsetof(Projectile, speedMult)");
var_offset Skyrim::Projectile::offsetof_age(sizeof_ObjectReference, 0xfc, "offsetof(Projectile, age)");
var_offset Skyrim::Projectile::offsetof_scale(sizeof_ObjectReference, 0x130, "offsetof(Projectile, scale)");
var_offset Skyrim::Projectile::sizeof_Projectile(sizeof_ObjectReference, 0x140, "sizeof(Projectile)");

var_offset Skyrim::ConeProjectile::offsetof_tanSpread(sizeof_Projectile, 8, "offsetof(ConeProjectile, tanSpread)");
var_offset Skyrim::ConeProjectile::offsetof_initialRadius(sizeof_Projectile, 0xc, "offsetof(ConeProjectile, initialRadius)");

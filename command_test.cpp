#include "cbase.h"
#include "props.h"
#include "datacache/imdlcache.h"
#include "tier0/memdbgon.h"

void Create_ExplosiveBarrel(void) {
	MDLCACHE_CRITICAL_SECTION();

	bool allowPrecache = CBaseEntity::IsPrecacheAllowed();
	CBaseEntity::SetAllowPrecache(true);

	CBaseEntity* entity = dynamic_cast<CBaseEntity*>(CreateEntityByName("prop_physics"));
	if (entity) {
		entity->PrecacheModel("models/props_c17/oildrum001_explosive.mdl");
		entity->SetModel("models/props_c17/oildrum001_explosive.mdl");
		entity->SetName(MAKE_STRING("barrel"));
		entity->AddSpawnFlags(SF_PHYSPROP_ENABLE_PICKUP_OUTPUT);
		entity->Precache();
		DispatchSpawn(entity);

		CBasePlayer* pPlayer = UTIL_GetCommandClient();
		trace_t tr;
		Vector forward;
		pPlayer->EyeVectors(&forward);
		UTIL_TraceLine(pPlayer->EyePosition(), pPlayer->EyePosition() + forward * MAX_TRACE_LENGTH, MASK_SOLID, pPlayer, COLLISION_GROUP_NONE, &tr);

		if (tr.fraction != 1.0) {
			tr.endpos.z += 12;
			entity->Teleport(&tr.endpos, NULL, NULL);
			UTIL_DropToFloor(entity, MASK_SOLID);
		}
	}
	CBaseEntity::SetAllowPrecache(allowPrecache);
}

static ConCommand ent_create_explosive_barrel("ent_create_explosive_barrel", Create_ExplosiveBarrel, "Creates an explosive barrel at the location the player is looking.", FCVAR_GAMEDLL | FCVAR_CHEAT);

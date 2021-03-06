// ======================= ZoneTool =======================
// zonetool, a fastfile linker for various
// Call of Duty titles. 
//
// Project: https://github.com/ZoneTool/zonetool
// Author: RektInator (https://github.com/RektInator)
// License: GNU GPL v3.0
// ========================================================
#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		IWeaponDef::IWeaponDef()
		{
		}

		IWeaponDef::~IWeaponDef()
		{
		}

#define WEAPON_READ_FIELD(__type__, __field__) \
	if (!data[#__field__].is_null()) weapon->__field__ = data[#__field__].get<__type__>()

#define WEAPON_READ_STRING(__field__) \
	if (!data[#__field__].is_null()) weapon->__field__ = mem->StrDup(data[#__field__].get<std::string>())

#define WEAPON_READ_ASSET(__type__, __datafield__, __field__) \
	if (!data[#__field__].is_null() && data[#__field__].is_string()) \
	{ \
		auto asset##__field__ = data[#__field__].get<std::string>(); \
		if (asset##__field__.empty()) \
		{ \
			weapon->__field__ = nullptr; \
		} \
		else \
		{ \
			weapon->__field__ = DB_FindXAssetHeader(XAssetType::__type__, asset##__field__.data(), 1).__datafield__; \
		} \
	}

		WeaponDef* IWeaponDef::parse_weapondef(Json& data, WeaponCompleteDef* baseAsset,
		                                       std::shared_ptr<ZoneMemory>& mem)
		{
			auto weapon = mem->Alloc<WeaponDef>();

			if (baseAsset)
			{
				memcpy(weapon, baseAsset->WeaponDef, sizeof WeaponDef);
			}

			WEAPON_READ_STRING(szInternalName);

			weapon->gunXModel = mem->Alloc<XModel*>(16);
			weapon->worldModel = mem->Alloc<XModel*>(16);

			for (int i = 0; i < 16; i++)
			{
				auto gunmodel = data["gunXModel"][i].get<std::string>();

				if (!gunmodel.empty())
				{
					weapon->gunXModel[i] = DB_FindXAssetHeader(xmodel, gunmodel.data(), 1).xmodel;
				}
			}
			for (int i = 0; i < 16; i++)
			{
				auto gunmodel = data["worldXModel"][i].get<std::string>();

				if (!gunmodel.empty())
				{
					weapon->worldModel[i] = DB_FindXAssetHeader(xmodel, gunmodel.data(), 1).xmodel;
				}
			}

			WEAPON_READ_ASSET(xmodel, xmodel, handXModel);

			weapon->notetrackSoundMapKeys = mem->Alloc<short>(24);
			weapon->notetrackSoundMapValues = mem->Alloc<short>(24);
			for (int i = 0; i < 24; i++)
			{
				auto notetrack = data["notetrackSoundMapKeys"][i].get<std::string>();
				weapon->notetrackSoundMapKeys[i] = SL_AllocString(notetrack);
				ZONETOOL_INFO("notetrack %s %i", notetrack.data(), weapon->notetrackSoundMapKeys[i]);
			}
			for (int i = 0; i < 24; i++)
			{
				auto notetrack = data["notetrackSoundMapValues"][i].get<std::string>();
				weapon->notetrackSoundMapValues[i] = SL_AllocString(notetrack);
				ZONETOOL_INFO("notetrack %s %i", notetrack.data(), weapon->notetrackSoundMapValues[i]);
			}

			weapon->notetrackRumbleMapKeys = mem->Alloc<short>(16);
			weapon->notetrackRumbleMapValues = mem->Alloc<short>(16);
			for (int i = 0; i < 16; i++)
			{
				auto notetrack = data["notetrackRumbleMapKeys"][i].get<std::string>();
				weapon->notetrackRumbleMapKeys[i] = SL_AllocString(notetrack);
				ZONETOOL_INFO("notetrack %s %i", notetrack.data(), weapon->notetrackRumbleMapKeys[i]);
			}
			for (int i = 0; i < 16; i++)
			{
				auto notetrack = data["notetrackRumbleMapValues"][i].get<std::string>();
				weapon->notetrackRumbleMapValues[i] = SL_AllocString(notetrack);
				ZONETOOL_INFO("notetrack %s %i", notetrack.data(), weapon->notetrackRumbleMapValues[i]);
			}

			WEAPON_READ_FIELD(int, playerAnimType);
			WEAPON_READ_FIELD(int, weaponType);
			WEAPON_READ_FIELD(int, weaponClass);
			WEAPON_READ_FIELD(int, penetrateType);
			WEAPON_READ_FIELD(int, inventoryType);
			WEAPON_READ_FIELD(int, fireType);
			WEAPON_READ_FIELD(int, offhandClass);
			WEAPON_READ_FIELD(int, stance);
			WEAPON_READ_ASSET(fx, fx, viewFlashEffect);
			WEAPON_READ_ASSET(fx, fx, worldFlashEffect);
			WEAPON_READ_ASSET(fx, fx, viewShellEjectEffect);
			WEAPON_READ_ASSET(fx, fx, worldShellEjectEffect);
			WEAPON_READ_ASSET(fx, fx, viewLastShotEjectEffect);
			WEAPON_READ_ASSET(fx, fx, worldLastShotEjectEffect);
			WEAPON_READ_ASSET(material, material, reticleCenter);
			WEAPON_READ_ASSET(material, material, reticleSide);

			for (int i = 0; i < 48; i++)
			{
				auto sound = data["sounds"][i].get<std::string>();

				if (!sound.empty())
				{
					weapon->sounds[i] = DB_FindXAssetHeader(XAssetType::sound, sound.data(), 1).sound;
				}
			}

			WEAPON_READ_FIELD(int, iReticleCenterSize);
			WEAPON_READ_FIELD(int, iReticleSideSize);
			WEAPON_READ_FIELD(int, iReticleMinOfs);
			// WEAPON_READ_FIELD(activeReticleType_t, activeReticleType);
			/*WEAPON_READ_FIELD(float, vStandMove[3]);
			WEAPON_READ_FIELD(float, vStandRot[3]);
			WEAPON_READ_FIELD(float, vStrafeMove[3]);
			WEAPON_READ_FIELD(float, vStrafeRot[3]);
			WEAPON_READ_FIELD(float, vDuckedOfs[3]);
			WEAPON_READ_FIELD(float, vDuckedMove[3]);
			WEAPON_READ_FIELD(float, vDuckedRot[3]);
			WEAPON_READ_FIELD(float, vProneOfs[3]);
			WEAPON_READ_FIELD(float, vProneMove[3]);
			WEAPON_READ_FIELD(float, vProneRot[3]);*/
			WEAPON_READ_FIELD(float, fPosMoveRate);
			WEAPON_READ_FIELD(float, fPosProneMoveRate);
			WEAPON_READ_FIELD(float, fStandMoveMinSpeed);
			WEAPON_READ_FIELD(float, fDuckedMoveMinSpeed);
			WEAPON_READ_FIELD(float, fProneMoveMinSpeed);
			WEAPON_READ_FIELD(float, fPosRotRate);
			WEAPON_READ_FIELD(float, fPosProneRotRate);
			WEAPON_READ_FIELD(float, fStandRotMinSpeed);
			WEAPON_READ_FIELD(float, fDuckedRotMinSpeed);
			WEAPON_READ_FIELD(float, fProneRotMinSpeed);
			WEAPON_READ_ASSET(xmodel, xmodel, worldClipModel);
			WEAPON_READ_ASSET(xmodel, xmodel, rocketModel);
			WEAPON_READ_ASSET(xmodel, xmodel, knifeModel);
			WEAPON_READ_ASSET(xmodel, xmodel, worldKnifeModel);
			WEAPON_READ_ASSET(material, material, hudIcon);
			WEAPON_READ_ASSET(material, material, pickupIcon);
			WEAPON_READ_ASSET(material, material, ammoCounterIcon);
			// WEAPON_READ_FIELD(weaponIconRatioType_t, hudIconRatio);
			// WEAPON_READ_FIELD(weaponIconRatioType_t, pickupIconRatio);
			// WEAPON_READ_FIELD(weaponIconRatioType_t, ammoCounterIconRatio);
			// WEAPON_READ_FIELD(ammoCounterClipType_t, ammoCounterClip);
			WEAPON_READ_FIELD(int, iStartAmmo);
			WEAPON_READ_STRING(szAmmoName);
			WEAPON_READ_FIELD(int, iAmmoIndex);
			WEAPON_READ_STRING(szClipName);
			WEAPON_READ_FIELD(int, iClipIndex);
			WEAPON_READ_FIELD(int, iMaxAmmo);
			WEAPON_READ_FIELD(int, shotCount);
			WEAPON_READ_STRING(szSharedAmmoCapName);
			WEAPON_READ_FIELD(int, iSharedAmmoCapIndex);
			WEAPON_READ_FIELD(int, iSharedAmmoCap);
			WEAPON_READ_FIELD(int, damage);
			WEAPON_READ_FIELD(int, playerDamage);
			WEAPON_READ_FIELD(int, iMeleeDamage);
			WEAPON_READ_FIELD(int, iDamageType);
			WEAPON_READ_FIELD(int, iFireDelay);
			WEAPON_READ_FIELD(int, iMeleeDelay);
			WEAPON_READ_FIELD(int, meleeChargeDelay);
			WEAPON_READ_FIELD(int, iDetonateDelay);
			WEAPON_READ_FIELD(int, iRechamberTime);
			WEAPON_READ_FIELD(int, iRechamberOneHanded);
			WEAPON_READ_FIELD(int, iRechamberBoltTime);
			WEAPON_READ_FIELD(int, iHoldFireTime);
			WEAPON_READ_FIELD(int, iDetonateTime);
			WEAPON_READ_FIELD(int, iMeleeTime);
			WEAPON_READ_FIELD(int, meleeChargeTime);
			WEAPON_READ_FIELD(int, iReloadTime);
			WEAPON_READ_FIELD(int, reloadShowRocketTime);
			WEAPON_READ_FIELD(int, iReloadEmptyTime);
			WEAPON_READ_FIELD(int, iReloadAddTime);
			WEAPON_READ_FIELD(int, iReloadStartTime);
			WEAPON_READ_FIELD(int, iReloadStartAddTime);
			WEAPON_READ_FIELD(int, iReloadEndTime);
			WEAPON_READ_FIELD(int, iDropTime);
			WEAPON_READ_FIELD(int, iRaiseTime);
			WEAPON_READ_FIELD(int, iAltDropTime);
			WEAPON_READ_FIELD(int, quickDropTime);
			WEAPON_READ_FIELD(int, quickRaiseTime);
			WEAPON_READ_FIELD(int, iFirstRaiseTime);
			WEAPON_READ_FIELD(int, iEmptyRaiseTime);
			WEAPON_READ_FIELD(int, iEmptyDropTime);
			WEAPON_READ_FIELD(int, sprintInTime);
			WEAPON_READ_FIELD(int, sprintLoopTime);
			WEAPON_READ_FIELD(int, sprintOutTime);
			WEAPON_READ_FIELD(int, stunnedTimeBegin);
			WEAPON_READ_FIELD(int, stunnedTimeLoop);
			WEAPON_READ_FIELD(int, stunnedTimeEnd);
			WEAPON_READ_FIELD(int, nightVisionWearTime);
			WEAPON_READ_FIELD(int, nightVisionWearTimeFadeOutEnd);
			WEAPON_READ_FIELD(int, nightVisionWearTimePowerUp);
			WEAPON_READ_FIELD(int, nightVisionRemoveTime);
			WEAPON_READ_FIELD(int, nightVisionRemoveTimePowerDown);
			WEAPON_READ_FIELD(int, nightVisionRemoveTimeFadeInStart);
			WEAPON_READ_FIELD(int, fuseTime);
			WEAPON_READ_FIELD(int, aifuseTime);
			WEAPON_READ_FIELD(float, autoAimRange);
			WEAPON_READ_FIELD(float, aimAssistRange);
			WEAPON_READ_FIELD(float, aimAssistRangeAds);
			WEAPON_READ_FIELD(float, aimPadding);
			WEAPON_READ_FIELD(float, enemyCrosshairRange);
			WEAPON_READ_FIELD(float, moveSpeedScale);
			WEAPON_READ_FIELD(float, adsMoveSpeedScale);
			WEAPON_READ_FIELD(float, sprintDurationScale);
			WEAPON_READ_FIELD(float, adsZoomInFrac);
			WEAPON_READ_FIELD(float, adsZoomOutFrac);
			WEAPON_READ_ASSET(material, material, AdsOverlayShader);
			WEAPON_READ_ASSET(material, material, AdsOverlayShaderLowRes);
			WEAPON_READ_ASSET(material, material, AdsOverlayShaderEMP);
			WEAPON_READ_ASSET(material, material, AdsOverlayShaderEMPLowRes);
			// WEAPON_READ_FIELD(weapOverlayReticle_t, adsOverlayReticle);
			// WEAPON_READ_FIELD(weapOverlayInterface_t, adsOverlayInterface);
			WEAPON_READ_FIELD(float, adsOverlayWidth);
			WEAPON_READ_FIELD(float, adsOverlayHeight);
			WEAPON_READ_FIELD(float, adsOverlayWidthSplitscreen);
			WEAPON_READ_FIELD(float, adsOverlayHeightSplitscreen);
			WEAPON_READ_FIELD(float, fAdsBobFactor);
			WEAPON_READ_FIELD(float, fAdsViewBobMult);
			WEAPON_READ_FIELD(float, fHipSpreadStandMin);
			WEAPON_READ_FIELD(float, fHipSpreadDuckedMin);
			WEAPON_READ_FIELD(float, fHipSpreadProneMin);
			WEAPON_READ_FIELD(float, hipSpreadStandMax);
			WEAPON_READ_FIELD(float, hipSpreadDuckedMax);
			WEAPON_READ_FIELD(float, hipSpreadProneMax);
			WEAPON_READ_FIELD(float, fHipSpreadDecayRate);
			WEAPON_READ_FIELD(float, fHipSpreadFireAdd);
			WEAPON_READ_FIELD(float, fHipSpreadTurnAdd);
			WEAPON_READ_FIELD(float, fHipSpreadMoveAdd);
			WEAPON_READ_FIELD(float, fHipSpreadDuckedDecay);
			WEAPON_READ_FIELD(float, fHipSpreadProneDecay);
			WEAPON_READ_FIELD(float, fHipReticleSidePos);
			WEAPON_READ_FIELD(float, fAdsIdleAmount);
			WEAPON_READ_FIELD(float, fHipIdleAmount);
			WEAPON_READ_FIELD(float, adsIdleSpeed);
			WEAPON_READ_FIELD(float, hipIdleSpeed);
			WEAPON_READ_FIELD(float, fIdleCrouchFactor);
			WEAPON_READ_FIELD(float, fIdleProneFactor);
			WEAPON_READ_FIELD(float, fGunMaxPitch);
			WEAPON_READ_FIELD(float, fGunMaxYaw);
			WEAPON_READ_FIELD(float, swayMaxAngle);
			WEAPON_READ_FIELD(float, swayLerpSpeed);
			WEAPON_READ_FIELD(float, swayPitchScale);
			WEAPON_READ_FIELD(float, swayYawScale);
			WEAPON_READ_FIELD(float, swayHorizScale);
			WEAPON_READ_FIELD(float, swayVertScale);
			WEAPON_READ_FIELD(float, swayShellShockScale);
			WEAPON_READ_FIELD(float, adsSwayMaxAngle);
			WEAPON_READ_FIELD(float, adsSwayLerpSpeed);
			WEAPON_READ_FIELD(float, adsSwayPitchScale);
			WEAPON_READ_FIELD(float, adsSwayYawScale);
			WEAPON_READ_FIELD(float, adsSwayHorizScale);
			WEAPON_READ_FIELD(float, adsSwayVertScale);
			WEAPON_READ_FIELD(float, adsViewErrorMin);
			WEAPON_READ_FIELD(float, adsViewErrorMax);
			// _BYTE idk[8]);
			// PhysCollmap* collisions);
			WEAPON_READ_ASSET(phys_collmap, physcollmap, collisions);
			WEAPON_READ_FIELD(float, dualWieldViewModelOffset);
			// WEAPON_READ_FIELD(weaponIconRatioType_t, killIconRatio);
			WEAPON_READ_FIELD(int, iReloadAmmoAdd);
			WEAPON_READ_FIELD(int, iReloadStartAdd);
			WEAPON_READ_FIELD(int, iDropAmmoMin);
			WEAPON_READ_FIELD(int, ammoDropClipPercentMin);
			WEAPON_READ_FIELD(int, ammoDropClipPercentMax);
			WEAPON_READ_FIELD(int, iExplosionRadius);
			WEAPON_READ_FIELD(int, iExplosionRadiusMin);
			WEAPON_READ_FIELD(int, iExplosionInnerDamage);
			WEAPON_READ_FIELD(int, iExplosionOuterDamage);
			WEAPON_READ_FIELD(float, damageConeAngle);
			WEAPON_READ_FIELD(float, bulletExplDmgMult);
			WEAPON_READ_FIELD(float, bulletExplRadiusMult);
			WEAPON_READ_FIELD(int, iProjectileSpeed);
			WEAPON_READ_FIELD(int, iProjectileSpeedUp);
			WEAPON_READ_FIELD(int, iProjectileSpeedForward);
			WEAPON_READ_FIELD(int, iProjectileActivateDist);
			WEAPON_READ_FIELD(float, projLifetime);
			WEAPON_READ_FIELD(float, timeToAccelerate);
			WEAPON_READ_FIELD(float, projectileCurvature);
			WEAPON_READ_ASSET(xmodel, xmodel, projectileModel);
			WEAPON_READ_ASSET(fx, fx, projExplosionEffect);
			WEAPON_READ_ASSET(fx, fx, projDudEffect);
			WEAPON_READ_ASSET(sound, sound, projExplosionSound);
			WEAPON_READ_ASSET(sound, sound, projDudSound);
			// WEAPON_READ_FIELD(weapProjExplosion_t, projExplosiveType);
			// WEAPON_READ_FIELD(WeapStickinessType, stickiness);
			WEAPON_READ_FIELD(float, lowAmmoWarningThreshold);
			WEAPON_READ_FIELD(float, ricochetChance);
			// WEAPON_READ_FIELD(float, * parallelBounce);            //Refer to surfaceNames_t
			// WEAPON_READ_FIELD(float, * perpendicularBounce);         //Refer to surfaceNames_t
			// _BYTE gap52D[4]);
			// WEAPON_READ_STRING(unkStruct124_1);
			// WEAPON_READ_STRING(unkStruct124_2);
			WEAPON_READ_ASSET(fx, fx, projTrailEffect);
			WEAPON_READ_ASSET(fx, fx, projBeaconEffect);
			// WEAPON_READ_FIELD(float, vProjectileColor[3]);
			// WEAPON_READ_FIELD(guidedMissileType_t, guidedMissileType);
			WEAPON_READ_FIELD(float, maxSteeringAccel);
			WEAPON_READ_FIELD(float, projIgnitionDelay);
			WEAPON_READ_ASSET(fx, fx, projIgnitionEffect);
			WEAPON_READ_ASSET(sound, sound, projIgnitionSound);
			WEAPON_READ_FIELD(float, fAdsAimPitch);
			WEAPON_READ_FIELD(float, fAdsCrosshairInFrac);
			WEAPON_READ_FIELD(float, fAdsCrosshairOutFrac);
			WEAPON_READ_FIELD(int, adsGunKickReducedKickBullets);
			WEAPON_READ_FIELD(float, adsGunKickReducedKickPercent);
			WEAPON_READ_FIELD(float, fAdsGunKickPitchMin);
			WEAPON_READ_FIELD(float, fAdsGunKickPitchMax);
			WEAPON_READ_FIELD(float, fAdsGunKickYawMin);
			WEAPON_READ_FIELD(float, fAdsGunKickYawMax);
			WEAPON_READ_FIELD(float, fAdsGunKickAccel);
			WEAPON_READ_FIELD(float, fAdsGunKickSpeedMax);
			WEAPON_READ_FIELD(float, fAdsGunKickSpeedDecay);
			WEAPON_READ_FIELD(float, fAdsGunKickStaticDecay);
			WEAPON_READ_FIELD(float, fAdsViewKickPitchMin);
			WEAPON_READ_FIELD(float, fAdsViewKickPitchMax);
			WEAPON_READ_FIELD(float, fAdsViewKickYawMin);
			WEAPON_READ_FIELD(float, fAdsViewKickYawMax);
			WEAPON_READ_FIELD(float, fAdsViewScatterMin);
			WEAPON_READ_FIELD(float, fAdsViewScatterMax);
			WEAPON_READ_FIELD(float, fAdsSpread);
			WEAPON_READ_FIELD(int, hipGunKickReducedKickBullets);
			WEAPON_READ_FIELD(float, hipGunKickReducedKickPercent);
			WEAPON_READ_FIELD(float, fHipGunKickPitchMin);
			WEAPON_READ_FIELD(float, fHipGunKickPitchMax);
			WEAPON_READ_FIELD(float, fHipGunKickYawMin);
			WEAPON_READ_FIELD(float, fHipGunKickYawMax);
			WEAPON_READ_FIELD(float, fHipGunKickAccel);
			WEAPON_READ_FIELD(float, fHipGunKickSpeedMax);
			WEAPON_READ_FIELD(float, fHipGunKickSpeedDecay);
			WEAPON_READ_FIELD(float, fHipGunKickStaticDecay);
			WEAPON_READ_FIELD(float, fHipViewKickPitchMin);
			WEAPON_READ_FIELD(float, fHipViewKickPitchMax);
			WEAPON_READ_FIELD(float, fHipViewKickYawMin);
			WEAPON_READ_FIELD(float, fHipViewKickYawMax);
			WEAPON_READ_FIELD(float, fHipViewScatterMin);
			WEAPON_READ_FIELD(float, fHipViewScatterMax);
			WEAPON_READ_FIELD(float, fightDist);
			WEAPON_READ_FIELD(float, maxDist);
			WEAPON_READ_FIELD(int, iPositionReloadTransTime);
			WEAPON_READ_FIELD(float, leftArc);
			WEAPON_READ_FIELD(float, rightArc);
			WEAPON_READ_FIELD(float, topArc);
			WEAPON_READ_FIELD(float, bottomArc);
			WEAPON_READ_FIELD(float, accuracy);
			WEAPON_READ_FIELD(float, aiSpread);
			WEAPON_READ_FIELD(float, playerSpread);
			WEAPON_READ_FIELD(float, minVertTurnSpeed);
			WEAPON_READ_FIELD(float, minHorTurnSpeed);
			WEAPON_READ_FIELD(float, maxVertTurnSpeed);
			WEAPON_READ_FIELD(float, maxHorTurnSpeed);
			WEAPON_READ_FIELD(float, pitchConvergenceTime);
			WEAPON_READ_FIELD(float, yawConvergenceTime);
			WEAPON_READ_FIELD(float, suppressTime);
			WEAPON_READ_FIELD(float, maxRange);
			WEAPON_READ_FIELD(float, fAnimHorRotateInc);
			WEAPON_READ_FIELD(float, fPlayerPositionDist);
			WEAPON_READ_STRING(szUseHintString);
			WEAPON_READ_STRING(dropHintString);
			WEAPON_READ_FIELD(int, iUseHintStringIndex);
			WEAPON_READ_FIELD(int, dropHintStringIndex);
			WEAPON_READ_FIELD(float, horizViewJitter);
			WEAPON_READ_FIELD(float, vertViewJitter);
			WEAPON_READ_FIELD(float, scanSpeed);
			WEAPON_READ_FIELD(float, scanAccel);
			WEAPON_READ_FIELD(int, scanPauseTime);
			WEAPON_READ_STRING(szScript);
			// WEAPON_READ_FIELD(float, fOOPosAnimLength[2]);
			WEAPON_READ_FIELD(int, minDamage);
			WEAPON_READ_FIELD(int, minPlayerDamage);
			WEAPON_READ_FIELD(float, maxDamageRange);
			WEAPON_READ_FIELD(float, minDamageRange);
			WEAPON_READ_FIELD(float, destabilizationRateTime);
			WEAPON_READ_FIELD(float, destabilizationCurvatureMax);
			WEAPON_READ_FIELD(int, destabilizeDistance);
			// float* locationDamageMultipliers);
			WEAPON_READ_STRING(fireRumble);
			WEAPON_READ_STRING(meleeImpactRumble);
			WEAPON_READ_ASSET(tracer, tracer, tracer);
			WEAPON_READ_ASSET(fx, fx, turretOverheatEffect);
			WEAPON_READ_ASSET(sound, sound, turretOverheatSound);
			WEAPON_READ_STRING(turretBarrelSpinRumble);
			WEAPON_READ_FIELD(int, turretBarrelSpinUpTime);
			WEAPON_READ_FIELD(int, turretBarrelSpinDownTime);
			WEAPON_READ_FIELD(int, turretBarrelSpinSpeed);
			WEAPON_READ_ASSET(sound, sound, turretBarrelSpinMaxSnd);
			WEAPON_READ_ASSET(sound, sound, missileConeSoundAlias);
			WEAPON_READ_ASSET(sound, sound, missileConeSoundAliasAtBase);
			// snd_alias_list_t* turretBarrelSpinUpSnds[4]);
			// snd_alias_list_t* turretBarrelSpinDownSnds[4]);
			WEAPON_READ_FIELD(float, missileConeSoundRadiusAtTop);
			WEAPON_READ_FIELD(float, missileConeSoundRadiusAtBase);
			WEAPON_READ_FIELD(float, missileConeSoundHeight);
			WEAPON_READ_FIELD(float, missileConeSoundOriginOffset);
			WEAPON_READ_FIELD(float, missileConeSoundVolumescaleAtCore);
			WEAPON_READ_FIELD(float, missileConeSoundVolumescaleAtEdge);
			WEAPON_READ_FIELD(float, missileConeSoundVolumescaleCoreSize);
			WEAPON_READ_FIELD(float, missileConeSoundPitchAtTop);
			WEAPON_READ_FIELD(float, missileConeSoundPitchAtBottom);
			WEAPON_READ_FIELD(float, missileConeSoundPitchTopSize);
			WEAPON_READ_FIELD(float, missileConeSoundPitchBottomSize);
			WEAPON_READ_FIELD(float, missileConeSoundCrossfadeTopSize);
			WEAPON_READ_FIELD(float, missileConeSoundCrossfadeBottomSize);
			WEAPON_READ_FIELD(bool, shareAmmo);
			WEAPON_READ_FIELD(bool, lockonSupported);
			WEAPON_READ_FIELD(bool, requireLockonToFire);
			WEAPON_READ_FIELD(bool, bigExplosion);
			WEAPON_READ_FIELD(bool, noAdsWhenMagEmpty);
			WEAPON_READ_FIELD(bool, avoidDropCleanup);
			WEAPON_READ_FIELD(bool, inheritsPerks);
			WEAPON_READ_FIELD(bool, crosshairColorChange);
			WEAPON_READ_FIELD(bool, rifleBullet);
			WEAPON_READ_FIELD(bool, armorPiercing);
			WEAPON_READ_FIELD(bool, boltAction);
			WEAPON_READ_FIELD(bool, aimDownSight);
			WEAPON_READ_FIELD(bool, rechamberWhileAds);
			WEAPON_READ_FIELD(bool, bBulletExplosiveDamage);
			WEAPON_READ_FIELD(bool, cookOffHold);
			WEAPON_READ_FIELD(bool, clipOnly);
			WEAPON_READ_FIELD(bool, noAmmoPickup);
			WEAPON_READ_FIELD(bool, adsFire);
			WEAPON_READ_FIELD(bool, cancelAutoHolsterWhenEmpty);
			WEAPON_READ_FIELD(bool, disableSwitchToWhenEmpty);
			WEAPON_READ_FIELD(bool, suppressAmmoReserveDisplay);
			WEAPON_READ_FIELD(bool, laserSightDuringNightvision);
			WEAPON_READ_FIELD(bool, markableViewmodel);
			WEAPON_READ_FIELD(bool, noDualWield);
			WEAPON_READ_FIELD(bool, flipKillIcon);
			WEAPON_READ_FIELD(bool, noPartialReload);
			WEAPON_READ_FIELD(bool, segmentedReload);
			WEAPON_READ_FIELD(bool, blocksProne);
			WEAPON_READ_FIELD(bool, silenced);
			WEAPON_READ_FIELD(bool, isRollingGrenade);
			WEAPON_READ_FIELD(bool, projExplosionEffectForceNormalUp);
			WEAPON_READ_FIELD(bool, projImpactExplode);
			WEAPON_READ_FIELD(bool, stickToPlayers);
			WEAPON_READ_FIELD(bool, hasDetonator);
			WEAPON_READ_FIELD(bool, disableFiring);
			WEAPON_READ_FIELD(bool, timedDetonation);
			WEAPON_READ_FIELD(bool, rotate);
			WEAPON_READ_FIELD(bool, holdButtonToThrow);
			WEAPON_READ_FIELD(bool, freezeMovementWhenFiring);
			WEAPON_READ_FIELD(bool, thermalScope);
			WEAPON_READ_FIELD(bool, altModeSameWeapon);
			WEAPON_READ_FIELD(bool, turretBarrelSpinEnabled);
			WEAPON_READ_FIELD(bool, missileConeSoundEnabled);
			WEAPON_READ_FIELD(bool, missileConeSoundPitchshiftEnabled);
			WEAPON_READ_FIELD(bool, missileConeSoundCrossfadeEnabled);
			WEAPON_READ_FIELD(bool, offhandHoldIsCancelable);

			return weapon;
		}

		WeaponCompleteDef* IWeaponDef::parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			sizeof WeaponCompleteDef;

			auto path = "weapons/mp/" + name;

			if (!FileSystem::FileExists(path))
			{
				return nullptr;
			}

			ZONETOOL_INFO("Parsing weapon \"%s\"...", name.data());

			auto weapon = mem->Alloc<WeaponCompleteDef>();

			// parse json file
			auto file = FileSystem::FileOpen(path, "rb");
			auto size = FileSystem::FileSize(file);
			auto bytes = FileSystem::ReadBytes(file, size);
			FileSystem::FileClose(file);
			nlohmann::json data = nlohmann::json::parse(bytes);

			// base asset
			auto base = data["baseAsset"].get<std::string>();
			WeaponCompleteDef* baseAsset = nullptr;

			if (!base.empty())
			{
				baseAsset = DB_FindXAssetHeader(XAssetType::weapon, base.data(), 1).weapon;
				memcpy(weapon, baseAsset, sizeof WeaponCompleteDef);
			}
			else
			{
				ZONETOOL_WARNING("No base asset is defined for weapon %s, stuff might go wrong!", name.data());
			}

			// parse weapondef
			weapon->WeaponDef = this->parse_weapondef(data["WeaponDef"], baseAsset, mem);

			WEAPON_READ_STRING(name);
			WEAPON_READ_STRING(displayName);

			weapon->hideTags = mem->Alloc<short>(32);
			for (int i = 0; i < 32; i++)
			{
				weapon->hideTags[i] = SL_AllocString(data["hideTags"][i].get<std::string>());
			}

			weapon->attachment1 = mem->Alloc<AttachmentDef*>(6);
			for (int i = 0; i < 6; i++)
			{
				auto attachment = data["attachment1"][i].get<std::string>();

				if (!attachment.empty())
				{
					weapon->attachment1[i] = DB_FindXAssetHeader(XAssetType::attachment, attachment.data(), 1).
						attachment;
				}
			}
			weapon->attachment2 = mem->Alloc<AttachmentDef*>(3);
			for (int i = 0; i < 3; i++)
			{
				auto attachment = data["attachment2"][i].get<std::string>();

				if (!attachment.empty())
				{
					weapon->attachment2[i] = DB_FindXAssetHeader(XAssetType::attachment, attachment.data(), 1).
						attachment;
				}
			}
			weapon->attachment3 = mem->Alloc<AttachmentDef*>(4);
			for (int i = 0; i < 4; i++)
			{
				auto attachment = data["attachment3"][i].get<std::string>();

				if (!attachment.empty())
				{
					weapon->attachment3[i] = DB_FindXAssetHeader(XAssetType::attachment, attachment.data(), 1).
						attachment;
				}
			}

			weapon->szXAnims = mem->Alloc<const char*>(42);
			for (int i = 0; i < 42; i++)
			{
				weapon->szXAnims[i] = mem->StrDup(data["szXAnims"][i].get<std::string>());
			}

			WEAPON_READ_FIELD(unsigned int, numAnimOverrides);
			WEAPON_READ_FIELD(unsigned int, numSoundOverrides);
			WEAPON_READ_FIELD(unsigned int, numFXOverrides);
			WEAPON_READ_FIELD(unsigned int, numReloadStateTimerOverrides);
			WEAPON_READ_FIELD(unsigned int, numNotetrackOverrides);

			if (weapon->numAnimOverrides)
			{
				weapon->animOverrides = mem->Alloc<AnimOverrideEntry>(weapon->numAnimOverrides);
				for (int i = 0; i < weapon->numAnimOverrides; i++)
				{
					weapon->animOverrides[i].altmodeAnim = mem->StrDup(
						data["animOverrides"][i]["altmodeAnim"].is_string()
							? data["animOverrides"][i]["altmodeAnim"].get<std::string>()
							: "");
					weapon->animOverrides[i].overrideAnim = mem->StrDup(
						data["animOverrides"][i]["overrideAnim"].is_string()
							? data["animOverrides"][i]["overrideAnim"].get<std::string>()
							: "");
					weapon->animOverrides[i].attachment1 = data["animOverrides"][i]["attachment1"].get<unsigned short
					>();
					weapon->animOverrides[i].attachment2 = data["animOverrides"][i]["attachment2"].get<unsigned short
					>();
					weapon->animOverrides[i].altTime = data["animOverrides"][i]["altTime"].get<int>();
					weapon->animOverrides[i].animTime = data["animOverrides"][i]["animTime"].get<int>();
					weapon->animOverrides[i].animTreeType = data["animOverrides"][i]["animTreeType"].get<unsigned int
					>();
				}
			}

			if (weapon->numSoundOverrides)
			{
				weapon->soundOverrides = mem->Alloc<SoundOverrideEntry>(weapon->numSoundOverrides);
				for (int i = 0; i < weapon->numSoundOverrides; i++)
				{
					weapon->soundOverrides[i].altmodeSound = (data["soundOverrides"][i]["altmodeSound"].is_string())
						                                         ? DB_FindXAssetHeader(
							                                         sound,
							                                         data["soundOverrides"][i]["altmodeSound"]
							                                         .get<std::string>().data(), 1).sound
						                                         : nullptr;
					weapon->soundOverrides[i].overrideSound = (data["soundOverrides"][i]["overrideSound"].is_string())
						                                          ? DB_FindXAssetHeader(
							                                          sound,
							                                          data["soundOverrides"][i]["overrideSound"]
							                                          .get<std::string>().data(), 1).sound
						                                          : nullptr;
					weapon->soundOverrides[i].attachment1 = data["soundOverrides"][i]["attachment1"].get<unsigned short
					>();
					weapon->soundOverrides[i].attachment2 = data["soundOverrides"][i]["attachment2"].get<unsigned short
					>();
					weapon->soundOverrides[i].soundType = data["soundOverrides"][i]["soundType"].get<unsigned int>();
				}
			}

			if (weapon->numFXOverrides)
			{
				weapon->fxOverrides = mem->Alloc<FXOverrideEntry>(weapon->numFXOverrides);
				for (int i = 0; i < weapon->numFXOverrides; i++)
				{
					weapon->fxOverrides[i].altmodeFX = (data["fxOverrides"][i]["altmodeFX"].is_string())
						                                   ? DB_FindXAssetHeader(
							                                   fx, data["fxOverrides"][i]["altmodeFX"]
							                                       .get<std::string>().data(), 1).fx
						                                   : nullptr;
					weapon->fxOverrides[i].overrideFX = (data["fxOverrides"][i]["overrideFX"].is_string())
						                                    ? DB_FindXAssetHeader(
							                                    fx, data["fxOverrides"][i]["overrideFX"]
							                                        .get<std::string>().data(), 1).fx
						                                    : nullptr;
					weapon->fxOverrides[i].attachment1 = data["fxOverrides"][i]["attachment1"].get<unsigned short>();
					weapon->fxOverrides[i].attachment2 = data["fxOverrides"][i]["attachment2"].get<unsigned short>();
					weapon->fxOverrides[i].fxType = data["fxOverrides"][i]["fxType"].get<unsigned int>();
				}
			}

			if (weapon->numReloadStateTimerOverrides)
			{
				weapon->reloadOverrides = mem->Alloc<ReloadStateTimerEntry>(weapon->numReloadStateTimerOverrides);
				for (int i = 0; i < weapon->numReloadStateTimerOverrides; i++)
				{
					weapon->reloadOverrides[i].attachment = data["attachment"].get<int>();
					weapon->reloadOverrides[i].reloadAddTime = data["reloadAddTime"].get<int>();
					weapon->reloadOverrides[i].reloadEmptyAddTime = data["reloadEmptyAddTime"].get<int>();
				}
			}

			if (weapon->numNotetrackOverrides)
			{
				// TODO
				weapon->numNotetrackOverrides = 0;
			}

			/*
				for (int j = 0; j < 24; j++)
				{
					data["notetrackOverrides"][i]["notetrackSoundMapKeys"][j] = SL_ConvertToString(asset->notetrackOverrides[i].notetrackSoundMapKeys[j]);
					data["notetrackOverrides"][i]["notetrackSoundMapValues"][j] = SL_ConvertToString(asset->notetrackOverrides[i].notetrackSoundMapValues[j]);
				}
			*/

			WEAPON_READ_FIELD(int, iAdsTransInTime);
			WEAPON_READ_FIELD(int, iAdsTransOutTime);
			WEAPON_READ_FIELD(int, iClipSize);
			WEAPON_READ_FIELD(int, impactType);
			WEAPON_READ_FIELD(int, iFireTime);
			WEAPON_READ_FIELD(int, iFireTimeAkimbo);
			WEAPON_READ_FIELD(int, dpadIconRatio);
			WEAPON_READ_FIELD(int, iPenetrateMultiplier);
			WEAPON_READ_FIELD(float, fAdsViewKickCenterSpeed);
			WEAPON_READ_FIELD(float, fHipViewKickCenterSpeed);
			WEAPON_READ_STRING(altWeaponName);
			WEAPON_READ_FIELD(unsigned int, altWeaponIndex);
			WEAPON_READ_FIELD(int, iAltRaiseTime);
			WEAPON_READ_FIELD(int, iAltRaiseTimeAkimbo);
			WEAPON_READ_ASSET(material, material, killIcon);
			WEAPON_READ_ASSET(material, material, dpadIcon);
			WEAPON_READ_FIELD(int, iDropAmmoMax);
			WEAPON_READ_FIELD(bool, motionTracker);
			WEAPON_READ_FIELD(bool, enhanced);
			WEAPON_READ_FIELD(bool, dpadIconShowsAmmo);

			return weapon;
		}

		void IWeaponDef::init(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_name = name;
			this->m_asset = this->parse(name, mem);

			if (!this->m_asset)
			{
				this->m_asset = DB_FindXAssetHeader(this->type(), this->name().data(), 1).weapon;
			}
		}

		void IWeaponDef::prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem)
		{
			auto weapon = mem->Alloc<WeaponCompleteDef>();
			memcpy(weapon, this->m_asset, sizeof WeaponCompleteDef);

			weapon->hideTags = mem->Alloc<short>(32);
			memcpy(weapon->hideTags, this->m_asset->hideTags, sizeof(short) * 32);

			if (weapon->hideTags)
			{
				for (int i = 0; i < 32; i++)
				{
					if (weapon->hideTags[i])
					{
						std::string tag = SL_ConvertToString(weapon->hideTags[i]);
						weapon->hideTags[i] = buf->write_scriptstring(tag);
					}
				}
			}

			if (weapon->notetrackOverrides)
			{
				for (int i = 0; i < weapon->numNotetrackOverrides; i++)
				{
					if (weapon->notetrackOverrides[i].notetrackSoundMapKeys)
					{
						weapon->notetrackOverrides[i].notetrackSoundMapKeys = mem->Alloc<short>(24);
						memcpy(weapon->notetrackOverrides[i].notetrackSoundMapKeys,
						       this->m_asset->notetrackOverrides[i].notetrackSoundMapKeys, sizeof(short) * 24);

						for (int nt = 0; nt < 24; nt++)
						{
							if (weapon->notetrackOverrides[i].notetrackSoundMapKeys[nt])
							{
								std::string tag = SL_ConvertToString(
									this->m_asset->notetrackOverrides[i].notetrackSoundMapKeys[nt]);
								weapon->notetrackOverrides[i].notetrackSoundMapKeys[nt] = buf->write_scriptstring(tag);
							}
						}
					}

					if (weapon->notetrackOverrides[i].notetrackSoundMapValues)
					{
						weapon->notetrackOverrides[i].notetrackSoundMapValues = mem->Alloc<short>(24);
						memcpy(weapon->notetrackOverrides[i].notetrackSoundMapValues,
						       this->m_asset->notetrackOverrides[i].notetrackSoundMapValues, sizeof(short) * 24);

						for (int nt = 0; nt < 24; nt++)
						{
							if (weapon->notetrackOverrides[i].notetrackSoundMapValues[nt])
							{
								std::string tag = SL_ConvertToString(
									this->m_asset->notetrackOverrides[i].notetrackSoundMapValues[nt]);
								weapon->notetrackOverrides[i].notetrackSoundMapValues[nt] = buf->
									write_scriptstring(tag);
							}
						}
					}
				}
			}

			// weaponDef shit
			auto data = mem->Alloc<WeaponDef>(); // weapon->WeaponDef;
			memcpy(data, weapon->WeaponDef, sizeof WeaponDef);

#define WEAPON_SCRIPTSTRING_ARRAY(__field__,__count__) \
			if (data->__field__) \
			{ \
				data->__field__ = mem->Alloc<short>(__count__); \
				memcpy(data->__field__, this->m_asset->WeaponDef->__field__, sizeof(short) * __count__); \
\
				for (int nt = 0; nt < __count__; nt++) \
				{ \
					std::string tag = SL_ConvertToString(data->__field__[nt]); \
					data->__field__[nt] = buf->write_scriptstring(tag); \
				} \
			}

			WEAPON_SCRIPTSTRING_ARRAY(notetrackSoundMapKeys, 24);
			WEAPON_SCRIPTSTRING_ARRAY(notetrackSoundMapValues, 24);
			WEAPON_SCRIPTSTRING_ARRAY(notetrackRumbleMapKeys, 16);
			WEAPON_SCRIPTSTRING_ARRAY(notetrackRumbleMapValues, 16);

			weapon->WeaponDef = data;
			this->m_asset = weapon;
		}

		void IWeaponDef::load_depending_WeaponDef(IZone* zone, WeaponDef* data)
		{
#define WEAPON_SUBASSET(__field__,__type__,__struct__) \
			if (data->__field__) \
			{ \
				zone->AddAssetOfType(__type__, data->__field__->name); \
			}

			for (auto i = 0u; i < 16; i++)
			{
				if (data->worldModel && data->worldModel[i])
				{
					zone->AddAssetOfType(xmodel, data->worldModel[i]->name);
				}
				if (data->gunXModel && data->gunXModel[i])
				{
					zone->AddAssetOfType(xmodel, data->gunXModel[i]->name);
				}
			}

			WEAPON_SUBASSET(viewFlashEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(worldFlashEffect, fx, FxEffectDef);

			for (auto i = 0u; i < 48; i++)
			{
				WEAPON_SUBASSET(sounds[i], sound, snd_alias_list_t);
			}

			if (data->sndArray1)
			{
				for (auto i = 0u; i < 31; i++)
				{
					if (data->sndArray1[i])
					{
						zone->AddAssetOfType(sound, data->sndArray1[i]->name);
					}
				}
			}

			if (data->sndArray2)
			{
				for (auto i = 0u; i < 31; i++)
				{
					if (data->sndArray2[i])
					{
						zone->AddAssetOfType(sound, data->sndArray2[i]->name);
					}
				}
			}

			WEAPON_SUBASSET(viewShellEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(worldShellEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(viewLastShotEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(worldLastShotEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(reticleCenter, material, Material);
			WEAPON_SUBASSET(reticleSide, material, Material);

			WEAPON_SUBASSET(worldClipModel, xmodel, XModel);
			WEAPON_SUBASSET(rocketModel, xmodel, XModel);
			WEAPON_SUBASSET(knifeModel, xmodel, XModel);
			WEAPON_SUBASSET(worldKnifeModel, xmodel, XModel);

			WEAPON_SUBASSET(hudIcon, material, Material);
			WEAPON_SUBASSET(pickupIcon, material, Material);
			WEAPON_SUBASSET(ammoCounterIcon, material, Material);

			WEAPON_SUBASSET(AdsOverlayShader, material, Material);
			WEAPON_SUBASSET(AdsOverlayShaderLowRes, material, Material);
			WEAPON_SUBASSET(AdsOverlayShaderEMP, material, Material);
			WEAPON_SUBASSET(AdsOverlayShaderEMPLowRes, material, Material);

			WEAPON_SUBASSET(collisions, phys_collmap, PhysCollmap);

			WEAPON_SUBASSET(projectileModel, xmodel, XModel);

			WEAPON_SUBASSET(projExplosionEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(projDudEffect, fx, FxEffectDef);

			WEAPON_SUBASSET(projExplosionSound, sound, snd_alias_list_t);
			WEAPON_SUBASSET(projDudSound, sound, snd_alias_list_t);

			WEAPON_SUBASSET(projTrailEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(projBeaconEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(projIgnitionEffect, fx, FxEffectDef);

			WEAPON_SUBASSET(projIgnitionSound, sound, snd_alias_list_t);

			WEAPON_SUBASSET(tracer, tracer, TracerDef);

			WEAPON_SUBASSET(turretOverheatSound, sound, snd_alias_list_t);
			WEAPON_SUBASSET(turretOverheatEffect, fx, FxEffectDef);

			WEAPON_SUBASSET(turretBarrelSpinMaxSnd, sound, snd_alias_list_t);

			for (int i = 0; i < 4; i++)
			{
				WEAPON_SUBASSET(turretBarrelSpinUpSnds[i], sound, snd_alias_list_t);
				WEAPON_SUBASSET(turretBarrelSpinDownSnds[i], sound, snd_alias_list_t);
			}

			WEAPON_SUBASSET(missileConeSoundAlias, sound, snd_alias_list_t);
			WEAPON_SUBASSET(missileConeSoundAliasAtBase, sound, snd_alias_list_t);

			// WEAPON_SUBASSET(unknownXModel, xmodel, XModel);
		}

		void IWeaponDef::load_depending(IZone* zone)
		{
			auto data = this->m_asset;

			for (auto i = 0u; i < 6; i++)
			{
				if (data->attachment1 && data->attachment1[i])
				{
					zone->AddAssetOfType(attachment, this->m_asset->attachment1[i]->szInternalName);
				}

				if (i >= 3) continue;
				if (data->attachment3 && data->attachment3[i])
				{
					zone->AddAssetOfType(attachment, this->m_asset->attachment3[i]->szInternalName);
				}

				// Projectile attachments require fixing.
				// if (i >= 4) continue;
				// if (data->attachment2[i])
				// {
				// 	zone->AddAssetOfType(attachment, this->m_asset->attachment2[i]->szInternalName);
				// }
			}

			if (data->soundOverrides)
			{
				for (int i = 0; i < data->numSoundOverrides; i++)
				{
					if (data->soundOverrides[i].overrideSound)
					{
						zone->AddAssetOfType(sound, data->soundOverrides[i].overrideSound->name);
					}

					if (data->soundOverrides[i].altmodeSound)
					{
						zone->AddAssetOfType(sound, data->soundOverrides[i].altmodeSound->name);
					}
				}
			}

			if (data->fxOverrides)
			{
				for (int i = 0; i < data->numSoundOverrides; i++)
				{
					// if (data->fxOverrides[i].overrideFX)
					// {
					// 	zone->AddAssetOfType(fx, data->fxOverrides[i].overrideFX->name);
					// }

					// if (data->fxOverrides[i].altmodeFX)
					// {
					// 	zone->AddAssetOfType(fx, data->fxOverrides[i].altmodeFX->name);
					// }
				}
			}

			if (data->dpadIcon)
			{
				zone->AddAssetOfType(material, data->dpadIcon->name);
			}

			if (data->killIcon)
			{
				zone->AddAssetOfType(material, data->killIcon->name);
			}

			if (data->szXAnims)
			{
				for (int i = 0; i < 42; i++)
				{
					if (data->szXAnims[i])
					{
						zone->AddAssetOfType(xanim, data->szXAnims[i]);
					}
				}
			}

			load_depending_WeaponDef(zone, data->WeaponDef);
		}

		std::string IWeaponDef::name()
		{
			return this->m_name;
		}

		std::int32_t IWeaponDef::type()
		{
			return weapon;
		}

		void IWeaponDef::write_WeaponDef(IZone* zone, std::shared_ptr<ZoneBuffer>& buf, WeaponCompleteDef* complete,
		                                 WeaponDef* data)
		{
			auto dest = buf->write(data);

			if (data->szInternalName)
			{
				dest->szInternalName = buf->write_str(data->szInternalName);
			}

			if (data->gunXModel)
			{
				buf->align(3);
				auto destModels = buf->write(data->gunXModel, 16);

				for (auto i = 0u; i < 16; i++)
				{
					if (destModels[i])
					{
						destModels[i] = reinterpret_cast<XModel*>(
							zone->GetAssetPointer(xmodel, destModels[i]->name)
						);
					}
				}

				ZoneBuffer::ClearPointer(&dest->gunXModel);
			}

			if (data->handXModel)
			{
				dest->handXModel = reinterpret_cast<XModel*>(
					zone->GetAssetPointer(xmodel, data->handXModel->name)
				);
			}

			if (data->szXAnimsR)
			{
				buf->align(3);
				auto strings = buf->write(data->szXAnimsR, 42);

				for (auto i = 0u; i < 42; i++)
				{
					if (strings[i])
					{
						strings[i] = buf->write_str(strings[i]);
					}
				}

				ZoneBuffer::ClearPointer(&dest->szXAnimsR);
			}

			if (data->szXAnimsL)
			{
				buf->align(3);
				auto strings = buf->write(data->szXAnimsL, 42);

				for (auto i = 0u; i < 42; i++)
				{
					if (strings[i])
					{
						strings[i] = buf->write_str(strings[i]);
					}
				}

				ZoneBuffer::ClearPointer(&dest->szXAnimsL);
			}

			if (data->szModeName)
			{
				dest->szModeName = buf->write_str(data->szModeName);
			}

#define WEAPON_SCRIPTSTRING_ARRAY(__field__,__count__) \
			if (data->__field__) \
			{ \
				buf->align(1); \
				buf->write(data->__field__,__count__); \
				ZoneBuffer::ClearPointer(&dest->__field__); \
			}

			WEAPON_SCRIPTSTRING_ARRAY(notetrackSoundMapKeys, 24);
			WEAPON_SCRIPTSTRING_ARRAY(notetrackSoundMapValues, 24);
			WEAPON_SCRIPTSTRING_ARRAY(notetrackRumbleMapKeys, 16);
			WEAPON_SCRIPTSTRING_ARRAY(notetrackRumbleMapValues, 16);

#define WEAPON_SUBASSET(__field__,__type__,__struct__) \
			if (data->__field__) \
			{ \
				dest->__field__ = reinterpret_cast<__struct__*>(zone->GetAssetPointer(__type__, data->__field__->name)); \
			}

			WEAPON_SUBASSET(viewFlashEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(worldFlashEffect, fx, FxEffectDef);

			for (auto i = 0u; i < 48; i++)
			{
				if (!data->sounds[i]) continue;

				auto ptr = -1;

				buf->align(3);
				buf->write(&ptr);
				buf->write_str(data->sounds[i]->name);
				ZoneBuffer::ClearPointer(&dest->sounds[i]);
			}

			if (data->sndArray1)
			{
				buf->align(3);
				auto destSounds = buf->write(data->sndArray1, 31);

				for (auto i = 0u; i < 31; i++)
				{
					if (destSounds[i])
					{
						destSounds[i] = reinterpret_cast<snd_alias_list_t*>(
							zone->GetAssetPointer(sound, destSounds[i]->name)
						);
					}
				}

				ZoneBuffer::ClearPointer(&dest->sndArray1);
			}

			if (data->sndArray2)
			{
				buf->align(3);
				auto destSounds = buf->write(data->sndArray2, 31);

				for (auto i = 0u; i < 31; i++)
				{
					if (destSounds[i])
					{
						destSounds[i] = reinterpret_cast<snd_alias_list_t*>(
							zone->GetAssetPointer(sound, destSounds[i]->name)
						);
					}
				}

				ZoneBuffer::ClearPointer(&dest->sndArray2);
			}

			WEAPON_SUBASSET(viewShellEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(worldShellEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(viewLastShotEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(worldLastShotEjectEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(reticleCenter, material, Material);
			WEAPON_SUBASSET(reticleSide, material, Material);

			if (data->worldModel)
			{
				buf->align(3);
				auto destModels = buf->write(data->worldModel, 16);

				for (auto i = 0u; i < 16; i++)
				{
					if (destModels[i])
					{
						destModels[i] = reinterpret_cast<XModel*>(
							zone->GetAssetPointer(xmodel, destModels[i]->name)
						);
					}
				}

				ZoneBuffer::ClearPointer(&dest->worldModel);
			}

			WEAPON_SUBASSET(worldClipModel, xmodel, XModel);
			WEAPON_SUBASSET(rocketModel, xmodel, XModel);
			WEAPON_SUBASSET(knifeModel, xmodel, XModel);
			WEAPON_SUBASSET(worldKnifeModel, xmodel, XModel);

			WEAPON_SUBASSET(hudIcon, material, Material);
			WEAPON_SUBASSET(pickupIcon, material, Material);
			WEAPON_SUBASSET(ammoCounterIcon, material, Material);

			if (data->szAmmoName)
			{
				dest->szAmmoName = buf->write_str(data->szAmmoName);
			}

			if (data->szClipName)
			{
				dest->szClipName = buf->write_str(data->szClipName);
			}

			if (data->szSharedAmmoCapName)
			{
				dest->szSharedAmmoCapName = buf->write_str(data->szSharedAmmoCapName);
			}

			WEAPON_SUBASSET(AdsOverlayShader, material, Material);
			WEAPON_SUBASSET(AdsOverlayShaderLowRes, material, Material);
			WEAPON_SUBASSET(AdsOverlayShaderEMP, material, Material);
			WEAPON_SUBASSET(AdsOverlayShaderEMPLowRes, material, Material);

			WEAPON_SUBASSET(collisions, phys_collmap, PhysCollmap);

			WEAPON_SUBASSET(projectileModel, xmodel, XModel);

			WEAPON_SUBASSET(projExplosionEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(projDudEffect, fx, FxEffectDef);

			WEAPON_SUBASSET(projExplosionSound, sound, snd_alias_list_t);
			WEAPON_SUBASSET(projDudSound, sound, snd_alias_list_t);

			if (data->unkStruct124_1)
			{
				buf->align(3);
				buf->write(data->unkStruct124_1, 124);
				ZoneBuffer::ClearPointer(&dest->unkStruct124_1);
			}

			if (data->unkStruct124_2)
			{
				buf->align(3);
				buf->write(data->unkStruct124_2, 124);
				ZoneBuffer::ClearPointer(&dest->unkStruct124_2);
			}

			WEAPON_SUBASSET(projTrailEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(projBeaconEffect, fx, FxEffectDef);
			WEAPON_SUBASSET(projIgnitionEffect, fx, FxEffectDef);

			WEAPON_SUBASSET(projIgnitionSound, sound, snd_alias_list_t);

			if (data->accuracyGraphName[0])
			{
				dest->accuracyGraphName[0] = buf->write_str(data->accuracyGraphName[0]);
			}

			if (data->accuracyGraphKnots)
			{
				buf->align(3);
				buf->write(data->accuracyGraphKnots, complete->accuracyGraphKnotCount);
				ZoneBuffer::ClearPointer(&dest->accuracyGraphKnots);
			}

			if (data->accuracyGraphName[1])
			{
				dest->accuracyGraphName[1] = buf->write_str(data->accuracyGraphName[1]);
			}

			if (data->originalAccuracyGraphKnots)
			{
				buf->align(3);
				buf->write(data->originalAccuracyGraphKnots, complete->originalAccuracyGraphKnotCount);
				ZoneBuffer::ClearPointer(&dest->originalAccuracyGraphKnots);
			}

			if (data->szUseHintString)
			{
				dest->szUseHintString = buf->write_str(data->szUseHintString);
			}

			if (data->dropHintString)
			{
				dest->dropHintString = buf->write_str(data->dropHintString);
			}

			if (data->szScript)
			{
				dest->szScript = buf->write_str(data->szScript);
			}

			if (data->locationDamageMultipliers)
			{
				buf->align(3);
				buf->write(data->locationDamageMultipliers, 20);
				ZoneBuffer::ClearPointer(&dest->locationDamageMultipliers);
			}

			if (data->fireRumble)
			{
				dest->fireRumble = buf->write_str(data->fireRumble);
			}

			if (data->meleeImpactRumble)
			{
				dest->meleeImpactRumble = buf->write_str(data->meleeImpactRumble);
			}

			WEAPON_SUBASSET(tracer, tracer, TracerDef);

			WEAPON_SUBASSET(turretOverheatSound, sound, snd_alias_list_t);
			WEAPON_SUBASSET(turretOverheatEffect, fx, FxEffectDef);

			if (data->turretBarrelSpinRumble)
			{
				dest->turretBarrelSpinRumble = buf->write_str(data->turretBarrelSpinRumble);
			}

			WEAPON_SUBASSET(turretBarrelSpinMaxSnd, sound, snd_alias_list_t);

			for (int i = 0; i < 4; i++)
			{
				WEAPON_SUBASSET(turretBarrelSpinUpSnds[i], sound, snd_alias_list_t);
				WEAPON_SUBASSET(turretBarrelSpinDownSnds[i], sound, snd_alias_list_t);
			}

			WEAPON_SUBASSET(missileConeSoundAlias, sound, snd_alias_list_t);
			WEAPON_SUBASSET(missileConeSoundAliasAtBase, sound, snd_alias_list_t);

			dest->unknownTag = nullptr;
			dest->unknownXModel = nullptr;
			// WEAPON_SUBASSET(unknownXModel, xmodel, XModel);

			sizeof WeaponDef;
		}

		void IWeaponDef::write(IZone* zone, std::shared_ptr<ZoneBuffer>& buf)
		{
			sizeof(WeaponCompleteDef);

			auto data = this->m_asset;
			auto dest = buf->write(data);

			buf->push_stream(3);
			START_LOG_STREAM;

			dest->name = buf->write_str(this->name());

			if (data->WeaponDef)
			{
				buf->align(3);
				write_WeaponDef(zone, buf, data, data->WeaponDef);
				ZoneBuffer::ClearPointer(&dest->WeaponDef);
			}

			if (data->displayName)
			{
				dest->displayName = buf->write_str(data->displayName);
			}

			if (data->hideTags)
			{
				buf->align(1);
				buf->write(data->hideTags, 32);
				ZoneBuffer::ClearPointer(&dest->hideTags);
			}

#define WEAPON_ATTACHMENT(__field__,__max__) \
			if (data->__field__) \
			{ \
				buf->align(3); \
				auto destAttachments = buf->write(data->__field__,__max__); \
\
				for (auto i = 0u; i < __max__; i++) \
				{ \
					if (destAttachments && destAttachments[i]) \
					{ \
						destAttachments[i] = reinterpret_cast<AttachmentDef*>( \
							zone->GetAssetPointer(attachment, destAttachments[i]->szInternalName) \
						); \
					} \
				} \
 \
				ZoneBuffer::ClearPointer(&dest->__field__); \
			}

			WEAPON_ATTACHMENT(attachment1, 6);
			WEAPON_ATTACHMENT(attachment2, 3);
			WEAPON_ATTACHMENT(attachment3, 4);

			if (data->szXAnims)
			{
				buf->align(3);
				auto destStrings = buf->write(data->szXAnims, 42);

				for (auto i = 0u; i < 42; i++)
				{
					if (destStrings[i])
					{
						destStrings[i] = buf->write_str(destStrings[i]);
					}
				}

				ZoneBuffer::ClearPointer(&dest->szXAnims);
			}

			if (data->animOverrides)
			{
				buf->align(3);
				auto destAnimOverrides = buf->write(data->animOverrides, data->numAnimOverrides);

				for (auto i = 0u; i < data->numAnimOverrides; i++)
				{
					if (destAnimOverrides[i].overrideAnim)
					{
						destAnimOverrides[i].overrideAnim = buf->write_str(destAnimOverrides[i].overrideAnim);
					}

					if (destAnimOverrides[i].altmodeAnim)
					{
						destAnimOverrides[i].altmodeAnim = buf->write_str(destAnimOverrides[i].altmodeAnim);
					}
				}

				ZoneBuffer::ClearPointer(&dest->animOverrides);
			}

			if (data->soundOverrides)
			{
				buf->align(3);
				auto destSoundOverrides = buf->write(data->soundOverrides, data->numSoundOverrides);

				for (auto i = 0u; i < data->numSoundOverrides; i++)
				{
					if (destSoundOverrides[i].overrideSound)
					{
						auto ptr = -1;
						buf->align(3);
						buf->write(&ptr);
						buf->write_str(destSoundOverrides[i].overrideSound->name);
						ZoneBuffer::ClearPointer(&destSoundOverrides[i].overrideSound);
					}

					if (destSoundOverrides[i].altmodeSound)
					{
						auto ptr = -1;
						buf->align(3);
						buf->write(&ptr);
						buf->write_str(destSoundOverrides[i].altmodeSound->name);
						ZoneBuffer::ClearPointer(&destSoundOverrides[i].altmodeSound);
					}
				}

				ZoneBuffer::ClearPointer(&dest->soundOverrides);
			}

			if (data->fxOverrides)
			{
				buf->align(3);
				auto destFxOverrides = buf->write(data->fxOverrides, data->numFXOverrides);

				for (auto i = 0u; i < data->numFXOverrides; i++)
				{
					if (destFxOverrides[i].overrideFX)
					{
						destFxOverrides[i].overrideFX = reinterpret_cast<FxEffectDef*>(zone->GetAssetPointer(
							fx, destFxOverrides[i].overrideFX->name
						));
					}

					if (destFxOverrides[i].altmodeFX)
					{
						destFxOverrides[i].altmodeFX = reinterpret_cast<FxEffectDef*>(zone->GetAssetPointer(
							fx, destFxOverrides[i].altmodeFX->name
						));
					}
				}

				ZoneBuffer::ClearPointer(&dest->fxOverrides);
			}

			if (data->reloadOverrides)
			{
				buf->align(3);
				buf->write(data->reloadOverrides, data->numReloadStateTimerOverrides);
				ZoneBuffer::ClearPointer(&dest->reloadOverrides);
			}

			if (data->notetrackOverrides)
			{
				buf->align(3);
				auto destNoteTrackOverrides = buf->write(data->notetrackOverrides, data->numNotetrackOverrides);

				for (auto i = 0u; i < data->numNotetrackOverrides; i++)
				{
					if (destNoteTrackOverrides[i].notetrackSoundMapKeys)
					{
						buf->align(1);
						buf->write(destNoteTrackOverrides[i].notetrackSoundMapKeys, 24);
						ZoneBuffer::ClearPointer(&destNoteTrackOverrides[i].notetrackSoundMapKeys);
					}

					if (destNoteTrackOverrides[i].notetrackSoundMapValues)
					{
						buf->align(1);
						buf->write(destNoteTrackOverrides[i].notetrackSoundMapValues, 24);
						ZoneBuffer::ClearPointer(&destNoteTrackOverrides[i].notetrackSoundMapValues);
					}
				}

				ZoneBuffer::ClearPointer(&dest->notetrackOverrides);
			}

			if (data->altWeaponName)
			{
				dest->altWeaponName = buf->write_str(data->altWeaponName);
			}

			if (data->killIcon)
			{
				dest->killIcon = reinterpret_cast<Material*>(
					zone->GetAssetPointer(material, dest->killIcon->name)
				);
			}

			if (data->dpadIcon)
			{
				dest->dpadIcon = reinterpret_cast<Material*>(
					zone->GetAssetPointer(material, dest->dpadIcon->name)
				);
			}

			if (data->accuracyGraphKnots)
			{
				buf->align(3);
				buf->write(data->accuracyGraphKnots, data->accuracyGraphKnotCount);
				ZoneBuffer::ClearPointer(&dest->accuracyGraphKnots);
			}

			if (data->originalAccuracyGraphKnots)
			{
				buf->align(3);
				buf->write(data->originalAccuracyGraphKnots, data->originalAccuracyGraphKnotCount);
				ZoneBuffer::ClearPointer(&dest->originalAccuracyGraphKnots);
			}

			END_LOG_STREAM;
			buf->pop_stream();

			encrypt_data(dest, sizeof WeaponCompleteDef);
		}

#define WEAPON_DUMP_FIELD(__field__) \
	data[#__field__] = asset->__field__

#define WEAPON_DUMP_ASSET(__field__) \
	if (asset->__field__) \
	{ \
		data[#__field__] = asset->__field__->name; \
	} \
	else \
	{ \
		data[#__field__] = ""; \
	}

		Json IWeaponDef::dump_weapondef(WeaponDef* asset, const std::function<const char*(uint16_t)>& convertToString)
		{
			Json data;

			WEAPON_DUMP_FIELD(szInternalName);

			for (int i = 0; i < 16; i++)
			{
				if (asset->gunXModel && asset->gunXModel[i])
				{
					data["gunXModel"][i] = asset->gunXModel[i]->name;
				}
				else
				{
					data["gunXModel"][i] = "";
				}

				if (asset->worldModel && asset->worldModel[i])
				{
					data["worldXModel"][i] = asset->worldModel[i]->name;
				}
				else
				{
					data["worldXModel"][i] = "";
				}
			}

			for (int i = 0; i < 24; i++)
			{
				if (asset->notetrackSoundMapKeys && asset->notetrackSoundMapKeys[i])
				{
					data["notetrackSoundMapKeys"][i] = convertToString(asset->notetrackSoundMapKeys[i]);
				}
				else
				{
					data["notetrackSoundMapKeys"][i] = "";
				}

				if (asset->notetrackSoundMapValues && asset->notetrackSoundMapValues[i])
				{
					data["notetrackSoundMapValues"][i] = convertToString(asset->notetrackSoundMapValues[i]);
				}
				else
				{
					data["notetrackSoundMapValues"][i] = "";
				}
			}

			for (int i = 0; i < 16; i++)
			{
				if (asset->notetrackRumbleMapKeys && asset->notetrackRumbleMapKeys[i])
				{
					data["notetrackRumbleMapKeys"][i] = convertToString(asset->notetrackRumbleMapKeys[i]);
				}
				else
				{
					data["notetrackRumbleMapKeys"][i] = "";
				}

				if (asset->notetrackRumbleMapValues && asset->notetrackRumbleMapValues[i])
				{
					data["notetrackRumbleMapValues"][i] = convertToString(asset->notetrackRumbleMapValues[i]);
				}
				else
				{
					data["notetrackRumbleMapValues"][i] = "";
				}
			}

			WEAPON_DUMP_ASSET(handXModel);

			/*XModel **gunXModel);
			XModel *handXModel);
			const WEAPON_DUMP_FIELD(*szXAnimsR); 
			const WEAPON_DUMP_FIELD(*szXAnimsL);
			const WEAPON_DUMP_FIELD(szModeName);
			WEAPON_DUMP_FIELD(*notetrackSoundMapKeys);
			WEAPON_DUMP_FIELD(*notetrackSoundMapValues);
			WEAPON_DUMP_FIELD(*notetrackRumbleMapKeys);
			WEAPON_DUMP_FIELD(*notetrackRumbleMapValues);*/
			WEAPON_DUMP_FIELD(playerAnimType);
			WEAPON_DUMP_FIELD(weaponType);
			WEAPON_DUMP_FIELD(weaponClass);
			WEAPON_DUMP_FIELD(penetrateType);
			WEAPON_DUMP_FIELD(inventoryType);
			WEAPON_DUMP_FIELD(fireType);
			WEAPON_DUMP_FIELD(offhandClass);
			WEAPON_DUMP_FIELD(stance);

			for (int i = 0; i < 48; i++)
			{
				if (asset->sounds && asset->sounds[i])
				{
					data["sounds"][i] = asset->sounds[i]->name;
				}
				else
				{
					data["sounds"][i] = "";
				}
			}

			for (int i = 0; i < 31; i++)
			{
				if (asset->sndArray1 && asset->sndArray1[i])
				{
					data["sndArray1"][i] = asset->sndArray1[i]->name;
				}
				else
				{
					data["sndArray1"][i] = "";
				}

				if (asset->sndArray2 && asset->sndArray2[i])
				{
					data["sndArray2"][i] = asset->sndArray2[i]->name;
				}
				else
				{
					data["sndArray2"][i] = "";
				}
			}

			WEAPON_DUMP_ASSET(viewFlashEffect);
			WEAPON_DUMP_ASSET(worldFlashEffect);
			WEAPON_DUMP_ASSET(viewShellEjectEffect);
			WEAPON_DUMP_ASSET(worldShellEjectEffect);
			WEAPON_DUMP_ASSET(viewLastShotEjectEffect);
			WEAPON_DUMP_ASSET(worldLastShotEjectEffect);
			WEAPON_DUMP_ASSET(reticleCenter);
			WEAPON_DUMP_ASSET(reticleSide);
			WEAPON_DUMP_FIELD(iReticleCenterSize);
			WEAPON_DUMP_FIELD(iReticleSideSize);
			WEAPON_DUMP_FIELD(iReticleMinOfs);
			/*activeReticleType_t activeReticleType);
			WEAPON_DUMP_FIELD(vStandMove[3]);
			WEAPON_DUMP_FIELD(vStandRot[3]);
			WEAPON_DUMP_FIELD(vStrafeMove[3]);
			WEAPON_DUMP_FIELD(vStrafeRot[3]);
			WEAPON_DUMP_FIELD(vDuckedOfs[3]);
			WEAPON_DUMP_FIELD(vDuckedMove[3]);
			WEAPON_DUMP_FIELD(vDuckedRot[3]);
			WEAPON_DUMP_FIELD(vProneOfs[3]);
			WEAPON_DUMP_FIELD(vProneMove[3]);
			WEAPON_DUMP_FIELD(vProneRot[3]);*/
			WEAPON_DUMP_FIELD(fPosMoveRate);
			WEAPON_DUMP_FIELD(fPosProneMoveRate);
			WEAPON_DUMP_FIELD(fStandMoveMinSpeed);
			WEAPON_DUMP_FIELD(fDuckedMoveMinSpeed);
			WEAPON_DUMP_FIELD(fProneMoveMinSpeed);
			WEAPON_DUMP_FIELD(fPosRotRate);
			WEAPON_DUMP_FIELD(fPosProneRotRate);
			WEAPON_DUMP_FIELD(fStandRotMinSpeed);
			WEAPON_DUMP_FIELD(fDuckedRotMinSpeed);
			WEAPON_DUMP_FIELD(fProneRotMinSpeed);
			WEAPON_DUMP_ASSET(worldClipModel);
			WEAPON_DUMP_ASSET(rocketModel);
			WEAPON_DUMP_ASSET(knifeModel);
			WEAPON_DUMP_ASSET(worldKnifeModel);
			WEAPON_DUMP_ASSET(hudIcon);
			// weaponIconRatioType_t hudIconRatio);
			WEAPON_DUMP_ASSET(pickupIcon);
			// weaponIconRatioType_t pickupIconRatio);
			WEAPON_DUMP_ASSET(ammoCounterIcon);
			// weaponIconRatioType_t ammoCounterIconRatio);
			// ammoCounterClipType_t ammoCounterClip);*/
			WEAPON_DUMP_FIELD(iStartAmmo);
			WEAPON_DUMP_FIELD(szAmmoName);
			WEAPON_DUMP_FIELD(iAmmoIndex);
			WEAPON_DUMP_FIELD(szClipName);
			WEAPON_DUMP_FIELD(iClipIndex);
			WEAPON_DUMP_FIELD(iMaxAmmo);
			WEAPON_DUMP_FIELD(shotCount);
			WEAPON_DUMP_FIELD(szSharedAmmoCapName);
			WEAPON_DUMP_FIELD(iSharedAmmoCapIndex);
			WEAPON_DUMP_FIELD(iSharedAmmoCap);
			WEAPON_DUMP_FIELD(damage);
			WEAPON_DUMP_FIELD(playerDamage);
			WEAPON_DUMP_FIELD(iMeleeDamage);
			WEAPON_DUMP_FIELD(iDamageType);
			WEAPON_DUMP_FIELD(iFireDelay);
			WEAPON_DUMP_FIELD(iMeleeDelay);
			WEAPON_DUMP_FIELD(meleeChargeDelay);
			WEAPON_DUMP_FIELD(iDetonateDelay);
			WEAPON_DUMP_FIELD(iRechamberTime);
			WEAPON_DUMP_FIELD(iRechamberOneHanded);
			WEAPON_DUMP_FIELD(iRechamberBoltTime);
			WEAPON_DUMP_FIELD(iHoldFireTime);
			WEAPON_DUMP_FIELD(iDetonateTime);
			WEAPON_DUMP_FIELD(iMeleeTime);
			WEAPON_DUMP_FIELD(meleeChargeTime);
			WEAPON_DUMP_FIELD(iReloadTime);
			WEAPON_DUMP_FIELD(reloadShowRocketTime);
			WEAPON_DUMP_FIELD(iReloadEmptyTime);
			WEAPON_DUMP_FIELD(iReloadAddTime);
			WEAPON_DUMP_FIELD(iReloadStartTime);
			WEAPON_DUMP_FIELD(iReloadStartAddTime);
			WEAPON_DUMP_FIELD(iReloadEndTime);
			WEAPON_DUMP_FIELD(iDropTime);
			WEAPON_DUMP_FIELD(iRaiseTime);
			WEAPON_DUMP_FIELD(iAltDropTime);
			WEAPON_DUMP_FIELD(quickDropTime);
			WEAPON_DUMP_FIELD(quickRaiseTime);
			WEAPON_DUMP_FIELD(iFirstRaiseTime);
			WEAPON_DUMP_FIELD(iEmptyRaiseTime);
			WEAPON_DUMP_FIELD(iEmptyDropTime);
			WEAPON_DUMP_FIELD(sprintInTime);
			WEAPON_DUMP_FIELD(sprintLoopTime);
			WEAPON_DUMP_FIELD(sprintOutTime);
			WEAPON_DUMP_FIELD(stunnedTimeBegin);
			WEAPON_DUMP_FIELD(stunnedTimeLoop);
			WEAPON_DUMP_FIELD(stunnedTimeEnd);
			WEAPON_DUMP_FIELD(nightVisionWearTime);
			WEAPON_DUMP_FIELD(nightVisionWearTimeFadeOutEnd);
			WEAPON_DUMP_FIELD(nightVisionWearTimePowerUp);
			WEAPON_DUMP_FIELD(nightVisionRemoveTime);
			WEAPON_DUMP_FIELD(nightVisionRemoveTimePowerDown);
			WEAPON_DUMP_FIELD(nightVisionRemoveTimeFadeInStart);
			WEAPON_DUMP_FIELD(fuseTime);
			WEAPON_DUMP_FIELD(aifuseTime);
			WEAPON_DUMP_FIELD(autoAimRange);
			WEAPON_DUMP_FIELD(aimAssistRange);
			WEAPON_DUMP_FIELD(aimAssistRangeAds);
			WEAPON_DUMP_FIELD(aimPadding);
			WEAPON_DUMP_FIELD(enemyCrosshairRange);
			WEAPON_DUMP_FIELD(moveSpeedScale);
			WEAPON_DUMP_FIELD(adsMoveSpeedScale);
			WEAPON_DUMP_FIELD(sprintDurationScale);
			WEAPON_DUMP_FIELD(adsZoomInFrac);
			WEAPON_DUMP_FIELD(adsZoomOutFrac);
			WEAPON_DUMP_ASSET(AdsOverlayShader);
			WEAPON_DUMP_ASSET(AdsOverlayShaderLowRes);
			WEAPON_DUMP_ASSET(AdsOverlayShaderEMP);
			WEAPON_DUMP_ASSET(AdsOverlayShaderEMPLowRes);
			// weapOverlayReticle_t adsOverlayReticle);
			// weapOverlayInterface_t adsOverlayInterface);*/
			WEAPON_DUMP_FIELD(adsOverlayWidth);
			WEAPON_DUMP_FIELD(adsOverlayHeight);
			WEAPON_DUMP_FIELD(adsOverlayWidthSplitscreen);
			WEAPON_DUMP_FIELD(adsOverlayHeightSplitscreen);
			WEAPON_DUMP_FIELD(fAdsBobFactor);
			WEAPON_DUMP_FIELD(fAdsViewBobMult);
			WEAPON_DUMP_FIELD(fHipSpreadStandMin);
			WEAPON_DUMP_FIELD(fHipSpreadDuckedMin);
			WEAPON_DUMP_FIELD(fHipSpreadProneMin);
			WEAPON_DUMP_FIELD(hipSpreadStandMax);
			WEAPON_DUMP_FIELD(hipSpreadDuckedMax);
			WEAPON_DUMP_FIELD(hipSpreadProneMax);
			WEAPON_DUMP_FIELD(fHipSpreadDecayRate);
			WEAPON_DUMP_FIELD(fHipSpreadFireAdd);
			WEAPON_DUMP_FIELD(fHipSpreadTurnAdd);
			WEAPON_DUMP_FIELD(fHipSpreadMoveAdd);
			WEAPON_DUMP_FIELD(fHipSpreadDuckedDecay);
			WEAPON_DUMP_FIELD(fHipSpreadProneDecay);
			WEAPON_DUMP_FIELD(fHipReticleSidePos);
			WEAPON_DUMP_FIELD(fAdsIdleAmount);
			WEAPON_DUMP_FIELD(fHipIdleAmount);
			WEAPON_DUMP_FIELD(adsIdleSpeed);
			WEAPON_DUMP_FIELD(hipIdleSpeed);
			WEAPON_DUMP_FIELD(fIdleCrouchFactor);
			WEAPON_DUMP_FIELD(fIdleProneFactor);
			WEAPON_DUMP_FIELD(fGunMaxPitch);
			WEAPON_DUMP_FIELD(fGunMaxYaw);
			WEAPON_DUMP_FIELD(swayMaxAngle);
			WEAPON_DUMP_FIELD(swayLerpSpeed);
			WEAPON_DUMP_FIELD(swayPitchScale);
			WEAPON_DUMP_FIELD(swayYawScale);
			WEAPON_DUMP_FIELD(swayHorizScale);
			WEAPON_DUMP_FIELD(swayVertScale);
			WEAPON_DUMP_FIELD(swayShellShockScale);
			WEAPON_DUMP_FIELD(adsSwayMaxAngle);
			WEAPON_DUMP_FIELD(adsSwayLerpSpeed);
			WEAPON_DUMP_FIELD(adsSwayPitchScale);
			WEAPON_DUMP_FIELD(adsSwayYawScale);
			WEAPON_DUMP_FIELD(adsSwayHorizScale);
			WEAPON_DUMP_FIELD(adsSwayVertScale);
			WEAPON_DUMP_FIELD(adsViewErrorMin);
			WEAPON_DUMP_FIELD(adsViewErrorMax);
			// _BYTE idk[8]);
			WEAPON_DUMP_ASSET(collisions);
			WEAPON_DUMP_FIELD(dualWieldViewModelOffset);
			// weaponIconRatioType_t killIconRatio);
			WEAPON_DUMP_FIELD(iReloadAmmoAdd);
			WEAPON_DUMP_FIELD(iReloadStartAdd);
			WEAPON_DUMP_FIELD(iDropAmmoMin);
			WEAPON_DUMP_FIELD(ammoDropClipPercentMin);
			WEAPON_DUMP_FIELD(ammoDropClipPercentMax);
			WEAPON_DUMP_FIELD(iExplosionRadius);
			WEAPON_DUMP_FIELD(iExplosionRadiusMin);
			WEAPON_DUMP_FIELD(iExplosionInnerDamage);
			WEAPON_DUMP_FIELD(iExplosionOuterDamage);
			WEAPON_DUMP_FIELD(damageConeAngle);
			WEAPON_DUMP_FIELD(bulletExplDmgMult);
			WEAPON_DUMP_FIELD(bulletExplRadiusMult);
			WEAPON_DUMP_FIELD(iProjectileSpeed);
			WEAPON_DUMP_FIELD(iProjectileSpeedUp);
			WEAPON_DUMP_FIELD(iProjectileSpeedForward);
			WEAPON_DUMP_FIELD(iProjectileActivateDist);
			WEAPON_DUMP_FIELD(projLifetime);
			WEAPON_DUMP_FIELD(timeToAccelerate);
			WEAPON_DUMP_FIELD(projectileCurvature);
			WEAPON_DUMP_ASSET(projectileModel);
			// weapProjExplosion_t projExplosiveType);
			WEAPON_DUMP_ASSET(projExplosionEffect);
			WEAPON_DUMP_ASSET(projDudEffect);
			WEAPON_DUMP_ASSET(projExplosionSound);
			WEAPON_DUMP_ASSET(projDudSound);
			// WeapStickinessType stickiness);*/
			WEAPON_DUMP_FIELD(lowAmmoWarningThreshold);
			WEAPON_DUMP_FIELD(ricochetChance);
			// WEAPON_DUMP_FIELD(* parallelBounce);            //Refer to surfaceNames_t
			// WEAPON_DUMP_FIELD(* perpendicularBounce);         //Refer to surfaceNames_t
			// _BYTE gap52D[4]);
			// WEAPON_DUMP_FIELD(unkStruct124_1);
			// WEAPON_DUMP_FIELD(unkStruct124_2);
			WEAPON_DUMP_ASSET(projTrailEffect);
			WEAPON_DUMP_ASSET(projBeaconEffect);
			// WEAPON_DUMP_FIELD(vProjectileColor[3]);
			// guidedMissileType_t guidedMissileType);
			WEAPON_DUMP_FIELD(maxSteeringAccel);
			WEAPON_DUMP_FIELD(projIgnitionDelay);
			WEAPON_DUMP_ASSET(projIgnitionEffect);
			WEAPON_DUMP_ASSET(projIgnitionSound);
			WEAPON_DUMP_FIELD(fAdsAimPitch);
			WEAPON_DUMP_FIELD(fAdsCrosshairInFrac);
			WEAPON_DUMP_FIELD(fAdsCrosshairOutFrac);
			WEAPON_DUMP_FIELD(adsGunKickReducedKickBullets);
			WEAPON_DUMP_FIELD(adsGunKickReducedKickPercent);
			WEAPON_DUMP_FIELD(fAdsGunKickPitchMin);
			WEAPON_DUMP_FIELD(fAdsGunKickPitchMax);
			WEAPON_DUMP_FIELD(fAdsGunKickYawMin);
			WEAPON_DUMP_FIELD(fAdsGunKickYawMax);
			WEAPON_DUMP_FIELD(fAdsGunKickAccel);
			WEAPON_DUMP_FIELD(fAdsGunKickSpeedMax);
			WEAPON_DUMP_FIELD(fAdsGunKickSpeedDecay);
			WEAPON_DUMP_FIELD(fAdsGunKickStaticDecay);
			WEAPON_DUMP_FIELD(fAdsViewKickPitchMin);
			WEAPON_DUMP_FIELD(fAdsViewKickPitchMax);
			WEAPON_DUMP_FIELD(fAdsViewKickYawMin);
			WEAPON_DUMP_FIELD(fAdsViewKickYawMax);
			WEAPON_DUMP_FIELD(fAdsViewScatterMin);
			WEAPON_DUMP_FIELD(fAdsViewScatterMax);
			WEAPON_DUMP_FIELD(fAdsSpread);
			WEAPON_DUMP_FIELD(hipGunKickReducedKickBullets);
			WEAPON_DUMP_FIELD(hipGunKickReducedKickPercent);
			WEAPON_DUMP_FIELD(fHipGunKickPitchMin);
			WEAPON_DUMP_FIELD(fHipGunKickPitchMax);
			WEAPON_DUMP_FIELD(fHipGunKickYawMin);
			WEAPON_DUMP_FIELD(fHipGunKickYawMax);
			WEAPON_DUMP_FIELD(fHipGunKickAccel);
			WEAPON_DUMP_FIELD(fHipGunKickSpeedMax);
			WEAPON_DUMP_FIELD(fHipGunKickSpeedDecay);
			WEAPON_DUMP_FIELD(fHipGunKickStaticDecay);
			WEAPON_DUMP_FIELD(fHipViewKickPitchMin);
			WEAPON_DUMP_FIELD(fHipViewKickPitchMax);
			WEAPON_DUMP_FIELD(fHipViewKickYawMin);
			WEAPON_DUMP_FIELD(fHipViewKickYawMax);
			WEAPON_DUMP_FIELD(fHipViewScatterMin);
			WEAPON_DUMP_FIELD(fHipViewScatterMax);
			WEAPON_DUMP_FIELD(fightDist);
			WEAPON_DUMP_FIELD(maxDist);
			WEAPON_DUMP_FIELD(iPositionReloadTransTime);
			WEAPON_DUMP_FIELD(leftArc);
			WEAPON_DUMP_FIELD(rightArc);
			WEAPON_DUMP_FIELD(topArc);
			WEAPON_DUMP_FIELD(bottomArc);
			WEAPON_DUMP_FIELD(accuracy);
			WEAPON_DUMP_FIELD(aiSpread);
			WEAPON_DUMP_FIELD(playerSpread);
			WEAPON_DUMP_FIELD(minVertTurnSpeed);
			WEAPON_DUMP_FIELD(minHorTurnSpeed);
			WEAPON_DUMP_FIELD(maxVertTurnSpeed);
			WEAPON_DUMP_FIELD(maxHorTurnSpeed);
			WEAPON_DUMP_FIELD(pitchConvergenceTime);
			WEAPON_DUMP_FIELD(yawConvergenceTime);
			WEAPON_DUMP_FIELD(suppressTime);
			WEAPON_DUMP_FIELD(maxRange);
			WEAPON_DUMP_FIELD(fAnimHorRotateInc);
			WEAPON_DUMP_FIELD(fPlayerPositionDist);
			WEAPON_DUMP_FIELD(szUseHintString);
			WEAPON_DUMP_FIELD(dropHintString);
			WEAPON_DUMP_FIELD(iUseHintStringIndex);
			WEAPON_DUMP_FIELD(dropHintStringIndex);
			WEAPON_DUMP_FIELD(horizViewJitter);
			WEAPON_DUMP_FIELD(vertViewJitter);
			WEAPON_DUMP_FIELD(scanSpeed);
			WEAPON_DUMP_FIELD(scanAccel);
			WEAPON_DUMP_FIELD(scanPauseTime);
			WEAPON_DUMP_FIELD(szScript);
			// WEAPON_DUMP_FIELD(fOOPosAnimLength[2]);
			WEAPON_DUMP_FIELD(minDamage);
			WEAPON_DUMP_FIELD(minPlayerDamage);
			WEAPON_DUMP_FIELD(maxDamageRange);
			WEAPON_DUMP_FIELD(minDamageRange);
			WEAPON_DUMP_FIELD(destabilizationRateTime);
			WEAPON_DUMP_FIELD(destabilizationCurvatureMax);
			WEAPON_DUMP_FIELD(destabilizeDistance);
			// float* locationDamageMultipliers);
			WEAPON_DUMP_FIELD(fireRumble);
			WEAPON_DUMP_FIELD(meleeImpactRumble);
			WEAPON_DUMP_ASSET(tracer);
			WEAPON_DUMP_ASSET(turretOverheatSound);
			WEAPON_DUMP_ASSET(turretOverheatEffect);
			WEAPON_DUMP_FIELD(turretBarrelSpinRumble);
			WEAPON_DUMP_FIELD(turretBarrelSpinUpTime);
			WEAPON_DUMP_FIELD(turretBarrelSpinDownTime);
			WEAPON_DUMP_FIELD(turretBarrelSpinSpeed);
			WEAPON_DUMP_ASSET(turretBarrelSpinMaxSnd);

			for (int i = 0; i < 4; i++)
			{
				if (asset->turretBarrelSpinUpSnds && asset->turretBarrelSpinUpSnds[i])
				{
					data["turretBarrelSpinUpSnds"][i] = asset->turretBarrelSpinUpSnds[i]->name;
				}
				else
				{
					data["turretBarrelSpinUpSnds"][i] = "";
				}

				if (asset->turretBarrelSpinDownSnds && asset->turretBarrelSpinDownSnds[i])
				{
					data["turretBarrelSpinDownSnds"][i] = asset->turretBarrelSpinDownSnds[i]->name;
				}
				else
				{
					data["turretBarrelSpinDownSnds"][i] = "";
				}
			}

			WEAPON_DUMP_ASSET(missileConeSoundAlias);
			WEAPON_DUMP_ASSET(missileConeSoundAliasAtBase);
			WEAPON_DUMP_FIELD(missileConeSoundRadiusAtTop);
			WEAPON_DUMP_FIELD(missileConeSoundRadiusAtBase);
			WEAPON_DUMP_FIELD(missileConeSoundHeight);
			WEAPON_DUMP_FIELD(missileConeSoundOriginOffset);
			WEAPON_DUMP_FIELD(missileConeSoundVolumescaleAtCore);
			WEAPON_DUMP_FIELD(missileConeSoundVolumescaleAtEdge);
			WEAPON_DUMP_FIELD(missileConeSoundVolumescaleCoreSize);
			WEAPON_DUMP_FIELD(missileConeSoundPitchAtTop);
			WEAPON_DUMP_FIELD(missileConeSoundPitchAtBottom);
			WEAPON_DUMP_FIELD(missileConeSoundPitchTopSize);
			WEAPON_DUMP_FIELD(missileConeSoundPitchBottomSize);
			WEAPON_DUMP_FIELD(missileConeSoundCrossfadeTopSize);
			WEAPON_DUMP_FIELD(missileConeSoundCrossfadeBottomSize);
			WEAPON_DUMP_FIELD(shareAmmo);
			WEAPON_DUMP_FIELD(lockonSupported);
			WEAPON_DUMP_FIELD(requireLockonToFire);
			WEAPON_DUMP_FIELD(bigExplosion);
			WEAPON_DUMP_FIELD(noAdsWhenMagEmpty);
			WEAPON_DUMP_FIELD(avoidDropCleanup);
			WEAPON_DUMP_FIELD(inheritsPerks);
			WEAPON_DUMP_FIELD(crosshairColorChange);
			WEAPON_DUMP_FIELD(rifleBullet);
			WEAPON_DUMP_FIELD(armorPiercing);
			WEAPON_DUMP_FIELD(boltAction);
			WEAPON_DUMP_FIELD(aimDownSight);
			WEAPON_DUMP_FIELD(rechamberWhileAds);
			WEAPON_DUMP_FIELD(bBulletExplosiveDamage);
			WEAPON_DUMP_FIELD(cookOffHold);
			WEAPON_DUMP_FIELD(clipOnly);
			WEAPON_DUMP_FIELD(noAmmoPickup);
			WEAPON_DUMP_FIELD(adsFire);
			WEAPON_DUMP_FIELD(cancelAutoHolsterWhenEmpty);
			WEAPON_DUMP_FIELD(disableSwitchToWhenEmpty);
			WEAPON_DUMP_FIELD(suppressAmmoReserveDisplay);
			WEAPON_DUMP_FIELD(laserSightDuringNightvision);
			WEAPON_DUMP_FIELD(markableViewmodel);
			WEAPON_DUMP_FIELD(noDualWield);
			WEAPON_DUMP_FIELD(flipKillIcon);
			WEAPON_DUMP_FIELD(noPartialReload);
			WEAPON_DUMP_FIELD(segmentedReload);
			WEAPON_DUMP_FIELD(blocksProne);
			WEAPON_DUMP_FIELD(silenced);
			WEAPON_DUMP_FIELD(isRollingGrenade);
			WEAPON_DUMP_FIELD(projExplosionEffectForceNormalUp);
			WEAPON_DUMP_FIELD(projImpactExplode);
			WEAPON_DUMP_FIELD(stickToPlayers);
			WEAPON_DUMP_FIELD(hasDetonator);
			WEAPON_DUMP_FIELD(disableFiring);
			WEAPON_DUMP_FIELD(timedDetonation);
			WEAPON_DUMP_FIELD(rotate);
			WEAPON_DUMP_FIELD(holdButtonToThrow);
			WEAPON_DUMP_FIELD(freezeMovementWhenFiring);
			WEAPON_DUMP_FIELD(thermalScope);
			WEAPON_DUMP_FIELD(altModeSameWeapon);
			WEAPON_DUMP_FIELD(turretBarrelSpinEnabled);
			WEAPON_DUMP_FIELD(missileConeSoundEnabled);
			WEAPON_DUMP_FIELD(missileConeSoundPitchshiftEnabled);
			WEAPON_DUMP_FIELD(missileConeSoundCrossfadeEnabled);
			WEAPON_DUMP_FIELD(offhandHoldIsCancelable);

			return data;
		}

		Json IWeaponDef::dump_complete(WeaponCompleteDef* asset,
		                               const std::function<const char*(uint16_t)>& convertToString)
		{
			Json data;
			data["baseAsset"] = asset->name;
			data["WeaponDef"] = dump_weapondef(asset->WeaponDef, convertToString);

			WEAPON_DUMP_FIELD(name);
			WEAPON_DUMP_FIELD(displayName);

			for (int i = 0; i < 32; i++)
			{
				if (asset->hideTags && asset->hideTags[i])
				{
					data["hideTags"][i] = convertToString(asset->hideTags[i]);
				}
				else
				{
					data["hideTags"][i] = "";
				}
			}

			for (int i = 0; i < 6; i++)
			{
				if (asset->attachment1 && asset->attachment1[i])
				{
					data["attachment1"][i] = asset->attachment1[i]->szInternalName;
				}
				else
				{
					data["attachment1"][i] = "";
				}

				if (i >= 4) continue;

				if (asset->attachment3 && asset->attachment3[i])
				{
					data["attachment3"][i] = asset->attachment3[i]->szInternalName;
				}
				else
				{
					data["attachment3"][i] = "";
				}

				if (i >= 3) continue;

				if (asset->attachment2 && asset->attachment2[i])
				{
					data["attachment2"][i] = asset->attachment2[i]->szInternalName;
				}
				else
				{
					data["attachment2"][i] = "";
				}
			}

			for (int i = 0; i < 42; i++)
			{
				if (asset->szXAnims && asset->szXAnims[i])
				{
					data["szXAnims"][i] = asset->szXAnims[i];
				}
				else
				{
					data["szXAnims"][i] = "";
				}
			}

			for (int i = 0; i < asset->numAnimOverrides; i++)
			{
				data["animOverrides"][i]["altmodeAnim"] = (asset->animOverrides[i].altmodeAnim)
					                                          ? asset->animOverrides[i].altmodeAnim
					                                          : "";
				data["animOverrides"][i]["overrideAnim"] = (asset->animOverrides[i].overrideAnim)
					                                           ? asset->animOverrides[i].overrideAnim
					                                           : "";
				data["animOverrides"][i]["attachment1"] = asset->animOverrides[i].attachment1;
				data["animOverrides"][i]["attachment2"] = asset->animOverrides[i].attachment2;
				data["animOverrides"][i]["altTime"] = asset->animOverrides[i].altTime;
				data["animOverrides"][i]["animTime"] = asset->animOverrides[i].animTime;
				data["animOverrides"][i]["animTreeType"] = asset->animOverrides[i].animTreeType;
			}

			for (int i = 0; i < asset->numSoundOverrides; i++)
			{
				data["soundOverrides"][i]["altmodeSound"] = (asset->soundOverrides[i].altmodeSound)
					                                            ? asset->soundOverrides[i].altmodeSound->name
					                                            : "";
				data["soundOverrides"][i]["attachment1"] = asset->soundOverrides[i].attachment1;
				data["soundOverrides"][i]["attachment2"] = asset->soundOverrides[i].attachment2;
				data["soundOverrides"][i]["overrideSound"] = (asset->soundOverrides[i].overrideSound)
					                                             ? asset->soundOverrides[i].overrideSound->name
					                                             : "";
				data["soundOverrides"][i]["soundType"] = asset->soundOverrides[i].soundType;
			}

			for (int i = 0; i < asset->numFXOverrides; i++)
			{
				data["fxOverrides"][i]["altmodeFX"] = (asset->fxOverrides[i].altmodeFX)
					                                      ? asset->fxOverrides[i].altmodeFX->name
					                                      : "";
				data["fxOverrides"][i]["attachment1"] = asset->fxOverrides[i].attachment1;
				data["fxOverrides"][i]["attachment2"] = asset->fxOverrides[i].attachment2;
				data["fxOverrides"][i]["fxType"] = asset->fxOverrides[i].fxType;
				data["fxOverrides"][i]["overrideFX"] = (asset->fxOverrides[i].overrideFX)
					                                       ? asset->fxOverrides[i].overrideFX->name
					                                       : "";
			}

			for (int i = 0; i < asset->numReloadStateTimerOverrides; i++)
			{
				data["reloadOverrides"][i]["attachment"] = asset->reloadOverrides[i].attachment;
				data["reloadOverrides"][i]["reloadAddTime"] = asset->reloadOverrides[i].reloadAddTime;
				data["reloadOverrides"][i]["reloadEmptyAddTime"] = asset->reloadOverrides[i].reloadEmptyAddTime;
			}

			for (int i = 0; i < asset->numNotetrackOverrides; i++)
			{
				data["notetrackOverrides"][i]["attachment"] = asset->notetrackOverrides[i].attachment;

				for (int j = 0; j < 24; j++)
				{
					data["notetrackOverrides"][i]["notetrackSoundMapKeys"][j] = convertToString(
						asset->notetrackOverrides[i].notetrackSoundMapKeys[j]);
					data["notetrackOverrides"][i]["notetrackSoundMapValues"][j] = convertToString(
						asset->notetrackOverrides[i].notetrackSoundMapValues[j]);
				}
			}

			WEAPON_DUMP_FIELD(numAnimOverrides);
			WEAPON_DUMP_FIELD(numSoundOverrides);
			WEAPON_DUMP_FIELD(numFXOverrides);
			WEAPON_DUMP_FIELD(numReloadStateTimerOverrides);
			WEAPON_DUMP_FIELD(numNotetrackOverrides);
			WEAPON_DUMP_FIELD(iAdsTransInTime);
			WEAPON_DUMP_FIELD(iAdsTransOutTime);
			WEAPON_DUMP_FIELD(iClipSize);
			WEAPON_DUMP_FIELD(impactType);
			WEAPON_DUMP_FIELD(iFireTime);
			WEAPON_DUMP_FIELD(dpadIconRatio);
			WEAPON_DUMP_FIELD(iPenetrateMultiplier);
			WEAPON_DUMP_FIELD(fAdsViewKickCenterSpeed);
			WEAPON_DUMP_FIELD(fHipViewKickCenterSpeed);
			WEAPON_DUMP_FIELD(altWeaponName);
			WEAPON_DUMP_FIELD(altWeaponIndex);
			WEAPON_DUMP_FIELD(iAltRaiseTime);
			WEAPON_DUMP_FIELD(iAltRaiseTimeAkimbo);
			WEAPON_DUMP_ASSET(killIcon);
			WEAPON_DUMP_ASSET(dpadIcon);
			WEAPON_DUMP_FIELD(iDropAmmoMax);
			WEAPON_DUMP_FIELD(motionTracker);
			WEAPON_DUMP_FIELD(enhanced);
			WEAPON_DUMP_FIELD(dpadIconShowsAmmo);

			return data;
		}

		void IWeaponDef::dump(WeaponCompleteDef* asset, const std::function<const char*(uint16_t)>& convertToString)
		{
			std::string path = "weapons/mp/"s + asset->name;
			std::string json = dump_complete(asset, convertToString).dump(4);

			auto file = FileSystem::FileOpen(path, "w"s);
			fwrite(json.data(), json.size(), 1, file);
			FileSystem::FileClose(file);
		}
	}
}

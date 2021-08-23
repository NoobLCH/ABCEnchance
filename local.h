#pragma once
#include "cvardef.h"

typedef struct
{
	void		(*R_Blood)					(float* org, float* dir, int pcolor, int speed);
	void		(*R_BloodSprite)			(float* org, int colorindex, int modelIndex, int modelIndex2, float size);
	void		(*R_Explosion)				(float* pos, int model, float scale, float framerate, int flags);
	void		(*R_RicochetSprite)			(float* pos, struct model_s* pmodel, float duration, float scale);
	void		(*R_SparkEffect)			(float* pos, int count, int velocityMin, int velocityMax);
	void		(*R_BloodStream)			(float* org, float* dir, int pcolor, int speed);
	float* (*GetClientColor)			(int clientIndex);

	void		(*VectorScale)				(float* pucnangle1, float scale, float* pucnangle2);
	void		(*R_NewMap)					(void);
	void(__fastcall* R_CalcDamageDirection)(void* pthis, int dummy, int x, float y, float z);

	TEMPENTITY* (*CL_TempEntAllocHigh)		(float* org, struct model_s* model);
	TEMPENTITY* (*CL_TempEntAlloc)			(float* org, struct model_s* model);
}cl_refHookfunc_t;

typedef struct
{
	cvar_t* pBloodSpriteSpeed;
	cvar_t* pBloodSpriteNumber;
	cvar_t* pExpSmokeNumber;
	cvar_t* pExpSmokeSpeed;
	cvar_t* pRicochetNumber;

	cvar_t* pPlayerTitle;

	cvar_t* pDynamicCrossHair;
	cvar_t* pDynamicCrossHairAH;
	cvar_t* pDynamicCrossHairL;
	cvar_t* pDynamicCrossHairW;
	cvar_t* pDynamicCrossHairO;
	cvar_t* pDynamicCrossHairM;
	cvar_t* pDynamicCrossHairA;
	cvar_t* pDynamicCrossHairCR;
	cvar_t* pDynamicCrossHairCG;
	cvar_t* pDynamicCrossHairCB;
	cvar_t* pDynamicCrossHairOTD;
	cvar_t* pDynamicCrossHairOTDW;
	cvar_t* pDynamicCrossHairT;
	cvar_t* pDynamicCrossHairD;

	cvar_t* pDynamicHUD;

	cvar_t* pCurDebug;

}cl_cvars_t;

extern cl_refHookfunc_t gHookFuncs;
extern cl_cvars_t gCVars;
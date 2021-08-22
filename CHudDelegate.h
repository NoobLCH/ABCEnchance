#pragma once
#define MAX_SPRITE_NAME_LENGTH	24
#include "baseammo.h"
#include "basemenu.h"

class CHudDelegate
{
public:
	virtual void HUD_Init(void);
	virtual void HUD_VidInit(void);
	virtual void HUD_Draw(float flTime);
	virtual void HUD_Reset(void);
	virtual void HUD_UpdateClientData(client_data_t* cdata, float time);

	~CHudDelegate();			// destructor, frees allocated memory

	bool m_fPlayerDead;
	bool m_fPlayerSpawned;

	client_sprite_t* m_pSpriteList;
	int m_iRes;
	int m_iSpriteCountAllRes;

	HSPRITE GetSprite(int index){ return (index < 0) ? 0 : m_rghSprites[index];}
	wrect_t& GetSpriteRect(int index){ return m_rgrcRects[index]; }
	int GetSpriteIndex(const char* SpriteName);	// gets a sprite index, for use in the m_rghSprites[] array

private:
	// the memory for these arrays are allocated in the first call to CHud::VidInit(), when the hud.txt and associated sprites are loaded.
	// freed in ~CHud()
	int	 m_iSpriteCount;
	HSPRITE* m_rghSprites;	/*[HUD_SPRITE_COUNT]*/			// the sprites loaded from hud.txt
	wrect_t* m_rgrcRects;	/*[HUD_SPRITE_COUNT]*/
	char* m_rgszSpriteNames; /*[HUD_SPRITE_COUNT][MAX_SPRITE_NAME_LENGTH]*/
};
extern CHudDelegate* gHudDelegate;
typedef struct {
	CHudMenu* m_Menu;
	CHudAmmo* m_Ammo;
} cl_hookedHud;
extern cl_hookedHud gHookHud;
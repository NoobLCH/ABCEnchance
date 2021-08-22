#define DMG_IMAGE_LIFE		2	// seconds that image is up

#define DMG_IMAGE_POISON	0
#define DMG_IMAGE_ACID		1
#define DMG_IMAGE_COLD		2
#define DMG_IMAGE_DROWN		3
#define DMG_IMAGE_BURN		4
#define DMG_IMAGE_NERVE		5
#define DMG_IMAGE_RAD		6
#define DMG_IMAGE_SHOCK		7
//tf defines
#define DMG_IMAGE_CALTROP	8
#define DMG_IMAGE_TRANQ		9
#define DMG_IMAGE_CONCUSS	10
#define DMG_IMAGE_HALLUC	11
#define NUM_DMG_TYPES		12
// instant damage

#define DMG_GENERIC			0			// generic damage was done
#define DMG_CRUSH			(1 << 0)	// crushed by falling or moving object
#define DMG_BULLET			(1 << 1)	// shot
#define DMG_SLASH			(1 << 2)	// cut, clawed, stabbed
#define DMG_BURN			(1 << 3)	// heat burned
#define DMG_FREEZE			(1 << 4)	// frozen
#define DMG_FALL			(1 << 5)	// fell too far
#define DMG_BLAST			(1 << 6)	// explosive blast damage
#define DMG_CLUB			(1 << 7)	// crowbar, punch, headbutt
#define DMG_SHOCK			(1 << 8)	// electric shock
#define DMG_SONIC			(1 << 9)	// sound pulse shockwave
#define DMG_ENERGYBEAM		(1 << 10)	// laser or other high energy beam 
#define DMG_NEVERGIB		(1 << 12)	// with this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB		(1 << 13)	// with this bit OR'd in, any damage type can be made to gib victims upon death.


// time-based damage
//mask off TF-specific stuff too
#define DMG_TIMEBASED		(~(0xff003fff))	// mask for time-based damage


#define DMG_DROWN			(1 << 14)	// Drowning
#define DMG_FIRSTTIMEBASED  DMG_DROWN

#define DMG_PARALYZE		(1 << 15)	// slows affected creature down
#define DMG_NERVEGAS		(1 << 16)	// nerve toxins, very bad
#define DMG_POISON			(1 << 17)	// blood poisioning
#define DMG_RADIATION		(1 << 18)	// radiation exposure
#define DMG_DROWNRECOVER	(1 << 19)	// drowning recovery
#define DMG_ACID			(1 << 20)	// toxic chemicals or acid burns
#define DMG_SLOWBURN		(1 << 21)	// in an oven
#define DMG_SLOWFREEZE		(1 << 22)	// in a subzero freezer
#define DMG_MORTAR			(1 << 23)	// Hit by air raid (done to distinguish grenade from mortar)

//TF ADDITIONS
#define DMG_IGNITE			(1 << 24)	// Players hit by this begin to burn
#define DMG_RADIUS_MAX		(1 << 25)	// Radius damage with this flag doesn't decrease over distance
#define DMG_RADIUS_QUAKE	(1 << 26)	// Radius damage is done like Quake. 1/2 damage at 1/2 radius.
#define DMG_IGNOREARMOR		(1 << 27)	// Damage ignores target's armor
#define DMG_AIMED			(1 << 28)   // Does Hit location damage
#define DMG_WALLPIERCING	(1 << 29)	// Blast Damages ents through walls

#define DMG_CALTROP				(1<<30)
#define DMG_HALLUC				(1<<31)

// TF Healing Additions for TakeHealth
#define DMG_IGNORE_MAXHEALTH	DMG_IGNITE
// TF Redefines since we never use the originals
#define DMG_NAIL				DMG_SLASH
#define DMG_NOT_SELF			DMG_FREEZE


#define DMG_TRANQ				DMG_MORTAR
#define DMG_CONCUSS				DMG_SONIC


#define FADE_TIME 100

#define PAIN_INDICAROT_TIME 5

typedef struct
{
	float fExpire;
	float fBaseline;
	int	x, y;
} DAMAGE_IMAGE;

class CHudArmorHealth
{
public:
	virtual void Init(void);
	virtual int Draw(float flTime);
	virtual void Reset(void);
	virtual int VidInit(void);
	int m_iHealth;
	int m_iBat;
	int m_iFlags;
	float m_fFade;
	int m_HUD_dmg_bio;
	int m_HUD_cross;

	DAMAGE_IMAGE m_dmg[NUM_DMG_TYPES];
	HSPRITE m_hSprite;
	HSPRITE m_hDamage;

	int iHealthIcon;
	int iArmorIconNull;
	int iArmorIconFull;
	int iPainIndicator;

	float flPainIndicatorKeepTime = 0;
	vec2_t vecPainIndicatorA;
	vec2_t vecPainIndicatorB;
	vec2_t vecPainIndicatorC;
	vec2_t vecPainIndicatorD;

	float BackGroundAlpha = 128;
	float StartX = 48;
	float IconSize = 0.5;
	float TextWidth = 1;
	float TextHeight = 0.667;
	float BarLength = 2;
	float BarWidth = 0.334;
	float ElementGap = 0.2;
	float BackGroundY = 0.95;
	float BackGroundLength = 3;

	Color HealthIconColor;
	Color HealthBarColor;
	Color HealthTextColor;
	Color ArmorIconColor;
	Color ArmorBarColor;
	Color ArmorTextColor;
	Color PainIndicatorColor;
	Color PainIndicatorColorA;
	Color BitDamageColor;
	Color BackGroundColor;

	vgui::HFont HUDFont;
	vgui::HFont HUDBigFont;

	int	m_bitsDamage;
	int m_takeDamage;
	int m_takeArmor;
	vec3_t vecDamageFrom;

	int DrawPain(float fTime);
	int DrawDamage(float fTime);
	void CalcDamageDirection(vec3_t vecFrom);
	void UpdateTiles(float fTime, long bits);
};
extern CHudArmorHealth m_HudArmorHealth;
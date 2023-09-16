#ifndef CNETEASEMUSIC_H
#define CNETEASEMUSIC_H

#include <string>
#include <list>
#include <memory>

#include "NeteaseApi.h"
#include "soundengine.h"
#include "liblrc/lrc_parser.h"

#include <vgui_controls/Frame.h>
#include "IViewportPanel.h"

namespace vgui {
	class Label;
	class ImagePanel;
	class ImageClipPanel;
	class Button;
}
class CNeteasePanel;

class CQRLoginPanel : public vgui::Frame {
public:
	DECLARE_CLASS_SIMPLE(CQRLoginPanel, vgui::Frame);
	CQRLoginPanel(vgui::Panel* parent, char* name);
	void Login();
	void ResetText();
	void SendMyInfo();
private:
	vgui::ImagePanel* m_pQRImagePanel = nullptr;
	vgui::Label* m_pNotice = nullptr;
	CNeteasePanel* m_pMusicPanel = nullptr;
};

class CNeteasePanel : public vgui::EditablePanel, public IViewportPanel{
public:
	DECLARE_CLASS_SIMPLE(CNeteasePanel, vgui::EditablePanel);

	CNeteasePanel();
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme) override;
	// IViewportPanel overrides
	virtual const char* GetName() override;
	virtual void Reset() override;
	virtual void ShowPanel(bool state) override;
	virtual bool IsVisible() override;
	virtual vgui::VPANEL GetVPanel() override;
	virtual void SetParent(vgui::VPANEL parent) override;

	//force think
	void Think();
	void PlayMusic(netease::neteaseid_t id);
	void PlayList(netease::neteaseid_t id);
	void PlayRecommendMusic();
	void PlayFM();

	void StopMusic();
	void NextMusic();
	void QRLogin();
	void GetMyInfo();

	void SetVolume(float vol);
	void Search(const char* keyword, netease::SearchType type);

	template<typename... Args>
	static void PrintF(const char* str, bool dev, const Args&& ...args);

	//NEVER KEEP THEM IN A VARLUE!
	netease::CMusic* GetNowPlaying();
	netease::CMy* GetNowUser();
private:
	enum class PLAYSTATE {
		NORMAL,
		FM,
		DJ
	};
	void SetPlayerState(PLAYSTATE state);
	void PlayListMusic();
	void RenewFM();

	void AddToList(netease::neteaseid_t id);
	void ChangeMusic();

	cvar_t* m_pQuality = nullptr;
	cvar_t* m_pVolume = nullptr;
	cvar_t* m_pSearchCount = nullptr;

	vgui::Label* m_pMusicNameLable = nullptr;
	vgui::Label* m_pArtistNameLable = nullptr;
	vgui::Label* m_pLyricLable = nullptr;
	vgui::Label* m_pTranslatedLyricLable = nullptr;

	vgui::Label* m_pTimeLable = nullptr;
	vgui::Label* m_pMaxTimeLable = nullptr;
	vgui::ImageClipPanel* m_pProgressLable = nullptr;
	vgui::ImagePanel* m_pProgressBackgroundPanel = nullptr;

	vgui::ImagePanel* m_pAlbumPanel = nullptr;

	CQRLoginPanel* m_pLoginPanel = nullptr;

	FMOD_SOUND* m_pSound = nullptr;
	FMOD_CHANNEL* m_pChannel = nullptr;

	std::shared_ptr<netease::CMusic> m_pPlaying = nullptr;
	std::unique_ptr<lrc::Lyrics> m_pLyric = nullptr;
	std::unique_ptr<lrc::Lyrics> m_pTransLyric = nullptr;
	std::shared_ptr<netease::CMy> m_pLogined = nullptr;

	PLAYSTATE m_pNowState;
	std::list<netease::neteaseid_t> m_aryPlayList;
};
#endif
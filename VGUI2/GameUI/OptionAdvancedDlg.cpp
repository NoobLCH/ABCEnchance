#include <algorithm>

#include "interface.h"
#include "IFileSystem.h"

#include "vgui_controls/PropertySheet.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/UrlLabel.h"
#include "vgui_controls/Slider.h"
#include "vgui_controls/ListPanel.h"
#include "vgui_controls/TextEntry.h"
#include "vgui_controls/Button.h"
#include "vgui_controls/CrossHairDisplay.h"
#include "vgui_controls/CvarLabelSlider.h"
#include "vgui_controls/CvarToggleCheckButton.h"
#include "vgui_controls/CvarComboBox.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/FileOpenDialog.h"
#include "vgui_controls/Menu.h"
#include "vgui_controls/MenuItem.h"
#include "vgui_controls/MemoryBitmap.h"
#include "vgui_controls/GroupBox.h"

#include "FreeImage.h"
#include "wadlib/wadfile.h"

#include <GaussianBlurPanel.h>
#include <ModelViewPanel.h>

#include "OptionAdvancedDlg.h"
#include <filesystem>

extern const char* CVAR_GET_STRING(const char* x);
extern void CVAR_SET_STRING(const char* x, const char* v);
extern float CVAR_GET_FLOAT(const char* x);
extern size_t ScreenWidth();
extern size_t ScreenHeight();
constexpr char* FAVMODEL_ICON = "#GameUI_ABC_Favorite";

using namespace vgui;

class ModelListPanel : public ListPanel {
public:
	ModelListPanel(Panel* parent, const char* name): ListPanel(parent, name) {
	}
	void ModelListPanel::OnMouseDoublePressed(MouseCode code){
		if (GetItemCount() > 0) {
			int itemID = GetSelectedItem(0);
			PostActionSignal(new KeyValues("FavChange", "itemID", itemID, "add", !std::strcmp(GetItem(itemID)->GetString("fav"), FAVMODEL_ICON) ? 0 : 1));
		}
	}
};
class ModelFilterButton : public CheckButton {
public:
	ModelFilterButton(Panel* parent, const char* name, const char* content) : CheckButton(parent, name, content) {

	}
	void SetSelected(bool state) override{
		KeyValues* msg = new KeyValues("FilterModelChecked", "state", (int)state);
		PostActionSignal(msg);
		Button::SetSelected(state);
	}
};
COptionsAdvanceSubMultiPlay::COptionsAdvanceSubMultiPlay(Panel* parent) : BaseClass(parent, "OptionsAdvanceSubMultiPlay"){
	m_pModelViewer = new ModelViewPanel(this, "ModelViewer");
	m_pModelController = new Slider(this, "ModelController");
	m_pModelList = new ModelListPanel(this, "ModelList");
	m_pModelFilter = new TextEntry(this, "ModelFilter");
	m_pModelFilterButton = new Button(this, "ModelFilterButton", "#GameUI_ABC_SubmitModelFilter", this, "FilterModel");
	m_pFavCheckButton = new ModelFilterButton(this, "ModelFavFilter", "#GameUI_ABC_FilteFavOnly");
	m_pPlayerName = new Label(this, "PlayerName", CVAR_GET_STRING("name"));
	m_pCrosshairDisplay = new CrossHairDisplay(this, "CrossHairDisplay");

	m_pColorLabel = new Label(this, "LabelC", "#GameUI_ABC_ColorLabel");
	m_pCrosshairr = new CCvarSlider(this, "CrosshairR", "#GameUI_ABC_ColorLabel", 0.0f, 255.0f, "cl_crosshaircolor_r");
	m_pCrosshairg = new CCvarSlider(this, "CrosshairG", "#GameUI_ABC_ColorLabel", 0.0f, 255.0f, "cl_crosshaircolor_g");
	m_pCrosshairb = new CCvarSlider(this, "CrosshairB", "#GameUI_ABC_ColorLabel", 0.0f, 255.0f, "cl_crosshaircolor_b");
	m_pCrosshaira = new CCvarSlider(this, "CrosshairA", "#GameUI_ABC_ColorLabel", 0.0f, 255.0f, "cl_crosshairalpha");
	m_OutlineLabel = new Label(this, "LabelOC", "#GameUI_ABC_OutlineColorLabel");
	m_pCrosshairOutliner = new CCvarSlider(this, "CrosshairOR", "#GameUI_ABC_OutlineColorLabel", 0.0f, 255.0f, "cl_crosshaircolor_outline_r");
	m_pCrosshairOutlineg = new CCvarSlider(this, "CrosshairOG", "#GameUI_ABC_OutlineColorLabel", 0.0f, 255.0f, "cl_crosshaircolor_outline_g");
	m_pCrosshairOutlineb = new CCvarSlider(this, "CrosshairOB", "#GameUI_ABC_OutlineColorLabel", 0.0f, 255.0f, "cl_crosshaircolor_outline_b");
	m_pCrosshairOutlinea = new CCvarSlider(this, "CrosshairOA", "#GameUI_ABC_OutlineColorLabel", 0.0f, 255.0f, "cl_crosshair_outline_alpha");
	m_pWidthLabel = new Label(this, "LabelW", "#GameUI_ABC_CrosshairWidth");
	m_pCrosshairWidth = new CCvarSlider(this, "CrosshairW", "#GameUI_ABC_CrosshairWidth", 0.0f, 100.0f, "cl_crosshairthickness");
	m_pLengthLabel = new Label(this, "LabelL", "#GameUI_ABC_CrosshairLength");
	m_pCrosshairLength = new CCvarSlider(this, "CrosshairL", "#GameUI_ABC_CrosshairLength", 0.0f, 100.0f, "cl_crosshairsize");
	m_pOutlineWidthLabel = new Label(this, "LabelOW", "#GameUI_ABC_CrosshairOutlineWidth");
	m_pCrosshairOutlineWidth = new CCvarSlider(this, "CrosshairO", "#GameUI_ABC_CrosshairOutlineWidth", 0.0f, 100.0f, "cl_crosshair_outline");
	m_pOffsetLable = new Label(this, "LabelOF", "#GameUI_ABC_CrosshairOffset");
	m_pCrosshairOffset = new CCvarSlider(this, "CrosshairOF", "#GameUI_ABC_CrosshairOffset", 0.0f, 100.0f, "cl_crosshairgap");
	m_pCrosshairDot = new CCvarToggleCheckButton(this, "CrosshairDot", "#GameUI_ABC_CrosshairD", "cl_crosshairdot");
	m_pCrosshairT = new CCvarToggleCheckButton(this, "CrosshairT", "#GameUI_ABC_CrosshairT", "cl_crosshair_t");
	m_pCrosshairOutline = new CCvarToggleCheckButton(this, "CrosshairOTL", "#GameUI_ABC_CrosshairOTL", "cl_crosshair_outline_draw");

	m_pSpliter = new Panel(this, "Spliter");

	m_pSparyWad = new WadFile();

	m_pSpary = new ImagePanel(this, "SparyImage");
	m_pLoadSpary = new Button(this, "SparyLoadButton", "#GameUI_ABC_LoadSpary", this, "OpenLoadSparyDialog");

	m_pUrl = new URLLabel(this, "URLLable", "ABCEnchance  ||  " __DATE__, "https://github.com/DrAbcOfficial/ABCEnchance");

	LoadControlSettings("abcenchance/res/gameui/OptionsAdvanceSubMultiPlay.res");

	float y, r;
	m_pModelViewer->SetupTexBuffer();
	m_pModelViewer->GetModelRotate(r, y, r);

	m_pModelController->SetRange(0, 360);
	m_pModelController->AddActionSignalTarget(this);
	m_pModelController->SetValue(y, true);

	m_pModelList->AddColumnHeader(0, "modelname", "#GameUI_ABC_Model", 0, ListPanel::ColumnFlags_e::COLUMN_RESIZEWITHWINDOW);
	m_pModelList->AddColumnHeader(1, "fav", "#GameUI_ABC_FavModel", m_pModelList->GetWide() / 5, ListPanel::ColumnFlags_e::COLUMN_RESIZEWITHWINDOW & ListPanel::ColumnFlags_e::COLUMN_FIXEDSIZE);
	m_pModelList->AddActionSignalTarget(this);
	m_pModelList->SetMultiselectEnabled(false);

	m_pModelFilter->AddActionSignalTarget(this);
	m_pCrosshairDot->AddActionSignalTarget(this);
	m_pCrosshairT->AddActionSignalTarget(this);
	m_pCrosshairOutline->AddActionSignalTarget(this);
	m_pFavCheckButton->AddActionSignalTarget(this);
}
COptionsAdvanceSubMultiPlay::~COptionsAdvanceSubMultiPlay(){
	delete m_pSparyWad;
}
void COptionsAdvanceSubMultiPlay::ResetModel(){
	ChangeModel(CVAR_GET_STRING("model"));
}
void COptionsAdvanceSubMultiPlay::ChangeModel(const char* mdl){
	char path[MAX_PATH] = {};
	std::snprintf(path, MAX_PATH, "models/player/%s/%s.mdl", mdl, mdl);
	m_pModelViewer->ChangeModel(path);
}
void COptionsAdvanceSubMultiPlay::BuildModelList(const char* filter){
	m_pModelList->RemoveAll();
	m_aryModelList.clear();
	FileFindHandle_t findHandle;
	auto pszFileName = filesystem()->FindFirst("models/player/*.*", &findHandle);
	if (!pszFileName)
		return;
	if (pszFileName){
		do{
			if (filesystem()->FindIsDirectory(findHandle) && *pszFileName != '.') {
				m_aryModelList.insert(std::make_pair<std::string, bool>(pszFileName, false));
			}
		} while ((pszFileName = filesystem()->FindNext(findHandle)) != nullptr);
	}
	filesystem()->FindClose(findHandle);

	FileHandle_t file = filesystem()->Open("abcenchance/model_favlist.txt","r");
	if (file) {
		char buffer[MAX_PATH];
		std::vector<std::string> favarray;
		while (!filesystem()->EndOfFile(file)) {
			filesystem()->ReadLine(buffer, MAX_PATH, file);
			std::string str = buffer;
			str.erase(str.end() - 1);
			favarray.push_back(str.c_str());
		}
		for (auto iter = m_aryModelList.begin(); iter != m_aryModelList.end(); iter++) {
			const std::string& name = iter->first;
			if (std::find(favarray.begin(), favarray.end(), name) != favarray.end()) {
				iter->second = true;
			}
		}
		filesystem()->Close(file);
	}
	
	size_t counter = 0;
	size_t plr = 0;
	std::string flt = filter ? filter : "";
	std::transform(flt.begin(), flt.end(), flt.begin(), ::tolower);
	const char* playermdl = CVAR_GET_STRING("model");
	for (auto iter = m_aryModelList.begin(); iter != m_aryModelList.end(); iter++) {
		if (m_pFavCheckButton->IsSelected() && !iter->second)
			continue;
		const char* mdl = iter->first.c_str();
		std::string str = mdl;
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		if ((flt.size() == 0) || (str.find(flt) != std::string::npos)) {
			if (!std::strcmp(mdl, playermdl))
				plr = counter;
			counter++;
			KeyValues kv(mdl, "modelname", mdl, "fav", iter->second ? FAVMODEL_ICON : "");
			m_pModelList->AddItem(&kv, 0, plr == counter, false);
		}
	}
	m_pModelList->SetSingleSelectedItem(plr);
}
void COptionsAdvanceSubMultiPlay::OnFavChange(int itemID, int add) {
	auto item = m_pModelList->GetItem(itemID);
	if (item) {
		item->SetString("fav", add == 1 ? FAVMODEL_ICON : "");
		auto it = m_aryModelList.find(std::string(item->GetName()));
		if (it != m_aryModelList.end())
			it->second = true;
		FileHandle_t file = filesystem()->Open("abcenchance/model_favlist.txt", "w");
		if (file) {
			for(auto iter = m_aryModelList.begin(); iter != m_aryModelList.end();iter++){
				if (iter->second) {
					std::string buffer = iter->first + '\n';
					filesystem()->Write(buffer.c_str(), buffer.size(), file);
				}
			}
			filesystem()->Close(file);
		}
	}
}
void COptionsAdvanceSubMultiPlay::OnOpenContextMenu(int itemID) {
	if (itemID < 0)
		return;
	Menu* menu = new Menu(this, "FavMenu");
	menu->SetAutoDelete(true);
	menu->SetVisible(false);
	menu->AddActionSignalTarget(this);
	if (!std::strcmp(m_pModelList->GetItem(itemID)->GetString("fav"), FAVMODEL_ICON))
		menu->AddMenuItem("AddFav", "#GameUI_ABC_RemoveFav", new KeyValues("FavChange", "itemID", itemID, "add", 0), this);
	else
		menu->AddMenuItem("AddFav", "#GameUI_ABC_AddFav", new KeyValues("FavChange", "itemID", itemID, "add", 1), this);
	menu->PositionRelativeToPanel(this, Menu::CURSOR, 0, true);
	menu->MakePopup();
}
void COptionsAdvanceSubMultiPlay::OnFilterModelChecked(int state) {
	FilterModel();
}
void COptionsAdvanceSubMultiPlay::OnSliderMoved() {
	m_pModelViewer->SetModelRotate(0, m_pModelController->GetValue(), 0);

	m_pCrosshairDisplay->SetLength(m_pCrosshairLength->GetValue());
	m_pCrosshairDisplay->SetWidth(m_pCrosshairWidth->GetValue());
	m_pCrosshairDisplay->SetOffset(m_pCrosshairOffset->GetValue());
	Color crosshairColor(m_pCrosshairr->GetValue(), m_pCrosshairg->GetValue(), m_pCrosshairb->GetValue(), m_pCrosshaira->GetValue());
	m_pCrosshairDisplay->SetCrossHairColor(crosshairColor);
	Color outlineColor(m_pCrosshairOutliner->GetValue(), m_pCrosshairOutlineg->GetValue(), m_pCrosshairOutlineb->GetValue(), m_pCrosshairOutlinea->GetValue());
	m_pCrosshairDisplay->SetOutlineColor(outlineColor);
	m_pCrosshairDisplay->SetOutlineWidth(m_pCrosshairOutlineWidth->GetValue());
	m_pCrosshairDisplay->InvalidateLayout();
}
void COptionsAdvanceSubMultiPlay::OnItemSelected() {
	ChangeModel(m_pModelList->GetItem(m_pModelList->GetSelectedItem(0))->GetName());
}
void COptionsAdvanceSubMultiPlay::OnButtonChanged(){
	m_pCrosshairDisplay->SetDot(m_pCrosshairDot->IsSelected());
	m_pCrosshairDisplay->SetT(m_pCrosshairT->IsSelected());
	m_pCrosshairDisplay->SetOutline(m_pCrosshairOutline->IsSelected());
	m_pCrosshairDisplay->InvalidateLayout();
}
void COptionsAdvanceSubMultiPlay::OnFileSelected(const char* fullpath) {
	FIBITMAP* img = nullptr;
	char ext[4];
	std::strcpy(ext, std::filesystem::path(fullpath).extension().u8string().c_str());
	if (!std::strncmp(ext, ".tga", 4))
		img = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_TARGA, fullpath, 0);
	else if(!std::strncmp(ext, ".bmp", 4))
		img = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_BMP, fullpath, 0);
	else if (!std::strncmp(ext, ".jpg", 4))
		img = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_JPEG, fullpath, 0);
	else if (!std::strncmp(ext, ".png", 4))
		img = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_PNG, fullpath, 0);
	if (img) {
		std::ofstream stream;
		char wadpath[MAX_PATH];
		filesystem()->GetLocalPath("tempdecal.wad", wadpath, MAX_PATH);
		stream.open(wadpath, std::ios::out | std::ios::binary);
		if (!stream.is_open()) {
			FreeImage_Unload(img);
			return;
		}
		size_t w = FreeImage_GetWidth(img);
		size_t h = FreeImage_GetHeight(img);
		size_t nw = w;
		size_t nh = h;
		GetValidateSparySize(nw, nh);
		FIBITMAP* nimg = FreeImage_Rescale(img, nw, nh);
		FreeImage_Unload(img);
		img = FreeImage_ColorQuantizeEx(nimg, FIQ_WUQUANT, 256);
		FreeImage_Unload(nimg);
		//header
		stream.write("WAD3", 4);
		unsigned int headerbuf = 1;
		stream.write((char*)&headerbuf, 4);
		headerbuf = sizeof(WAD3Header_t);
		stream.write((char*)&headerbuf, 4);
		//lump
		size_t size = nw * nh;
		WAD3Lump_t lump;
		lump.compression = 0;
		lump.dummy = 0;
		lump.type = 0x43; //miptex
		std::strncpy(lump.name, "{LOGO", 16);
		lump.offset = sizeof(WAD3Header_t) + sizeof(WAD3Lump_t);
		lump.size = lump.sizeOnDisk = sizeof(BSPMipTexHeader_t) + size + (size / 4) + (size / 16) + (size / 64) + sizeof(short) + 256 * 3;
		stream.write((char*)&lump, sizeof(WAD3Lump_t));
		//mips header
		BSPMipTexHeader_t header;
		std::strncpy(header.name, "{LOGO", 16);
		header.width = nw;
		header.height = nh;
		header.offsets[0] = sizeof(BSPMipTexHeader_t);
		header.offsets[1] = sizeof(BSPMipTexHeader_t) + size;
		header.offsets[2] = sizeof(BSPMipTexHeader_t) + size + (size / 4);
		header.offsets[3] = sizeof(BSPMipTexHeader_t) + size + (size / 4) + (size / 16);
		stream.write((char*)&header, sizeof(BSPMipTexHeader_t));
		BYTE* bits = FreeImage_GetBits(img);
		BYTE* flipped = new BYTE[size];
		for (int i = 0; i < nh; i++) {
			memcpy(flipped + i * nw, bits + (nh - i - 1) * nw, nw);
		}
		//mips data
		auto write_mips = [&](int mips_level) {
			int lv = pow(2, mips_level);
			for (size_t i = 0; i < (nh/lv); i++) {
				for (size_t j = 0; j < (nw/lv); j++) {
					char buf = flipped[i * lv * nw + j * lv];
					stream.write((char*)&buf, 1);
				}
			}
		};
		for (size_t i = 0; i < 4; i++) {
			write_mips(i);
		}
		delete[] flipped;
		short dummy = 0;
		stream.write((char*)&dummy, sizeof(short));
		//Palette
		RGBQUAD* palette = FreeImage_GetPalette(img);
		for (size_t i = 0; i < 256; i++) {
			RGBQUAD p = palette[i];
			stream.write((char*)&p.rgbRed, 1);
			stream.write((char*)&p.rgbGreen, 1);
			stream.write((char*)&p.rgbBlue, 1);
		}
		stream.close();
		FreeImage_Unload(img);

		m_pSparyWad->Load(wadpath);
		WadTexture* tex = m_pSparyWad->Get("{logo");
		if (tex)
			SetSparyPixel(tex->GetPixels(), tex->Width(), tex->Height());
	}
}
void COptionsAdvanceSubMultiPlay::OnResetData(){
	ResetModel();
	BuildModelList();

	m_pCrosshairDisplay->SetLength(CVAR_GET_FLOAT("cl_crosshairsize"));
	m_pCrosshairDisplay->SetWidth(CVAR_GET_FLOAT("cl_crosshairthickness"));
	m_pCrosshairDisplay->SetOffset(CVAR_GET_FLOAT("cl_crosshairgap"));
	Color crosshairColor(CVAR_GET_FLOAT("cl_crosshaircolor_r"), CVAR_GET_FLOAT("cl_crosshaircolor_g"), CVAR_GET_FLOAT("cl_crosshaircolor_b"), CVAR_GET_FLOAT("cl_crosshairalpha"));
	m_pCrosshairDisplay->SetCrossHairColor(crosshairColor);
	Color outlineColor(CVAR_GET_FLOAT("cl_crosshaircolor_outline_r"), CVAR_GET_FLOAT("cl_crosshaircolor_outline_g"), CVAR_GET_FLOAT("cl_crosshaircolor_outline_b"), CVAR_GET_FLOAT("cl_crosshair_outline_alpha"));
	m_pCrosshairDisplay->SetOutlineColor(outlineColor);
	m_pCrosshairDisplay->SetOutline(CVAR_GET_FLOAT("cl_crosshair_outline_draw") > 0);
	m_pCrosshairDisplay->SetOutlineWidth(CVAR_GET_FLOAT("cl_crosshair_outline"));
	m_pCrosshairDisplay->SetDot(CVAR_GET_FLOAT("cl_crosshairdot") > 0);
	m_pCrosshairDisplay->SetT(CVAR_GET_FLOAT("cl_crosshair_t") > 0);
	m_pCrosshairDisplay->InvalidateLayout();

	WadTexture* tex = m_pSparyWad->Get("{logo");
	if (!tex) {
		char fullpath[MAX_PATH];
		filesystem()->GetLocalPath("tempdecal.wad", fullpath, MAX_PATH);
		m_pSparyWad->Load(fullpath);
		tex = m_pSparyWad->Get("{logo");
	}

	if(tex)
		SetSparyPixel(tex->GetPixels(), tex->Width(), tex->Height());
	
}
void COptionsAdvanceSubMultiPlay::OnApplyChanges(){
	BaseClass::OnApplyChanges();
	int mdlIndex = m_pModelList->GetSelectedItem(0);
	if (mdlIndex != -1) 
		CVAR_SET_STRING("model", m_pModelList->GetItem(mdlIndex)->GetName());

	m_pCrosshairr->ApplyChanges();
	m_pCrosshairg->ApplyChanges();
	m_pCrosshairb->ApplyChanges();
	m_pCrosshaira->ApplyChanges();

	m_pCrosshairOutliner->ApplyChanges();
	m_pCrosshairOutlineg->ApplyChanges();
	m_pCrosshairOutlineb->ApplyChanges();
	m_pCrosshairOutlinea->ApplyChanges();

	m_pCrosshairWidth->ApplyChanges();
	m_pCrosshairLength->ApplyChanges();
	m_pCrosshairOutlineWidth->ApplyChanges();
	m_pCrosshairOffset->ApplyChanges();
	m_pCrosshairDot->ApplyChanges();
	m_pCrosshairT->ApplyChanges();
	m_pCrosshairOutline->ApplyChanges();
}
void COptionsAdvanceSubMultiPlay::OnCommand(const char* cmd){
	if (!std::strcmp(cmd, "FilterModel"))
		FilterModel();
	else if (!std::strcmp(cmd, "OpenLoadSparyDialog")) {
		if (m_pFileDialog) {
			delete m_pFileDialog;
			m_pFileDialog = nullptr;
		}
		m_pFileDialog = new FileOpenDialog(this, "#GameUI_ABC_LoadSpary", FileOpenDialogType_t::FOD_OPEN, new KeyValues("FileInfo"));
		m_pFileDialog->SetProportional(false);
		m_pFileDialog->AddFilter("*.tga;*.png;*.bmp;*.jpg", "#GameUI_ABC_SparyFilter", true, "image");
		m_pFileDialog->Activate();
		int w, h;
		m_pFileDialog->GetSize(w, h);
		m_pFileDialog->SetPos((ScreenWidth() - w) / 2, (ScreenHeight() - h) / 2);
	}
}
void COptionsAdvanceSubMultiPlay::ApplySchemeSettings(IScheme* pScheme){
	BaseClass::ApplySchemeSettings(pScheme);
	m_pModelViewer->SetBgColor(GetSchemeColor("ModelViewer/BgColor", GetSchemeColor("Panel.BgColor", pScheme), pScheme));
	m_pModelController->SetBgColor(GetSchemeColor("ModelViewer/SliderBgColor", GetSchemeColor("Panel.BgColor", pScheme), pScheme));
	m_pPlayerName->SetBgColor(GetSchemeColor("ModelViewer/NameTagBgColor", GetSchemeColor("Panel.BgColor", pScheme), pScheme));
}
void COptionsAdvanceSubMultiPlay::FilterModel(){
	char buf[MAX_PATH];
	m_pModelFilter->GetText(buf, MAX_PATH);
	if (std::strlen(buf) > 0) {
		m_pModelList->RemoveAll();
		m_pModelList->ResetScrollBar();
		BuildModelList(buf);
	}
	else
		BuildModelList(nullptr);
}
void COptionsAdvanceSubMultiPlay::GetValidateSparySize(size_t& ow, size_t& oh){
	float w = static_cast<float>(ow);
	float h = static_cast<float>(oh);
	if (w * h > 14336.0f) {
		if (w > h) {
			h = h / w * 256.0f;
			w = 256.0f;
		}
		else {
			w = w / h * 256.0f;
			h = 256.0f;
		}
		while (w * h > 14436.0f) {
			w *= 0.95f;
			h *= 0.95f;
		}
	}
	w = static_cast<size_t>(w);
	h = static_cast<size_t>(h);
	float gap = 16.0f;
	float dw = fmodf(w, gap);
	if (dw > gap / 2.0f)
		w = w + (gap - dw);
	else
		w = w - dw;
	float dh = fmodf(h, gap);
	if (dh > gap / 2)
		h = h + (gap - dh);
	else
		h = h - dh;
	ow = static_cast<size_t>(w);
	oh = static_cast<size_t>(h);
}
void COptionsAdvanceSubMultiPlay::SetSparyPixel(unsigned char* pixels, size_t wide, size_t height){
	m_pSpary->SetImage(new MemoryBitmap(pixels, wide, height));
}


vgui::COptionsAdvanceSubOtherOption::COptionsAdvanceSubOtherOption(Panel* parent) : BaseClass(parent, "OptionsAdvanceSubOtherOption"){
	m_pNewHud = new CCvarToggleCheckButton(this, "NewHud", "#GameUI_ABC_Cvar_NewHud", "cl_hud_csgo");
	m_pDynamicBackground = new CCvarToggleCheckButton(this, "DynamicBackground", "#GameUI_ABC_Cvar_DynamicBackground", "hud_dynamic_background");

	m_pScoreBoard = new GroupBox(this, "ScoreGroup", "#GameUI_ABC_ScoreOptions", 8);
	m_pScoreShowAvatar = new CCvarToggleCheckButton(this, "ScoreShowAvatar", "#GameUI_ABC_Cvar_ScoreShowAvatar", "hud_scoreboard_showavatars");
	m_pScoreShowLoss = new CCvarToggleCheckButton(this, "ScoreShowLoss", "#GameUI_ABC_Cvar_ScoreShowLoss", "hud_scoreboard_showloss");
	m_pScoreShowSteamId = new CCvarToggleCheckButton(this, "ScoreShowSteamId", "#GameUI_ABC_Cvar_ScoreShowSteamId", "hud_scoreboard_showsteamid");
	m_pScoreShowSteamIdType = new CCvarComboBox(this, "ScoreShowSteamIdType", "#GameUI_ABC_Cvar_ScoreShowSteamIdType", 2, "hud_scoreboard_showsteamidtype");
	m_pScoreShowSteamIdType->AddItem("#GameUI_ABC_SteamID", nullptr);
	m_pScoreShowSteamIdType->AddItem("#GameUI_ABC_SteamID64", nullptr);
	m_pScoreShowRealName = new CCvarToggleCheckButton(this, "ScoreShowRealName", "#GameUI_ABC_Cvar_ScoreShowRealName", "hud_scoreboard_showrealname");
	m_pScoreSpacingNormal = new CCvarLabelSlider(this, "ScoreSpacingNormal", "#GameUI_ABC_Cvar_ScoreSpacingNormal", "#GameUI_ABC_Cvar_ScoreSpacingNormal", 0, 10, "hud_scoreboard_spacing_normal");
	m_pScoreSpacingCompact = new CCvarLabelSlider(this, "ScoreSpacingCompact", "#GameUI_ABC_Cvar_ScoreSpacingCompact", "#GameUI_ABC_Cvar_ScoreSpacingCompact", 0, 10, "hud_scoreboard_spacing_compact");
	m_pScoreSize = new CCvarComboBox(this, "ScoreSize", "#GameUI_ABC_Cvar_ScoreSize", 3, "hud_scoreboard_size");
	m_pScoreSize->AddItem("#GameUI_ABC_Cvar_ScoreSizeAuto", nullptr);
	m_pScoreSize->AddItem("#GameUI_ABC_Cvar_ScoreSizeNormal", nullptr);
	m_pScoreSize->AddItem("#GameUI_ABC_Cvar_ScoreSizeCompact", nullptr);

	m_pPopNumber = new CCvarToggleCheckButton(this, "PopNumber", "#GameUI_ABC_Cvar_PopNumber", "cl_popnumber");

	m_pPlayerTitleGroup = new GroupBox(this, "PlayerTitleGroup", "#GameUI_ABC_PlayerTitleOptions", 2);
	m_pPlayerTitle = new CCvarToggleCheckButton(this, "PlayerTitle", "#GameUI_ABC_Cvar_PlayerTitle", "cl_popnumber");
	m_pPlayerTitleDanger = new CCvarLabelSlider(this, "PlayerTitleDanger", "#GameUI_ABC_Cvar_PlayerTitleDanger", "#GameUI_ABC_Cvar_PlayerTitleDanger", 0, 100, "cl_playertitle_danger");


	m_pCameraGroup = new GroupBox(this, "CameraGroup", "#GameUI_ABC_CameraGroup", 2);
	m_pCameraHeightValue = new CCvarLabelSlider(this, "CameraHeight", "#GameUI_ABC_Cvar_CameraHeight", "#GameUI_ABC_Cvar_CameraHeight", -100, 100, "cam_idealheight");
	m_pCameraRightValue = new CCvarLabelSlider(this, "CameraRight", "#GameUI_ABC_Cvar_CameraRight", "#GameUI_ABC_Cvar_CameraRight", -100, 100, "cam_idealright");

	m_pModelLagGroup = new GroupBox(this, "ModelLagGroup", "#GameUI_ABC_ModelLagGroup", 7);
	m_pModelLag = new CCvarToggleCheckButton(this, "ModelLag", "#GameUI_ABC_Cvar_ModelLag", "cl_modellag");
	m_pModelLagAutoStop = new CCvarToggleCheckButton(this, "ModelLagAutoStop", "#GameUI_ABC_Cvar_ModelLagAutoStop", "cl_modellag_autostop");
	m_pModelLagAutoValue = new CCvarLabelSlider(this, "ModelLagValue", "#GameUI_ABC_Cvar_ModelLagValue", "#GameUI_ABC_Cvar_ModelLagValue", 0.0f, 5.0f, "cl_modellag_value", false, true);
	m_pModelSlide = new CCvarToggleCheckButton(this, "ModelSlide", "#GameUI_ABC_Cvar_ModelSlide", "cl_modelslide");
	m_pModelSlideAngle = new CCvarLabelSlider(this, "ModelSlideAngle", "#GameUI_ABC_Cvar_ModelSlideAngle", "#GameUI_ABC_Cvar_ModelSlideAngle", 0.0f, 90.0f, "cl_modelslide_angle", false, true);
	m_pModelSlideLength = new CCvarLabelSlider(this, "ModelSlideLength", "#GameUI_ABC_Cvar_ModelSlideLength", "#GameUI_ABC_Cvar_ModelSlideLength", -30.0f, 30.0f, "cl_modelslide_length");
	m_pModelSlideHeight = new CCvarLabelSlider(this, "ModelSlideHeight", "#GameUI_ABC_Cvar_ModelSlideHeight", "#GameUI_ABC_Cvar_ModelSlideHeight", -30.0f, 30.0f, "cl_modelslide_height");

	m_pHealthArmor = new GroupBox(this, "HPAPGroup", "#GameUI_ABC_HPAPGroup", 5);
	m_pDamageShock = new CCvarToggleCheckButton(this, "DamageShock", "#GameUI_ABC_Cvar_DamageShock", "cl_damageshock_factor");
	m_pDamageShockFactor = new CCvarLabelSlider(this, "DamageShockFactor", "#GameUI_ABC_Cvar_DamageShockFactor", "#GameUI_ABC_Cvar_DamageShockFactor", 0.0f, 0.1f, "cl_damageshock_factor", false, true);
	m_pDamageShockDmg = new CCvarLabelSlider(this, "DamageShockDmg", "#GameUI_ABC_Cvar_DamageShockDmg", "#GameUI_ABC_Cvar_DamageShockDmg", 0, 100, "cl_damageshock_base");
	m_pDangerHealth = new CCvarLabelSlider(this, "DangerHealth", "#GameUI_ABC_Cvar_DangerHealth", "#GameUI_ABC_Cvar_DangerHealth", 0, 100, "cl_dangerhealth");
	m_pDangerArmor = new CCvarLabelSlider(this, "DangerArmor", "#GameUI_ABC_Cvar_DangerArmor", "#GameUI_ABC_Cvar_DangerArmor", 0, 100, "cl_dangerarmor");

	m_pItemHighLightGroup = new GroupBox(this, "ItemHighLightGroup", "#GameUI_ABC_ItemHighLightGroup", 2);
	m_pItemHighLight = new CCvarToggleCheckButton(this, "ItemHighLight", "#GameUI_ABC_Cvar_ItemHighLight", "cl_itemhighlight");
	m_pItemHighLightRange = new CCvarLabelSlider(this, "ItemHighLightRange", "#GameUI_ABC_Cvar_ItemHighLightRange", "#GameUI_ABC_Cvar_ItemHighLightRange", 0, 344, "cl_itemhighlightrange");

	m_pAutojump = new CCvarToggleCheckButton(this, "AutoJump", "#GameUI_ABC_Cvar_AutoJump", "cl_autojump");
	m_pRainbowMenu = new CCvarToggleCheckButton(this, "RainbowMenu", "#GameUI_ABC_Cvar_RainbowMenu", "cl_rainbowmenu");

	LoadControlSettings("abcenchance/res/gameui/OptionsAdvanceSubOtherOption.res");
}

void COptionsAdvanceSubOtherOption::OnApplyChanges(){
	m_pNewHud->ApplyChanges();
	m_pDynamicBackground->ApplyChanges();

	m_pScoreShowAvatar->ApplyChanges();
	m_pScoreShowLoss->ApplyChanges();
	m_pScoreShowSteamId->ApplyChanges();
	m_pScoreShowSteamIdType->ApplyChanges();
	m_pScoreShowRealName->ApplyChanges();
	m_pScoreSpacingNormal->ApplyChanges();
	m_pScoreSpacingCompact->ApplyChanges();
	m_pScoreSize->ApplyChanges();

	m_pPopNumber->ApplyChanges();

	m_pPlayerTitle->ApplyChanges();
	m_pPlayerTitleDanger->ApplyChanges();


	m_pCameraHeightValue->ApplyChanges();
	m_pCameraRightValue->ApplyChanges();

	m_pModelLag->ApplyChanges();
	m_pModelLagAutoStop->ApplyChanges();
	m_pModelLagAutoValue->ApplyChanges();
	m_pModelSlide->ApplyChanges();
	m_pModelSlideAngle->ApplyChanges();
	m_pModelSlideLength->ApplyChanges();
	m_pModelSlideHeight->ApplyChanges();

	m_pDamageShock->ApplyChanges();
	m_pDamageShockFactor->ApplyChanges();
	m_pDamageShockDmg->ApplyChanges();
	m_pDangerHealth->ApplyChanges();
	m_pDangerArmor->ApplyChanges();

	m_pItemHighLight->ApplyChanges();
	m_pItemHighLightRange->ApplyChanges();

	m_pAutojump->ApplyChanges();
	m_pRainbowMenu->ApplyChanges();
}



COptionsAdvanceDialog::COptionsAdvanceDialog(Panel* parent) : BaseClass(parent, "OptionsAdvanceDialog") {
	SetDeleteSelfOnClose(true);
	SetSizeable(false);
	SetMoveable(false);
	SetProportional(true);
	SetScheme("OptionsAdvanceDialogScheme");
	m_pBlur = new GaussianBlurPanel(this, "BlurPanel");
	LoadControlSettings("abcenchance/res/gameui/OptionsAdvanceDialog.res");
	m_pMultiPlayPage = new COptionsAdvanceSubMultiPlay(this);
	m_pOtherOption = new COptionsAdvanceSubOtherOption(this);
	AddPage(m_pMultiPlayPage, "#GameUI_ABC_MultiPlayPage");
	AddPage(m_pOtherOption, "#GameUI_ABC_OtherOption");

	EnableApplyButton(true);
}
void COptionsAdvanceDialog::Activate(){
	BaseClass::Activate();
	input()->SetAppModalSurface(GetVPanel());
}
void COptionsAdvanceDialog::ApplySettings(KeyValues* inResourceData){
	BaseClass::ApplySettings(inResourceData);
}
void COptionsAdvanceDialog::ApplySchemeSettings(IScheme* pScheme){
	BaseClass::ApplySchemeSettings(pScheme);
	
	SetBgColor(GetSchemeColor("OptionsAdvanceDialog/BgColor", GetSchemeColor("Panel.BgColor", pScheme), pScheme));
	SetBorder(pScheme->GetBorder("DialogBorder"));

	auto sheet = GetPropertySheet();
	sheet->SetBgColor(GetSchemeColor("OptionsAdvanceDialog/SheetBgColor", GetSchemeColor("Panel.BgColor", pScheme), pScheme));
	sheet->SetBorder(pScheme->GetBorder("SheetBorder"));

	m_pMultiPlayPage->SetBgColor(GetSchemeColor("OptionsAdvanceDialog/SheetBgColor", GetSchemeColor("Panel.BgColor", pScheme), pScheme));
	m_pMultiPlayPage->SetBorder(pScheme->GetBorder("SheetBorder"));
	

	//ofc
	m_pBlur->SetBgColor(Color(255, 255, 255, 255));
}

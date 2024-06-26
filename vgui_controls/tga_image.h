#ifndef VGUI_TGA_IMAGE_H
#define VGUI_TGA_IMAGE_H
#include <vgui/IImage.h>
#include <Color.h>

class CTGAImage : public vgui::IImage_HL25
{
public:
	/**
	 * Creates an empty image.
	 */
	CTGAImage();

	/**
	 * Creates an image from file.
	 * @see LoadImage
	 */
	CTGAImage(const char *pFilePath);

	/**
	 * Loads a .tga image from path pFilePath.
	 * .tga extension doesn't need to be specified.
	 */
	void LoadImage(const char *pFilePath);
	virtual void Paint();
	virtual void SetPos(int x, int y);
	virtual void GetContentSize(int &wide, int &tall);
	virtual void GetSize(int &wide, int &tall);
	virtual void SetSize(int wide, int tall);
	virtual void SetColor(Color col);
	void Destroy(void) override;
	void SetAdditive(bool bIsAdditive) override;

	int GetTextureId();

private:
	Color m_Color;
	int m_iTextureID = -1;
	int m_nX = 0, m_nY = 0;
	int m_wide = 64, m_tall = 64;
	bool _bAdditive;
};

#endif

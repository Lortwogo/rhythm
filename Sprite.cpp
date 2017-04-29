#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

Sprite::Sprite(string fileName, double x, double y) : m_x((int)x), m_y((int)y), m_w(0), m_h(0), m_performanceBMP(nullptr)
{
	if (loadBitmap(fileName) == false)
	{
		loadBitmap("images\\error.bmp"); // load a standard error bmp - should be a resource so we are guaranteed it is there
	}
}

// Loads a bitmap and stores its data in the sprite.  
bool Sprite::loadBitmap(string fname)
{
	wstring file;
	int w, h;

	file.assign(fname.begin(), fname.end());

	Bitmap loadBmp(file.c_str(), FALSE);
	if (loadBmp.GetLastStatus() != Ok)
	{
		// Unable to load bitmap
		return false;
	}

	w = loadBmp.GetWidth();
	h = loadBmp.GetHeight();

	// If we already have a bmp in this sprite, delete it.  
	// Safety check necessary in case we try to load twice, or we want to replace the existing image with a new one
	if (m_performanceBMP)
		delete m_performanceBMP;

	m_performanceBMP = new Bitmap(w, h, PixelFormat32bppPARGB); // create optimized bitmap
	Graphics *graphics = new Graphics(m_performanceBMP); // create a graphics object

	graphics->DrawImage(&loadBmp, 0, 0, w, h); // blit original into performance format
	delete graphics;

	// store width/height used for collision checks if needed
	m_w = w;
	m_h = h;
	return true;
}

Sprite::~Sprite()
{
	if (m_performanceBMP)
	{
		delete m_performanceBMP; // NOTE: You CANNOT delete a Bitmap ptr that was drawn on a graphics object after Gdi is shutdown (crashes)
		m_performanceBMP = NULL;
	}
}

void Sprite::setX(int x)
{
	m_x = x;
}

void Sprite::setY(int y)
{
	m_y = y;
}

int Sprite::getX()
{
	return m_x;
}

int Sprite::getY()
{
	return m_y;
}

Sprite::Sprite(const Sprite & other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_w = other.m_w;
	m_h = other.m_h;
	m_performanceBMP = other.m_performanceBMP->Clone(0, 0, m_w, m_h, other.m_performanceBMP->GetPixelFormat());
}

Sprite & Sprite::operator=(const Sprite & other)
{
	if (this != &other)
	{
		m_x = other.m_x;
		m_y = other.m_y;
		m_w = other.m_w;
		m_h = other.m_h;
		m_performanceBMP = other.m_performanceBMP->Clone(0, 0, m_w, m_h, other.m_performanceBMP->GetPixelFormat());
	}

	return *this;
}

void Sprite::draw(Graphics &graphics)
{
	// Best settings for performance versus quality
	graphics.SetCompositingMode(CompositingModeSourceOver);
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);
	graphics.SetSmoothingMode(SmoothingModeNone);
	graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

	graphics.DrawImage(m_performanceBMP, m_x, m_y);

	graphics.Flush();
}

Bitmap * Sprite::getBMP()
{
	return m_performanceBMP;
}


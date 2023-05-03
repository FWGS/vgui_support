/*
vgui_main.h - vgui main header
Copyright (C) 2011 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

In addition, as a special exception, the author gives permission
to link the code of this program with VGUI library developed by
Valve, L.L.C ("Valve"). You must obey the GNU General Public License
in all respects for all of the code used other than VGUI library.
If you modify this file, you may extend this exception to your
version of the file, but you are not obligated to do so. If
you do not wish to do so, delete this exception statement
from your version.

*/
#ifndef VGUI_MAIN_H
#define VGUI_MAIN_H

#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#endif
#include <assert.h>
#include <VGUI.h>
#include <VGUI_App.h>
#include <VGUI_Font.h>
#include <VGUI_Panel.h>
#include <VGUI_Cursor.h>
#include <VGUI_SurfaceBase.h>
#include <VGUI_InputSignal.h>
#include <VGUI_MouseCode.h>
#include <VGUI_KeyCode.h>
#include <VGUI2.h>
#include <VGuiVertex.h>
#include "vgui_api.h"


namespace vgui_support
{
extern vgui_support_api_t *g_api;

struct VGUIPanel
{
	VGUIPanel( vgui::Panel *pPanel )
	{
		this->pPanel = pPanel;
		this->vgui2 = false;
	}
	VGUIPanel( vgui2::VPANEL vPanel )
	{
		this->vPanel = vPanel;
		this->vgui2 = true;
	}
	VGUIPanel()
	{
		pPanel = nullptr;
		vgui2 = false;
	}

	union
	{
		vgui::Panel *pPanel;
		vgui2::VPANEL vPanel;
	};
	bool vgui2;
};

struct PaintStack
{
	VGUIPanel panel;
	int	iTranslateX;
	int	iTranslateY;
	int	iScissorLeft;
	int	iScissorRight;
	int	iScissorTop;
	int	iScissorBottom;
};

class CEngineSurface : public vgui::SurfaceBase
{
public:
	CEngineSurface( vgui::Panel *embeddedPanel );
	~CEngineSurface();

	virtual bool setFullscreenMode( int wide, int tall, int bpp ) override;
	virtual void setWindowedMode() override;
	virtual void setTitle( const char *title ) override;
	virtual void createPopup( vgui::Panel *embeddedPanel ) override;
	virtual bool isWithin( int x, int y ) override;
	virtual bool hasFocus() override;
	virtual void GetMousePos( int &x, int &y ) override;

	vgui::Panel *getEmbeddedPanel();
	void drawPrintChar( int x, int y, int wide, int tall, float s0, float t0, float s1, float t1, int color[] );

protected:
	virtual int createNewTextureID() override;
	virtual void drawSetColor( int r, int g, int b, int a ) override;
	virtual void drawSetTextColor( int r, int g, int b, int a ) override;
	virtual void drawFilledRect( int x0, int y0, int x1, int y1 ) override;
	virtual void drawOutlinedRect( int x0, int y0, int x1, int y1 ) override;
	virtual void drawSetTextFont( vgui::Font *font ) override;
	virtual void drawSetTextPos( int x, int y ) override;
	virtual void drawPrintText( const char *text, int textLen ) override;
	virtual void drawSetTextureRGBA( int id, const char *rgba, int wide, int tall ) override;
	virtual void drawSetTexture( int id ) override;
	virtual void drawTexturedRect( int x0, int y0, int x1, int y1 ) override;
	virtual void setCursor( vgui::Cursor *cursor ) override;
	virtual void pushMakeCurrent( vgui::Panel *panel, bool useInsets ) override;
	virtual void popMakeCurrent( vgui::Panel *panel ) override;
	virtual void enableMouseCapture( bool state );
	virtual void invalidate( vgui::Panel *panel );
	virtual void setAsTopMost( bool state );
	virtual void applyChanges();
	virtual void swapBuffers();

private:
	void SetupPaintState( const PaintStack *paintState );
	void InitVertex( vpoint_t &vertex, int x, int y, float u, float v );

	// VGUI2 Surface funcs
	void PushMakeCurrent( VGUIPanel panel, int insets[4], int absExtents[4], int clipRect[4] );
	void PopMakeCurrent( VGUIPanel panel );
	void drawLine( int x0, int y0, int x1, int y1 );
	void drawPolyLine( int *px, int *py, int numPoints );
	void drawGetTextPos( int &x, int &y );
	void drawSetTextureFile( int id, const char *filename );
	void drawGetTextureSize( int id, int &wide, int &tall );
	vgui2::HFont createFont();
	bool addGlyphSetToFont( vgui2::HFont font, const char *fontName, int tall, int weight, bool italic, bool underline, bool strikeout, bool symbol );
	bool addCustomFontFile( const char *fontFileName );
	int  getFontTall( vgui2::HFont font );
	void getCharABCWide( vgui2::HFont font, int ch, int &a, int &b, int &c );
	int  getCharWidth( vgui2::HFont font, int ch );
	void getTextSize( vgui2::HFont font, const char *text, int &wide, int &tall );
	void playSound( const char *filename );
	void drawTexturedPolygon( vgui2::VGuiVertex *verts, int n );
	int  getFontAscent( vgui2::HFont font, int ch );
	bool deleteTextureByID( int id );
	void drawSubTextureBGRA( int id, int x, int y, const byte *bgra, int wide, int tall );

	// point translation for current panel
	int		_translateX;
	int		_translateY;

	// the size of the window to draw into
	int		_surfaceExtents[4];
protected:
	vgui::Cursor *_hCurrentCursor;
	int _drawTextPos[2];
	int _drawColor[4];
	int _drawTextColor[4];
	friend class App;
	friend class Panel;
	friend class VGUI2Surface;
};

extern CEngineSurface *surface;

//
// vgui_input.cpp
//
void VGUI_Mouse( VGUI_MouseAction action, int code );
void VGUI_Key( VGUI_KeyAction action, VGUI_KeyCode code );
void VGUI_MouseMove( int x, int y );
void VGUI_TextInput( const char *text );

//
// vgui_clip.cpp
//
void EnableScissor( qboolean enable );
void SetScissorRect( int left, int top, int right, int bottom );
qboolean ClipRect( const vpoint_t &inUL, const vpoint_t &inLR, vpoint_t *pOutUL, vpoint_t *pOutLR );

//
// vgui2_int.cpp
//
void VGUI2_Startup( void *clientlib, int width, int height );
void VGUI2_Shutdown();
bool VGUI2_UseVGUI1();
void VGUI2_Paint();
void VGUI2_ScreenSize( int &width, int &height );
void VGUI2_Key( VGUI_KeyAction action, VGUI_KeyCode code );
void VGUI2_Mouse( VGUI_MouseAction action, int code );
void VGUI2_MouseMove( int x, int y );
void VGUI2_TextInput( const char *text );

}

#endif // VGUI_MAIN_H

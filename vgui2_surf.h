/*
vgui2_surf.cpp - vgui2 surface header
Copyright (C) 2022 FWGS

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

#ifndef VGUI2_SURFACE_H
#define VGUI2_SURFACE_H

#include <VGui2Interfaces.h>
#include <Panel.h>
#include <IChromeHTMLWrapper.h>
#include <VGUI_Dar.h>

class VGUI2Surface : public vgui2::ISurface
{
public:
	virtual void Shutdown();
	virtual void RunFrame();
	virtual vgui2::VPANEL GetEmbeddedPanel();
	virtual void SetEmbeddedPanel( vgui2::VPANEL panel );
	virtual void PushMakeCurrent( vgui2::VPANEL panel, bool useInsets );
	virtual void PopMakeCurrent( vgui2::VPANEL panel );
	virtual void DrawSetColor( int r, int g, int b, int a );
	virtual void DrawSetColor( Color col );
	virtual void DrawFilledRect( int x0, int y0, int x1, int y1 );
	virtual void DrawOutlinedRect( int x0, int y0, int x1, int y1 );
	virtual void DrawLine( int x0, int y0, int x1, int y1 );
	virtual void DrawPolyLine( int *px, int *py, int numPoints );
	virtual void DrawSetTextFont( vgui2::HFont font );
	virtual void DrawSetTextColor( int r, int g, int b, int a );
	virtual void DrawSetTextColor( Color col );
	virtual void DrawSetTextPos( int x, int y );
	virtual void DrawGetTextPos( int &x, int &y );
	virtual void DrawPrintText( const wchar_t *text, int textLen );
	virtual void DrawUnicodeChar( wchar_t wch );
	virtual void DrawUnicodeCharAdd( wchar_t wch );
	virtual void DrawFlushText();
	virtual vgui2::IHTML *CreateHTMLWindow( vgui2::IHTMLEvents *events, vgui2::VPANEL context );
	virtual void PaintHTMLWindow( vgui2::IHTML *htmlwin );
	virtual void DeleteHTMLWindow( vgui2::IHTML *htmlwin );
	virtual void DrawSetTextureFile( int id, const char *filename, int hardwareFilter, bool forceReload );
	virtual void DrawSetTextureRGBA( int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload );
	virtual void DrawSetTexture( int id );
	virtual void DrawGetTextureSize( int id, int &wide, int &tall );
	virtual void DrawTexturedRect( int x0, int y0, int x1, int y1 );
	virtual bool IsTextureIDValid( int id );
	virtual int CreateNewTextureID( bool procedural );
	virtual void GetScreenSize( int &wide, int &tall );
	virtual void SetAsTopMost( vgui2::VPANEL panel, bool state );
	virtual void BringToFront( vgui2::VPANEL panel );
	virtual void SetForegroundWindow( vgui2::VPANEL panel );
	virtual void SetPanelVisible( vgui2::VPANEL panel, bool state );
	virtual void SetMinimized( vgui2::VPANEL panel, bool state );
	virtual bool IsMinimized( vgui2::VPANEL panel );
	virtual void FlashWindow( vgui2::VPANEL panel, bool state );
	virtual void SetTitle( vgui2::VPANEL panel, const wchar_t *title );
	virtual void SetAsToolBar( vgui2::VPANEL panel, bool state );
	virtual void CreatePopup( vgui2::VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput );
	virtual void SwapBuffers( vgui2::VPANEL panel );
	virtual void Invalidate( vgui2::VPANEL panel );
	virtual void SetCursor( vgui2::HCursor cursor );
	virtual bool IsCursorVisible();
	virtual void ApplyChanges();
	virtual bool IsWithin( int x, int y );
	virtual bool HasFocus();
	virtual bool SupportsFeature( SurfaceFeature_e feature );
	virtual void RestrictPaintToSinglePanel( vgui2::VPANEL panel );
	virtual void SetModalPanel( vgui2::VPANEL panel );
	virtual vgui2::VPANEL GetModalPanel();
	virtual void UnlockCursor();
	virtual void LockCursor();
	virtual void SetTranslateExtendedKeys( bool state );
	virtual vgui2::VPANEL GetTopmostPopup();
	virtual void SetTopLevelFocus( vgui2::VPANEL panel );
	virtual vgui2::HFont CreateFont();
	virtual bool AddGlyphSetToFont( vgui2::HFont font, const char *fontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange );
	virtual bool AddCustomFontFile( const char *fontFileName );
	virtual int GetFontTall( vgui2::HFont font );
	virtual void GetCharABCwide( vgui2::HFont font, int ch, int &a, int &b, int &c );
	virtual int GetCharacterWidth( vgui2::HFont font, int ch );
	virtual void GetTextSize( vgui2::HFont font, const wchar_t *text, int &wide, int &tall );
	virtual vgui2::VPANEL GetNotifyPanel();
	virtual void SetNotifyIcon( vgui2::VPANEL context, vgui2::HTexture icon, vgui2::VPANEL panelToReceiveMessages, const char *text );
	virtual void PlaySound( const char *fileName );
	virtual int GetPopupCount();
	virtual vgui2::VPANEL GetPopup( int index );
	virtual bool ShouldPaintChildPanel( vgui2::VPANEL panel );
	virtual bool RecreateContext( vgui2::VPANEL panel );
	virtual void AddPanel( vgui2::VPANEL panel );
	virtual void ReleasePanel( vgui2::VPANEL panel );
	virtual void MovePopupToFront( vgui2::VPANEL panel );
	virtual void MovePopupToBack( vgui2::VPANEL panel );
	virtual void SolveTraverse( vgui2::VPANEL panel, bool forceApplySchemeSettings );
	virtual void PaintTraverse( vgui2::VPANEL panel );
	virtual void EnableMouseCapture( vgui2::VPANEL panel, bool state );
	virtual void GetWorkspaceBounds( int &x, int &y, int &wide, int &tall );
	virtual void GetAbsoluteWindowBounds( int &x, int &y, int &wide, int &tall );
	virtual void GetProportionalBase( int &width, int &height );
	virtual void CalculateMouseVisible();
	virtual bool NeedKBInput();
	virtual bool HasCursorPosFunctions();
	virtual void SurfaceGetCursorPos( int &x, int &y );
	virtual void SurfaceSetCursorPos( int x, int y );
	virtual void DrawTexturedPolygon( vgui2::VGuiVertex *pVertices, int n );
	virtual int GetFontAscent( vgui2::HFont font, wchar_t wch );
	virtual void SetAllowHTMLJavaScript( bool state );
	virtual void SetLanguage( const char *pchLang );
	virtual const char *GetLanguage();
	virtual bool DeleteTextureByID( int id );
	virtual void DrawUpdateRegionTextureBGRA( int nTextureID, int x, int y, const unsigned char *pchData, int wide, int tall );
	virtual void DrawSetTextureBGRA( int id, const unsigned char *bgra, int wide, int tall );
	virtual void CreateBrowser( vgui2::VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier );
	virtual void RemoveBrowser( vgui2::VPANEL panel, IHTMLResponses *pBrowser );
	virtual IHTMLChromeController *AccessChromeHTMLController();

	VGUI2Surface();
	void Init( vgui2::VPANEL _embeddedPanel, IHTMLChromeController *pChromeController );
	void SetScreenBounds( int x, int y, int wide, int tall );
	void SetIgnoreMouseVisCalc( bool state );
	void InternalSchemeSettingsTraverse( vgui2::VPANEL panel, bool forceApplySchemeSettings );
	void InternalThinkTraverse( vgui2::VPANEL panel );
	void InternalSolveTraverse( vgui2::VPANEL panel );

private:
	vgui2::VPANEL embeddedPanel;
	vgui2::VPANEL restrictedPanel;
	char language[32];
	IHTMLChromeController *chromeController;
	int surfaceBounds[4];
	vgui::Dar<vgui2::VPANEL> popups;
	bool allowJavaScript;
	bool needKB;
	bool needMouse;
	bool cursorLocked;
	bool translateExtendedKeys;
	bool ignoreMouseVisCalc;
	vgui2::HCursor currentCursor;
};

#endif // VGUI2_SURFACE_H

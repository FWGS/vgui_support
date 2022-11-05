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
	void Shutdown() override;
	void RunFrame() override;
	vgui2::VPANEL GetEmbeddedPanel() override;
	void SetEmbeddedPanel( vgui2::VPANEL panel ) override;
	void PushMakeCurrent( vgui2::VPANEL panel, bool useInsets ) override;
	void PopMakeCurrent( vgui2::VPANEL panel ) override;
	void DrawSetColor( int r, int g, int b, int a ) override;
	void DrawSetColor( Color col ) override;
	void DrawFilledRect( int x0, int y0, int x1, int y1 ) override;
	void DrawOutlinedRect( int x0, int y0, int x1, int y1 ) override;
	void DrawLine( int x0, int y0, int x1, int y1 ) override;
	void DrawPolyLine( int *px, int *py, int numPoints ) override;
	void DrawSetTextFont( vgui2::HFont font ) override;
	void DrawSetTextColor( int r, int g, int b, int a ) override;
	void DrawSetTextColor( Color col ) override;
	void DrawSetTextPos( int x, int y ) override;
	void DrawGetTextPos( int &x, int &y ) override;
	void DrawPrintText( const wchar_t *text, int textLen ) override;
	void DrawUnicodeChar( wchar_t wch ) override;
	void DrawUnicodeCharAdd( wchar_t wch ) override;
	void DrawFlushText() override;
	vgui2::IHTML *CreateHTMLWindow( vgui2::IHTMLEvents *events, vgui2::VPANEL context ) override;
	void PaintHTMLWindow( vgui2::IHTML *htmlwin ) override;
	void DeleteHTMLWindow( vgui2::IHTML *htmlwin ) override;
	void DrawSetTextureFile( int id, const char *filename, int hardwareFilter, bool forceReload ) override;
	void DrawSetTextureRGBA( int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload ) override;
	void DrawSetTexture( int id ) override;
	void DrawGetTextureSize( int id, int &wide, int &tall ) override;
	void DrawTexturedRect( int x0, int y0, int x1, int y1 ) override;
	bool IsTextureIDValid( int id ) override;
	int CreateNewTextureID( bool procedural ) override;
	void GetScreenSize( int &wide, int &tall ) override;
	void SetAsTopMost( vgui2::VPANEL panel, bool state ) override;
	void BringToFront( vgui2::VPANEL panel ) override;
	void SetForegroundWindow( vgui2::VPANEL panel ) override;
	void SetPanelVisible( vgui2::VPANEL panel, bool state ) override;
	void SetMinimized( vgui2::VPANEL panel, bool state ) override;
	bool IsMinimized( vgui2::VPANEL panel ) override;
	void FlashWindow( vgui2::VPANEL panel, bool state ) override;
	void SetTitle( vgui2::VPANEL panel, const wchar_t *title ) override;
	void SetAsToolBar( vgui2::VPANEL panel, bool state ) override;
	void CreatePopup( vgui2::VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput ) override;
	void SwapBuffers( vgui2::VPANEL panel ) override;
	void Invalidate( vgui2::VPANEL panel ) override;
	void SetCursor( vgui2::HCursor cursor ) override;
	bool IsCursorVisible() override;
	void ApplyChanges() override;
	bool IsWithin( int x, int y ) override;
	bool HasFocus() override;
	bool SupportsFeature( SurfaceFeature_e feature ) override;
	void RestrictPaintToSinglePanel( vgui2::VPANEL panel ) override;
	void SetModalPanel( vgui2::VPANEL panel ) override;
	vgui2::VPANEL GetModalPanel() override;
	void UnlockCursor() override;
	void LockCursor() override;
	void SetTranslateExtendedKeys( bool state ) override;
	vgui2::VPANEL GetTopmostPopup() override;
	void SetTopLevelFocus( vgui2::VPANEL panel ) override;
	vgui2::HFont CreateFont() override;
	bool AddGlyphSetToFont( vgui2::HFont font, const char *fontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange ) override;
	bool AddCustomFontFile( const char *fontFileName ) override;
	int GetFontTall( vgui2::HFont font ) override;
	void GetCharABCwide( vgui2::HFont font, int ch, int &a, int &b, int &c ) override;
	int GetCharacterWidth( vgui2::HFont font, int ch ) override;
	void GetTextSize( vgui2::HFont font, const wchar_t *text, int &wide, int &tall ) override;
	vgui2::VPANEL GetNotifyPanel() override;
	void SetNotifyIcon( vgui2::VPANEL context, vgui2::HTexture icon, vgui2::VPANEL panelToReceiveMessages, const char *text ) override;
	void PlaySound( const char *fileName ) override;
	int GetPopupCount() override;
	vgui2::VPANEL GetPopup( int index ) override;
	bool ShouldPaintChildPanel( vgui2::VPANEL panel ) override;
	bool RecreateContext( vgui2::VPANEL panel ) override;
	void AddPanel( vgui2::VPANEL panel ) override;
	void ReleasePanel( vgui2::VPANEL panel ) override;
	void MovePopupToFront( vgui2::VPANEL panel ) override;
	void MovePopupToBack( vgui2::VPANEL panel ) override;
	void SolveTraverse( vgui2::VPANEL panel, bool forceApplySchemeSettings ) override;
	void PaintTraverse( vgui2::VPANEL panel ) override;
	void EnableMouseCapture( vgui2::VPANEL panel, bool state ) override;
	void GetWorkspaceBounds( int &x, int &y, int &wide, int &tall ) override;
	void GetAbsoluteWindowBounds( int &x, int &y, int &wide, int &tall ) override;
	void GetProportionalBase( int &width, int &height ) override;
	void CalculateMouseVisible() override;
	bool NeedKBInput() override;
	bool HasCursorPosFunctions() override;
	void SurfaceGetCursorPos( int &x, int &y ) override;
	void SurfaceSetCursorPos( int x, int y ) override;
	void DrawTexturedPolygon( vgui2::VGuiVertex *pVertices, int n ) override;
	int GetFontAscent( vgui2::HFont font, wchar_t wch ) override;
	void SetAllowHTMLJavaScript( bool state ) override;
	void SetLanguage( const char *pchLang ) override;
	const char *GetLanguage() override;
	bool DeleteTextureByID( int id ) override;
	void DrawUpdateRegionTextureBGRA( int nTextureID, int x, int y, const unsigned char *pchData, int wide, int tall ) override;
	void DrawSetTextureBGRA( int id, const unsigned char *bgra, int wide, int tall ) override;
	void CreateBrowser( vgui2::VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier ) override;
	void RemoveBrowser( vgui2::VPANEL panel, IHTMLResponses *pBrowser ) override;
	IHTMLChromeController *AccessChromeHTMLController() override;

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

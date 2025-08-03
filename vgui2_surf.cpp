/*
vgui2_surf.cpp - vgui2 surface implementation
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

#include <wchar.h>
#include <Cursor.h>
#include <crtlib.h>
#include "vgui_main.h"
#include "vgui2_surf.h"

using namespace vgui_support;

static const auto VPANEL_NORMAL = ( (vgui2::SurfacePlat *)NULL );
static const auto VPANEL_MINIMIZED = ( (vgui2::SurfacePlat *)0x00000001 );

void VGUI2Surface::Shutdown()
{
	if( chromeController )
		chromeController->Shutdown();
}

void VGUI2Surface::RunFrame()
{
	if( chromeController )
		chromeController->RunFrame();
}

vgui2::VPANEL VGUI2Surface::GetEmbeddedPanel()
{
	return embeddedPanel;
}

void VGUI2Surface::SetEmbeddedPanel( vgui2::VPANEL panel )
{
	embeddedPanel = panel;
}

void VGUI2Surface::PushMakeCurrent( vgui2::VPANEL panel, bool useInsets )
{
	int insets[4] = { 0, 0, 0, 0 };
	int absExtents[4];
	int clipRect[4];
	int wide, tall;

	if( useInsets )
		vgui2::ipanel()->GetInset( panel, insets[0], insets[1], insets[2], insets[3] );

	vgui2::ipanel()->GetAbsPos( panel, absExtents[0], absExtents[1] );
	vgui2::ipanel()->GetSize( panel, wide, tall );
	absExtents[2] = absExtents[0] + wide;
	absExtents[3] = absExtents[1] + tall;

	vgui2::ipanel()->GetClipRect( panel, clipRect[0], clipRect[1], clipRect[2], clipRect[3] );

	VGUIPanel p( panel );
	surface->PushMakeCurrent( p, insets, absExtents, clipRect );
}

void VGUI2Surface::PopMakeCurrent( vgui2::VPANEL panel )
{
	VGUIPanel p( panel );
	surface->PopMakeCurrent( p );
}

void VGUI2Surface::DrawSetColor( int r, int g, int b, int a )
{
	// VGUI2 inverts alpha

	surface->drawSetColor( r, g, b, 255 - a );
}

void VGUI2Surface::DrawSetColor( Color col )
{
	DrawSetColor( col[0], col[1], col[2], col[3] );
}

void VGUI2Surface::DrawFilledRect( int x0, int y0, int x1, int y1 )
{
	surface->drawFilledRect( x0, y0, x1, y1 );
}

void VGUI2Surface::DrawOutlinedRect( int x0, int y0, int x1, int y1 )
{
	surface->drawOutlinedRect( x0, y0, x1, y1 );
}

void VGUI2Surface::DrawLine( int x0, int y0, int x1, int y1 )
{
	surface->drawLine( x0, y0, x1, y1 );
}

void VGUI2Surface::DrawPolyLine( int *px, int *py, int numPoints )
{
	surface->drawPolyLine( px, py, numPoints );
}

void VGUI2Surface::DrawSetTextFont( vgui2::HFont font )
{
	surface->drawSetTextFont( font );
}

void VGUI2Surface::DrawSetTextColor( int r, int g, int b, int a )
{
	surface->drawSetTextColor( r, g, b, 255 - a );
}

void VGUI2Surface::DrawSetTextColor( Color col )
{
	DrawSetTextColor( col[0], col[1], col[2], col[3] );
}

void VGUI2Surface::DrawSetTextPos( int x, int y )
{
	surface->drawSetTextPos( x, y );
}

void VGUI2Surface::DrawGetTextPos( int &x, int &y )
{
	surface->drawGetTextPos( x, y );
}

void VGUI2Surface::DrawPrintText( const wchar_t *text, int textLen )
{
	if( !text || textLen <= 0 )
		return;

	auto str = new char[textLen + 1];

	wcstombs( str, text, textLen );
	surface->drawPrintText( str, textLen );

	delete[] str;
	// engineSurface->DrawPrintText( text, textLen );
}

void VGUI2Surface::DrawUnicodeChar( wchar_t wch )
{
	char s[16]; // MB_CUR_MAX makes it VLA
	auto len = wctomb( s, wch );

	surface->drawPrintText( s, len );
}

void VGUI2Surface::DrawUnicodeCharAdd( wchar_t wch )
{
	// TODO: any difference?
	DrawUnicodeChar( wch );
}

void VGUI2Surface::DrawFlushText()
{
	// TODO: stub
}

vgui2::IHTML *VGUI2Surface::CreateHTMLWindow( vgui2::IHTMLEvents *events, vgui2::VPANEL context )
{
	return nullptr;
}

void VGUI2Surface::PaintHTMLWindow( vgui2::IHTML *htmlwin )
{
}

void VGUI2Surface::DeleteHTMLWindow( vgui2::IHTML *htmlwin )
{
}

void VGUI2Surface::DrawSetTextureFile( int id, const char *filename, int hardwareFilter, bool forceReload )
{
	surface->drawSetTextureFile( id, filename );
}

void VGUI2Surface::DrawSetTextureRGBA( int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload )
{
	surface->drawSetTextureRGBA( id, (const char *)rgba, wide, tall );
}

void VGUI2Surface::DrawSetTexture( int id )
{
	surface->drawSetTexture( id );
}

void VGUI2Surface::DrawGetTextureSize( int id, int &wide, int &tall )
{
	surface->drawGetTextureSize( id, wide, tall );
}

void VGUI2Surface::DrawTexturedRect( int x0, int y0, int x1, int y1 )
{
	surface->drawTexturedRect( x0, y0, x1, y1 );
}

bool VGUI2Surface::IsTextureIDValid( int id )
{
	// TODO: do anything here?
	return true;
}

int VGUI2Surface::CreateNewTextureID( bool procedural )
{
	return surface->createNewTextureID();
}

void VGUI2Surface::GetScreenSize( int &wide, int &tall )
{
	VGUI2_ScreenSize( wide, tall );
}

void VGUI2Surface::SetAsTopMost( vgui2::VPANEL panel, bool state )
{
}

void VGUI2Surface::BringToFront( vgui2::VPANEL panel )
{
	vgui2::ipanel()->MoveToFront( panel );

	if( vgui2::ipanel()->IsPopup( panel ) )
		MovePopupToFront( panel );
}

void VGUI2Surface::SetForegroundWindow( vgui2::VPANEL panel )
{
	BringToFront( panel );
}

void VGUI2Surface::SetPanelVisible( vgui2::VPANEL panel, bool state )
{
}

void VGUI2Surface::SetMinimized( vgui2::VPANEL panel, bool state )
{
	if( !state )
	{
		vgui2::ipanel()->SetPlat( panel, VPANEL_NORMAL );
		return;
	}

	vgui2::ipanel()->SetPlat( panel, VPANEL_MINIMIZED );
	vgui2::ipanel()->SetVisible( panel, false );
}

bool VGUI2Surface::IsMinimized( vgui2::VPANEL panel )
{
	return vgui2::ipanel()->Plat( panel ) == VPANEL_MINIMIZED;
}

void VGUI2Surface::FlashWindow( vgui2::VPANEL panel, bool state )
{
}

void VGUI2Surface::SetTitle( vgui2::VPANEL panel, const wchar_t *title )
{
}

void VGUI2Surface::SetAsToolBar( vgui2::VPANEL panel, bool state )
{
}

void VGUI2Surface::CreatePopup( vgui2::VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput )
{
	if( vgui2::ipanel()->GetParent( panel ) == NULL )
		vgui2::ipanel()->SetParent( panel, GetEmbeddedPanel() );

	vgui2::ipanel()->SetPopup( panel, true );
	vgui2::ipanel()->SetKeyBoardInputEnabled( panel, kbInput );
	vgui2::ipanel()->SetMouseInputEnabled( panel, mouseInput );

	if( !popups.hasElement( panel ) )
		popups.addElement( panel );
}

void VGUI2Surface::SwapBuffers( vgui2::VPANEL panel )
{
}

void VGUI2Surface::Invalidate( vgui2::VPANEL panel )
{
}

void VGUI2Surface::SetCursor( vgui2::HCursor cursor )
{
	// These cursors are not available in VGUI1, so ignore them for now.
	if( cursor >= vgui2::dc_last || cursor == vgui2::dc_waitarrow || cursor == vgui2::dc_blank )
		return;

	if( currentCursor != cursor )
	{
		currentCursor = cursor;

		// Hack to make it compatible with VGUI1
		if( cursor > vgui2::dc_waitarrow )
			--cursor;

		g_api->CursorSelect( (VGUI_DefaultCursor)cursor );
	}
}

bool VGUI2Surface::IsCursorVisible()
{
	return currentCursor != vgui2::dc_none;
}

void VGUI2Surface::ApplyChanges()
{
}

bool VGUI2Surface::IsWithin( int x, int y )
{
	return true;
}

bool VGUI2Surface::HasFocus()
{
	return true;
}

bool VGUI2Surface::SupportsFeature( SurfaceFeature_e feature )
{
	return false;
}

void VGUI2Surface::RestrictPaintToSinglePanel( vgui2::VPANEL panel )
{
	restrictedPanel = panel;
	vgui2::input()->SetAppModalSurface( panel );
}

void VGUI2Surface::SetModalPanel( vgui2::VPANEL panel )
{
}

vgui2::VPANEL VGUI2Surface::GetModalPanel()
{
	return restrictedPanel;
}

void VGUI2Surface::UnlockCursor()
{
	cursorLocked = false;
}

void VGUI2Surface::LockCursor()
{
	cursorLocked = true;
}

void VGUI2Surface::SetTranslateExtendedKeys( bool state )
{
	translateExtendedKeys = state;
}

vgui2::VPANEL VGUI2Surface::GetTopmostPopup()
{
	if( popups.getCount() > 0 )
		return popups[popups.getCount() - 1];

	return NULL;
}

void VGUI2Surface::SetTopLevelFocus( vgui2::VPANEL panel )
{
	while ( panel )
	{
		if( vgui2::ipanel()->IsPopup( panel ) && vgui2::ipanel()->IsMouseInputEnabled( panel ) )
		{
			BringToFront( panel );
			break;
		}

		panel = vgui2::ipanel()->GetParent( panel );
	}
}

vgui2::HFont VGUI2Surface::CreateFont()
{
	return surface->createFont();
}

bool VGUI2Surface::AddGlyphSetToFont( vgui2::HFont font, const char *fontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange )
{
	return surface->addGlyphSetToFont( font, fontName, tall, weight, flags & FONTFLAG_ITALIC, flags & FONTFLAG_UNDERLINE, flags & FONTFLAG_STRIKEOUT, flags & FONTFLAG_SYMBOL );
}

bool VGUI2Surface::AddCustomFontFile( const char *fontFileName )
{
	return surface->addCustomFontFile( fontFileName );
}

int VGUI2Surface::GetFontTall( vgui2::HFont font )
{
	return surface->getFontTall( font );
}

void VGUI2Surface::GetCharABCwide( vgui2::HFont font, int ch, int &a, int &b, int &c )
{
	a = b = c = 1;
	surface->getCharABCWide( font,ch, a, b, c );
}

int VGUI2Surface::GetCharacterWidth( vgui2::HFont font, int ch )
{
	int a, b, c;
	GetCharABCwide( font, ch, a, b, c );
	return a + b + c;
}

void VGUI2Surface::GetTextSize( vgui2::HFont font, const wchar_t *text, int &wide, int &tall )
{
	if( !text )
	{
		wide = 1;
		tall = 1;
		return;
	}

	auto len = wcslen( text );
	auto str = new char[len + 1];

	wcstombs( str, text, len );
	surface->getTextSize( font, str, wide, tall );

	delete[] str;
}

vgui2::VPANEL VGUI2Surface::GetNotifyPanel()
{
	return NULL;
}

void VGUI2Surface::SetNotifyIcon( vgui2::VPANEL context, vgui2::HTexture icon, vgui2::VPANEL panelToReceiveMessages, const char *text )
{
}

void VGUI2Surface::PlaySound( const char *fileName )
{
	surface->playSound( fileName );
}

int VGUI2Surface::GetPopupCount()
{
	return popups.getCount();
}

vgui2::VPANEL VGUI2Surface::GetPopup( int index )
{
	if( 0 <= index && index < popups.getCount() )
		return popups[index];

	return NULL;
}

bool VGUI2Surface::ShouldPaintChildPanel( vgui2::VPANEL panel )
{
	if( restrictedPanel != NULL && vgui2::ipanel()->HasParent( panel, restrictedPanel ) )
		return false;

	if( !vgui2::ipanel()->IsPopup( panel ) )
		return true;

	if( popups.hasElement( panel ) )
		vgui2::ipanel()->Render_SetPopupVisible( panel, true );

	return false;
}

bool VGUI2Surface::RecreateContext( vgui2::VPANEL panel )
{
	return true;
}

void VGUI2Surface::AddPanel( vgui2::VPANEL panel )
{
	if( !vgui2::ipanel()->IsPopup( panel ) )
		return;

	CreatePopup( panel, false, false, false, true, true );
}

void VGUI2Surface::ReleasePanel( vgui2::VPANEL panel )
{
	popups.removeElement( panel );

	if( restrictedPanel == panel )
		restrictedPanel = NULL;
}

void VGUI2Surface::MovePopupToFront( vgui2::VPANEL panel )
{
	if( popups.hasElement( panel ) )
	{
		popups.removeElement( panel );
		popups.addElement( panel );
	}
}

void VGUI2Surface::MovePopupToBack( vgui2::VPANEL panel )
{
	if( popups.hasElement( panel ) )
	{
		popups.removeElement( panel );
		popups.insertElementAt( panel, 0 );
	}
}

void VGUI2Surface::SolveTraverse( vgui2::VPANEL panel, bool forceApplySchemeSettings )
{
	InternalSchemeSettingsTraverse( panel, forceApplySchemeSettings );
	InternalThinkTraverse( panel );
	InternalSolveTraverse( panel );
}

void VGUI2Surface::PaintTraverse( vgui2::VPANEL panel )
{
	if( !vgui2::ipanel()->IsVisible( panel ) )
		return;

	if( panel != GetEmbeddedPanel() )
	{
		vgui2::ipanel()->PaintTraverse( panel, true, true );
		return;
	}

	if( restrictedPanel )
		panel = restrictedPanel;

	for( int i = 0; i < popups.getCount(); ++i )
		vgui2::ipanel()->Render_SetPopupVisible( popups[i], false );

	vgui2::ipanel()->PaintTraverse( panel, true, true );

	for( int i = 0; i < popups.getCount(); ++i )
	{
		if( vgui2::ipanel()->Render_GetPopupVisible( popups[i] ) )
			vgui2::ipanel()->PaintTraverse( popups[i], true, true );
	}
}

void VGUI2Surface::EnableMouseCapture( vgui2::VPANEL panel, bool state )
{
}

void VGUI2Surface::GetWorkspaceBounds( int &x, int &y, int &wide, int &tall )
{
	x = 0;
	y = 0;
	vgui2::ipanel()->GetSize( embeddedPanel, wide, tall );
}

void VGUI2Surface::GetAbsoluteWindowBounds( int &x, int &y, int &wide, int &tall )
{
	x = surfaceBounds[0];
	y = surfaceBounds[1];
	wide = surfaceBounds[2];
	tall = surfaceBounds[3];
}

void VGUI2Surface::GetProportionalBase( int &width, int &height )
{
	width = 640;
	height = 480;
}

void VGUI2Surface::CalculateMouseVisible()
{
	if( ignoreMouseVisCalc )
		return;

	needMouse = false;
	needKB = false;

	for( int i = 0; i < popups.getCount(); ++i )
	{
		bool visible = vgui2::ipanel()->IsVisible( popups[i] );
		vgui2::VPANEL parent = vgui2::ipanel()->GetParent( popups[i] );

		while ( parent != NULL && visible )
		{
			if( !vgui2::ipanel()->IsVisible( parent ) )
			{
				visible = false;
				break;
			}

			parent = vgui2::ipanel()->GetParent( parent );
		}

		if( !visible )
			continue;

		// TODO: Uncomment these once fully implemented to enable input capture
		needMouse = needMouse || vgui2::ipanel()->IsMouseInputEnabled( popups[i] );
		needKB = needKB || vgui2::ipanel()->IsKeyBoardInputEnabled( popups[i] );
	}

	UnlockCursor();

	if( needMouse )
	{
		SetCursor( vgui2::dc_arrow );
	}
	else
	{
		SetCursor( vgui2::dc_none );
		LockCursor();
	}
}

bool VGUI2Surface::NeedKBInput()
{
	return needKB;
}

bool VGUI2Surface::HasCursorPosFunctions()
{
	return true;
}

void VGUI2Surface::SurfaceGetCursorPos( int &x, int &y )
{
	g_api->GetCursorPos( &x, &y );
}

void VGUI2Surface::SurfaceSetCursorPos( int x, int y )
{
	// g_api->SetCursorPos( x, y );
}

void VGUI2Surface::DrawTexturedPolygon( vgui2::VGuiVertex *pVertices, int n )
{
	surface->drawTexturedPolygon( pVertices, n );
}

int VGUI2Surface::GetFontAscent( vgui2::HFont font, wchar_t wch )
{
	return surface->getFontAscent( font, wch );
}

void VGUI2Surface::SetAllowHTMLJavaScript( bool state )
{
	allowJavaScript = state;
}

void VGUI2Surface::SetLanguage( const char *pchLang )
{
	if( pchLang != nullptr )
	{
		Q_strncpy( language, pchLang, sizeof( language ));
	}
	else
	{
		Q_strncpy( language, "english", sizeof( language ));
	}
}

const char *VGUI2Surface::GetLanguage()
{
	return language;
}

bool VGUI2Surface::DeleteTextureByID( int id )
{
	return surface->deleteTextureByID( id );
}

void VGUI2Surface::DrawUpdateRegionTextureBGRA( int id, int x, int y, const unsigned char *bgra, int wide, int tall )
{
	auto p = new unsigned char[wide * tall * 4];
	for( int i = 0; i < wide * tall * 4; i += 4 )
	{
		p[i + 0] = bgra[i + 2];
		p[i + 1] = bgra[i + 1];
		p[i + 2] = bgra[i + 0];
		p[i + 3] = bgra[i + 3];
	}
	surface->drawSubTextureRGBA( id, x, y, p, wide, tall );
	delete[] p;
}

void VGUI2Surface::DrawSetTextureBGRA( int id, const unsigned char *bgra, int wide, int tall )
{
	auto p = new unsigned char[wide * tall * 4];
	for( int i = 0; i < wide * tall * 4; i += 4 )
	{
		p[i + 0] = bgra[i + 2];
		p[i + 1] = bgra[i + 1];
		p[i + 2] = bgra[i + 0];
		p[i + 3] = bgra[i + 3];
	}
	surface->drawSetTextureRGBA( id, (const char *)p, wide, tall );
	delete[] p;
}

void VGUI2Surface::CreateBrowser( vgui2::VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier )
{
	if( chromeController )
		chromeController->CreateBrowser( pBrowser, false, "Valve Half-Life" );
}

void VGUI2Surface::RemoveBrowser( vgui2::VPANEL panel, IHTMLResponses *pBrowser )
{
	if( chromeController )
		chromeController->RemoveBrowser( pBrowser );
}

IHTMLChromeController *VGUI2Surface::AccessChromeHTMLController()
{
	return chromeController;
}

VGUI2Surface::VGUI2Surface()
{
	embeddedPanel = NULL;
	restrictedPanel = NULL;
	chromeController = nullptr;
	allowJavaScript = false;
	cursorLocked = false;
	translateExtendedKeys = true;
	ignoreMouseVisCalc = false;
	currentCursor = vgui2::dc_none;

	// TODO: Change these to true once fully implemented to enable input capture
	needKB = false;
	needMouse = false;
}

void VGUI2Surface::Init( vgui2::VPANEL _embeddedPanel, IHTMLChromeController *pChromeController )
{
	SetEmbeddedPanel( _embeddedPanel );

	chromeController = pChromeController;
	if( chromeController )
	{
		chromeController->Init( "htmlcache", "htmlcookies" );
		chromeController->SetCefThreadTargetFrameRate( 60 );
	}
}

void VGUI2Surface::SetScreenBounds( int x, int y, int wide, int tall )
{
	surfaceBounds[0] = x;
	surfaceBounds[1] = y;
	surfaceBounds[2] = wide;
	surfaceBounds[3] = tall;
}

void VGUI2Surface::SetIgnoreMouseVisCalc( bool state )
{
	ignoreMouseVisCalc = state;
}

void VGUI2Surface::InternalSchemeSettingsTraverse( vgui2::VPANEL panel, bool forceApplySchemeSettings )
{
	for( int i = 0; i < vgui2::ipanel()->GetChildCount( panel ); ++i )
	{
		vgui2::VPANEL child = vgui2::ipanel()->GetChild( panel, i );
		if( forceApplySchemeSettings || vgui2::ipanel()->IsVisible( child ) )
			InternalSchemeSettingsTraverse( child, forceApplySchemeSettings );
	}

	vgui2::ipanel()->PerformApplySchemeSettings( panel );
}

void VGUI2Surface::InternalThinkTraverse( vgui2::VPANEL panel )
{
	vgui2::ipanel()->Think( panel );

	for( int i = 0; i < vgui2::ipanel()->GetChildCount( panel ); ++i )
	{
		vgui2::VPANEL child = vgui2::ipanel()->GetChild( panel, i );
		if( vgui2::ipanel()->IsVisible( child ) )
			InternalThinkTraverse( child );
	}
}

void VGUI2Surface::InternalSolveTraverse( vgui2::VPANEL panel )
{
	vgui2::ipanel()->Solve( panel );

	for( int i = 0; i < vgui2::ipanel()->GetChildCount( panel ); ++i )
	{
		vgui2::VPANEL child = vgui2::ipanel()->GetChild( panel, i );
		if( vgui2::ipanel()->IsVisible( child ) )
			InternalSolveTraverse( child );
	}
}

void VGUI2Surface::DrawTexturedRectAdd( int x0, int y0, int x1, int y1 )
{
	// TODO: Implement additive textured rectangle drawing
	// This is similar to DrawTexturedRect but with additive blending
}

void VGUI2Surface::SetSupportsEsc( bool bSupportsEsc )
{
	// TODO: Implement ESC key support setting
	// This controls whether the surface supports ESC key handling
}

int VGUI2Surface::GetFontBlur( vgui2::HFont font )
{
	// TODO: Implement font blur retrieval
	// This should return the blur value for the given font
	return 0; // Default no blur
}

bool VGUI2Surface::IsAdditive( vgui2::HFont font )
{
	// TODO: Implement additive font check
	// This should return whether the font uses additive blending
	return false; // Default non-additive
}

void VGUI2Surface::SetProportionalBase( int width, int height )
{
	// TODO: Implement proportional base setting
	// This sets the base resolution for proportional scaling
}

void VGUI2Surface::GetHDProportionalBase( int &width, int &height )
{
	// TODO: Implement HD proportional base retrieval
	// This gets the HD base resolution for proportional scaling
	width = 1920;  // Default HD width
	height = 1080; // Default HD height
}

void VGUI2Surface::SetHDProportionalBase( int nWidth, int nHeight )
{
	// TODO: Implement HD proportional base setting
	// This sets the HD base resolution for proportional scaling
}

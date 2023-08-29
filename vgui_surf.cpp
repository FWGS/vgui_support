/*
vgui_surf.cpp - main vgui layer
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

#include <ctype.h>
#include <crtlib.h>
#include "vgui_main.h"

#define MAX_PAINT_STACK	16
#define FONT_SIZE		512
#define FONT_PAGES	8

using namespace vgui_support;
using namespace vgui;

struct FontInfo
{
	FontInfo( int id, int contextCount ) : id( id ), pageCount(),
		pageForChar(), bindIndex(), contextCount( contextCount )
	{

	}

	int	id;
	int	pageCount;
	int	pageForChar[256];
	int	bindIndex[FONT_PAGES];
	float	texCoord[256][FONT_PAGES];
	int	contextCount;
};

static int staticContextCount = 0;
static char staticRGBA[FONT_SIZE * FONT_SIZE * 4];
static Font* staticFont = NULL;
static FontInfo* staticFontInfo;
static Dar<FontInfo*> staticFontInfoDar;
static PaintStack paintStack[MAX_PAINT_STACK];
static int staticPaintStackPos = 0;

#define ColorIndex( c )((( c ) - '0' ) & 7 )

CEngineSurface::CEngineSurface( Panel *embeddedPanel ) :
	SurfaceBase( embeddedPanel )
{
	_embeddedPanel = embeddedPanel;
	_drawColor[0] = _drawColor[1] = _drawColor[2] = _drawColor[3] = 255;
	_drawTextColor[0] = _drawTextColor[1] = _drawTextColor[2] = _drawTextColor[3] = 255;

	_surfaceExtents[0] = _surfaceExtents[1] = 0;
	//_surfaceExtents[2] = menu.globals->scrWidth;
	//_surfaceExtents[3] = menu.globals->scrHeight;
	embeddedPanel->getSize(_surfaceExtents[2], _surfaceExtents[3]);
	_drawTextPos[0] = _drawTextPos[1] = 0;
	_hCurrentCursor = null;

	staticFont = NULL;
	staticFontInfo = NULL;
	staticFontInfoDar.setCount( 0 );
	staticPaintStackPos = 0;
	staticContextCount++;

	_translateX = _translateY = 0;
}

CEngineSurface::~CEngineSurface( void )
{
	g_api->DrawShutdown ();
}

Panel *CEngineSurface::getEmbeddedPanel( void )
{
	return _embeddedPanel;
}

bool CEngineSurface::hasFocus( void )
{
	return true;
}
	
void CEngineSurface::setCursor( Cursor *cursor )
{
	_currentCursor = cursor;

	if( cursor )
	{
		g_api->CursorSelect( (VGUI_DefaultCursor)cursor->getDefaultCursor() );
	}
}

void CEngineSurface::SetupPaintState( const PaintStack *paintState )
{
	_translateX = paintState->iTranslateX;
	_translateY = paintState->iTranslateY;
	SetScissorRect( paintState->iScissorLeft, paintState->iScissorTop,
		paintState->iScissorRight, paintState->iScissorBottom );
}

void CEngineSurface::InitVertex( vpoint_t &vertex, int x, int y, float u, float v )
{
	vertex.point[0] = x + _translateX;
	vertex.point[1] = y + _translateY;
	vertex.coord[0] = u;
	vertex.coord[1] = v;
}

int CEngineSurface::createNewTextureID( void )
{
	return g_api->GenerateTexture();
}

void CEngineSurface::drawSetColor( int r, int g, int b, int a )
{
	_drawColor[0] = r;
	_drawColor[1] = g;
	_drawColor[2] = b;
	_drawColor[3] = a;
}

void CEngineSurface::drawSetTextColor( int r, int g, int b, int a )
{
	_drawTextColor[0] = r;
	_drawTextColor[1] = g;
	_drawTextColor[2] = b;
	_drawTextColor[3] = a;
}

void CEngineSurface::drawFilledRect( int x0, int y0, int x1, int y1 )
{
	vpoint_t rect[2];
	vpoint_t clippedRect[2];

	if( _drawColor[3] >= 255 ) return;

	InitVertex( rect[0], x0, y0, 0, 0 );
	InitVertex( rect[1], x1, y1, 0, 0 );

	// fully clipped?
	if( !ClipRect( rect[0], rect[1], &clippedRect[0], &clippedRect[1] ))
		return;	

	g_api->SetupDrawingRect( _drawColor );	
	g_api->EnableTexture( false );
	g_api->DrawQuad( &clippedRect[0], &clippedRect[1] );
	g_api->EnableTexture( true );
}

void CEngineSurface::drawOutlinedRect( int x0, int y0, int x1, int y1 )
{
	if( _drawColor[3] >= 255 ) return;

	drawFilledRect( x0, y0, x1, y0 + 1 );		// top
	drawFilledRect( x0, y1 - 1, x1, y1 );		// bottom
	drawFilledRect( x0, y0 + 1, x0 + 1, y1 - 1 );	// left
	drawFilledRect( x1 - 1, y0 + 1, x1, y1 - 1 );	// right
}
	
void CEngineSurface::drawSetTextFont( Font *font )
{
	staticFont = font;

	if( font )
	{
		bool	buildFont = false;

		staticFontInfo = NULL;

		for( int i = 0; i < staticFontInfoDar.getCount(); i++ )
		{
			if( staticFontInfoDar[i]->id == font->getId( ))
			{
				staticFontInfo = staticFontInfoDar[i];
				if( staticFontInfo->contextCount != staticContextCount )
					buildFont = true;
			}
		}

		if( !staticFontInfo || buildFont )
		{
			staticFontInfo = new FontInfo( staticFont->getId(), staticContextCount );
			staticFontInfoDar.putElement( staticFontInfo );

			int currentPage = 0;
			int x = 0, y = 0;

			memset( staticRGBA, 0, sizeof( staticRGBA ));

			for( int i = 0; i < 256; i++ )
			{
				int abcA, abcB, abcC;
				staticFont->getCharABCwide( i, abcA, abcB, abcC );

				int wide = abcB;

				if( isspace( i )) continue;

				int tall = staticFont->getTall();

				if( x + wide + 1 > FONT_SIZE )
				{
					x = 0;
					y += tall + 1;
				}

				if( y + tall + 1 > FONT_SIZE )
				{
					if( !staticFontInfo->bindIndex[currentPage] )
						staticFontInfo->bindIndex[currentPage] = createNewTextureID();
					drawSetTextureRGBA( staticFontInfo->bindIndex[currentPage], staticRGBA, FONT_SIZE, FONT_SIZE );
					currentPage++;

					if( currentPage == FONT_PAGES )
						break;

					memset( staticRGBA, 0, sizeof( staticRGBA ));
					x = y = 0;
				}

				staticFont->getCharRGBA( i, x, y, FONT_SIZE, FONT_SIZE, (byte *)staticRGBA );
				staticFontInfo->pageForChar[i] = currentPage;
				staticFontInfo->texCoord[i][0] = (float)((double)x / (double)FONT_SIZE );
				staticFontInfo->texCoord[i][1] = (float)((double)y / (double)FONT_SIZE );
				staticFontInfo->texCoord[i][2] = (float)((double)(x + wide)/(double)FONT_SIZE );
				staticFontInfo->texCoord[i][3] = (float)((double)(y + tall)/(double)FONT_SIZE );
				x += wide + 1;
			}

			if( currentPage != FONT_PAGES )
			{
				if( !staticFontInfo->bindIndex[currentPage] )
					staticFontInfo->bindIndex[currentPage] = createNewTextureID();
				drawSetTextureRGBA( staticFontInfo->bindIndex[currentPage], staticRGBA, FONT_SIZE, FONT_SIZE );
			}
			staticFontInfo->pageCount = currentPage + 1;
		}
	}
}

void CEngineSurface::drawSetTextPos( int x, int y )
{
	_drawTextPos[0] = x;
	_drawTextPos[1] = y;
}

void CEngineSurface::drawPrintChar( int x, int y, int wide, int tall, float s0, float t0, float s1, float t1, int color[4] )
{
	vpoint_t	ul, lr;

	ul.point[0] = x;
	ul.point[1] = y;
	lr.point[0] = x + wide;
	lr.point[1] = y + tall;

	// gets at the texture coords for this character in its texture page
	ul.coord[0] = s0;
	ul.coord[1] = t0;
	lr.coord[0] = s1;
	lr.coord[1] = t1;

	vpoint_t clippedRect[2];

	if( !ClipRect( ul, lr, &clippedRect[0], &clippedRect[1] ))
		return;

	g_api->SetupDrawingImage( color );
	g_api->DrawQuad( &clippedRect[0], &clippedRect[1] ); // draw the letter
}

void CEngineSurface::drawPrintText( const char* text, int textLen )
{
	//return;
	static bool hasColor = 0;
	static int numColor = 7;

	if( !text || !staticFont || _drawTextColor[3] >= 255 )
		return;

	int x = _drawTextPos[0] + _translateX;
	int y = _drawTextPos[1] + _translateY;

	int tall = staticFont->getTall();

	int j, iTotalWidth = 0;
	int curTextColor[4];

	// HACKHACK: allow color strings in VGUI
	if( numColor != 7 )
	{
		for( j = 0; j < 3; j++ ) // grab predefined color
			curTextColor[j] = g_api->GetColor( numColor, j );
	}
	else
	{
		for( j = 0; j < 3; j++ ) // revert default color
			curTextColor[j] = _drawTextColor[j];
	}
	curTextColor[3] = _drawTextColor[3]; // copy alpha

	if( textLen == 1 )
	{
		if( *text == '^' )
		{
			hasColor = true;
			return; // skip '^'
		}

		if( hasColor && isdigit( *text ))
		{
			numColor = ColorIndex( *text );
			hasColor = false; // handled
			return; // skip colornum
		}
		else hasColor = false;
	}

	// reset
	g_api->ProcessUtfChar( 0 );

	for( int i = 0; i < textLen; i++ )
	{
		int curCh = g_api->ProcessUtfChar( (unsigned char)text[i] );

		if( !curCh )
			continue;

		int abcA, abcB, abcC;

		staticFont->getCharABCwide( curCh, abcA, abcB, abcC );

		float s0 = staticFontInfo->texCoord[curCh][0];
		float t0 = staticFontInfo->texCoord[curCh][1];
		float s1 = staticFontInfo->texCoord[curCh][2];
		float t1 = staticFontInfo->texCoord[curCh][3];
		int wide = abcB;

		iTotalWidth += abcA;
		drawSetTexture( staticFontInfo->bindIndex[staticFontInfo->pageForChar[curCh]] );
		drawPrintChar( x + iTotalWidth, y, wide, tall, s0, t0, s1, t1, curTextColor );
		iTotalWidth += wide + abcC;
	}

	_drawTextPos[0] += iTotalWidth;
}

void CEngineSurface::drawSetTextureRGBA( int id, const char* rgba, int wide, int tall )
{
	g_api->UploadTexture( id, rgba, wide, tall );
}
	
void CEngineSurface::drawSetTexture( int id )
{
	g_api->BindTexture( id );
}
	
void CEngineSurface::drawTexturedRect( int x0, int y0, int x1, int y1 )
{
	vpoint_t rect[2];
	vpoint_t clippedRect[2];

	InitVertex( rect[0], x0, y0, 0, 0 );
	InitVertex( rect[1], x1, y1, 1, 1 );

	// fully clipped?
	if( !ClipRect( rect[0], rect[1], &clippedRect[0], &clippedRect[1] ))
		return;	

	g_api->SetupDrawingImage( _drawColor );	
	g_api->DrawQuad( &clippedRect[0], &clippedRect[1] );
}
	
void CEngineSurface::pushMakeCurrent( Panel* panel, bool useInsets )
{
	int insets[4] = { 0, 0, 0, 0 };
	int absExtents[4];
	int clipRect[4];

	if( useInsets )
		panel->getInset( insets[0], insets[1], insets[2], insets[3] );
	panel->getAbsExtents( absExtents[0], absExtents[1], absExtents[2], absExtents[3] );
	panel->getClipRect( clipRect[0], clipRect[1], clipRect[2], clipRect[3] );

	VGUIPanel p( panel );
	PushMakeCurrent( p, insets, absExtents, clipRect );
}
	
void CEngineSurface::popMakeCurrent( Panel *panel )
{
	VGUIPanel p( panel );
	PopMakeCurrent( p );
}

void CEngineSurface::GetMousePos( int &x, int &y )
{
	g_api->GetCursorPos( &x, &y );
}

bool CEngineSurface::setFullscreenMode( int wide, int tall, int bpp )
{
	// stub
	return false;
}
	
void CEngineSurface::setWindowedMode( void )
{
	// stub
}

void CEngineSurface::setTitle( const char *title )
{
	// stub
}

void CEngineSurface::createPopup( vgui::Panel *embeddedPanel )
{
	// stub
}

bool CEngineSurface::isWithin( int x, int y )
{
	// stub
	return true;
}

void CEngineSurface::enableMouseCapture( bool state )
{
	// stub
}

void CEngineSurface::invalidate( vgui::Panel *panel )
{
	// stub
}

void CEngineSurface::setAsTopMost( bool state )
{
	// stub
}

void CEngineSurface::applyChanges()
{
	// stub
}

void CEngineSurface::swapBuffers()
{
	// stub
}

//
// VGUI2 extended surface
//
void CEngineSurface::PushMakeCurrent( VGUIPanel panel, int insets[4], int absExtents[4], int clipRect[4] )
{
	PaintStack *paintState = &paintStack[staticPaintStackPos];

	assert( staticPaintStackPos < MAX_PAINT_STACK );

	paintState->panel = panel;

	// determine corrected top left origin
	paintState->iTranslateX = insets[0] + absExtents[0];
	paintState->iTranslateY = insets[1] + absExtents[1];
	// setup clipping rectangle for scissoring
	paintState->iScissorLeft = clipRect[0];
	paintState->iScissorTop = clipRect[1];
	paintState->iScissorRight = clipRect[2];
	paintState->iScissorBottom = clipRect[3];

	SetupPaintState( paintState );

	staticPaintStackPos++;
}

void CEngineSurface::PopMakeCurrent( VGUIPanel p )
{
	int top = staticPaintStackPos - 1;

	// more pops that pushes?
	assert( top >= 0 );

	// a1ba: replace asserts by proper checks!!!
	// didn't pop in reverse order of push?
	assert( paintStack[top].panel.vgui2 == p.vgui2 );
	if( p.vgui2 )
		assert( paintStack[top].panel.vPanel == p.vPanel );
	else
		assert( paintStack[top].panel.pPanel == p.pPanel );

	staticPaintStackPos--;

	if( top > 0 ) SetupPaintState( &paintStack[top-1] );
}

void CEngineSurface::drawLine( int x0, int y0, int x1, int y1 )
{
	// TODO: implement this
	// will require updating RefAPI or implement VGUI drawing
	// through TriAPI?
}

void CEngineSurface::drawPolyLine( int *px, int *py, int numPoints )
{
	// TODO: see drawLine comment
}

void CEngineSurface::drawGetTextPos( int &x, int &y )
{
	x = _drawTextPos[0];
	y = _drawTextPos[1];
}

void CEngineSurface::drawSetTextureFile( int id, const char *filename )
{
	g_api->BindTexture( id );
	g_api->UploadTextureFile( id, filename );
}

void CEngineSurface::drawGetTextureSize( int id, int &wide, int &tall )
{
	g_api->BindTexture( id );
	g_api->GetTextureSizes( &wide, &tall );
}

vgui2::HFont CEngineSurface::createFont()
{
	return -1;
}

bool CEngineSurface::addGlyphSetToFont( vgui2::HFont font, const char *fontName, int tall, int weight, bool italic, bool underline, bool strikeout, bool symbol )
{
	return false;
}

bool CEngineSurface::addCustomFontFile( const char *fontFileName )
{
	return false;
}

void CEngineSurface::drawSetTextFont( vgui2::HFont font )
{
	// TODO: use mainui_cpp font renderer here
}

int  CEngineSurface::getFontTall( vgui2::HFont font )
{
	// add random tall
	return 32;
}

void CEngineSurface::getCharABCWide( vgui2::HFont font, int ch, int &a, int &b, int &c )
{
	a = b = c = 1;
}

void CEngineSurface::getTextSize( vgui2::HFont font, const char *text, int &wide, int &tall )
{
	int lines = 0;
	for( const char *p = text; *p; p = Q_strchrnul( p, '\n' ))
		lines++;

	// TODO: add some height because client.dll uses this function to determine
	// engine string drawing functions height (as GoldSrc uses VGUI2 internally anyway)
	// this will be rewritten to real function later
	tall = lines * getFontTall( font );
	wide = 1;
}

void CEngineSurface::playSound( const char *filename )
{
	g_api->StartSound( filename );
}

void CEngineSurface::drawTexturedPolygon( vgui2::VGuiVertex *verts, int n )
{
	// TODO: see drawLine comment
}

int CEngineSurface::getFontAscent( vgui2::HFont font, int ch )
{
	return getFontTall( font ) * 0.8;
}

bool CEngineSurface::deleteTextureByID( int id )
{
	// TODO: see drawLine comment
	return false;
}

void CEngineSurface::drawSubTextureRGBA( int id, int x, int y, const byte *rgba, int wide, int tall )
{
	g_api->UploadTextureBlock( id, x, y, rgba, wide, tall );
}



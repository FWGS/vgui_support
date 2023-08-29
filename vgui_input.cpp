/*
vgui_input.cpp - handle kb & mouse
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

#define OEMRESOURCE		// for OCR_* cursor junk
#include "vgui_main.h"

namespace vgui_support
{
void VGUI_Key( VGUI_KeyAction action, VGUI_KeyCode code )
{
	if( !VGUI2_UseVGUI1( ))
	{
		VGUI2_Key( action, code );
		return;
	}

	if( !surface )
		return;

	vgui::App *pApp = vgui::App::getInstance();

	switch( action )
	{
		case KA_PRESSED:
			pApp->internalKeyPressed((vgui::KeyCode)code, surface );
			break;
		case KA_RELEASED:
			pApp->internalKeyReleased((vgui::KeyCode)code, surface );
			break;
		case KA_TYPED:
			pApp->internalKeyTyped((vgui::KeyCode)code, surface );
			break;
	}
}

void VGUI_Mouse( VGUI_MouseAction action, int code )
{
	if( !VGUI2_UseVGUI1( ))
	{
		VGUI2_Mouse( action, code );
		return;
	}

	if( !surface )
		return;

	vgui::App *pApp = vgui::App::getInstance();

	switch( action )
	{
		case MA_PRESSED:
			pApp->internalMousePressed((vgui::MouseCode)code, surface );
			break;
		case MA_RELEASED:
			pApp->internalMouseReleased((vgui::MouseCode)code, surface );
			break;
		case MA_DOUBLE:
			pApp->internalMouseDoublePressed((vgui::MouseCode)code, surface );
			break;
		case MA_WHEEL:
			pApp->internalMouseWheeled( code, surface );
			break;
	}
}

void VGUI_MouseMove( int x, int y )
{
	if( !VGUI2_UseVGUI1( ))
	{
		VGUI2_MouseMove( x, y );
		return;
	}

	if( !surface )
		return;

	vgui::App::getInstance()->internalCursorMoved( x, y, surface );
}

void VGUI_TextInput( const char *text )
{
	if( !VGUI2_UseVGUI1( ))
		VGUI2_TextInput( text );
}

} // namespace vgui_support

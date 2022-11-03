/*
vgui2_gameui.h - vgui2 GameUIFuncs header
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

#ifndef VGUI2_GAMEUI_H
#define VGUI2_GAMEUI_H

#include <IGameUIFuncs.h>

class GameUIFuncs : public IGameUIFuncs
{
public:
	virtual bool IsKeyDown( const char *keyname, bool &isdown );
	virtual const char *Key_NameForKey( int keynum );
	virtual const char *Key_BindingForKey( int keynum );
	virtual vgui2::KeyCode GetVGUI2KeyCodeForBind( const char *bind );
	virtual void GetVideoModes( vmode_t **liststart, int *count );
	virtual void GetCurrentVideoMode( int *wide, int *tall, int *bpp );
	virtual void GetCurrentRenderer( char *name, int namelen, int *windowed, int *hdmodels, int *addons_folder, int *vid_level );
	virtual bool IsConnectedToVACSecureServer();
	virtual int Key_KeyStringToKeyNum( const char *pchKey );
};

#endif // VGUI2_GAMEUI_H

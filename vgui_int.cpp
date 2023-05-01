/*
vgui_int.c - vgui dll interaction
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

#include "vgui_main.h"
#include "xash3d_types.h"

void VGUI2_Startup( void *clientlib, int width, int height );
void VGUI2_Shutdown( void );
bool VGUI2_UseVGUI1( void );
void VGUI2_Paint( void );

namespace vgui_support {

vgui_support_api_t *g_api;

Panel	*rootpanel = NULL;
CEngineSurface	*surface = NULL;
CEngineApp          staticApp;

void VGui_ClientStartup( void *clientInstance, int width, int height )
{
	VGUI2_Startup( clientInstance, width, height );
}

void VGui_Startup( int width, int height )
{
	if( rootpanel )
	{
		VGUI2_Startup( NULL, width, height );
		rootpanel->setSize( width, height );
		return;
	}

	rootpanel = new Panel;
	rootpanel->setSize( width, height );
	rootpanel->setPaintBorderEnabled( false );
	rootpanel->setPaintBackgroundEnabled( false );
	rootpanel->setVisible( true );
	rootpanel->setCursor( new Cursor( Cursor::dc_none ));

	staticApp.start();
	staticApp.setMinimumTickMillisInterval( 0 );

	surface = new CEngineSurface( rootpanel );
	rootpanel->setSurfaceBaseTraverse( surface );


	//ASSERT( rootpanel->getApp() != NULL );
	//ASSERT( rootpanel->getSurfaceBase() != NULL );

	g_api->DrawInit ();
}

void VGui_Shutdown( void )
{
	staticApp.stop();

	delete rootpanel;
	delete surface;

	rootpanel = NULL;
	surface = NULL;
}

void VGui_Paint( void )
{
	int w, h;

	if( !g_api->IsInGame() || !rootpanel )
		return;

	// setup the base panel to cover the screen
	Panel *pVPanel = surface->getEmbeddedPanel();
	if( !pVPanel )
		return;

	rootpanel->getSize(w, h);
	EnableScissor( true );

	if( VGUI2_UseVGUI1( ))
	{
		staticApp.externalTick ();

		pVPanel->setBounds( 0, 0, w, h );
		pVPanel->repaint();

		// paint everything
		pVPanel->paintTraverse();
	}
	else
	{
		VGUI2_Paint();
	}

	EnableScissor( false );
}
void *VGui_GetPanel( void )
{
	return (void *)rootpanel;
}
}

static vgui_support_interface_t vguifuncs =
{
	VGui_Startup,
	VGui_Shutdown,
	VGui_GetPanel,
	VGui_Paint,
	VGUI_Mouse,
	VGUI_Key,
	VGUI_MouseMove,
	VGUI_TextInput,
	VGui_ClientStartup
};

extern "C" int EXPORT GetVGUISupportAPI( int version, vgui_support_interface_t *iface, vgui_support_api_t *engfuncs )
{
	if( version != VGUI_SUPPORT_API_VERSION )
		return 0;

	g_api = engfuncs; // TODO: do not store the pointer
	memcpy( iface, &vguifuncs, sizeof( vguifuncs ));

	return VGUI_SUPPORT_API_VERSION;
}

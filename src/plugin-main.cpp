/*
yt-live-thumbnails
Copyright (C) <0222> <Logan Lim> <loglim11@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs.h>
#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QMainWindow>
#include <QAction>

#include "plugin-macros.generated.h"
#include "live-thumb-ui.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

void addOutputUI(void) 
{
	// for reference: 
	// https://obsproject.com/forum/threads/obs-add-a-new-custom-panel-to-the-dock.78945/

	// Create Dock objects 
	YTHDock *liveThumbDock = new YTHDock();
	YTHWidget *liveThumbWidget = new YTHWidget();
	
	// Set Dock properties 
	liveThumbDock->setWidget(liveThumbWidget);
	liveThumbDock->setObjectName(QStringLiteral("liveThumbDock"));
	liveThumbDock->setWindowTitle("YouTube Live Thumbnails");
	liveThumbDock->resize(600, 400);
	liveThumbDock->setVisible(false);

	// Add Dock to main OBS window & docks menu
	QMainWindow *main = (QMainWindow *)obs_frontend_get_main_window();
	main->addDockWidget(Qt::BottomDockWidgetArea, liveThumbDock);
	obs_frontend_add_dock(liveThumbDock);
}

bool obs_module_load(void)
{
	addOutputUI();

	blog(LOG_INFO, "plugin loaded successfully (version %s)",
	     PLUGIN_VERSION);

	return true;
}

void obs_module_unload()
{
	blog(LOG_INFO, "plugin unloaded successfully", NULL);
}

void obs_module_post_load()
{
	; 
}
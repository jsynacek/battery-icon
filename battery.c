/*
 * Copyright (C) 2014 Jan Synáček
 *
 * Author: Jan Synáček <jan.synacek@gmail.com>
 * URL: https://github.com/jsynacek/battery-icon
 * Created: May 2014
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

#include "battery.h"
#include "upower.h"

const gchar *battery_icons_primary[] = {
	"gpm-primary-000",
	"gpm-primary-020",
	"gpm-primary-040",
	"gpm-primary-060",
	"gpm-primary-080",
	"gpm-primary-100",
	"gpm-primary-000-charging",
	"gpm-primary-020-charging",
	"gpm-primary-040-charging",
	"gpm-primary-060-charging",
	"gpm-primary-080-charging",
	"gpm-primary-100-charging",
	"gpm-primary-charged",
	NULL
};

const gchar *battery_icons_default[] = {
	"battery-empty",
	"battery-caution",
	"battery-low",
	"battery-good",
	/* defaults have only 5 levels, so use "good" twice */
	"battery-good"
	"battery-full",
	"battery-empty-charging",
	"battery-caution-charging",
	"battery-low-charging",
	"battery-good-charging",
	"battery-good-charging",
	"battery-full-charging",
	"battery-full-charged",
	NULL
};

const gchar *battery_status_messages[] = {
	"Battery empty.",
	"Battery critical.",
	"Battery low.",
	"Battery medium.",
	"Battery high.",
	"Battery full.",
	"Battery charging."
};

/* requires percentage and state to be already set */
static void battery_update_gui_items(battery_info_t *binfo)
{
 	GtkIconTheme *icon_theme = gtk_icon_theme_get_default();
 	const gchar **battery_icons;
 	gint info_index;

 	if (gtk_icon_theme_has_icon(icon_theme, battery_icons_primary[0])) {
 		battery_icons = battery_icons_primary;
 	}
 	else {
 		battery_icons = battery_icons_default;
 	}

 	gint percentage = (gint)binfo->percentage; /* TODO possible rounding error */
 	gboolean is_charging = binfo->state == BATTERY_STATE_CHARGING;

 	if (percentage >= 99 && is_charging) {
 		info_index = BATTERY_CHARGED;
 	}
 	else if (percentage >= 90) {
 		info_index = BATTERY_FULL;
 	}
 	else if (percentage >= 70) {
 		info_index = BATTERY_HIGH;
 	}
 	else if (percentage >= 50) {
 		info_index = BATTERY_MEDIUM;
 	}
 	else if (percentage >= 30) {
 		info_index = BATTERY_LOW;
 	}
 	else if (percentage >= 10) {
 		info_index = BATTERY_CRITICAL;
 	}
 	else {
 		info_index = BATTERY_EMPTY;
 	}

 	if (percentage < 99 && is_charging == TRUE) {
 		binfo->icon_name = battery_icons[info_index + BATTERY_CHARGING];
 	} else {
 		binfo->icon_name = battery_icons[info_index];
 	}

 	binfo->notification_status = battery_status_messages[info_index];
}

static void time_to_str(gchar *buf, gint64 time)
{
	gint hrs  = time / 3600;
	gint mins = time / 60 - (hrs * 60);
	sprintf(buf, "%d:%02d", hrs, mins);
}

battery_info_t *battery_info_init_from_dbus(GDBusProxy *proxy)
{
	/* get stuff from upower first */
	battery_info_t *binfo = g_malloc0(sizeof(battery_info_t));
	binfo->percentage = upower_get_percentage(proxy);
  	binfo->state = upower_get_state(proxy);
  	binfo->time_to_full = upower_get_time_to_full(proxy);
	binfo->time_to_empty = upower_get_time_to_empty(proxy);

	/* set up gtk stuff second */
	battery_update_gui_items(binfo);

	/* tooltip third */
	const gchar *what = "";
	gchar tstr[128];
	gchar out[1024];
	if (binfo->state == BATTERY_STATE_CHARGING) {
		what = "until charged";
		time_to_str(tstr, binfo->time_to_full);
	}
	else if (binfo->state == BATTERY_STATE_DISCHARGING) {
		what = "remaining";
		time_to_str(tstr, binfo->time_to_empty);
	}
	sprintf(out, "%.1f%%, %s %s", binfo->percentage, tstr, what);
	binfo->tooltip = g_strdup(out);

	return binfo;
}

void battery_info_destroy(battery_info_t *binfo)
{
	if (binfo->tooltip) {
		g_free(binfo->tooltip);
	}
	g_free(binfo);
}

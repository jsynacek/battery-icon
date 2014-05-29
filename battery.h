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

#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <gtk/gtk.h>

enum {
	BATTERY_EMPTY,
	BATTERY_CRITICAL,
	BATTERY_LOW,
	BATTERY_MEDIUM,
	BATTERY_HIGH,
	BATTERY_FULL,
	BATTERY_CHARGING,
	BATTERY_CHARGED = 12
};

/* battery states corresponding to state values from upower device */
enum {
	BATTERY_STATE_UNKNOWN,
	BATTERY_STATE_CHARGING,
	BATTERY_STATE_DISCHARGING,
	BATTERY_STATE_EMPTY,
	BATTERY_STATE_FULLY_CHARGED,
	BATTERY_STATE_PENDING_CHARGE,
	BATTERY_STATE_PENDING_DISCHARGE
};

typedef struct battery_info {
	/* dbus properties */
	gdouble percentage;
	guint32 state;
	gint64  time_to_full;
	gint64  time_to_empty;
	/* gui */
	const gchar *notification_status;
	const gchar *icon_name;
	gchar *tooltip;
} battery_info_t;

extern const gchar *battery_icons_primary[];
extern const gchar *battery_icons_default[];
extern const gchar *battery_status_messages[];

battery_info_t *battery_info_init_from_dbus(GDBusProxy *proxy);
void battery_info_destroy(battery_info_t *binfo);

#endif

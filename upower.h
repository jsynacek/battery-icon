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

#ifndef _UPOWER_H_
#define _UPOWER_H_

#include <gtk/gtk.h>

GDBusProxy *upower_init_proxy(GCallback callback);
void upower_destroy_proxy(GDBusProxy *proxy);

GVariant *upower_get_property(GDBusProxy *proxy, const gchar *property);
gdouble upower_get_percentage(GDBusProxy *proxy);
gint64 upower_get_time_to_empty(GDBusProxy *proxy);
gint64 upower_get_time_to_full(GDBusProxy *proxy);
gint64 upower_get_state(GDBusProxy *proxy);

#endif

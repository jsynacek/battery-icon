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

#include "upower.h"

GDBusProxy *upower_init_proxy(GCallback callback)
{
	GDBusProxy *proxy = NULL;
	GError *error = NULL;

	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
					      G_DBUS_PROXY_FLAGS_NONE,
					      NULL,
					      "org.freedesktop.UPower",
					      "/org/freedesktop/UPower/devices/battery_BAT0",
					      "org.freedesktop.UPower.Device",
					      NULL, &error);
	g_signal_connect(G_OBJECT(proxy), "g-signal",
			 callback, NULL);

	return proxy;
}

void upower_destroy_proxy(GDBusProxy *proxy)
{
	if (proxy) {
		g_object_unref(proxy);
	}
}

GVariant *upower_get_property(GDBusProxy *proxy, const gchar *property)
{
	GVariant *result;
	GVariant *value = NULL;
	GError   *error = NULL;

	result = g_dbus_proxy_call_sync(proxy,
					"org.freedesktop.DBus.Properties.Get",
					g_variant_new("(ss)", "org.freedesktop.UPower.Device", property),
					G_DBUS_CALL_FLAGS_NONE, -1, NULL,
					&error);
	if (error) {
		g_printerr(error->message);
		return NULL;
	}

	g_variant_get(result, "(v)", &value);
	return value;
}

gdouble upower_get_percentage(GDBusProxy *proxy)
{
	GVariant *value = upower_get_property(proxy, "Percentage");
	gdouble result = g_variant_get_double(value);
	g_variant_unref(value);
	return result;
}

gint64 upower_get_time_to_empty(GDBusProxy *proxy)
{
	GVariant *value = upower_get_property(proxy, "TimeToEmpty");
	guint64 result = g_variant_get_int64(value);
	g_variant_unref(value);
	return result;
}

gint64 upower_get_time_to_full(GDBusProxy *proxy)
{
	GVariant *value = upower_get_property(proxy, "TimeToFull");
	gint64 result = g_variant_get_int64(value);
	g_variant_unref(value);
	return result;
}

gint64 upower_get_state(GDBusProxy *proxy)
{
	GVariant *value = upower_get_property(proxy, "State");
	guint32 result = g_variant_get_uint32(value);
	g_variant_unref(value);
	return result;
}

/* TODO multiple batteries support

 $ dbus-send --print-reply \
             --system \
             --dest=org.freedesktop.UPower \
             /org/freedesktop/UPower \
             org.freedesktop.UPower.EnumerateDevices


 GError *error = NULL;
 GVariant *result = NULL;
 GDBusProxy *proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
 				      G_DBUS_PROXY_FLAGS_NONE,
 				      NULL,
 				      "org.freedesktop.UPower",
 				      "/org/freedesktop/UPower",
 				      "org.freedesktop.UPower",
 				      NULL, &error);
 if (!proxy) {
 	g_printerr("could'nt get proxy");
 	return 1;
 }
 error = NULL;
 result = g_dbus_proxy_call_sync(proxy,
 				"EnumerateDevices", NULL,
 				G_DBUS_CALL_FLAGS_NONE, -1, NULL,
 				&error);
 if (error) {
 	g_printerr(error->message);
 	return 1;
 }
 GVariantIter *iter = NULL;
 g_variant_get(result, "(ao)", &iter);
 gchar *str;
 while (g_variant_iter_loop(iter, "o", &str)) {
 	g_print("battery: %s\n", str);
 }

 g_variant_iter_free(iter);
*/

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

#include <stdlib.h>
#include <gtk/gtk.h>
#include <libnotify/notify.h>
#include "battery.h"
#include "upower.h"
#include "config.h"

static GtkStatusIcon *status_icon = NULL;
#ifdef COMPILEWITH_NOTIFY
static NotifyNotification *notification = NULL;
#endif


static void status_icon_on_popup_menu(GtkStatusIcon *status_icon, guint button,
				      guint activation_time, gpointer user_data)
{
	GtkWidget *gtkMenu = gtk_menu_new();
	GtkWidget *quit = gtk_image_menu_item_new_from_stock("gtk-quit", NULL);

	g_signal_connect(G_OBJECT(quit), "activate",
			 G_CALLBACK(gtk_main_quit), NULL);

	gtk_menu_shell_append(GTK_MENU_SHELL(gtkMenu), quit);

	gtk_widget_show_all(gtkMenu);

	gtk_menu_popup(GTK_MENU(gtkMenu), NULL, NULL,
		       gtk_status_icon_position_menu, status_icon,
		       button, activation_time);
}
void on_upower_changed(GDBusProxy *proxy,
		       gchar      *sender_name,
		       gchar      *signal_name,
		       GVariant   *parameters,
		       gpointer    user_data)
{
	battery_info_t *binfo = battery_info_init_from_dbus(proxy);

 	gtk_status_icon_set_tooltip_text(status_icon, binfo->tooltip);
	gtk_status_icon_set_from_icon_name(status_icon, binfo->icon_name);

#ifdef COMPILEWITH_NOTIFY
	if (binfo->percentage < 20) {
		GError *error = NULL;
		notify_notification_update(notification,
					   binfo->notification_status,
					   binfo->tooltip,
					   binfo->icon_name);
		notify_notification_show(notification, &error);
		if(error) {
			g_printerr("%s\n", error->message);
		}
	}
#endif

	/* DEBUG */
	g_print("tooltip: \"%s\"\n", binfo->tooltip);

	battery_info_destroy(binfo);
}
int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	status_icon = gtk_status_icon_new_from_icon_name("gpm-battery-charged");
	gtk_status_icon_set_visible(status_icon, TRUE);
	g_signal_connect(G_OBJECT(status_icon), "popup-menu",
			 G_CALLBACK(status_icon_on_popup_menu), NULL);

	GDBusProxy *proxy = upower_init_proxy(G_CALLBACK(on_upower_changed));

#ifdef COMPILEWITH_NOTIFY
	notify_init(PACKAGE_STRING);
	notification = notify_notification_new(NULL, NULL, NULL);
#endif

	/* initialize for the first time */
	battery_info_t *binfo = battery_info_init_from_dbus(proxy);
 	gtk_status_icon_set_tooltip_text(status_icon, binfo->tooltip);
	gtk_status_icon_set_from_icon_name(status_icon, binfo->icon_name);
	battery_info_destroy(binfo);

	gtk_main();

	upower_destroy_proxy(proxy);

#ifdef COMPILEWITH_NOTIFY
	notify_uninit();
#endif
	return EXIT_SUCCESS;
}

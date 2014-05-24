/* Copyright (C) 2014 Jan Synáček
 *
 * Author: Jan Synáček <jan.synacek@gmail.com>
 * URL: https://github.com/jsynacek/battery-icons
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
#include <string.h>
#include <gtk/gtk.h>
#include <libnotify/notify.h>

static GtkStatusIcon *status_icon = NULL;
static NotifyNotification *notification = NULL;

static gboolean get_battery_info(const gchar *acpi_out,
				 gint *percentage,
				 gboolean *is_charging,
				 gchar **icon_name,
				 gchar **status_summary)
{
	static gint last_percentage = -1;
	static gboolean last_is_charging = FALSE;

	GRegex *re = g_regex_new("Battery (\\d+): (\\w+), (...?)%, ([0-9:]*) (.*)",
				 0, 0, NULL);
	GMatchInfo *mi;
	g_regex_match(re, acpi_out, 0, &mi);

	const gchar *status = g_match_info_fetch(mi, 2);
	const gchar *percentage_match = g_match_info_fetch(mi, 3);

	*percentage = atoi(percentage_match);
	*is_charging = strcmp(status, "Charging") == 0;

	*icon_name = g_malloc0(64 * sizeof(gchar));
	strcat(*icon_name, "gpm-primary");

	if (*percentage >= 99 && *is_charging) {
		strcat(*icon_name, "-charged");
		*status_summary = g_strdup("battery level full");
	}
	else if (*percentage >= 90) {
		strcat(*icon_name, "-100");
		*status_summary = g_strdup("battery level high");
	}
	else if (*percentage >= 70) {
		strcat(*icon_name, "-080");
		*status_summary = g_strdup("battery level medium");
	}
	else if (*percentage >= 50) {
		strcat(*icon_name, "-060");
		*status_summary = g_strdup("battery level medium");
	}
	else if (*percentage >= 30) {
		strcat(*icon_name, "-040");
		*status_summary = g_strdup("battery level medium");
	}
	else if (*percentage >= 10) {
		strcat(*icon_name, "-020");
		*status_summary = g_strdup("battery level low");
	}
	else {
		strcat(*icon_name, "-000");
		*status_summary = g_strdup("battery level critical");
	}

	if (*is_charging == TRUE)
		strcat(*icon_name, "-charging");

	gboolean status_changed = FALSE;
	/* TODO: make this work in zones, see if/else tree above */
	if (abs(last_percentage - *percentage) > 10) {
		status_changed = TRUE;
		last_percentage = *percentage;
	}
	if (last_is_charging != *is_charging) {
		status_changed = TRUE;
	}
	last_is_charging = *is_charging;

	return status_changed;
}

static gboolean update_status(gpointer data)
{
	gchar *out;
	gint percentage;
	gboolean is_charging;
	gchar *icon_name;
	gchar *summary;
	gboolean status_changed;
	GError *error = NULL;

	g_spawn_command_line_sync("acpi -b", &out, NULL, NULL, NULL);
	out[strlen(out) - 1] = '\0'; /* trim off trailing '\n' */

	status_changed = get_battery_info(out, &percentage, &is_charging,
					  &icon_name, &summary);

	gtk_status_icon_set_tooltip_text(status_icon, out);
	if (status_changed) {
		gtk_status_icon_set_from_icon_name(status_icon, icon_name);
		notify_notification_update(notification, summary, out, icon_name);
		notify_notification_show(notification, &error);

		if(error) {
			g_printerr("%s\n", error->message);
		}
	}

	g_free(icon_name);
	g_free(summary);

	return TRUE;
}

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

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	status_icon = gtk_status_icon_new_from_icon_name("gpm-battery-charged");
	gtk_status_icon_set_visible(status_icon, TRUE);
	g_signal_connect(G_OBJECT(status_icon), "popup-menu",
			 G_CALLBACK(status_icon_on_popup_menu), NULL);

	/* TODO: conditional compilation */
	notify_init("battery-icon");
	notification = notify_notification_new(NULL, NULL, NULL);

	g_timeout_add(1000, update_status, NULL);

	gtk_main();

	notify_uninit();
	return EXIT_SUCCESS;
}

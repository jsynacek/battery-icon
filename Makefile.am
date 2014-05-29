AM_CFLAGS  = -std=c99 -pedantic -Wall
AM_CFLAGS += @GTK_CFLAGS@ @NOTIFY_CFLAGS@

LIBS = @GTK_LIBS@ @NOTIFY_LIBS@

bin_PROGRAMS = battery-icon
battery_icon_SOURCES = \
	upower.h \
	battery.h \
	upower.c \
	battery.c \
	battery-icon.c \
	config.h

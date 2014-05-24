SRC=battery-icon.c
CFLAGS?=-g -std=c99 -pedantic -Wall

all:
	gcc $(CFLAGS) $(SRC) $(shell pkg-config --cflags --libs glib-2.0) $(shell pkg-config --cflags --libs gtk+-3.0) $(shell pkg-config --cflags --libs libnotify) -o battery-icon

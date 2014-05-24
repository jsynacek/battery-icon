battery-icon
============

Simple battery monitor inspired by volumeicon.

Why not name it batteryicon? Because there's software called like that already
(some funny application for Blackberry).

Why another battery systray plugin? Because I couldn't find anything that was
simple, functional and wasn't part of a window manager or desktop environment.

Running
=======

Simply 'make' and './battery-icon'.

Note that the 'acpi' tool has to be installed on the system, since it's
currently used to poll the battery status.

TODO
====
* ~~autotolize~~ DONE

* ~~add conditional compilation for libnotify~~ DONE

* implement detailed mode, which would use upower to provide useful battery information

* implement simple preferences

* get rid of g_spawn_* and implement acpi polling natively

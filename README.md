battery-icon
============

Simple battery monitor inspired by volumeicon.

Why not name it batteryicon? Because there's software called like that already
(some funny application for Blackberry).

Why another battery systray plugin? Because I couldn't find anything that was
simple, functional and wasn't part of a window manager or desktop environment.

Running
=======

Simply './autogen.sh', './configure', 'make' and './battery-icon'.

UPower with its DBus interface must be running on the system.

TODO
====
* ~~autotolize~~ DONE

* ~~add conditional compilation for libnotify~~ DONE

* ~~get rid of g_spawn_* and implement acpi polling natively~~

* implement detailed mode, which would use upower to provide useful battery information

* implement simple preferences

* implement debug mode

* implement support for multiple batteries

* implement Qt version

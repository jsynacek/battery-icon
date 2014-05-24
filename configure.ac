AC_INIT([battery-icon], [0.1], [jan.synacek@gmail.com])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_CHECK_PROG([acpi_bin], [acpi], [acpi], [no])
if test "x${acpi_bin}" = xno; then
   echo "'acpi' binary not found" 2>&1
   exit 1
 fi

# Args.
AC_ARG_ENABLE([notify],
              [AC_HELP_STRING(
                [--enable-notify],
                [enable libnotify support (default yes)]
              )],
              [notify=${enableval}],
              [notify=yes])

# Checks for libraries.
if test "x${notify}" = xyes; then
   # Check for libnotify
   PKG_CHECK_MODULES([NOTIFY], [libnotify >= 0.5.0])
   NOTIFY_CFLAGS+="-DCOMPILEWITH_NOTIFY"
fi

PKG_CHECK_MODULES([GTK], [gtk+-3.0])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
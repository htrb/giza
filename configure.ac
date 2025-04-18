AC_PREREQ(2.61)
m4_define(giza_version_major,1)
m4_define(giza_version_minor,5)
m4_define(giza_version_micro,0)
AC_INIT([giza],
        [giza_version_major.giza_version_minor.giza_version_micro],
        [daniel.price@monash.edu],[],
        [http://danieljprice.github.io/giza])
AC_CONFIG_AUX_DIR([build])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror -Wno-extra-portability])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIRS([m4])
AM_MAINTAINER_MODE([disable])
AC_PROG_CC
AC_PROG_FC
AC_CONFIG_HEADERS([config.h])
#
# conditional on whether Fortran compiler found,
# skip compilation of libpgplot.a otherwise
#
AM_CONDITIONAL([HAVE_FC], [test "x$FC" != "x"])
#
# set giza version
#
AC_SUBST(GIZA_VERSION_MAJOR,giza_version_major)
AC_SUBST(GIZA_VERSION_MINOR,giza_version_minor)
AC_SUBST(GIZA_VERSION_MICRO,giza_version_micro)
#
# The following specify the interface version numbers for giza.
# LT_CURRENT is the current interface number
# LT_REVISION is the implementation number of the current interface
# LT_AGE is the difference between the newest and oldest interface implemented
#
# Rules are:
# 1) update LT_REVISION for every public giza release
# 2) update LT_CURRENT if giza interface updated and set LT_REVISION to 0
# 3) increment LT_AGE if interface is backwards compatible
# 4) set LT_AGE to 0 if interface is not backwards compatible
#
AC_SUBST(LT_CURRENT, 2)
AC_SUBST(LT_REVISION, 0)
AC_SUBST(LT_AGE, 0)
LT_INIT([shared])
#
# cairo and X11 libraries. Use pkg-config if available but provide fallback options if not (OS/X)
#
x11_libs="-L/usr/X11/lib -lX11"
cairo_libs_osx="-L/usr/X11/lib -lcairo"
cairo_cflags_osx="-I/usr/X11/include"
ft_libs_osx="-L/usr/X11/lib -lfreetype"
ft_cflags_osx="-I/usr/X11/include/freetype2/"
fc_libs_osx="-L/usr/X11/lib -lfontconfig"
fc_cflags_osx="-I/usr/X11/include/fontconfig/"
#
# for cairo have to be careful to return the /usr/X11/include not /usr/X11/include/cairo
# because in the code we always write "include <cairo/cairo.h>" not "include <cairo.h>"
#
PKG_CHECK_MODULES([CAIRO], [cairo],
    [
        CAIRO_INCLUDEDIR=`$PKG_CONFIG --variable=includedir cairo`
        CAIRO_LIBS=`$PKG_CONFIG --libs cairo`
        CAIRO_CFLAGS="-I$CAIRO_INCLUDEDIR"
    ],
    [
        AC_SUBST(CAIRO_LIBS,$cairo_libs_osx)
        AC_SUBST(CAIRO_CFLAGS,$cairo_cflags_osx)
    ])
PKG_CHECK_MODULES([X11], [x11], [], [AC_SUBST(X11_LIBS,$x11_libs)])
PKG_CHECK_MODULES([FT], [freetype2], [], [AC_SUBST(FT_LIBS,$ft_libs_osx)  AC_SUBST(FT_CFLAGS,$ft_cflags_osx)])
PKG_CHECK_MODULES([FC], [fontconfig], [], [AC_SUBST(FC_LIBS,$fc_libs_osx) AC_SUBST(FC_CFLAGS,$fc_cflags_osx)])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/cpgplot.pc
 src/giza.pc
 src/pgplot.pc
 src/giza-version.h
 test/Makefile
 test/C/Makefile
 test/F90/Makefile
])

AC_MSG_NOTICE([====== Compilation flags being used ======])
AC_MSG_NOTICE([CFLAGS = $CFLAGS])
AC_MSG_NOTICE([CPPFLAGS = $CPPFLAGS])
AC_MSG_NOTICE([CAIRO_CFLAGS = $CAIRO_CFLAGS])
AC_MSG_NOTICE([CAIRO_LIBS = $CAIRO_LIBS])
AC_MSG_NOTICE([X11_CFLAGS = $X11_CFLAGS])
AC_MSG_NOTICE([X11_LIBS = $X11_LIBS])
AC_MSG_NOTICE([FT_CFLAGS = $FT_CFLAGS])
AC_MSG_NOTICE([FT_LIBS = $FT_LIBS])
AC_MSG_NOTICE([FC_CFLAGS = $FC_CFLAGS])
AC_MSG_NOTICE([FC_LIBS = $FC_LIBS])
AC_MSG_NOTICE([PKG_CONFIG_PATH = $PKG_CONFIG_PATH])
AC_MSG_NOTICE([================================])
AS_IF([test "x$FC" = "x"],
    [AC_MSG_WARN([*** NO FORTRAN COMPILER FOUND: skipping compilation of libpglot and giza Fortran module *** ])])

AC_OUTPUT

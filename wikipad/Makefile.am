INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-DBINDIR=\"$(bindir)\" \
	-DDATADIR=\"$(datadir)\" \
	-DLIBDIR=\"$(libdir)\" \
	-DG_LOG_DOMAIN=\"Mousepad\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	-DGDK_PIXBUF_DISABLE_SINGLE_INCLUDES \
	-DGTK_DISABLE_SINGLE_INCLUDES \
	$(PLATFORM_CPPFLAGS)

bin_PROGRAMS = \
	wikipad

wikipad_built_sources = \
	wikipad-marshal.c \
	wikipad-marshal.h

wikipad_SOURCES = \
	$(wikipad_built_sources) \
	$(wikipad_dbus_sources) \
	main.c \
	wikipad-action-group.c \
	wikipad-action-group.h \
	wikipad-application.c \
	wikipad-application.h \
	wikipad-close-button.c \
	wikipad-close-button.h \
	wikipad-dialogs.c \
	wikipad-dialogs.h \
	wikipad-document.c \
	wikipad-document.h \
	wikipad-encoding.c \
	wikipad-encoding.h \
	wikipad-encoding-dialog.c \
	wikipad-encoding-dialog.h \
	wikipad-file.c \
	wikipad-file.h \
	wikipad-gtkcompat.h \
	wikipad-language-action.c \
	wikipad-language-action.h \
	wikipad-prefs-dialog.c \
	wikipad-prefs-dialog.h \
	wikipad-prefs-dialog-ui.h \
	wikipad-print.c \
	wikipad-print.h \
	wikipad-private.h \
	wikipad-replace-dialog.c \
	wikipad-replace-dialog.h \
	wikipad-search-bar.c \
	wikipad-search-bar.h \
	wikipad-settings.c \
	wikipad-settings.h \
	wikipad-settings-store.c \
	wikipad-settings-store.h \
	wikipad-statusbar.c \
	wikipad-statusbar.h \
	wikipad-style-scheme-action.c \
	wikipad-style-scheme-action.h \
	wikipad-view.c \
	wikipad-view.h \
	wikipad-util.c \
	wikipad-util.h \
	wikipad-window.c \
	wikipad-window.h \
	wikipad-window-ui.h

wikipad_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS) \
	$(GTHREAD_CFLAGS) \
	$(GTKSOURCEVIEW_CFLAGS) \
	$(PLATFORM_CFLAGS) \
	-DWIKIPAD_GSETTINGS_SCHEMA_DIR=\""$(datadir)/glib-2.0/schemas"\"

wikipad_LDFLAGS = \
	-no-undefined \
	$(PLATFORM_LDFLAGS)

wikipad_LDADD = \
	$(GLIB_LIBS) \
	$(GTK_LIBS)	\
	$(GTHREAD_LIBS) \
	$(GTKSOURCEVIEW_LIBS)

if HAVE_DBUS
wikipad_built_sources +=	\
	wikipad-dbus-infos.h

wikipad_dbus_sources = \
	wikipad-dbus.c \
	wikipad-dbus.h

wikipad_CFLAGS += \
	-DDBUS_API_SUBJECT_TO_CHANGE \
	$(DBUS_CFLAGS)

wikipad_LDADD +=	\
	$(DBUS_LIBS)
endif

if WITH_KEYFILE_SETTINGS
wikipad_CFLAGS += \
	-DWIKIPAD_SETTINGS_KEYFILE_BACKEND=1
endif

if MAINTAINER_MODE
DISTCLEANFILES = \
	$(wikipad_built_sources) \
	wikipad-window-ui.h \
	wikipad-prefs-dialog-ui.h

BUILT_SOURCES = \
	$(wikipad_built_sources) \
	wikipad-window-ui.h \
	wikipad-prefs-dialog-ui.h

if HAVE_DBUS
wikipad-dbus-infos.h: wikipad-dbus-infos.xml Makefile
	$(AM_V_GEN) dbus-binding-tool --prefix=wikipad_dbus_service --mode=glib-server $< > $@
endif

wikipad-window-ui.h: wikipad-window-ui.xml Makefile
	$(AM_V_GEN) exo-csource --strip-comments --strip-content --static --name=wikipad_window_ui $< > $@

wikipad-prefs-dialog-ui.h: wikipad-prefs-dialog.glade Makefile
	$(AM_V_GEN) exo-csource --strip-comments --strip-content --static --name=wikipad_prefs_dialog_ui $< > $@

wikipad-marshal.h: wikipad-marshal.list Makefile
	$(AM_V_GEN) glib-genmarshal --header --prefix=_wikipad_marshal $< > $@

wikipad-marshal.c: wikipad-marshal.list Makefile
	$(AM_V_GEN) echo "#include <wikipad/wikipad-marshal.h>" > $@ \
	&& glib-genmarshal --body --prefix=_wikipad_marshal $< >> $@
endif

EXTRA_DIST = \
	wikipad-dbus-infos.xml \
	wikipad-marshal.list \
	wikipad-window-ui.xml \
	wikipad-prefs-dialog.glade \
	org.xfce.wikipad.gschema.xml

gsettings_SCHEMAS = org.xfce.wikipad.gschema.xml
@GSETTINGS_RULES@

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:

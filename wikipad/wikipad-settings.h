/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __WIKIPAD_SETTINGS_H__
#define __WIKIPAD_SETTINGS_H__ 1

#include <gio/gio.h>

G_BEGIN_DECLS

/* Setting names */
#define WIKIPAD_SETTING_AUTO_INDENT                  "/preferences/view/auto-indent"
#define WIKIPAD_SETTING_FONT_NAME                    "/preferences/view/font-name"
#define WIKIPAD_SETTING_USE_DEFAULT_FONT             "/preferences/view/use-default-monospace-font"
#define WIKIPAD_SETTING_SHOW_WHITESPACE              "/preferences/view/show-whitespace"
#define WIKIPAD_SETTING_SHOW_LINE_ENDINGS            "/preferences/view/show-line-endings"
#define WIKIPAD_SETTING_HIGHLIGHT_CURRENT_LINE       "/preferences/view/highlight-current-line"
#define WIKIPAD_SETTING_INDENT_ON_TAB                "/preferences/view/indent-on-tab"
#define WIKIPAD_SETTING_INDENT_WIDTH                 "/preferences/view/indent-width"
#define WIKIPAD_SETTING_INSERT_SPACES                "/preferences/view/insert-spaces"
#define WIKIPAD_SETTING_RIGHT_MARGIN_POSITION        "/preferences/view/right-margin-position"
#define WIKIPAD_SETTING_SHOW_LINE_MARKS              "/preferences/view/show-line-marks"
#define WIKIPAD_SETTING_SHOW_LINE_NUMBERS            "/preferences/view/show-line-numbers"
#define WIKIPAD_SETTING_SHOW_RIGHT_MARGIN            "/preferences/view/show-right-margin"
#define WIKIPAD_SETTING_SMART_HOME_END               "/preferences/view/smart-home-end"
#define WIKIPAD_SETTING_TAB_WIDTH                    "/preferences/view/tab-width"
#define WIKIPAD_SETTING_WORD_WRAP                    "/preferences/view/word-wrap"
#define WIKIPAD_SETTING_MATCH_BRACES                 "/preferences/view/match-braces"
#define WIKIPAD_SETTING_COLOR_SCHEME                 "/preferences/view/color-scheme"
#define WIKIPAD_SETTING_TOOLBAR_STYLE                "/preferences/window/toolbar-style"
#define WIKIPAD_SETTING_TOOLBAR_ICON_SIZE            "/preferences/window/toolbar-icon-size"
#define WIKIPAD_SETTING_ALWAYS_SHOW_TABS             "/preferences/window/always-show-tabs"
#define WIKIPAD_SETTING_CYCLE_TABS                   "/preferences/window/cycle-tabs"
#define WIKIPAD_SETTING_DEFAULT_TAB_SIZES            "/preferences/window/default-tab-sizes"
#define WIKIPAD_SETTING_PATH_IN_TITLE                "/preferences/window/path-in-title"
#define WIKIPAD_SETTING_RECENT_MENU_ITEMS            "/preferences/window/recent-menu-items"
#define WIKIPAD_SETTING_REMEMBER_SIZE                "/preferences/window/remember-size"
#define WIKIPAD_SETTING_REMEMBER_POSITION            "/preferences/window/remember-position"
#define WIKIPAD_SETTING_REMEMBER_STATE               "/preferences/window/remember-state"
#define WIKIPAD_SETTING_MENUBAR_VISIBLE              "/preferences/window/menubar-visible"
#define WIKIPAD_SETTING_TOOLBAR_VISIBLE              "/preferences/window/toolbar-visible"
#define WIKIPAD_SETTING_STATUSBAR_VISIBLE            "/preferences/window/statusbar-visible"
#define WIKIPAD_SETTING_MENUBAR_VISIBLE_FULLSCREEN   "/preferences/window/menubar-visible-in-fullscreen"
#define WIKIPAD_SETTING_TOOLBAR_VISIBLE_FULLSCREEN   "/preferences/window/toolbar-visible-in-fullscreen"
#define WIKIPAD_SETTING_STATUSBAR_VISIBLE_FULLSCREEN "/preferences/window/statusbar-visible-in-fullscreen"

/* State setting names */
#define WIKIPAD_SETTING_SEARCH_DIRECTION            "/state/search/direction"
#define WIKIPAD_SETTING_SEARCH_MATCH_CASE           "/state/search/match-case"
#define WIKIPAD_SETTING_SEARCH_MATCH_WHOLE_WORD     "/state/search/match-whole-word"
#define WIKIPAD_SETTING_SEARCH_REPLACE_ALL          "/state/search/replace-all"
#define WIKIPAD_SETTING_SEARCH_REPLACE_ALL_LOCATION "/state/search/replace-all-location"
#define WIKIPAD_SETTING_WINDOW_HEIGHT               "/state/window/height"
#define WIKIPAD_SETTING_WINDOW_WIDTH                "/state/window/width"
#define WIKIPAD_SETTING_WINDOW_TOP                  "/state/window/top"
#define WIKIPAD_SETTING_WINDOW_LEFT                 "/state/window/left"
#define WIKIPAD_SETTING_WINDOW_MAXIMIZED            "/state/window/maximized"
#define WIKIPAD_SETTING_WINDOW_FULLSCREEN           "/state/window/fullscreen"

void     wikipad_settings_init          (void);
void     wikipad_settings_finalize      (void);

gboolean wikipad_setting_bind           (const gchar       *path,
                                          gpointer           object,
                                          const gchar       *prop,
                                          GSettingsBindFlags flags);

gulong   wikipad_setting_connect        (const gchar       *path,
                                          GCallback          callback,
                                          gpointer           user_data,
                                          GConnectFlags      connect_flags);

gulong   wikipad_setting_connect_object (const gchar       *path,
                                          GCallback          callback,
                                          gpointer           gobject,
                                          GConnectFlags      connect_flags);

void     wikipad_setting_disconnect     (const gchar       *path,
                                          gulong             handler_id);

/* functions for reading and writing settings */

gboolean wikipad_setting_get            (const gchar       *path,
                                          const gchar       *format_string,
                                          ...);

gboolean wikipad_setting_set            (const gchar       *path,
                                          const gchar       *format_string,
                                          ...);

/* convenience functions for reading/writing common types */

gboolean wikipad_setting_get_boolean    (const gchar       *path);

void     wikipad_setting_set_boolean    (const gchar       *path,
                                          gboolean           value);

gint     wikipad_setting_get_int        (const gchar       *path);

void     wikipad_setting_set_int        (const gchar       *path,
                                          gint               value);

gchar   *wikipad_setting_get_string     (const gchar       *path);

void     wikipad_setting_set_string     (const gchar       *path,
                                          const gchar       *value);

gint     wikipad_setting_get_enum       (const gchar       *path);

void     wikipad_setting_set_enum       (const gchar       *path,
                                          gint               value);

/* wrappers for above read/write functions with shorter arguments */

#define WIKIPAD_SETTING_BIND(setting, object, prop, flags) \
  wikipad_setting_bind (WIKIPAD_SETTING_##setting, object, prop, flags)

#define WIKIPAD_SETTING_CONNECT(setting, callback, user_data, connect_flags) \
  wikipad_setting_connect (WIKIPAD_SETTING_##setting, callback, user_data, connect_flags)

#define WIKIPAD_SETTING_CONNECT_OBJECT(setting, callback, object, connect_flags) \
  wikipad_setting_connect_object (WIKIPAD_SETTING_##setting, callback, object, connect_flags)

#define WIKIPAD_SETTING_DISCONNECT(setting, id) \
  wikipad_setting_disconnect (WIKIPAD_SETTING_##setting, id)

#define WIKIPAD_SETTING_GET(setting, ...)           wikipad_setting_get (WIKIPAD_SETTING_##setting, __VA_ARGS__)
#define WIKIPAD_SETTING_GET_BOOLEAN(setting)        wikipad_setting_get_boolean (WIKIPAD_SETTING_##setting)
#define WIKIPAD_SETTING_GET_INT(setting)            wikipad_setting_get_int (WIKIPAD_SETTING_##setting)
#define WIKIPAD_SETTING_GET_STRING(setting)         wikipad_setting_get_string (WIKIPAD_SETTING_##setting)
#define WIKIPAD_SETTING_GET_ENUM(setting)           wikipad_setting_get_enum (WIKIPAD_SETTING_##setting)

#define WIKIPAD_SETTING_SET(setting, ...)           wikipad_setting_set (WIKIPAD_SETTING_##setting, __VA_ARGS__)
#define WIKIPAD_SETTING_SET_BOOLEAN(setting, value) wikipad_setting_set_boolean (WIKIPAD_SETTING_##setting, value)
#define WIKIPAD_SETTING_SET_INT(setting, value)     wikipad_setting_set_int (WIKIPAD_SETTING_##setting, value)
#define WIKIPAD_SETTING_SET_STRING(setting, value)  wikipad_setting_set_string (WIKIPAD_SETTING_##setting, value)
#define WIKIPAD_SETTING_SET_ENUM(setting, value)    wikipad_setting_set_enum (WIKIPAD_SETTING_##setting, value)

G_END_DECLS

#endif /* __WIKIPAD_SETTINGS_H__ */

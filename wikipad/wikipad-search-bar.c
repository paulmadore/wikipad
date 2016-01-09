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

#include <wikipad/wikipad-private.h>
#include <wikipad/wikipad-gtkcompat.h>
#include <wikipad/wikipad-settings.h>
#include <wikipad/wikipad-marshal.h>
#include <wikipad/wikipad-document.h>
#include <wikipad/wikipad-search-bar.h>
#include <wikipad/wikipad-util.h>
#include <wikipad/wikipad-window.h>

#include <gdk/gdk.h>



#define TOOL_BAR_ICON_SIZE  GTK_ICON_SIZE_MENU
#define HIGHTLIGHT_TIMEOUT  225



static void      wikipad_search_bar_finalize                   (GObject                 *object);
static void      wikipad_search_bar_find_string                (WikipadSearchBar       *bar,
                                                                 WikipadSearchFlags   flags);
static void      wikipad_search_bar_hide_clicked               (WikipadSearchBar       *bar);
static void      wikipad_search_bar_entry_activate             (GtkWidget               *entry,
                                                                 WikipadSearchBar       *bar);
static void      wikipad_search_bar_entry_activate_backward    (GtkWidget               *entry,
                                                                 WikipadSearchBar       *bar);
static void      wikipad_search_bar_entry_changed              (GtkWidget               *entry,
                                                                 WikipadSearchBar       *bar);
static void      wikipad_search_bar_highlight_toggled          (GtkWidget               *button,
                                                                 WikipadSearchBar       *bar);
static void      wikipad_search_bar_match_case_toggled         (GtkWidget               *button,
                                                                 WikipadSearchBar       *bar);
static void      wikipad_search_bar_highlight_schedule         (WikipadSearchBar       *bar);
static gboolean  wikipad_search_bar_highlight_timeout          (gpointer                 user_data);
static void      wikipad_search_bar_highlight_timeout_destroy  (gpointer                 user_data);



enum
{
  HIDE_BAR,
  SEARCH,
  LAST_SIGNAL
};

struct _WikipadSearchBarClass
{
  GtkToolbarClass __parent__;
};

struct _WikipadSearchBar
{
  GtkToolbar           __parent__;

  /* text entry */
  GtkWidget           *entry;

  /* menu entries */
  GtkWidget           *match_case_entry;

  /* flags */
  guint                highlight_all : 1;
  guint                match_case : 1;

  /* highlight id */
  guint                highlight_id;
};



static guint search_bar_signals[LAST_SIGNAL];



GtkWidget *
wikipad_search_bar_new (void)
{
  return g_object_new (WIKIPAD_TYPE_SEARCH_BAR,
                       "toolbar-style", GTK_TOOLBAR_BOTH_HORIZ,
                       "icon-size", TOOL_BAR_ICON_SIZE,
                       NULL);
}



G_DEFINE_TYPE (WikipadSearchBar, wikipad_search_bar, GTK_TYPE_TOOLBAR)



static void
wikipad_search_bar_class_init (WikipadSearchBarClass *klass)
{
  GObjectClass  *gobject_class;
  GObjectClass  *entry_class;
  GtkBindingSet *binding_set;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = wikipad_search_bar_finalize;

  /* signals */
  search_bar_signals[HIDE_BAR] =
    g_signal_new (I_("hide-bar"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0, NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  search_bar_signals[SEARCH] =
    g_signal_new (I_("search"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  0, NULL, NULL,
                  _wikipad_marshal_INT__FLAGS_STRING_STRING,
                  G_TYPE_INT, 3,
                  WIKIPAD_TYPE_SEARCH_FLAGS,
                  G_TYPE_STRING, G_TYPE_STRING);

  /* setup key bindings for the search bar */
  binding_set = gtk_binding_set_by_class (klass);
  gtk_binding_entry_add_signal (binding_set, GDK_Escape, 0, "hide-bar", 0);

  /* In GTK3, gtkrc is deprecated */
#if GTK_CHECK_VERSION(3, 0, 0) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ > 2)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

  /* hide the shadow around the toolbar */
  gtk_rc_parse_string ("style \"wikipad-search-bar-style\"\n"
                         "{\n"
                           "GtkToolbar::shadow-type = GTK_SHADOW_NONE\n"
                         "}\n"
                       "class \"WikipadSearchBar\" style \"wikipad-search-bar-style\"\n"

                       /* add 2px space between the toolbar buttons */
                       "style \"wikipad-button-style\"\n"
                         "{\n"
                           "GtkToolButton::icon-spacing = 2\n"
                         "}\n"
                       "widget \"WikipadWindow.*.Gtk*ToolButton\" style \"wikipad-button-style\"\n");

#if GTK_CHECK_VERSION(3, 0, 0) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ > 2)
# pragma GCC diagnostic pop
#endif

  /* add an activate-backwards signal to GtkEntry */
  entry_class = g_type_class_ref (GTK_TYPE_ENTRY);
  if (G_LIKELY (g_signal_lookup("activate-backward", GTK_TYPE_ENTRY) == 0))
    {
      /* install the signal */
      g_signal_new("activate-backward",
                   GTK_TYPE_ENTRY,
                   G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                   0, NULL, NULL,
                   g_cclosure_marshal_VOID__VOID,
                   G_TYPE_NONE, 0);
      binding_set = gtk_binding_set_by_class(entry_class);
      gtk_binding_entry_add_signal(binding_set, GDK_Return, GDK_SHIFT_MASK, "activate-backward", 0);
      gtk_binding_entry_add_signal(binding_set, GDK_KP_Enter, GDK_SHIFT_MASK, "activate-backward", 0);
    }
  g_type_class_unref (entry_class);
}



static void
wikipad_search_bar_init (WikipadSearchBar *bar)
{
  GtkWidget   *label, *image, *check, *menuitem;
  GtkToolItem *item;
  gboolean     match_case;

  /* load some saved state */
  match_case = WIKIPAD_SETTING_GET_BOOLEAN (SEARCH_MATCH_CASE);

  /* init variables */
  bar->highlight_id = 0;
  bar->match_case = match_case;

  /* the close button */
  item = gtk_tool_button_new_from_stock (GTK_STOCK_CLOSE);
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  g_signal_connect_swapped (G_OBJECT (item), "clicked", G_CALLBACK (wikipad_search_bar_hide_clicked), bar);
  gtk_widget_show (GTK_WIDGET (item));

  /* the find label */
  item = gtk_tool_item_new ();
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  gtk_widget_show (GTK_WIDGET (item));

  label = gtk_label_new_with_mnemonic (_("Fi_nd:"));
  gtk_container_add (GTK_CONTAINER (item), label);
  gtk_misc_set_padding (GTK_MISC (label), 2, 0);
  gtk_widget_show (label);

  /* the entry field */
  item = gtk_tool_item_new ();
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  gtk_widget_show (GTK_WIDGET (item));

  bar->entry = gtk_entry_new ();
  gtk_container_add (GTK_CONTAINER (item), bar->entry);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), bar->entry);
  g_signal_connect (G_OBJECT (bar->entry), "changed", G_CALLBACK (wikipad_search_bar_entry_changed), bar);
  g_signal_connect (G_OBJECT (bar->entry), "activate", G_CALLBACK (wikipad_search_bar_entry_activate), bar);
  g_signal_connect (G_OBJECT (bar->entry), "activate-backward", G_CALLBACK (wikipad_search_bar_entry_activate_backward), bar);
  gtk_widget_show (bar->entry);

  /* next button */
  image = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN, TOOL_BAR_ICON_SIZE);
  gtk_widget_show (image);

  item = gtk_tool_button_new (image, _("_Next"));
  gtk_tool_item_set_is_important (item, TRUE);
  gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (item), TRUE);
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  g_signal_connect_swapped (G_OBJECT (item), "clicked", G_CALLBACK (wikipad_search_bar_find_next), bar);
  gtk_widget_show (GTK_WIDGET (item));

  /* previous button */
  image = gtk_image_new_from_stock (GTK_STOCK_GO_UP, TOOL_BAR_ICON_SIZE);
  gtk_widget_show (image);

  item = gtk_tool_button_new (image, _("_Previous"));
  gtk_tool_item_set_is_important (item, TRUE);
  gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (item), TRUE);
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  g_signal_connect_swapped (G_OBJECT (item), "clicked", G_CALLBACK (wikipad_search_bar_find_previous), bar);
  gtk_widget_show (GTK_WIDGET (item));

  /* highlight all */
  item = (GtkToolItem *) gtk_toggle_tool_button_new ();
  g_signal_connect_object (G_OBJECT (bar), "destroy", G_CALLBACK (gtk_widget_destroy), item, G_CONNECT_SWAPPED);
  gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (item), GTK_STOCK_SELECT_ALL);
  gtk_tool_button_set_label (GTK_TOOL_BUTTON (item), _("Highlight _All"));
  gtk_tool_item_set_is_important (item, TRUE);
  gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (item), TRUE);
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  g_signal_connect (G_OBJECT (item), "clicked", G_CALLBACK (wikipad_search_bar_highlight_toggled), bar);
  gtk_widget_show (GTK_WIDGET (item));

  /* check button for case sensitive, including the proxy menu item */
  item = gtk_tool_item_new ();
  g_signal_connect_object (G_OBJECT (bar), "destroy", G_CALLBACK (gtk_widget_destroy), item, G_CONNECT_SWAPPED);
  gtk_toolbar_insert (GTK_TOOLBAR (bar), item, -1);
  gtk_widget_show (GTK_WIDGET (item));

  check = gtk_check_button_new_with_mnemonic (_("Mat_ch Case"));
  g_signal_connect_object (G_OBJECT (bar), "destroy", G_CALLBACK (gtk_widget_destroy), item, G_CONNECT_SWAPPED);
  gtk_container_add (GTK_CONTAINER (item), check);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), match_case);
  g_signal_connect (G_OBJECT (check), "toggled", G_CALLBACK (wikipad_search_bar_match_case_toggled), bar);
  gtk_widget_show (check);

  /* keep the widgets in sync with the GSettings */
  WIKIPAD_SETTING_BIND (SEARCH_MATCH_CASE, check, "active", G_SETTINGS_BIND_DEFAULT);

  /* overflow menu item for when window is too narrow to show the tool bar item */
  bar->match_case_entry = menuitem = gtk_check_menu_item_new_with_mnemonic (_("Mat_ch Case"));
  g_signal_connect_object (G_OBJECT (bar), "destroy", G_CALLBACK (gtk_widget_destroy), item, G_CONNECT_SWAPPED);
  gtk_tool_item_set_proxy_menu_item (item, "case-sensitive", menuitem);
  /* Keep toolbar check button and overflow proxy menu item in sync */
  g_object_bind_property (check, "active", menuitem, "active", G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
  gtk_widget_show (menuitem);
}



static void
wikipad_search_bar_finalize (GObject *object)
{
  WikipadSearchBar *bar = WIKIPAD_SEARCH_BAR (object);

  /* stop a running highlight timeout */
  if (bar->highlight_id != 0)
    g_source_remove (bar->highlight_id);

  (*G_OBJECT_CLASS (wikipad_search_bar_parent_class)->finalize) (object);
}



static void
wikipad_search_bar_find_string (WikipadSearchBar   *bar,
                                 WikipadSearchFlags  flags)
{
  const gchar *string;
  gint         nmatches;

  /* search the entire document */
  flags |= WIKIPAD_SEARCH_FLAGS_AREA_DOCUMENT;

  /* if we don't hightlight, we select with wrapping */
  if ((flags & WIKIPAD_SEARCH_FLAGS_ACTION_HIGHTLIGHT) == 0)
    flags |= WIKIPAD_SEARCH_FLAGS_ACTION_SELECT
             | WIKIPAD_SEARCH_FLAGS_WRAP_AROUND;

  /* append the insensitive flags when needed */
  if (bar->match_case)
    flags |= WIKIPAD_SEARCH_FLAGS_MATCH_CASE;

  /* get the entry string */
  string = gtk_entry_get_text (GTK_ENTRY (bar->entry));

  /* emit signal */
  g_signal_emit (G_OBJECT (bar), search_bar_signals[SEARCH], 0, flags, string, NULL, &nmatches);

  /* do nothing with the error entry when highlight when trigged with highlight */
  if ((flags & WIKIPAD_SEARCH_FLAGS_ACTION_HIGHTLIGHT) == 0)
    {
      /* make sure the search entry is not red when no text was typed */
      if (string == NULL || *string == '\0')
        nmatches = 1;

      /* change the entry style */
      wikipad_util_entry_error (bar->entry, nmatches < 1);
    }
}



static void
wikipad_search_bar_hide_clicked (WikipadSearchBar *bar)
{
  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* emit the signal */
  g_signal_emit (G_OBJECT (bar), search_bar_signals[HIDE_BAR], 0);
}



static void
wikipad_search_bar_entry_activate (GtkWidget         *entry,
                                    WikipadSearchBar *bar)
{
  wikipad_search_bar_find_next (bar);
}



static void
wikipad_search_bar_entry_activate_backward (GtkWidget         *entry,
                                             WikipadSearchBar *bar)
{
  wikipad_search_bar_find_previous (bar);
}



static void
wikipad_search_bar_entry_changed (GtkWidget         *entry,
                                   WikipadSearchBar *bar)
{
  WikipadSearchFlags flags;

  /* set the search flags */
  flags = WIKIPAD_SEARCH_FLAGS_ITER_SEL_START
          | WIKIPAD_SEARCH_FLAGS_DIR_FORWARD;

  /* find */
  wikipad_search_bar_find_string (bar, flags);

  /* schedule a new highlight */
  wikipad_search_bar_highlight_schedule (bar);
}



static void
wikipad_search_bar_highlight_toggled (GtkWidget         *button,
                                       WikipadSearchBar *bar)
{
  WikipadSearchFlags flags;

  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* set the new state */
  bar->highlight_all = gtk_toggle_tool_button_get_active (GTK_TOGGLE_TOOL_BUTTON (button));

  if (bar->highlight_all)
    {
      /* reschedule the highlight */
      wikipad_search_bar_highlight_schedule (bar);
    }
  else
    {
      /* stop timeout */
      if (bar->highlight_id != 0)
        g_source_remove (bar->highlight_id);

      /* set search flags */
      flags = WIKIPAD_SEARCH_FLAGS_ACTION_HIGHTLIGHT
              | WIKIPAD_SEARCH_FLAGS_ACTION_CLEANUP;

      /* emit signal to cleanup the highlight */
      wikipad_search_bar_find_string (bar, flags);
    }
}



static void
wikipad_search_bar_match_case_toggled (GtkWidget         *button,
                                        WikipadSearchBar *bar)
{
  gboolean active;

  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* get the state of the toggle button */
  active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button));

  /* save the state */
  bar->match_case = active;

  /* search ahead with this new flags */
  wikipad_search_bar_entry_changed (NULL, bar);

  /* schedule a new hightlight */
  wikipad_search_bar_highlight_schedule (bar);
}



static void
wikipad_search_bar_highlight_schedule (WikipadSearchBar *bar)
{
  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* stop a pending timeout */
  if (bar->highlight_id != 0)
    g_source_remove (bar->highlight_id);

  /* schedule a new timeout */
  if (bar->highlight_all)
    {
      bar->highlight_id = g_timeout_add_full (G_PRIORITY_LOW, HIGHTLIGHT_TIMEOUT, wikipad_search_bar_highlight_timeout,
                                              bar, wikipad_search_bar_highlight_timeout_destroy);
    }
}



static void
wikipad_search_bar_highlight_timeout_destroy (gpointer user_data)
{
  WIKIPAD_SEARCH_BAR (user_data)->highlight_id = 0;
}



static gboolean
wikipad_search_bar_highlight_timeout (gpointer user_data)
{
  WikipadSearchBar   *bar = WIKIPAD_SEARCH_BAR (user_data);
  WikipadSearchFlags  flags;

  /* set search flags */
  flags = WIKIPAD_SEARCH_FLAGS_DIR_FORWARD
          | WIKIPAD_SEARCH_FLAGS_ITER_AREA_START
          | WIKIPAD_SEARCH_FLAGS_ACTION_HIGHTLIGHT;

  /* emit signal */
  wikipad_search_bar_find_string (bar, flags);

  /* stop the timeout */
  return FALSE;
}



GtkEditable *
wikipad_search_bar_entry (WikipadSearchBar *bar)
{
  if (bar && gtk_widget_has_focus (bar->entry))
    return GTK_EDITABLE (bar->entry);
  else
    return NULL;
}



void
wikipad_search_bar_focus (WikipadSearchBar *bar)
{
  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* focus the entry field */
  gtk_widget_grab_focus (bar->entry);

  /* trigger search function */
  wikipad_search_bar_entry_changed (NULL, bar);

  /* update the highlight */
  wikipad_search_bar_highlight_schedule (bar);

  /* select the entire entry */
  gtk_editable_select_region (GTK_EDITABLE (bar->entry), 0, -1);
}



void
wikipad_search_bar_find_next (WikipadSearchBar *bar)
{
  WikipadSearchFlags flags;

  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* set search flags */
  flags = WIKIPAD_SEARCH_FLAGS_ITER_SEL_END
          | WIKIPAD_SEARCH_FLAGS_DIR_FORWARD;

  /* search */
  wikipad_search_bar_find_string (bar, flags);
}



void
wikipad_search_bar_find_previous (WikipadSearchBar *bar)
{
  WikipadSearchFlags flags;

  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  /* set search flags */
  flags = WIKIPAD_SEARCH_FLAGS_ITER_SEL_START
          | WIKIPAD_SEARCH_FLAGS_DIR_BACKWARD;

  /* search */
  wikipad_search_bar_find_string (bar, flags);
}



void
wikipad_search_bar_set_text (WikipadSearchBar *bar, gchar *text)
{
  g_return_if_fail (WIKIPAD_IS_SEARCH_BAR (bar));

  gtk_entry_set_text (GTK_ENTRY (bar->entry), text);
}

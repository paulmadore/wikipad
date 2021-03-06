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
#include <wikipad/wikipad-settings.h>
#include <wikipad/wikipad-document.h>
#include <wikipad/wikipad-replace-dialog.h>
#include <wikipad/wikipad-dialogs.h>
#include <wikipad/wikipad-util.h>
#include <wikipad/wikipad-marshal.h>
#include <wikipad/wikipad-gtkcompat.h>



static void                 wikipad_replace_dialog_unrealize           (GtkWidget             *widget);
static void                 wikipad_replace_dialog_finalize            (GObject               *object);
static void                 wikipad_replace_dialog_response            (GtkWidget             *widget,
                                                                         gint                   response_id);
static void                 wikipad_replace_dialog_changed             (WikipadReplaceDialog *dialog);
static void                 wikipad_replace_dialog_settings_changed    (WikipadReplaceDialog *dialog,
                                                                         gchar                 *key,
                                                                         GSettings             *settings);
static void                 wikipad_replace_dialog_history_combo_box   (GtkComboBoxText       *combo_box);
static void                 wikipad_replace_dialog_history_insert_text (const gchar           *text);



struct _WikipadReplaceDialogClass
{
  GtkDialogClass __parent__;
};

struct _WikipadReplaceDialog
{
  GtkDialog __parent__;

  /* dialog widgets */
  GtkWidget           *search_entry;
  GtkWidget           *replace_entry;
  GtkWidget           *find_button;
  GtkWidget           *replace_button;
  GtkWidget           *search_location_combo;
  GtkWidget           *hits_label;
};

enum
{
  IN_SELECTION = 0,
  IN_DOCUMENT,
  IN_ALL_DOCUMENTS
};

enum
{
  DIRECTION_UP = 0,
  DIRECTION_DOWN,
  DIRECTION_BOTH
};

enum
{
  SEARCH,
  LAST_SIGNAL
};



static GSList *history_list = NULL;
static guint   dialog_signals[LAST_SIGNAL];



G_DEFINE_TYPE (WikipadReplaceDialog, wikipad_replace_dialog, GTK_TYPE_DIALOG)


static void
wikipad_replace_dialog_class_init (WikipadReplaceDialogClass *klass)
{
  GObjectClass   *gobject_class;
  GtkWidgetClass *gtkwidget_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = wikipad_replace_dialog_finalize;

  gtkwidget_class = GTK_WIDGET_CLASS (klass);
  gtkwidget_class->unrealize = wikipad_replace_dialog_unrealize;

  dialog_signals[SEARCH] =
    g_signal_new (I_("search"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  0, NULL, NULL,
                  _wikipad_marshal_INT__FLAGS_STRING_STRING,
                  G_TYPE_INT, 3,
                  WIKIPAD_TYPE_SEARCH_FLAGS,
                  G_TYPE_STRING, G_TYPE_STRING);
}



static void
wikipad_replace_dialog_bind_setting (WikipadReplaceDialog *dialog,
                                      const gchar           *path,
                                      gpointer               object,
                                      const gchar           *property)
{
  wikipad_setting_bind (path, object, property, G_SETTINGS_BIND_DEFAULT);

  wikipad_setting_connect (path,
                            G_CALLBACK (wikipad_replace_dialog_settings_changed),
                            dialog,
                            G_CONNECT_SWAPPED);
}



static void
wikipad_replace_dialog_init (WikipadReplaceDialog *dialog)
{
  GtkWidget    *vbox, *hbox, *combo, *label, *check;
  GtkSizeGroup *size_group;

  /* set dialog properties */
  gtk_window_set_title (GTK_WINDOW (dialog), _("Replace"));
  gtk_window_set_default_size (GTK_WINDOW (dialog), 400, -1);
  gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);
  g_signal_connect (G_OBJECT (dialog), "response", G_CALLBACK (wikipad_replace_dialog_response), NULL);

  /* dialog buttons */
  dialog->find_button = gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_FIND, WIKIPAD_RESPONSE_FIND);
  dialog->replace_button = wikipad_util_image_button (GTK_STOCK_FIND_AND_REPLACE, _("_Replace"));
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog), dialog->replace_button, WIKIPAD_RESPONSE_REPLACE);
  gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_CLOSE, WIKIPAD_RESPONSE_CLOSE);
  gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_FIND);

  /* create main vertical box */
  vbox = g_object_new (GTK_TYPE_VBOX, "border-width", 6, "spacing", 4, NULL);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(GTK_DIALOG (dialog))), vbox, TRUE, TRUE, 0);
  gtk_widget_show (vbox);

  /* horizontal box for search string */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  /* create a size group */
  size_group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);

  label = gtk_label_new_with_mnemonic (_("_Search for:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_size_group_add_widget (size_group, label);
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_widget_show (label);

  combo = gtk_combo_box_text_new_with_entry ();
  wikipad_replace_dialog_history_combo_box (GTK_COMBO_BOX_TEXT (combo));
  gtk_box_pack_start (GTK_BOX (hbox), combo, TRUE, TRUE, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL(label), combo);
  gtk_widget_show (combo);

  /* store as an entry widget */
  dialog->search_entry = gtk_bin_get_child (GTK_BIN (combo));
  g_signal_connect_swapped (G_OBJECT (dialog->search_entry), "changed", G_CALLBACK (wikipad_replace_dialog_changed), dialog);

  /* horizontal box for replace string */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  label = gtk_label_new_with_mnemonic (_("Replace _with:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_size_group_add_widget (size_group, label);
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_widget_show (label);

  combo = gtk_combo_box_text_new_with_entry ();
  wikipad_replace_dialog_history_combo_box (GTK_COMBO_BOX_TEXT (combo));
  gtk_box_pack_start (GTK_BOX (hbox), combo, TRUE, TRUE, 0);
  gtk_widget_show (combo);

  /* store as an entry widget */
  dialog->replace_entry = gtk_bin_get_child (GTK_BIN (combo));

  /* search direction */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL(label), combo);
  gtk_widget_show (hbox);

  label = gtk_label_new_with_mnemonic (_("Search _direction:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_size_group_add_widget (size_group, label);
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_widget_show (label);

  combo = gtk_combo_box_text_new ();
  gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL(label), combo);
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), _("Up"));
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), _("Down"));
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), _("Both"));
  gtk_widget_show (combo);

  wikipad_replace_dialog_bind_setting (dialog, WIKIPAD_SETTING_SEARCH_DIRECTION, combo, "active");

  /* release size group */
  g_object_unref (G_OBJECT (size_group));

  /* case sensitive */
  check = gtk_check_button_new_with_mnemonic (_("Case sensi_tive"));
  gtk_box_pack_start (GTK_BOX (vbox), check, FALSE, FALSE, 0);
  gtk_widget_show (check);

  wikipad_replace_dialog_bind_setting (dialog, WIKIPAD_SETTING_SEARCH_MATCH_CASE, check, "active");

  /* match whole word */
  check = gtk_check_button_new_with_mnemonic (_("_Match whole word"));
  gtk_box_pack_start (GTK_BOX (vbox), check, FALSE, FALSE, 0);
  gtk_widget_show (check);

  wikipad_replace_dialog_bind_setting (dialog, WIKIPAD_SETTING_SEARCH_MATCH_WHOLE_WORD, check, "active");

  /* horizontal box for the replace all options */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  check = gtk_check_button_new_with_mnemonic (_("Replace _all in:"));
  gtk_box_pack_start (GTK_BOX (hbox), check, FALSE, FALSE, 0);
  gtk_widget_show (check);

  wikipad_replace_dialog_bind_setting (dialog, WIKIPAD_SETTING_SEARCH_REPLACE_ALL, check, "active");

  combo = dialog->search_location_combo = gtk_combo_box_text_new ();
  gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 0);
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), _("Selection"));
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), _("Document"));
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), _("All Documents"));
  gtk_widget_set_sensitive (combo, FALSE);
  gtk_widget_show (combo);

  wikipad_replace_dialog_bind_setting (dialog, WIKIPAD_SETTING_SEARCH_REPLACE_ALL_LOCATION, combo, "active");

  label = dialog->hits_label = gtk_label_new (NULL);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  /* update the state of the widgets */
  wikipad_replace_dialog_changed (dialog);
}



static void
wikipad_replace_dialog_unrealize (GtkWidget *widget)
{
  WikipadReplaceDialog *dialog = WIKIPAD_REPLACE_DIALOG (widget);
  const gchar           *text;

  g_return_if_fail (GTK_IS_ENTRY (dialog->replace_entry));
  g_return_if_fail (GTK_IS_ENTRY (dialog->search_entry));

  text = gtk_entry_get_text (GTK_ENTRY (dialog->search_entry));
  wikipad_replace_dialog_history_insert_text (text);

  text = gtk_entry_get_text (GTK_ENTRY (dialog->replace_entry));
  wikipad_replace_dialog_history_insert_text (text);

  (*GTK_WIDGET_CLASS (wikipad_replace_dialog_parent_class)->unrealize) (widget);
}



static void
wikipad_replace_dialog_finalize (GObject *object)
{
  /*WikipadReplaceDialog *dialog = WIKIPAD_REPLACE_DIALOG (object);*/

  (*G_OBJECT_CLASS (wikipad_replace_dialog_parent_class)->finalize) (object);
}



static void
wikipad_replace_dialog_response (GtkWidget *widget,
                                  gint       response_id)
{
  WikipadSearchFlags    flags;
  WikipadReplaceDialog *dialog = WIKIPAD_REPLACE_DIALOG (widget);
  gint                   matches;
  const gchar           *search_str, *replace_str;
  gchar                 *message;
  gint                   search_direction, replace_all_location;
  gboolean               match_case, match_whole_word, replace_all;

  /* read the search settings */
  search_direction = WIKIPAD_SETTING_GET_INT (SEARCH_DIRECTION);
  replace_all_location = WIKIPAD_SETTING_GET_INT (SEARCH_REPLACE_ALL_LOCATION);
  match_case = WIKIPAD_SETTING_GET_BOOLEAN (SEARCH_MATCH_CASE);
  match_whole_word = WIKIPAD_SETTING_GET_BOOLEAN (SEARCH_MATCH_WHOLE_WORD);
  replace_all = WIKIPAD_SETTING_GET_BOOLEAN (SEARCH_REPLACE_ALL);

  /* close dialog */
  if (response_id == WIKIPAD_RESPONSE_CLOSE)
    goto destroy_dialog;

  /* search direction */
  if (search_direction == DIRECTION_UP && ! replace_all)
    flags = WIKIPAD_SEARCH_FLAGS_DIR_BACKWARD;
  else
    flags = WIKIPAD_SEARCH_FLAGS_DIR_FORWARD;

  /* case sensitive searching */
  if (match_case)
    flags |= WIKIPAD_SEARCH_FLAGS_MATCH_CASE;

  /* only match whole words */
  if (match_whole_word)
    flags |= WIKIPAD_SEARCH_FLAGS_WHOLE_WORD;

  /* wrap around */
  if (search_direction == DIRECTION_BOTH && ! replace_all)
    flags |= WIKIPAD_SEARCH_FLAGS_WRAP_AROUND;

  /* search area */
  if (replace_all && replace_all_location == IN_SELECTION)
    flags |= WIKIPAD_SEARCH_FLAGS_AREA_SELECTION;
  else
    flags |= WIKIPAD_SEARCH_FLAGS_AREA_DOCUMENT;

  /* start position */
  if (response_id == WIKIPAD_RESPONSE_CHECK_ENTRY)
    {
      /* no visible actions */
      flags |= WIKIPAD_SEARCH_FLAGS_ACTION_NONE;

      if (replace_all)
        goto replace_flags;
      else
        goto search_flags;
    }
  else if (response_id == WIKIPAD_RESPONSE_FIND)
    {
      /* select the first match */
      flags |= WIKIPAD_SEARCH_FLAGS_ACTION_SELECT;

      search_flags:

      /* start at the 'end' of the selection */
      if (flags & WIKIPAD_SEARCH_FLAGS_DIR_BACKWARD)
        flags |= WIKIPAD_SEARCH_FLAGS_ITER_SEL_START;
      else
        flags |= WIKIPAD_SEARCH_FLAGS_ITER_SEL_END;
    }
  else if (response_id == WIKIPAD_RESPONSE_REPLACE)
    {
      /* replace matches */
      flags |= WIKIPAD_SEARCH_FLAGS_ACTION_REPLACE;

      if (replace_all)
        {
          replace_flags:

          /* replace all from the beginning of the document */
          flags |= WIKIPAD_SEARCH_FLAGS_ITER_AREA_START
                   | WIKIPAD_SEARCH_FLAGS_ENTIRE_AREA;

          /* search all opened documents (flag used in wikipad-window.c) */
          if (replace_all_location == IN_ALL_DOCUMENTS)
            flags |= WIKIPAD_SEARCH_FLAGS_ALL_DOCUMENTS;
        }
      else
        {
          /* start at the 'beginning' of the selection */
          if (flags & WIKIPAD_SEARCH_FLAGS_DIR_BACKWARD)
            flags |= WIKIPAD_SEARCH_FLAGS_ITER_SEL_END;
          else
            flags |= WIKIPAD_SEARCH_FLAGS_ITER_SEL_START;
        }
    }
  else
    {
      destroy_dialog:

      /* destroy the window */
      gtk_widget_destroy (widget);

      /* leave */
      return;
    }

  /* get strings */
  search_str = gtk_entry_get_text (GTK_ENTRY (dialog->search_entry));
  replace_str = gtk_entry_get_text (GTK_ENTRY (dialog->replace_entry));

  /* emit the signal */
  g_signal_emit (G_OBJECT (dialog), dialog_signals[SEARCH], 0, flags, search_str, replace_str, &matches);

  /* reset counter */
  if (response_id == WIKIPAD_RESPONSE_REPLACE && replace_all)
    matches = 0;

  /* update entry color */
  wikipad_util_entry_error (dialog->search_entry, matches == 0);

  /* update counter */
  if (replace_all)
    {
      message = g_strdup_printf (ngettext ("%d occurence", "%d occurences", matches), matches);
      gtk_label_set_markup (GTK_LABEL (dialog->hits_label), message);
      g_free (message);
    }
}


static void
wikipad_replace_dialog_changed (WikipadReplaceDialog *dialog)
{
  const gchar *text;
  gboolean     sensitive;
  gboolean     replace_all;

  replace_all = WIKIPAD_SETTING_GET_BOOLEAN (SEARCH_REPLACE_ALL);

  /* set the sensitivity of some dialog widgets */
  gtk_widget_set_sensitive (dialog->search_location_combo, replace_all);

  /* set new label of the replace button */
  gtk_button_set_label (GTK_BUTTON (dialog->replace_button),
                        replace_all ? _("_Replace All") : _("_Replace"));

  /* get the search entry text */
  text = gtk_entry_get_text (GTK_ENTRY (dialog->search_entry));

  if (text != NULL && *text != '\0')
    {
      /* do an invisible search to give the user some visible feedback */
      gtk_dialog_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_CHECK_ENTRY);

      /* buttons are sensitive */
      sensitive = TRUE;
    }
  else
    {
      /* not text, means no error */
      wikipad_util_entry_error (dialog->search_entry, FALSE);

      /* reset occurences label */
      gtk_label_set_text (GTK_LABEL (dialog->hits_label), NULL);

      /* buttons are not sensitive */
      sensitive = FALSE;
    }

  /* set the sensitivity */
  gtk_widget_set_sensitive (dialog->find_button, sensitive);
  gtk_widget_set_sensitive (dialog->replace_button, sensitive);
}



static void
wikipad_replace_dialog_settings_changed (WikipadReplaceDialog *dialog,
                                          gchar                 *key,
                                          GSettings             *settings)
{
  /* reset occurences label */
  gtk_label_set_text (GTK_LABEL (dialog->hits_label), NULL);

  wikipad_replace_dialog_changed (dialog);
}



/**
 * History functions
 **/
static void
wikipad_replace_dialog_history_combo_box (GtkComboBoxText *combo_box)
{
  GSList *li;

  g_return_if_fail (GTK_IS_COMBO_BOX (combo_box));

  /* append the items from the history to the combobox */
  for (li = history_list; li != NULL; li = li->next)
    gtk_combo_box_text_append_text (combo_box, li->data);
}



static void
wikipad_replace_dialog_history_insert_text (const gchar *text)
{
  GSList *li;

  /* quit if the box is empty */
  if (text == NULL || *text == '\0')
    return;

  /* check if the string is already in the history */
  for (li = history_list; li != NULL; li = li->next)
    if (strcmp (li->data, text) == 0)
      return;

  /* prepend the string */
  history_list = g_slist_prepend (history_list, g_strdup (text));
}



GtkWidget *
wikipad_replace_dialog_new (void)
{
  return g_object_new (WIKIPAD_TYPE_REPLACE_DIALOG, NULL);
}



void
wikipad_replace_dialog_history_clean (void)
{
  GSList *li;

  if (history_list)
    {
      /* remove all the entries */
      for (li = history_list; li != NULL; li = li->next)
        {
          /* cleanup the string */
          g_free (li->data);

          /* remove the item from the list */
          history_list = g_slist_delete_link (history_list, li);
        }

      /* cleanup the list */
      g_slist_free (history_list);
    }
}



void
wikipad_replace_dialog_page_switched (WikipadReplaceDialog *dialog)
{
  wikipad_replace_dialog_changed (dialog);
}



void
wikipad_replace_dialog_set_text (WikipadReplaceDialog *dialog, gchar *text)
{
  gtk_entry_set_text (GTK_ENTRY (dialog->search_entry), text);
}

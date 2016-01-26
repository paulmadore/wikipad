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
#include <wikipad/wikipad-dialogs.h>
#include <wikipad/wikipad-util.h>

#include <gtksourceview/gtksourcestylescheme.h>
#include <gtksourceview/gtksourcestyleschememanager.h>



void
wikipad_dialogs_show_about (GtkWindow *parent)
{
  static const gchar *authors[] =
  {
    "Paul Madore <moonpunter@gmail.com>",
    NULL
  };

  /* show the dialog */
  gtk_show_about_dialog (parent,
                         "authors", authors,
                         "comments", _("Wikipad is a Mousepad derivative for strictly for editing Wikipedia articles."),
                         "destroy-with-parent", TRUE,
                         "logo-icon-name", "accessories-text-editor",
                         "program-name", PACKAGE_NAME,
                         "version", PACKAGE_VERSION,
                         "translator-credits", _("translator-credits"),
                         "website", "http://www.phm.link/wikipad",
                         NULL);
}



void
wikipad_dialogs_show_error (GtkWindow    *parent,
                             const GError *error,
                             const gchar  *message)
{
  GtkWidget *dialog;

  /* create the warning dialog */
  dialog = gtk_message_dialog_new (parent,
                                   GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_CLOSE,
                                   "%s.", message);

  /* set secondary text if an error is provided */
  if (G_LIKELY (error != NULL))
    gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), "%s.", error->message);

  /* display the dialog */
  gtk_dialog_run (GTK_DIALOG (dialog));

  /* cleanup */
  gtk_widget_destroy (dialog);
}



void
wikipad_dialogs_show_help (GtkWindow   *parent,
                            const gchar *page,
                            const gchar *offset)
{
  GdkScreen   *screen;
  GError      *error = NULL;
  const gchar *uri;

  /* get screen */
  if (G_LIKELY (parent))
    screen = gtk_widget_get_screen (GTK_WIDGET (parent));
  else
    screen = gdk_screen_get_default ();

  uri = "http://docs.xfce.org";

  /* try to run the documentation browser */
  if (!gtk_show_uri (screen, uri, gtk_get_current_event_time (), &error))
    {
      /* display an error message to the user */
      wikipad_dialogs_show_error (parent, error, _("Failed to open the documentation browser"));
      g_error_free (error);
    }
}



gint
wikipad_dialogs_other_tab_size (GtkWindow *parent,
                                 gint      active_size)
{
  GtkWidget *dialog;
  GtkWidget *scale;

  /* build dialog */
  dialog = gtk_dialog_new_with_buttons (_("Select Tab Size"),
                                        parent,
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_STOCK_CANCEL, WIKIPAD_RESPONSE_CANCEL,
                                        GTK_STOCK_OK, WIKIPAD_RESPONSE_OK,
                                        NULL);
  gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_OK);

  /* create scale widget */
  scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 1, 32, 1);
  gtk_range_set_value (GTK_RANGE (scale), active_size);
  gtk_scale_set_digits (GTK_SCALE (scale), 0);
  gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
  gtk_scale_set_value_pos (GTK_SCALE (scale), GTK_POS_TOP);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG(dialog))), scale, TRUE, TRUE, 0);
  gtk_widget_show (scale);

  /* run the dialog */
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == WIKIPAD_RESPONSE_OK)
    active_size = gtk_range_get_value (GTK_RANGE (scale));

  /* destroy the dialog */
  gtk_widget_destroy (dialog);

  return active_size;
}



static void
wikipad_dialogs_go_to_line_changed (GtkSpinButton *line_spin,
                                     GtkSpinButton *col_spin)
{
  GtkTextBuffer *buffer;
  GtkTextIter    iter;

  g_return_if_fail (GTK_IS_SPIN_BUTTON (line_spin));
  g_return_if_fail (GTK_IS_SPIN_BUTTON (col_spin));

  /* get the text buffer */
  buffer = wikipad_object_get_data (G_OBJECT (col_spin), "buffer");

  /* debug check */
  g_return_if_fail (GTK_IS_TEXT_BUFFER (buffer));

  /* get iter at line */
  gtk_text_buffer_get_iter_at_line (buffer, &iter, gtk_spin_button_get_value_as_int (line_spin) - 1);

  /* move the iter to the end of the line if needed */
  if (!gtk_text_iter_ends_line (&iter))
    gtk_text_iter_forward_to_line_end (&iter);

  /* update column spin button range */
  gtk_spin_button_set_range (col_spin, 0, gtk_text_iter_get_line_offset (&iter));
}



gboolean
wikipad_dialogs_go_to (GtkWindow     *parent,
                        GtkTextBuffer *buffer)
{
  GtkWidget    *dialog;
  GtkWidget    *vbox;
  GtkWidget    *hbox;
  GtkWidget    *label;
  GtkWidget    *line_spin;
  GtkWidget    *col_spin;
  GtkSizeGroup *size_group;
  GtkTextIter   iter;
  gint          line, column, lines;
  gint          response;

  /* get cursor iter */
  gtk_text_buffer_get_iter_at_mark (buffer, &iter, gtk_text_buffer_get_insert (buffer));
  line = gtk_text_iter_get_line (&iter) + 1;

  /* get number of lines */
  lines = gtk_text_buffer_get_line_count (buffer);

  /* build the dialog */
  dialog = gtk_dialog_new_with_buttons (_("Go To"),
                                        parent,
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_STOCK_CANCEL, WIKIPAD_RESPONSE_CANCEL,
                                        GTK_STOCK_JUMP_TO, WIKIPAD_RESPONSE_JUMP_TO,
                                        NULL);
  gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_JUMP_TO);
  gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG(dialog))), vbox, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
  gtk_widget_show (vbox);

  /* create size group */
  size_group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);

  /* line number box */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  label = gtk_label_new_with_mnemonic (_("_Line number:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
  gtk_size_group_add_widget (size_group, label);
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_widget_show (label);

  line_spin = gtk_spin_button_new_with_range (1, lines, 1);
  gtk_entry_set_activates_default (GTK_ENTRY (line_spin), TRUE);
  gtk_box_pack_start (GTK_BOX (hbox), line_spin, FALSE, FALSE, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), line_spin);
  gtk_spin_button_set_snap_to_ticks (GTK_SPIN_BUTTON (line_spin), TRUE);
  gtk_entry_set_width_chars (GTK_ENTRY (line_spin), 8);
  gtk_widget_show (line_spin);

  /* column box */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  label = gtk_label_new_with_mnemonic (_("C_olumn number:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
  gtk_size_group_add_widget (size_group, label);
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_widget_show (label);

  col_spin = gtk_spin_button_new_with_range (0, 0, 1);
  gtk_entry_set_activates_default (GTK_ENTRY (col_spin), TRUE);
  wikipad_object_set_data (G_OBJECT (col_spin), "buffer", buffer);
  gtk_box_pack_start (GTK_BOX (hbox), col_spin, FALSE, FALSE, 0);
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), col_spin);
  gtk_spin_button_set_snap_to_ticks (GTK_SPIN_BUTTON (col_spin), TRUE);
  gtk_entry_set_width_chars (GTK_ENTRY (col_spin), 8);
  gtk_widget_show (col_spin);

  /* signal to monitor column number */
  g_signal_connect (G_OBJECT (line_spin), "value-changed", G_CALLBACK (wikipad_dialogs_go_to_line_changed), col_spin);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (line_spin), line);

  /* run the dialog */
  response = gtk_dialog_run (GTK_DIALOG (dialog));
  if (response == WIKIPAD_RESPONSE_JUMP_TO)
    {
      /* hide the dialog */
      gtk_widget_hide (dialog);

      /* get new position */
      line = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (line_spin)) - 1;
      column = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (col_spin));

      /* get iter */
      gtk_text_buffer_get_iter_at_line_offset (buffer, &iter, line, column);

      /* get cursor position */
      gtk_text_buffer_place_cursor (buffer, &iter);
    }

  /* release size group */
  g_object_unref (G_OBJECT (size_group));

  /* destroy the dialog */
  gtk_widget_destroy (dialog);

  return (response == WIKIPAD_RESPONSE_JUMP_TO);
}



gboolean
wikipad_dialogs_clear_recent (GtkWindow *parent)
{
  GtkWidget *dialog;
  GtkWidget *image;
  gboolean   succeed = FALSE;

  /* create the question dialog */
  dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_OTHER, GTK_BUTTONS_NONE,
                                   _("Remove all entries from the documents history?"));
  gtk_dialog_add_buttons (GTK_DIALOG (dialog),
                          GTK_STOCK_CANCEL, WIKIPAD_RESPONSE_CANCEL,
                          GTK_STOCK_CLEAR, WIKIPAD_RESPONSE_CLEAR,
                          NULL);
  gtk_window_set_title (GTK_WINDOW (dialog), _("Clear Documents History"));
  gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_CANCEL);
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                            _("Clearing the documents history will permanently "
                                              "remove all currently listed entries."));

  /* the dialog icon */
  image = gtk_image_new_from_stock (GTK_STOCK_CLEAR, GTK_ICON_SIZE_DIALOG);
  gtk_message_dialog_set_image (GTK_MESSAGE_DIALOG (dialog), image);
  gtk_widget_show (image);

  /* popup the dialog */
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == WIKIPAD_RESPONSE_CLEAR)
    succeed = TRUE;

  /* destroy the dialog */
  gtk_widget_destroy (dialog);

  return succeed;
}



gint
wikipad_dialogs_save_changes (GtkWindow *parent,
                               gboolean   readonly)
{
  GtkWidget *dialog;
  GtkWidget *image;
  gint       response;

  /* create the question dialog */
  dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_OTHER, GTK_BUTTONS_NONE,
                                   _("Do you want to save the changes before closing?"));
  gtk_window_set_title (GTK_WINDOW (dialog), _("Save Changes"));
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog), wikipad_util_image_button (GTK_STOCK_DELETE, _("_Don't Save")), WIKIPAD_RESPONSE_DONT_SAVE);
  gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_CANCEL, WIKIPAD_RESPONSE_CANCEL);

  /* we show the save as button instead of save for readonly document */
  if (G_UNLIKELY (readonly))
    {
      image = gtk_image_new_from_stock (GTK_STOCK_SAVE_AS, GTK_ICON_SIZE_DIALOG);
      gtk_dialog_add_buttons (GTK_DIALOG (dialog), GTK_STOCK_SAVE_AS, WIKIPAD_RESPONSE_SAVE_AS, NULL);
      gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_SAVE_AS);
    }
  else
    {
      image = gtk_image_new_from_stock (GTK_STOCK_SAVE, GTK_ICON_SIZE_DIALOG);
      gtk_dialog_add_buttons (GTK_DIALOG (dialog), GTK_STOCK_SAVE, WIKIPAD_RESPONSE_SAVE, NULL);
      gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_SAVE);
    }

  /* the dialog icon */
  gtk_message_dialog_set_image (GTK_MESSAGE_DIALOG (dialog), image);
  gtk_widget_show (image);

  /* secondary text */
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), _("If you don't save the document, all the changes will be lost."));

  /* run the dialog and wait for a response */
  response = gtk_dialog_run (GTK_DIALOG (dialog));

  /* destroy the dialog */
  gtk_widget_destroy (dialog);

  return response;
}



gint
wikipad_dialogs_externally_modified (GtkWindow *parent)
{
  GtkWidget *dialog;
  gint       response;

  /* create the question dialog */
  dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE,
                                   _("The document has been externally modified. Do you want to continue saving?"));
  gtk_window_set_title (GTK_WINDOW (dialog), _("Externally Modified"));
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                            _("If you save the document, all of the external changes will be lost."));
  gtk_dialog_add_buttons (GTK_DIALOG (dialog),
                          GTK_STOCK_CANCEL, WIKIPAD_RESPONSE_CANCEL,
                          GTK_STOCK_SAVE_AS, WIKIPAD_RESPONSE_SAVE_AS,
                          GTK_STOCK_SAVE, WIKIPAD_RESPONSE_SAVE, NULL);
  gtk_dialog_set_default_response (GTK_DIALOG (dialog), WIKIPAD_RESPONSE_CANCEL);

  /* run the dialog */
  response = gtk_dialog_run (GTK_DIALOG (dialog));

  /* destroy the dialog */
  gtk_widget_destroy (dialog);

  return response;
}



gint
wikipad_dialogs_revert (GtkWindow *parent)
{
  GtkWidget *dialog;
  gint       response;

  /* setup the question dialog */
  dialog = gtk_message_dialog_new (parent,
                                   GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE,
                                   _("Do you want to save your changes before reloading?"));
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                   _("If you revert the file, all unsaved changes will be lost."));
  gtk_dialog_add_buttons (GTK_DIALOG (dialog),
                          GTK_STOCK_CANCEL, WIKIPAD_RESPONSE_CANCEL,
                          GTK_STOCK_SAVE_AS, WIKIPAD_RESPONSE_SAVE_AS,
                          GTK_STOCK_REVERT_TO_SAVED, WIKIPAD_RESPONSE_REVERT,
                          NULL);

  /* run the dialog */
  response = gtk_dialog_run (GTK_DIALOG (dialog));

  /* destroy the dialog */
  gtk_widget_destroy (dialog);

  return response;
}

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

#ifndef __WIKIPAD_DIALOGS_H__
#define __WIKIPAD_DIALOGS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/* dialog responses */
enum {
  WIKIPAD_RESPONSE_CANCEL,
  WIKIPAD_RESPONSE_CLEAR,
  WIKIPAD_RESPONSE_CLOSE,
  WIKIPAD_RESPONSE_DONT_SAVE,
  WIKIPAD_RESPONSE_FIND,
  WIKIPAD_RESPONSE_JUMP_TO,
  WIKIPAD_RESPONSE_OK,
  WIKIPAD_RESPONSE_OVERWRITE,
  WIKIPAD_RESPONSE_REPLACE,
  WIKIPAD_RESPONSE_REVERT,
  WIKIPAD_RESPONSE_SAVE,
  WIKIPAD_RESPONSE_SAVE_AS,
  WIKIPAD_RESPONSE_CHECK_ENTRY
};

GtkWidget *wikipad_dialogs_image_button        (const gchar   *stock_id,
                                                 const gchar   *label);

void       wikipad_dialogs_show_about          (GtkWindow     *parent);

void       wikipad_dialogs_show_error          (GtkWindow     *parent,
                                                 const GError  *error,
                                                 const gchar   *message);

void       wikipad_dialogs_show_help           (GtkWindow     *parent,
                                                 const gchar   *page,
                                                 const gchar   *offset);

gint       wikipad_dialogs_other_tab_size      (GtkWindow     *parent,
                                                 gint           active_size);

gboolean   wikipad_dialogs_go_to               (GtkWindow     *parent,
                                                 GtkTextBuffer *buffer);

gboolean   wikipad_dialogs_clear_recent        (GtkWindow     *parent);

gint       wikipad_dialogs_save_changes        (GtkWindow     *parent,
                                                 gboolean       readonly);

gint       wikipad_dialogs_externally_modified (GtkWindow     *parent);

gint       wikipad_dialogs_revert              (GtkWindow     *parent);

G_END_DECLS

#endif /* !__WIKIPAD_DIALOGS_H__ */

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

#ifndef __WIKIPAD_REPLACE_DIALOG_H__
#define __WIKIPAD_REPLACE_DIALOG_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_REPLACE_DIALOG            (wikipad_replace_dialog_get_type ())
#define WIKIPAD_REPLACE_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_REPLACE_DIALOG, MousepadReplaceDialog))
#define WIKIPAD_REPLACE_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_REPLACE_DIALOG, MousepadReplaceDialogClass))
#define WIKIPAD_IS_REPLACE_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_REPLACE_DIALOG))
#define WIKIPAD_IS_REPLACE_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MOUSEPADL_TYPE_REPLACE_DIALOG))
#define WIKIPAD_REPLACE_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_REPLACE_DIALOG, MousepadReplaceDialogClass))

typedef struct _MousepadReplaceDialogClass MousepadReplaceDialogClass;
typedef struct _MousepadReplaceDialog      MousepadReplaceDialog;

GType           wikipad_replace_dialog_get_type       (void) G_GNUC_CONST;

GtkWidget      *wikipad_replace_dialog_new            (void);

void            wikipad_replace_dialog_history_clean  (void);

void            wikipad_replace_dialog_page_switched  (MousepadReplaceDialog *dialog);

void            wikipad_replace_dialog_set_text       (MousepadReplaceDialog *dialog, gchar *text);

G_END_DECLS

#endif /* !__WIKIPAD_REPLACE_DIALOG_H__ */

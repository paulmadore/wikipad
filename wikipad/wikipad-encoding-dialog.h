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

#ifndef __WIKIPAD_ENCODING_DIALOG_H__
#define __WIKIPAD_ENCODING_DIALOG_H__

G_BEGIN_DECLS

#include <wikipad/wikipad-encoding.h>

#define WIKIPAD_TYPE_ENCODING_DIALOG            (wikipad_encoding_dialog_get_type ())
#define WIKIPAD_ENCODING_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_ENCODING_DIALOG, WikipadEncodingDialog))
#define WIKIPAD_ENCODING_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_ENCODING_DIALOG, WikipadEncodingDialogClass))
#define WIKIPAD_IS_ENCODING_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_ENCODING_DIALOG))
#define WIKIPAD_IS_ENCODING_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_ENCODING_DIALOG))
#define WIKIPAD_ENCODING_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_ENCODING_DIALOG, WikipadEncodingDialogClass))

typedef struct _WikipadEncodingDialogClass WikipadEncodingDialogClass;
typedef struct _WikipadEncodingDialog      WikipadEncodingDialog;

GType             wikipad_encoding_dialog_get_type            (void) G_GNUC_CONST;

GtkWidget        *wikipad_encoding_dialog_new                 (GtkWindow              *parent,
                                                                WikipadFile           *file);

WikipadEncoding  wikipad_encoding_dialog_get_encoding        (WikipadEncodingDialog *dialog);

const gchar      *wikipad_encoding_dialog_get_encoding_custom (WikipadEncodingDialog *dialog);

G_END_DECLS

#endif /* !__WIKIPAD_ENCODING_DIALOG_H__ */

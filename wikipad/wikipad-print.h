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

#ifndef __WIKIPAD_PRINT_H__
#define __WIKIPAD_PRINT_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _WikipadPrintClass WikipadPrintClass;
typedef struct _WikipadPrint      WikipadPrint;

#define WIKIPAD_TYPE_PRINT            (wikipad_print_get_type ())
#define WIKIPAD_PRINT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_PRINT, WikipadPrint))
#define WIKIPAD_PRINT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_PRINT, WikipadPrintClass))
#define WIKIPAD_IS_PRINT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_PRINT))
#define WIKIPAD_IS_PRINT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_PRINT))
#define WIKIPAD_PRINT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_PRINT, WikipadPrintClass))

GType          wikipad_print_get_type             (void) G_GNUC_CONST;

WikipadPrint *wikipad_print_new                  (void);

gboolean       wikipad_print_document_interactive (WikipadPrint     *print,
                                                    WikipadDocument  *document,
                                                    GtkWindow         *parent,
                                                    GError           **error);

G_END_DECLS

#endif /* !__WIKIPAD_PRINT_H__ */

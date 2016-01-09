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

#ifndef __WIKIPAD_STATUSBAR_H__
#define __WIKIPAD_STATUSBAR_H__

#include <gtk/gtk.h>
#include <gtksourceview/gtksourcelanguage.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_STATUSBAR            (wikipad_statusbar_get_type ())
#define WIKIPAD_STATUSBAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_STATUSBAR, WikipadStatusbar))
#define WIKIPAD_STATUSBAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_STATUSBAR, WikipadStatusbarClass))
#define WIKIPAD_IS_STATUSBAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_STATUSBAR))
#define WIKIPAD_IS_STATUSBAR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_STATUSBAR))
#define WIKIPAD_STATUSBAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_STATUSBAR, WikipadStatusbarClass))

typedef struct _WikipadStatusbarClass WikipadStatusbarClass;
typedef struct _WikipadStatusbar      WikipadStatusbar;

GType       wikipad_statusbar_get_type             (void) G_GNUC_CONST;

GtkWidget  *wikipad_statusbar_new                  (void);

void        wikipad_statusbar_set_cursor_position  (WikipadStatusbar *statusbar,
                                                     gint               line,
                                                     gint               column,
                                                     gint               selection);

void        wikipad_statusbar_set_overwrite        (WikipadStatusbar *statusbar,
                                                     gboolean           overwrite);

void        wikipad_statusbar_set_language         (WikipadStatusbar *statusbar,
                                                     GtkSourceLanguage *language);

gboolean    wikipad_statusbar_push_tooltip         (WikipadStatusbar *statusbar,
                                                     GtkWidget         *widget);

void        wikipad_statusbar_pop_tooltip          (WikipadStatusbar *statusbar,
                                                     GtkWidget         *widget);

G_END_DECLS

#endif /* !__WIKIPAD_STATUSBAR_H__ */

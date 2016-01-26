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

#ifndef __WIKIPAD_WINDOW_H__
#define __WIKIPAD_WINDOW_H__

#include <wikipad/wikipad-document.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_WINDOW            (wikipad_window_get_type ())
#define WIKIPAD_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_WINDOW, WikipadWindow))
#define WIKIPAD_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_WINDOW, WikipadWindowClass))
#define WIKIPAD_IS_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_WINDOW))
#define WIKIPAD_IS_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MOUSEPADL_TYPE_WINDOW))
#define WIKIPAD_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_WINDOW, WikipadWindowClass))

enum
{
  TARGET_TEXT_URI_LIST,
  TARGET_GTK_NOTEBOOK_TAB
};

static const GtkTargetEntry drop_targets[] =
{
  { (gchar *) "text/uri-list", 0, TARGET_TEXT_URI_LIST },
  { (gchar *) "GTK_NOTEBOOK_TAB", GTK_TARGET_SAME_APP, TARGET_GTK_NOTEBOOK_TAB }
};

typedef struct _WikipadWindowClass WikipadWindowClass;
typedef struct _WikipadWindow      WikipadWindow;

GType           wikipad_window_get_type         (void) G_GNUC_CONST;

GtkWidget      *wikipad_window_new              (void);

void            wikipad_window_add              (WikipadWindow   *window,
                                                  WikipadDocument *document);

gboolean        wikipad_window_open_files       (WikipadWindow  *window,
                                                  const gchar     *working_directory,
                                                  gchar          **filenames);

void            wikipad_window_show_preferences (WikipadWindow  *window);

G_END_DECLS

#endif /* !__WIKIPAD_WINDOW_H__ */

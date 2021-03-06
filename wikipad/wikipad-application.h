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

#ifndef __WIKIPAD_APPLICATION_H__
#define __WIKIPAD_APPLICATION_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _WikipadApplicationClass WikipadApplicationClass;
typedef struct _WikipadApplication      WikipadApplication;

#define WIKIPAD_TYPE_APPLICATION            (wikipad_application_get_type ())
#define WIKIPAD_APPLICATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_APPLICATION, WikipadApplication))
#define WIKIPAD_APPLICATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_APPLICATION, WikipadApplicationClass))
#define WIKIPAD_IS_APPLICATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_APPLICATION))
#define WIKIPAD_IS_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_APPLICATION))
#define WIKIPAD_APPLICATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_APPLICATION, WikipadApplicationClass))

GType                wikipad_application_get_type               (void) G_GNUC_CONST;

WikipadApplication *wikipad_application_get                    (void);

gboolean             wikipad_application_has_windows            (WikipadApplication  *application);

void                 wikipad_application_take_window            (WikipadApplication  *application,
                                                                  GtkWindow            *window);

void                 wikipad_application_new_window_with_files  (WikipadApplication  *application,
                                                                  GdkScreen            *screen,
                                                                  const gchar          *working_directory,
                                                                  gchar               **filenames);

void                 wikipad_application_show_preferences       (WikipadApplication  *application,
                                                                  GtkWindow            *transient_for);

G_END_DECLS

#endif /* !__WIKIPAD_APPLICATION_H__ */

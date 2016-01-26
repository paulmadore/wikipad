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

#ifndef __WIKIPAD_FILE_H__
#define __WIKIPAD_FILE_H__

G_BEGIN_DECLS

#include <wikipad/wikipad-encoding.h>

#include <gtksourceview/gtksourcelanguage.h>

typedef struct _WikipadFileClass  WikipadFileClass;
typedef struct _WikipadFile       WikipadFile;

#define WIKIPAD_TYPE_FILE            (wikipad_file_get_type ())
#define WIKIPAD_FILE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_FILE, WikipadFile))
#define WIKIPAD_FILE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_FILE, WikipadFileClass))
#define WIKIPAD_IS_FILE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_FILE))
#define WIKIPAD_IS_FILE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_FILE))
#define WIKIPAD_FILE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_FILE, WikipadFileClass))

typedef enum
{
  WIKIPAD_EOL_UNIX,
  WIKIPAD_EOL_MAC,
  WIKIPAD_EOL_DOS
}
WikipadLineEnding;

GType               wikipad_file_get_type                 (void) G_GNUC_CONST;

WikipadFile       *wikipad_file_new                      (GtkTextBuffer       *buffer);

void                wikipad_file_set_filename             (WikipadFile        *file,
                                                            const gchar         *filename);

const gchar        *wikipad_file_get_filename             (WikipadFile        *file);

gchar              *wikipad_file_get_uri                  (WikipadFile        *file);

void                wikipad_file_set_encoding             (WikipadFile        *file,
                                                            WikipadEncoding     encoding);

WikipadEncoding    wikipad_file_get_encoding             (WikipadFile        *file);

gboolean            wikipad_file_get_read_only            (WikipadFile        *file);

void                wikipad_file_set_write_bom            (WikipadFile        *file,
                                                            gboolean             write_bom);

gboolean            wikipad_file_get_write_bom            (WikipadFile        *file,
                                                            gboolean            *sensitive);

void                wikipad_file_set_line_ending          (WikipadFile        *file,
                                                            WikipadLineEnding   line_ending);

WikipadLineEnding  wikipad_file_get_line_ending          (WikipadFile        *file);

void                wikipad_file_set_language             (WikipadFile        *file,
                                                            GtkSourceLanguage   *language);

GtkSourceLanguage  *wikipad_file_get_language             (WikipadFile        *file);

void                wikipad_file_set_language_id          (WikipadFile        *file,
                                                            const gchar         *language_id);

const gchar        *wikipad_file_get_language_id          (WikipadFile        *file);

GtkSourceLanguage  *wikipad_file_guess_language           (WikipadFile        *file);

gint                wikipad_file_open                     (WikipadFile        *file,
                                                            const gchar         *template_filename,
                                                            GError             **error);

gboolean            wikipad_file_save                     (WikipadFile        *file,
                                                            GError             **error);

gboolean            wikipad_file_reload                   (WikipadFile        *file,
                                                            GError             **error);

gboolean            wikipad_file_get_externally_modified  (WikipadFile        *file,
                                                            GError             **error);

void                wikipad_file_set_user_set_language    (WikipadFile        *file,
                                                            gboolean             set_by_user);
G_END_DECLS

#endif /* !__WIKIPAD_FILE_H__ */

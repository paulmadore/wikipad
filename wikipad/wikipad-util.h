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

#ifndef __WIKIPAD_UTIL_H__
#define __WIKIPAD_UTIL_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_SEARCH_FLAGS (wikipad_util_search_flags_get_type ())

typedef enum
{
  /* search area */
  WIKIPAD_SEARCH_FLAGS_AREA_DOCUMENT     = 1 << 0,  /* search the entire document */
  WIKIPAD_SEARCH_FLAGS_AREA_SELECTION    = 1 << 1,  /* search inside selection */

  /* iter start point */
  WIKIPAD_SEARCH_FLAGS_ITER_AREA_START   = 1 << 2,  /* search from the beginning of the area */
  WIKIPAD_SEARCH_FLAGS_ITER_AREA_END     = 1 << 3,  /* search from the end of the area */
  WIKIPAD_SEARCH_FLAGS_ITER_SEL_START    = 1 << 4,  /* start at from the beginning of the selection */
  WIKIPAD_SEARCH_FLAGS_ITER_SEL_END      = 1 << 5,  /* start at from the end of the selection */

  /* direction */
  WIKIPAD_SEARCH_FLAGS_DIR_FORWARD       = 1 << 6,  /* search forward to end of area */
  WIKIPAD_SEARCH_FLAGS_DIR_BACKWARD      = 1 << 7,  /* search backwards to start of area */

  /* search settings */
  WIKIPAD_SEARCH_FLAGS_MATCH_CASE        = 1 << 8,  /* match case */
  WIKIPAD_SEARCH_FLAGS_WHOLE_WORD        = 1 << 9,  /* only match whole words */
  WIKIPAD_SEARCH_FLAGS_WRAP_AROUND       = 1 << 10, /* wrap around */
  WIKIPAD_SEARCH_FLAGS_ENTIRE_AREA       = 1 << 11, /* keep searching until the end of the area is reached */
  WIKIPAD_SEARCH_FLAGS_ALL_DOCUMENTS     = 1 << 12, /* search all documents */


  /* actions */
  WIKIPAD_SEARCH_FLAGS_ACTION_NONE       = 1 << 13, /* no visible actions */
  WIKIPAD_SEARCH_FLAGS_ACTION_HIGHTLIGHT = 1 << 14, /* highlight all the occurences */
  WIKIPAD_SEARCH_FLAGS_ACTION_CLEANUP    = 1 << 15, /* cleanup the highlighted occurences */
  WIKIPAD_SEARCH_FLAGS_ACTION_SELECT     = 1 << 16, /* select the match */
  WIKIPAD_SEARCH_FLAGS_ACTION_REPLACE    = 1 << 17, /* replace the match */
}
WikipadSearchFlags;

gboolean   wikipad_util_iter_starts_word                 (const GtkTextIter   *iter);

gboolean   wikipad_util_iter_ends_word                   (const GtkTextIter   *iter);

gboolean   wikipad_util_iter_inside_word                 (const GtkTextIter   *iter);

gboolean   wikipad_util_iter_forward_word_end            (GtkTextIter         *iter);

gboolean   wikipad_util_iter_backward_word_start         (GtkTextIter         *iter);

gboolean   wikipad_util_iter_forward_text_start          (GtkTextIter         *iter);

gboolean   wikipad_util_iter_backward_text_start         (GtkTextIter         *iter);

gchar     *wikipad_util_config_name                      (const gchar         *name);

gchar     *wikipad_util_key_name                         (const gchar         *name);

gchar     *wikipad_util_utf8_strcapital                  (const gchar         *str);

gchar     *wikipad_util_utf8_stropposite                 (const gchar         *str);

gchar     *wikipad_util_escape_underscores               (const gchar         *str);

GtkWidget *wikipad_util_image_button                     (const gchar         *stock_id,
                                                           const gchar         *label);

void       wikipad_util_entry_error                      (GtkWidget           *widget,
                                                           gboolean             error);

void       wikipad_util_dialog_header                    (GtkDialog           *dialog,
                                                           const gchar         *title,
                                                           const gchar         *subtitle,
                                                           const gchar         *icon);

gint       wikipad_util_get_real_line_offset             (const GtkTextIter   *iter,
                                                           gint                 tab_size);

gboolean   wikipad_util_forward_iter_to_text             (GtkTextIter         *iter,
                                                           const GtkTextIter   *limit);

gchar     *wikipad_util_get_save_location                (const gchar         *relpath,
                                                           gboolean             create_parents);

void       wikipad_util_save_key_file                    (GKeyFile            *keyfile,
                                                           const gchar         *filename);

GType      wikipad_util_search_flags_get_type            (void) G_GNUC_CONST;

gint       wikipad_util_highlight                        (GtkTextBuffer       *buffer,
                                                           GtkTextTag          *tag,
                                                           const gchar         *string,
                                                           WikipadSearchFlags  flags);

gint       wikipad_util_search                           (GtkTextBuffer       *buffer,
                                                           const gchar         *string,
                                                           const gchar         *replace,
                                                           WikipadSearchFlags  flags);

GtkAction *wikipad_util_find_related_action              (GtkWidget           *widget);

GIcon     *wikipad_util_icon_for_mime_type               (const gchar         *mime_type);

gboolean   wikipad_util_container_has_children           (GtkContainer        *container);

void       wikipad_util_container_clear                  (GtkContainer        *container);

void       wikipad_util_container_move_children          (GtkContainer        *source,
                                                           GtkContainer        *destination);

G_END_DECLS

#endif /* !__WIKIPAD_UTIL_H__ */

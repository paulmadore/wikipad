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

#ifndef __WIKIPAD_VIEW_H__
#define __WIKIPAD_VIEW_H__

G_BEGIN_DECLS

#define WIKIPAD_TYPE_VIEW            (wikipad_view_get_type ())
#define WIKIPAD_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_VIEW, WikipadView))
#define WIKIPAD_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_VIEW, WikipadViewClass))
#define WIKIPAD_IS_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_VIEW))
#define WIKIPAD_IS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MOUSEPADL_TYPE_VIEW))
#define WIKIPAD_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_VIEW, WikipadViewClass))

typedef struct _WikipadViewClass WikipadViewClass;
typedef struct _WikipadView      WikipadView;

enum
{
  LOWERCASE,
  UPPERCASE,
  TITLECASE,
  OPPOSITE_CASE
};

enum
{
  SPACES_TO_TABS,
  TABS_TO_SPACES
};

enum
{
  MOVE_LINE_UP,
  MOVE_LINE_DOWN
};

enum
{
  INCREASE_INDENT,
  DECREASE_INDENT
};

GType           wikipad_view_get_type                  (void) G_GNUC_CONST;

void            wikipad_view_scroll_to_cursor          (WikipadView      *view);

void            wikipad_view_transpose                 (WikipadView      *view);

void            wikipad_view_clipboard_cut             (WikipadView      *view);

void            wikipad_view_clipboard_copy            (WikipadView      *view);

void            wikipad_view_clipboard_paste           (WikipadView      *view,
                                                         const gchar       *string,
                                                         gboolean           paste_as_column);

void            wikipad_view_delete_selection          (WikipadView      *view);

void            wikipad_view_select_all                (WikipadView      *view);

void            wikipad_view_change_selection          (WikipadView      *view);

void            wikipad_view_convert_selection_case    (WikipadView      *view,
                                                         gint               type);

void            wikipad_view_convert_spaces_and_tabs   (WikipadView      *view,
                                                         gint               type);

void            wikipad_view_strip_trailing_spaces     (WikipadView      *view);

void            wikipad_view_move_selection            (WikipadView      *view,
                                                         gint               type);

void            wikipad_view_duplicate                 (WikipadView      *view);

void            wikipad_view_indent                    (WikipadView      *view,
                                                         gint               type);

gint            wikipad_view_get_selection_length      (WikipadView      *view,
                                                         gboolean          *is_column_selection);

void            wikipad_view_set_font_name             (WikipadView      *view,
                                                         const gchar       *font_name);

const gchar    *wikipad_view_get_font_name             (WikipadView      *view);

void            wikipad_view_set_show_whitespace       (WikipadView      *view,
                                                         gboolean           show);

gboolean        wikipad_view_get_show_whitespace       (WikipadView      *view);

void            wikipad_view_set_show_line_endings     (WikipadView      *view,
                                                         gboolean           show);

gboolean        wikipad_view_get_show_line_endings     (WikipadView      *view);

void            wikipad_view_set_color_scheme          (WikipadView      *view,
                                                         const gchar       *color_scheme);

const gchar    *wikipad_view_get_color_scheme          (WikipadView      *view);

void            wikipad_view_set_word_wrap             (WikipadView      *view,
                                                         gboolean           enabled);

gboolean        wikipad_view_get_word_wrap             (WikipadView      *view);

void            wikipad_view_set_match_braces          (WikipadView      *view,
                                                         gboolean           enabled);

gboolean        wikipad_view_get_match_braces          (WikipadView      *view);

G_END_DECLS

#endif /* !__WIKIPAD_VIEW_H__ */

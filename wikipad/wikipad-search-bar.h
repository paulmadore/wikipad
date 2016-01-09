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

#ifndef __WIKIPAD_SEARCH_BAR_H__
#define __WIKIPAD_SEARCH_BAR_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_SEARCH_BAR            (wikipad_search_bar_get_type ())
#define WIKIPAD_SEARCH_BAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_SEARCH_BAR, WikipadSearchBar))
#define WIKIPAD_SEARCH_BAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_SEARCH_BAR, WikipadSearchBarClass))
#define WIKIPAD_IS_SEARCH_BAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_SEARCH_BAR))
#define WIKIPAD_IS_SEARCH_BAR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MOUSEPADL_TYPE_SEARCH_BAR))
#define WIKIPAD_SEARCH_BAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_SEARCH_BAR, WikipadSearchBarClass))

typedef struct _WikipadSearchBarClass WikipadSearchBarClass;
typedef struct _WikipadSearchBar      WikipadSearchBar;

GType           wikipad_search_bar_get_type        (void) G_GNUC_CONST;

GtkWidget      *wikipad_search_bar_new             (void);

GtkEditable    *wikipad_search_bar_entry           (WikipadSearchBar *bar);

void            wikipad_search_bar_focus           (WikipadSearchBar *bar);

void            wikipad_search_bar_find_next       (WikipadSearchBar *bar);

void            wikipad_search_bar_find_previous   (WikipadSearchBar *bar);

void            wikipad_search_bar_set_text        (WikipadSearchBar *bar, gchar *text);

G_END_DECLS

#endif /* !__WIKIPAD_SEARCH_BAR_H__ */

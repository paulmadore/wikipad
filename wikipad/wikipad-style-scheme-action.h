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

#ifndef __WIKIPAD_STYLESCHEME_ACTION_H__
#define __WIKIPAD_STYLESCHEME_ACTION_H__ 1

#include <gtk/gtk.h>
#include <gtksourceview/gtksourcestylescheme.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_STYLE_SCHEME_ACTION            (wikipad_style_scheme_action_get_type ())
#define WIKIPAD_STYLE_SCHEME_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_STYLE_SCHEME_ACTION, MousepadStyleSchemeAction))
#define WIKIPAD_STYLE_SCHEME_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_STYLE_SCHEME_ACTION, MousepadStyleSchemeActionClass))
#define WIKIPAD_IS_STYLE_SCHEME_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_STYLE_SCHEME_ACTION))
#define WIKIPAD_IS_STYLE_SCHEME_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_STYLE_SCHEME_ACTION))
#define WIKIPAD_STYLE_SCHEME_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_STYLE_SCHEME_ACTION, MousepadStyleSchemeActionClass))

typedef struct MousepadStyleSchemeAction_      MousepadStyleSchemeAction;
typedef struct MousepadStyleSchemeActionClass_ MousepadStyleSchemeActionClass;

GType                 wikipad_style_scheme_action_get_type         (void);

GtkAction            *wikipad_style_scheme_action_new              (GtkSourceStyleScheme      *scheme);

GtkSourceStyleScheme *wikipad_style_scheme_action_get_style_scheme (MousepadStyleSchemeAction *action);

G_END_DECLS

#endif /* __WIKIPAD_STYLESCHEME_ACTION_H__ */

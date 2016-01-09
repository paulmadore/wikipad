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

#ifndef __WIKIPAD_ACTION_GROUP_H__
#define __WIKIPAD_ACTION_GROUP_H__ 1

#include <gtk/gtk.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcestylescheme.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_ACTION_GROUP            (wikipad_action_group_get_type ())
#define WIKIPAD_ACTION_GROUP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_ACTION_GROUP, WikipadActionGroup))
#define WIKIPAD_ACTION_GROUP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_ACTION_GROUP, WikipadActionGroupClass))
#define WIKIPAD_IS_ACTION_GROUP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_ACTION_GROUP))
#define WIKIPAD_IS_ACTION_GROUP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_ACTION_GROUP))
#define WIKIPAD_ACTION_GROUP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_ACTION_GROUP, WikipadActionGroupClass))

typedef struct WikipadActionGroup_      WikipadActionGroup;
typedef struct WikipadActionGroupClass_ WikipadActionGroupClass;

GType                 wikipad_action_group_get_type                 (void);

GtkActionGroup       *wikipad_action_group_new                      (void);

void                  wikipad_action_group_set_active_language      (WikipadActionGroup *group,
                                                                      GtkSourceLanguage   *language);

GtkSourceLanguage    *wikipad_action_group_get_active_language      (WikipadActionGroup *group);

GtkWidget            *wikipad_action_group_create_language_menu     (WikipadActionGroup *group);

void                  wikipad_action_group_set_active_style_scheme  (WikipadActionGroup  *group,
                                                                      GtkSourceStyleScheme *scheme);

GtkSourceStyleScheme *wikipad_action_group_get_active_style_scheme  (WikipadActionGroup *group);

GtkWidget            *wikipad_action_group_create_style_scheme_menu (WikipadActionGroup *group);

G_END_DECLS

#endif /* __WIKIPAD_ACTION_GROUP_H__ */

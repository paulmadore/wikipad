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

#ifndef __WIKIPAD_PREFS_DIALOG_H__
#define __WIKIPAD_PREFS_DIALOG_H__ 1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_PREFS_DIALOG             (wikipad_prefs_dialog_get_type ())
#define WIKIPAD_PREFS_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_PREFS_DIALOG, WikipadPrefsDialog))
#define WIKIPAD_PREFS_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_PREFS_DIALOG, WikipadPrefsDialogClass))
#define WIKIPAD_IS_PREFS_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_PREFS_DIALOG))
#define WIKIPAD_IS_PREFS_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_PREFS_DIALOG))
#define WIKIPAD_PREFS_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_PREFS_DIALOG, WikipadPrefsDialogClass))

typedef struct WikipadPrefsDialog_         WikipadPrefsDialog;
typedef struct WikipadPrefsDialogClass_    WikipadPrefsDialogClass;

GType      wikipad_prefs_dialog_get_type (void);

GtkWidget *wikipad_prefs_dialog_new      (void);

G_END_DECLS

#endif /* __WIKIPAD_PREFS_DIALOG_H__ */

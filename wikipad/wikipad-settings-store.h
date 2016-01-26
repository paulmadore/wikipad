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

#ifndef __WIKIPAD_SETTINGS_STORE_H__
#define __WIKIPAD_SETTINGS_STORE_H__ 1

#include <gio/gio.h>

G_BEGIN_DECLS


#define WIKIPAD_TYPE_SETTINGS_STORE            (wikipad_settings_store_get_type ())
#define WIKIPAD_SETTINGS_STORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_SETTINGS_STORE, WikipadSettingsStore))
#define WIKIPAD_SETTINGS_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_SETTINGS_STORE, WikipadSettingsStoreClass))
#define WIKIPAD_IS_SETTINGS_STORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_SETTINGS_STORE))
#define WIKIPAD_IS_SETTINGS_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_SETTINGS_STORE))
#define WIKIPAD_SETTINGS_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_SETTINGS_STORE, WikipadSettingsStoreClass))


typedef struct WikipadSettingsStore_      WikipadSettingsStore;
typedef struct WikipadSettingsStoreClass_ WikipadSettingsStoreClass;


GType                  wikipad_settings_store_get_type        (void);

WikipadSettingsStore *wikipad_settings_store_new             (void);

const gchar           *wikipad_settings_store_lookup_key_name (WikipadSettingsStore *store,
                                                                const gchar           *path);

GSettings             *wikipad_settings_store_lookup_settings (WikipadSettingsStore *store,
                                                                const gchar           *path);

gboolean               wikipad_settings_store_lookup          (WikipadSettingsStore *store,
                                                                const gchar           *path,
                                                                const gchar          **key_name,
                                                                GSettings            **settings);

G_END_DECLS

#endif /* __WIKIPAD_SETTINGS_STORE_H__ */

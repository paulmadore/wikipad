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

#ifndef __WIKIPAD_DBUS_H__
#define __WIKIPAD_DBUS_H__

typedef struct _MousepadDBusServiceClass MousepadDBusServiceClass;
typedef struct _MousepadDBusService      MousepadDBusService;

#define WIKIPAD_TYPE_DBUS_SERVICE            (wikipad_dbus_service_get_type ())
#define WIKIPAD_DBUS_SERVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_DBUS_SERVICE, MousepadDBusService))
#define WIKIPAD_DBUS_SERVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_DBUS_SERVICE, MousepadDBusServiceClass))
#define WIKIPAD_IS_DBUS_SERVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_DBUS_SERVICE))
#define WIKIPAD_IS_DBUS_SERVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_DBUS_BRIGDE))
#define WIKIPAD_DBUS_SERVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_DBUS_SERVICE, MousepadDBusServiceClass))

GType     wikipad_dbus_service_get_type    (void) G_GNUC_CONST;

gboolean  wikipad_dbus_client_terminate    (GError      **error);

gboolean  wikipad_dbus_client_launch_files (gchar       **filenames,
                                             const gchar  *working_directory,
                                             GError      **error);

G_END_DECLS

#endif /* !__WIKIPAD_DBUS_H__ */

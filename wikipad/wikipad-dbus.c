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

#include <wikipad/wikipad-private.h>
#include <wikipad/wikipad-dbus.h>
#include <wikipad/wikipad-application.h>

#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif



#define WIKIPAD_DBUS_PATH      "/org/phm/Wikipad"
#define WIKIPAD_DBUS_INTERFACE "org.phm.Wikipad"



static void      wikipad_dbus_service_finalize      (GObject              *object);
static gboolean  wikipad_dbus_service_launch_files  (WikipadDBusService  *dbus_service,
                                                      const gchar          *working_directory,
                                                      gchar               **filenames,
                                                      GError              **error);
static gboolean  wikipad_dbus_service_terminate     (WikipadDBusService  *dbus_service,
                                                      GError              **error);



/* include the dbus glue generated by dbus-binding-tool */
#include <wikipad/wikipad-dbus-infos.h>



struct _WikipadDBusServiceClass
{
  GObjectClass __parent__;
};

struct _WikipadDBusService
{
  GObject __parent__;

  DBusGConnection *connection;
};



G_DEFINE_TYPE (WikipadDBusService, wikipad_dbus_service, G_TYPE_OBJECT)



static void
wikipad_dbus_service_class_init (WikipadDBusServiceClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = wikipad_dbus_service_finalize;

  /* install the D-BUS info for our class */
  dbus_g_object_type_install_info (G_TYPE_FROM_CLASS (klass), &dbus_glib_wikipad_dbus_service_object_info);
}



static void
wikipad_dbus_service_init (WikipadDBusService *dbus_service)
{
  GError *error = NULL;

  /* try to connect to the session bus */
  dbus_service->connection = dbus_g_bus_get (DBUS_BUS_SESSION, &error);

  if (G_LIKELY (dbus_service->connection != NULL))
    {
      /* register the /org/xfce/TextEditor object for Mousepad */
      dbus_g_connection_register_g_object (dbus_service->connection, WIKIPAD_DBUS_PATH, G_OBJECT (dbus_service));

      /* request the org.xfce.Mousepad name for Mousepad */
      dbus_bus_request_name (dbus_g_connection_get_connection (dbus_service->connection),
                             WIKIPAD_DBUS_INTERFACE, DBUS_NAME_FLAG_REPLACE_EXISTING, NULL);
    }
  else
    {
#ifdef NDEBUG
      /* hide this warning when the user is root and debug is disabled */
      if (geteuid () != 0)
#endif
        {
          /* notify the user that D-BUS service won't be available */
          g_message ("Failed to connect to the D-BUS session bus: %s\n", error->message);
        }

      g_error_free (error);
    }
}



static void
wikipad_dbus_service_finalize (GObject *object)
{
  WikipadDBusService *dbus_service = WIKIPAD_DBUS_SERVICE (object);

  /* release the D-BUS connection object */
  if (G_LIKELY (dbus_service->connection != NULL))
    dbus_g_connection_unref (dbus_service->connection);

  (*G_OBJECT_CLASS (wikipad_dbus_service_parent_class)->finalize) (object);
}



/**
 * wikipad_dbus_service_launch_files:
 * @dbus_service      : A #WikipadDBusService.
 * @working_directory : The default working directory for this window.
 * @filenames         : A list of filenames we try to open in tabs. The file names
 *                      can either be absolute paths, supported URIs or relative file
 *                      names to @working_directory or %NULL for an untitled document.
 * @error             : Return location for errors, not used atm.
 *
 * This function is activated by DBus (service) and opens a new window in this instance of
 * Mousepad.
 *
 * Return value: %TRUE on success, %FALSE if @error is set.
 **/
static gboolean
wikipad_dbus_service_launch_files (WikipadDBusService  *dbus_service,
                                    const gchar          *working_directory,
                                    gchar               **filenames,
                                    GError              **error)
{
  WikipadApplication *application;

  g_return_val_if_fail (g_path_is_absolute (working_directory), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  /* open a wikipad window */
  application = wikipad_application_get ();
  wikipad_application_new_window_with_files (application, NULL, working_directory, filenames);
  g_object_unref (G_OBJECT (application));

  return TRUE;
}



/**
 * wikipad_dbus_service_terminate:
 * @dbus_service : A #WikipadDBusService.
 * @error        : Return location for errors, not used atm.
 *
 * This function quits this instance of Mousepad.
 *
 * Return value: %TRUE on success.
 **/
static gboolean
wikipad_dbus_service_terminate (WikipadDBusService  *dbus_service,
                                 GError              **error)
{
  /* leave the Gtk main loop as soon as possible */
  gtk_main_quit ();

  /* we cannot fail */
  return TRUE;
}



/**
 * wikipad_dbus_client_send:
 * @message : A #DBusMessage.
 * @error   : Return location for errors or %NULL.
 *
 * This function sends the DBus message and should avoid
 * code duplication in the functions below.
 *
 * Return value: %TRUE on succeed or %FALSE if @error is set.
 **/
static gboolean
wikipad_dbus_client_send (DBusMessage  *message,
                           GError      **error)
{
  DBusConnection *connection;
  DBusMessage    *result;
  DBusError       derror;

  dbus_error_init (&derror);

  /* try to connect to the session bus */
  connection = dbus_bus_get (DBUS_BUS_SESSION, &derror);
  if (G_UNLIKELY (connection == NULL))
    {
      dbus_set_g_error (error, &derror);
      dbus_error_free (&derror);
      return FALSE;
    }

  /* send the message */
  result = dbus_connection_send_with_reply_and_block (connection, message, -1, &derror);

  /* check if no reply was received */
  if (result == NULL)
    {
      /* check if there was just no instance running */
      if (!dbus_error_has_name (&derror, DBUS_ERROR_NAME_HAS_NO_OWNER))
        dbus_set_g_error (error, &derror);

      dbus_error_free (&derror);
      return FALSE;
    }

  /* but maybe we received an error */
  if (G_UNLIKELY (dbus_message_get_type (result) == DBUS_MESSAGE_TYPE_ERROR))
    {
      dbus_set_error_from_message (&derror, result);
      dbus_set_g_error (error, &derror);
      dbus_message_unref (result);
      dbus_error_free (&derror);
      return FALSE;
    }

  /* it seems everything worked */
  dbus_message_unref (result);

  return TRUE;
}



/**
 * wikipad_dbus_client_terminate:
 * @error : Return location for errors or %NULL.
 *
 * Function called from this instance of the application and tries to invoke
 * with an already running instance and ties to quit it.
 * The wikipad_dbus_service_terminate function is activated in the running instance.
 *
 * Return value: %TRUE on success.
 **/
gboolean
wikipad_dbus_client_terminate (GError **error)
{
  DBusMessage *message;

  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  /* generate the message */
  message = dbus_message_new_method_call (WIKIPAD_DBUS_INTERFACE, WIKIPAD_DBUS_PATH,
                                          WIKIPAD_DBUS_INTERFACE, "Terminate");
  dbus_message_set_auto_start (message, FALSE);


  /* send the message */
  wikipad_dbus_client_send (message, error);

  /* unref the message */
  dbus_message_unref (message);

  /* we return false if an error was set */
  return (error != NULL);
}



/**
 * wikipad_dbus_client_launch_files:
 * @filenames         : A list of filenames we try to open in tabs. The file names
 *                      can either be absolute paths, supported URIs or relative file
 *                      names to @working_directory or %NULL for an untitled document.
 * @working_directory : Working directory for the new Mousepad window.
 * @error             : Return location for errors or %NULL.
 *
 * This function is called within this instance and tries to connect a running instance
 * of Mousepad via DBus. The function wikipad_dbus_service_launch_files is activated in the
 * running instance.
 *
 * Return value: %TRUE on success.
 **/
gboolean
wikipad_dbus_client_launch_files (gchar       **filenames,
                                   const gchar  *working_directory,
                                   GError      **error)
{
  DBusMessage *message;
  guint        length = 0;
  gboolean     succeed;
  GPtrArray   *utf8_filenames;
  gchar       *utf8_dir = NULL;

  g_return_val_if_fail (g_path_is_absolute (working_directory), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  utf8_filenames = g_ptr_array_new_with_free_func(g_free);
  if (filenames != NULL)
    {
      guint i;
      /* get the length of the filesname string */
      length = g_strv_length (filenames);
      g_ptr_array_set_size(utf8_filenames, length + 1);
      /* encode locale filenames to UTF-8 for DBus */
      for (i = 0; i < length; i++)
        {
          gchar *utf8_fn = g_filename_to_utf8(filenames[i], -1, NULL, NULL, error);
          if (utf8_fn == NULL)
            {
              g_ptr_array_free(utf8_filenames, TRUE);
              return FALSE;
            }
          utf8_filenames->pdata[i] = utf8_fn;
        }
    }
  g_ptr_array_add(utf8_filenames, NULL);

  if (working_directory != NULL)
    {
      /* encode working directory to UTF-8 for DBus */
      utf8_dir = g_filename_to_utf8(working_directory, -1, NULL, NULL, error);
      if (utf8_dir == NULL)
        {
          g_ptr_array_free(utf8_filenames, TRUE);
          return FALSE;
        }
    }

  /* generate the message */
  message = dbus_message_new_method_call (WIKIPAD_DBUS_INTERFACE, WIKIPAD_DBUS_PATH,
                                          WIKIPAD_DBUS_INTERFACE, "LaunchFiles");
  dbus_message_set_auto_start (message, FALSE);
  dbus_message_append_args (message,
                            DBUS_TYPE_STRING, &utf8_dir,
                            DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &utf8_filenames->pdata, length,
                            DBUS_TYPE_INVALID);

  /* send the message */
  succeed = wikipad_dbus_client_send (message, error);

  /* unref the message */
  dbus_message_unref (message);

  /* cleanup the UTF-8 strings */
  g_ptr_array_free(utf8_filenames, TRUE);
  g_free(utf8_dir);

  return succeed;
}

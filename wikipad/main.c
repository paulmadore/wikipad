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
#include <wikipad/wikipad-application.h>
#ifdef HAVE_DBUS
#include <wikipad/wikipad-dbus.h>
#endif

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif


/* globals */
static gchar    **filenames = NULL;
static gboolean   opt_version = FALSE;
#ifdef HAVE_DBUS
static gboolean   opt_disable_server = FALSE;
static gboolean   opt_quit = FALSE;
#endif



/* command line options */
static const GOptionEntry option_entries[] =
{
#ifdef HAVE_DBUS
  { "disable-server", '\0', 0, G_OPTION_ARG_NONE, &opt_disable_server, N_("Do not register with the D-BUS session message bus"), NULL },
  { "quit", 'q', 0, G_OPTION_ARG_NONE, &opt_quit, N_("Quit a running Mousepad instance"), NULL },
#endif
  { "version", 'v', 0, G_OPTION_ARG_NONE, &opt_version, N_("Print version information and exit"), NULL },
  { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &filenames, NULL, NULL },
  { NULL }
};



gint
main (gint argc, gchar **argv)
{
  WikipadApplication *application;
  GError              *error = NULL;
  gchar               *working_directory;
#ifdef HAVE_DBUS
  WikipadDBusService *dbus_service;
#endif

  /* bind the text domain to the locale directory */
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  /* set the package textdomain */
  textdomain (GETTEXT_PACKAGE);

  /* default application name */
  g_set_application_name (_("Wikipad"));

#ifdef G_ENABLE_DEBUG
  /* crash when something went wrong */
  g_log_set_always_fatal (G_LOG_LEVEL_CRITICAL | G_LOG_LEVEL_WARNING);
#endif

#if !GLIB_CHECK_VERSION (2, 30, 0)
  /* initialize the gthread system */
  if (G_LIKELY (!g_thread_supported ()))
    g_thread_init (NULL);
#endif

  /* initialize gtk+ */
  if (!gtk_init_with_args (&argc, &argv, _("[FILES...]"), (GOptionEntry *) option_entries, (gchar *) GETTEXT_PACKAGE, &error))
    {
      /* check if we have an error message */
      if (G_LIKELY (error == NULL))
        {
          /* no error message, the gui initialization failed */
          g_error ("%s", _("Failed to open display."));
        }
      else
        {
          /* print the error message */
          g_error ("%s", error->message);
          g_error_free (error);
        }

      return EXIT_FAILURE;
    }

  /* check if we should print version information */
  if (G_UNLIKELY (opt_version))
    {
      g_print ("%s %s\n\n", PACKAGE_NAME, PACKAGE_VERSION);
      g_print ("%s\n", "Copyright (c) 2016");
      g_print ("\t%s\n\n", _("phm. Some rights reserved."));
      g_print (_("Please report bugs to <%s>."), PACKAGE_BUGREPORT);
      g_print ("\n");

      return EXIT_SUCCESS;
    }

#ifdef HAVE_DBUS
  /* check if we need to terminate a running Mousepad instance */
  if (G_UNLIKELY (opt_quit))
    {
      /* try to terminate whatever is running */
      if (!wikipad_dbus_client_terminate (&error))
        {
          g_error ("Failed to terminate a running instance: %s\n", error->message);
          g_error_free (error);
          return EXIT_FAILURE;
        }

      return EXIT_SUCCESS;
    }
#endif /* !HAVE_DBUS */

  /* get the current working directory */
  working_directory = g_get_current_dir ();

#ifdef HAVE_DBUS
  if (G_LIKELY (!opt_disable_server))
    {
      /* check if we can reuse an existing instance */
      if (wikipad_dbus_client_launch_files (filenames, working_directory, &error))
        {
          /* stop any running startup notification */
          gdk_notify_startup_complete ();

          /* cleanup */
          g_free (working_directory);
          g_strfreev (filenames);

          /* print errors, if needed */
          if (G_UNLIKELY (error))
            {
              g_error ("Wikipad: %s\n", error->message);
              g_error_free (error);

              return EXIT_FAILURE;
            }

          return EXIT_SUCCESS;
        }
    }
#endif /* !HAVE_DBUS */

  /* use the Mousepad icon as default for new windows */
  gtk_window_set_default_icon_name ("accessories-text-editor");

  /* create a new wikipad application */
  application = wikipad_application_get ();

  /* open an empty window (with an empty document or the files) */
  wikipad_application_new_window_with_files (application, NULL, working_directory, filenames);

  /* cleanup */
  g_free (working_directory);
  g_strfreev (filenames);

  /* do not enter the main loop, unless we have atleast one window */
  if (G_LIKELY (wikipad_application_has_windows (application)))
    {
#ifdef HAVE_DBUS
      /* register with dbus */
      dbus_service = g_object_new (WIKIPAD_TYPE_DBUS_SERVICE, NULL);
#endif

      /* enter the main loop */
      gtk_main ();

#ifdef HAVE_DBUS
      /* release dbus service reference */
      g_object_unref (G_OBJECT (dbus_service));
#endif
    }

  /* release application reference */
  g_object_unref (G_OBJECT (application));

  return EXIT_SUCCESS;
}

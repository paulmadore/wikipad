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
#include <wikipad/wikipad-settings.h>
#include <wikipad/wikipad-application.h>
#include <wikipad/wikipad-document.h>
#include <wikipad/wikipad-prefs-dialog.h>
#include <wikipad/wikipad-replace-dialog.h>
#include <wikipad/wikipad-window.h>



static void        wikipad_application_finalize                  (GObject                   *object);
static void        wikipad_application_window_destroyed          (GtkWidget                 *window,
                                                                   MousepadApplication        *application);
static GtkWidget  *wikipad_application_create_window             (MousepadApplication        *application);
static void        wikipad_application_new_window_with_document  (WikipadWindow             *existing,
                                                                   WikipadDocument           *document,
                                                                   gint                        x,
                                                                   gint                        y,
                                                                   MousepadApplication        *application);
static void        wikipad_application_new_window                (WikipadWindow             *existing,
                                                                   MousepadApplication        *application);



struct _MousepadApplicationClass
{
  GObjectClass __parent__;
};

struct _MousepadApplication
{
  GObject    __parent__;

  /* internal list of all the opened windows */
  GSList    *windows;

  /* the preferences dialog when shown */
  GtkWidget *prefs_dialog;
};



G_DEFINE_TYPE (MousepadApplication, wikipad_application, G_TYPE_OBJECT)



static void
wikipad_application_class_init (MousepadApplicationClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = wikipad_application_finalize;
}



static void
wikipad_application_init (MousepadApplication *application)
{
  gchar *filename;

  wikipad_settings_init ();
  application->prefs_dialog = NULL;

  /* check if we have a saved accel map */
  filename = wikipad_util_get_save_location (WIKIPAD_ACCELS_RELPATH, FALSE);
  if (G_LIKELY (filename != NULL))
    {
      /* load the accel map */
      gtk_accel_map_load (filename);

      /* cleanup */
      g_free (filename);
    }
}



static void
wikipad_application_finalize (GObject *object)
{
  MousepadApplication *application = WIKIPAD_APPLICATION (object);
  GSList              *li;
  gchar               *filename;

  if (GTK_IS_WIDGET (application->prefs_dialog))
    gtk_widget_destroy (application->prefs_dialog);

  /* flush the history items of the replace dialog
   * this is a bit of an ugly place, but cleaning on a window close
   * isn't a good option eighter */
  wikipad_replace_dialog_history_clean ();

  /* save the current accel map */
  filename = wikipad_util_get_save_location (WIKIPAD_ACCELS_RELPATH, TRUE);
  if (G_LIKELY (filename != NULL))
    {
      /* save the accel map */
      gtk_accel_map_save (filename);

      /* cleanup */
      g_free (filename);
    }

  /* destroy the windows if they are still opened */
  for (li = application->windows; li != NULL; li = li->next)
    {
      wikipad_disconnect_by_func (G_OBJECT (li->data), wikipad_application_window_destroyed, application);
      gtk_widget_destroy (GTK_WIDGET (li->data));
    }

  /* cleanup the list of windows */
  g_slist_free (application->windows);

  wikipad_settings_finalize ();

  (*G_OBJECT_CLASS (wikipad_application_parent_class)->finalize) (object);
}



MousepadApplication*
wikipad_application_get (void)
{
  static MousepadApplication *application = NULL;

  if (G_UNLIKELY (application == NULL))
    {
      application = g_object_new (WIKIPAD_TYPE_APPLICATION, NULL);
      g_object_add_weak_pointer (G_OBJECT (application), (gpointer) &application);
    }
  else
    {
      g_object_ref (G_OBJECT (application));
    }

  return application;
}



gboolean
wikipad_application_has_windows (MousepadApplication *application)
{
  g_return_val_if_fail (WIKIPAD_IS_APPLICATION (application), FALSE);

  return (application->windows != NULL);
}



static void
wikipad_application_window_destroyed (GtkWidget           *window,
                                       MousepadApplication *application)
{
  g_return_if_fail (GTK_IS_WINDOW (window));
  g_return_if_fail (WIKIPAD_IS_APPLICATION (application));
  g_return_if_fail (g_slist_find (application->windows, window) != NULL);

  /* remove the window from the list */
  application->windows = g_slist_remove (application->windows, window);

  /* quit if there are no windows opened */
  if (application->windows == NULL)
    gtk_main_quit ();
}



void
wikipad_application_take_window (MousepadApplication *application,
                                  GtkWindow           *window)
{
  g_return_if_fail (WIKIPAD_IS_WINDOW (window));
  g_return_if_fail (WIKIPAD_IS_APPLICATION (application));
  g_return_if_fail (g_slist_find (application->windows, window) == NULL);

  /* connect to the "destroy" signal */
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (wikipad_application_window_destroyed), application);

  /* add the window to our internal list */
  application->windows = g_slist_prepend (application->windows, window);
}



static GtkWidget *
wikipad_application_create_window (MousepadApplication *application)
{
  GtkWidget *window;

  /* create a new window */
  window = wikipad_window_new ();

  /* hook up the new window */
  wikipad_application_take_window (application, GTK_WINDOW (window));

  /* connect signals */
  g_signal_connect (G_OBJECT (window), "new-window-with-document", G_CALLBACK (wikipad_application_new_window_with_document), application);
  g_signal_connect (G_OBJECT (window), "new-window", G_CALLBACK (wikipad_application_new_window), application);

  return window;
}



static void
wikipad_application_new_window_with_document (WikipadWindow      *existing,
                                               WikipadDocument    *document,
                                               gint                 x,
                                               gint                 y,
                                               MousepadApplication *application)
{
  GtkWidget *window;
  GdkScreen *screen;

  g_return_if_fail (WIKIPAD_IS_WINDOW (existing));
  g_return_if_fail (document == NULL || WIKIPAD_IS_DOCUMENT (document));
  g_return_if_fail (WIKIPAD_IS_APPLICATION (application));

  /* create a new window (signals added and already hooked up) */
  window = wikipad_application_create_window (application);

  /* place the new window on the same screen as the existing window */
  screen = gtk_window_get_screen (GTK_WINDOW (existing));
  if (G_LIKELY (screen != NULL))
    gtk_window_set_screen (GTK_WINDOW (window), screen);

  /* move the window on valid cooridinates */
  if (x > -1 && y > -1)
    gtk_window_move (GTK_WINDOW (window), x, y);

  /* create an empty document if no document was send */
  if (document == NULL)
    document = wikipad_document_new ();

  /* add the document to the new window */
  wikipad_window_add (WIKIPAD_WINDOW (window), document);

  /* show the window */
  gtk_widget_show (window);
}



static void
wikipad_application_new_window (WikipadWindow      *existing,
                                 MousepadApplication *application)
{
  /* trigger new document function */
  wikipad_application_new_window_with_document (existing, NULL, -1, -1, application);
}



void
wikipad_application_new_window_with_files (MousepadApplication  *application,
                                            GdkScreen            *screen,
                                            const gchar          *working_directory,
                                            gchar               **filenames)
{
  GtkWidget        *window;
  gboolean          succeed = FALSE;
  WikipadDocument *document;

  g_return_if_fail (WIKIPAD_IS_APPLICATION (application));
  g_return_if_fail (screen == NULL || GDK_IS_SCREEN (screen));

  /* create a new window (signals added and already hooked up) */
  window = wikipad_application_create_window (application);

  /* place the window on the right screen */
  gtk_window_set_screen (GTK_WINDOW (window), screen ? screen : gdk_screen_get_default ());

  /* try to open the files */
  if (working_directory && filenames && g_strv_length (filenames))
    succeed = wikipad_window_open_files (WIKIPAD_WINDOW (window), working_directory, filenames);

  /* open an empty document */
  if (succeed == FALSE)
    {
      /* create a new document */
      document = wikipad_document_new ();

      /* add the document to the new window */
      wikipad_window_add (WIKIPAD_WINDOW (window), document);
    }

  /* show the window */
  gtk_widget_show (window);
}



static void
wikipad_application_prefs_dialog_response (MousepadApplication *application,
                                            gint                 response_id,
                                            WikipadPrefsDialog *dialog)
{
  gtk_widget_destroy (GTK_WIDGET (application->prefs_dialog));
  application->prefs_dialog = NULL;
}



void
wikipad_application_show_preferences (MousepadApplication  *application,
                                       GtkWindow            *transient_for)
{
  /* if the dialog isn't already shown, create one */
  if (! GTK_IS_WIDGET (application->prefs_dialog))
    {
      application->prefs_dialog = wikipad_prefs_dialog_new ();

      /* destroy the dialog when it's close button is pressed */
      g_signal_connect_swapped (application->prefs_dialog,
                                "response",
                                G_CALLBACK (wikipad_application_prefs_dialog_response),
                                application);
    }

  /* if no transient window was specified, used the first application window
   * or NULL if no windows exists (shouldn't happen) */
  if (! GTK_IS_WINDOW (transient_for))
    {
      if (application->windows && GTK_IS_WINDOW (application->windows->data))
        transient_for = GTK_WINDOW (application->windows->data);
      else
        transient_for = NULL;
    }

  /* associate it with one of the windows */
  gtk_window_set_transient_for (GTK_WINDOW (application->prefs_dialog), transient_for);

  /* show it to the user */
  gtk_window_present (GTK_WINDOW (application->prefs_dialog));
}

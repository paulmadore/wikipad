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
#include <wikipad/wikipad-gtkcompat.h>
#include <wikipad/wikipad-file.h>

#include <glib/gstdio.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <errno.h>


enum
{
  /* EXTERNALLY_MODIFIED, */
  FILENAME_CHANGED,
  READONLY_CHANGED,
  LAST_SIGNAL
};

struct _WikipadFileClass
{
  GObjectClass __parent__;
};

struct _WikipadFile
{
  GObject             __parent__;

  /* the text buffer this file belongs to */
  GtkTextBuffer      *buffer;

  /* filename */
  gchar              *filename;

  /* encoding of the file */
  WikipadEncoding    encoding;

  /* line ending of the file */
  WikipadLineEnding  line_ending;

  /* our last modification time */
  gint                mtime;

  /* if file is read-only */
  guint               readonly : 1;

  /* whether we write the bom at the start of the file */
  guint               write_bom : 1;

  /* whether the filetype has been set by user or we should guess it */
  gboolean            user_set_language;
};



static void  wikipad_file_finalize         (GObject            *object);
static void  wikipad_file_set_readonly     (WikipadFile       *file,
                                             gboolean            readonly);



static guint file_signals[LAST_SIGNAL];



G_DEFINE_TYPE (WikipadFile, wikipad_file, G_TYPE_OBJECT)



static void
wikipad_file_class_init (WikipadFileClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = wikipad_file_finalize;

#if 0
  /* TODO implement this signal */
  file_signals[EXTERNALLY_MODIFIED] =
    g_signal_new (I_("externally-modified"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  0, NULL, NULL,
                  g_cclosure_marshal_VOID__BOOLEAN,
                  G_TYPE_NONE, 1, G_TYPE_BOOLEAN);
#endif

  file_signals[READONLY_CHANGED] =
    g_signal_new (I_("readonly-changed"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  0, NULL, NULL,
                  g_cclosure_marshal_VOID__BOOLEAN,
                  G_TYPE_NONE, 1, G_TYPE_BOOLEAN);

  file_signals[FILENAME_CHANGED] =
    g_signal_new (I_("filename-changed"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  0, NULL, NULL,
                  g_cclosure_marshal_VOID__STRING,
                  G_TYPE_NONE, 1, G_TYPE_STRING);
}



static void
wikipad_file_init (WikipadFile *file)
{
  /* initialize */
  file->filename          = NULL;
  file->encoding          = WIKIPAD_ENCODING_UTF_8;
#ifdef G_OS_WIN32
  file->line_ending       = WIKIPAD_EOL_DOS;
#else
  file->line_ending       = WIKIPAD_EOL_UNIX;
#endif
  file->readonly          = TRUE;
  file->mtime             = 0;
  file->write_bom         = FALSE;
  file->user_set_language = FALSE;
}



static void
wikipad_file_finalize (GObject *object)
{
  WikipadFile *file = WIKIPAD_FILE (object);

  /* cleanup */
  g_free (file->filename);

  /* release the reference from the buffer */
  g_object_unref (G_OBJECT (file->buffer));

  (*G_OBJECT_CLASS (wikipad_file_parent_class)->finalize) (object);
}



static void
wikipad_file_set_readonly (WikipadFile *file,
                            gboolean      readonly)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));

  if (G_LIKELY (file->readonly != readonly))
    {
      /* store new value */
      file->readonly = readonly;

      /* emit signal */
      g_signal_emit (G_OBJECT (file), file_signals[READONLY_CHANGED], 0, readonly);
    }
}



static WikipadEncoding
wikipad_file_encoding_read_bom (const gchar *contents,
                                 gsize        length,
                                 gsize       *bom_length)
{
  const guchar     *bom = (const guchar *) contents;
  WikipadEncoding  encoding = WIKIPAD_ENCODING_NONE;
  gsize             bytes = 0;

  g_return_val_if_fail (contents != NULL && length > 0, WIKIPAD_ENCODING_NONE);
  g_return_val_if_fail (contents != NULL && length > 0, WIKIPAD_ENCODING_NONE);

  switch (bom[0])
    {
      case 0xef:
        if (length >= 3 && bom[1] == 0xbb && bom[2] == 0xbf)
          {
            bytes = 3;
            encoding = WIKIPAD_ENCODING_UTF_8;
          }
        break;

      case 0x00:
        if (length >= 4 && bom[1] == 0x00 && bom[2] == 0xfe && bom[3] == 0xff)
          {
            bytes = 4;
            encoding = WIKIPAD_ENCODING_UTF_32BE;
          }
        break;

      case 0xff:
        if (length >= 4 && bom[1] == 0xfe && bom[2] == 0x00 && bom[3] == 0x00)
          {
            bytes = 4;
            encoding = WIKIPAD_ENCODING_UTF_32LE;
          }
        else if (length >= 2 && bom[1] == 0xfe)
          {
            bytes = 2;
            encoding = WIKIPAD_ENCODING_UTF_16LE;
          }
        break;

      case 0x2b:
        if (length >= 4 && (bom[0] == 0x2b && bom[1] == 0x2f && bom[2] == 0x76) &&
            (bom[3] == 0x38 || bom[3] == 0x39 || bom[3] == 0x2b || bom[3] == 0x2f))
          {
            bytes = 4;
            encoding = WIKIPAD_ENCODING_UTF_7;
          }
        break;

      case 0xfe:
        if (length >= 2 && bom[1] == 0xff)
          {
            bytes = 2;
            encoding = WIKIPAD_ENCODING_UTF_16BE;
          }
        break;
    }

  if (bom_length)
    *bom_length = bytes;

  return encoding;
}



WikipadFile *
wikipad_file_new (GtkTextBuffer *buffer)
{
  WikipadFile *file;

  g_return_val_if_fail (GTK_IS_TEXT_BUFFER (buffer), NULL);

  file = g_object_new (WIKIPAD_TYPE_FILE, NULL);

  /* set the buffer */
  file->buffer = g_object_ref (G_OBJECT (buffer));

  return file;
}



void
wikipad_file_set_filename (WikipadFile *file,
                            const gchar  *filename)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));

  /* free the old filename */
  g_free (file->filename);

  /* set the filename */
  file->filename = g_strdup (filename);

  /* send a signal that the name has been changed */
  g_signal_emit (G_OBJECT (file), file_signals[FILENAME_CHANGED], 0, file->filename);
}



const gchar *
wikipad_file_get_filename (WikipadFile *file)
{
  g_return_val_if_fail (WIKIPAD_IS_FILE (file), NULL);

  return file->filename;
}



gchar *
wikipad_file_get_uri (WikipadFile *file)
{
  g_return_val_if_fail (WIKIPAD_IS_FILE (file), NULL);

  return g_filename_to_uri (file->filename, NULL, NULL);
}



void
wikipad_file_set_encoding (WikipadFile     *file,
                            WikipadEncoding  encoding)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));

  /* set new encoding */
  file->encoding = encoding;
}



WikipadEncoding
wikipad_file_get_encoding (WikipadFile *file)
{
  g_return_val_if_fail (WIKIPAD_IS_FILE (file), WIKIPAD_ENCODING_NONE);

  return file->encoding;
}



gboolean
wikipad_file_get_read_only (WikipadFile *file)
{
  g_return_val_if_fail (WIKIPAD_IS_FILE (file), FALSE);

  return file->filename ? file->readonly : FALSE;
}



void
wikipad_file_set_write_bom (WikipadFile *file,
                             gboolean      write_bom)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));
  g_return_if_fail (wikipad_encoding_is_unicode (file->encoding));

  /* set new value */
  file->write_bom = write_bom;
}



gboolean
wikipad_file_get_write_bom (WikipadFile *file,
                             gboolean     *sensitive)
{
  g_return_val_if_fail (WIKIPAD_IS_FILE (file), FALSE);

  /* return if we can write a bom */
  if (G_LIKELY (sensitive))
    *sensitive = wikipad_encoding_is_unicode (file->encoding);

  return file->write_bom;
}



void
wikipad_file_set_line_ending (WikipadFile       *file,
                               WikipadLineEnding  line_ending)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));

  file->line_ending = line_ending;
}



WikipadLineEnding
wikipad_file_get_line_ending (WikipadFile *file)
{
  return file->line_ending;
}



void
wikipad_file_set_language (WikipadFile      *file,
                            GtkSourceLanguage *language)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));
  g_return_if_fail (GTK_SOURCE_IS_BUFFER (file->buffer));

  gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (file->buffer), language);
}



GtkSourceLanguage *
wikipad_file_get_language (WikipadFile *file)
{
  g_return_val_if_fail (WIKIPAD_IS_FILE (file), NULL);
  g_return_val_if_fail (GTK_SOURCE_IS_BUFFER (file->buffer), NULL);

  return gtk_source_buffer_get_language (GTK_SOURCE_BUFFER (file->buffer));
}



void
wikipad_file_set_language_id (WikipadFile *file,
                               const gchar  *language_id)
{
  GtkSourceLanguage *lang;

  g_return_if_fail (WIKIPAD_IS_FILE (file));
  g_return_if_fail (GTK_SOURCE_IS_BUFFER (file->buffer));

  if (G_UNLIKELY (language_id == NULL))
    {
      gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (file->buffer), NULL);
      return;
    }

  lang = gtk_source_language_manager_get_language (gtk_source_language_manager_get_default (), language_id);
  wikipad_file_set_language (file, lang);
}



const gchar *
wikipad_file_get_language_id (WikipadFile *file)
{
  GtkSourceLanguage *lang;

  lang = wikipad_file_get_language (file);
  return (lang != NULL) ? gtk_source_language_get_id (lang) : NULL;
}



GtkSourceLanguage *
wikipad_file_guess_language (WikipadFile *file)
{
  gchar             *content_type;
  gboolean           result_uncertain;
  GtkSourceLanguage *language = NULL;

  content_type = g_content_type_guess (file->filename, NULL, 0, &result_uncertain);
  if (result_uncertain)
    {
      g_free (content_type);
      content_type = NULL;
    }

  if (G_LIKELY (content_type != NULL || file->filename != NULL))
    {
      language = gtk_source_language_manager_guess_language (gtk_source_language_manager_get_default (),
                                                             file->filename,
                                                             content_type);
    }

  g_free (content_type);

  return language;
}



gint
wikipad_file_open (WikipadFile  *file,
                    const gchar   *template_filename,
                    GError       **error)
{
  GMappedFile      *mapped_file;
  const gchar      *filename;
  gint              retval = ERROR_READING_FAILED;
  gsize             file_size, written;
  gsize             bom_length;
  const gchar      *contents;
  gchar            *encoded = NULL;
  const gchar      *charset;
  GtkTextIter       start_iter, end_iter;
  struct stat       statb;
  const gchar      *end, *n, *m;
  WikipadEncoding  bom_encoding;

  g_return_val_if_fail (WIKIPAD_IS_FILE (file), FALSE);
  g_return_val_if_fail (GTK_IS_TEXT_BUFFER (file->buffer), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (file->filename != NULL || template_filename != NULL, FALSE);

  /* get the filename */
  if (G_UNLIKELY (template_filename != NULL))
    filename = template_filename;
  else
    filename = file->filename;

  /* check if the file exists, if not, it's a filename from the command line */
  if (g_file_test (filename, G_FILE_TEST_EXISTS) == FALSE)
    {
      /* update readonly status */
      wikipad_file_set_readonly (file, FALSE);

      return 0;
    }

  /* try to open the file */
  mapped_file = g_mapped_file_new (filename, FALSE, error);

  if (G_LIKELY (mapped_file))
    {
      /* get the mapped file contents and size */
      contents = g_mapped_file_get_contents (mapped_file);
      file_size = g_mapped_file_get_length (mapped_file);

      if (G_LIKELY (contents != NULL && file_size > 0))
        {
          /* detect if there is a bom with the encoding type */
          bom_encoding = wikipad_file_encoding_read_bom (contents, file_size, &bom_length);
          if (G_UNLIKELY (bom_encoding != WIKIPAD_ENCODING_NONE))
            {
              /* we've found a valid bom at the start of the contents */
              file->write_bom = TRUE;

              /* advance the contents offset and decrease size */
              contents += bom_length;
              file_size -= bom_length;

              /* set the detected encoding */
              file->encoding = bom_encoding;
            }

          /* handle encoding and check for utf-8 valid text */
          if (G_LIKELY (file->encoding == WIKIPAD_ENCODING_UTF_8))
            {
              validate:

              /* leave when the contents is not utf-8 valid */
              if (g_utf8_validate (contents, file_size, &end) == FALSE)
                {
                  /* set return value */
                  retval = ERROR_NOT_UTF8_VALID;

                  /* set an error */
                  g_set_error (error, G_CONVERT_ERROR, G_CONVERT_ERROR_ILLEGAL_SEQUENCE,
                               _("Invalid byte sequence in conversion input"));

                  goto failed;
                }
            }
          else
            {
              /* get the encoding charset */
              charset = wikipad_encoding_get_charset (file->encoding);

              /* convert the contents */
              encoded = g_convert (contents, file_size, "UTF-8", charset, NULL, &written, error);

              /* check if the string is utf-8 valid */
              if (G_UNLIKELY (encoded == NULL))
                {
                  /* set return value */
                  retval = ERROR_CONVERTING_FAILED;

                  goto failed;
                }

              /* set new values */
              contents = encoded;
              file_size = written;

              /* validate the converted content */
              goto validate;
            }

          /* detect the line ending, based on the first eol we match */
          for (n = contents; n < end; n = g_utf8_next_char (n))
            {
              if (G_LIKELY (*n == '\n'))
                {
                  /* set unix line ending */
                  file->line_ending = WIKIPAD_EOL_UNIX;

                  break;
                }
              else if (*n == '\r')
                {
                  /* get next character */
                  n = g_utf8_next_char (n);

                  /* set dos or mac line ending */
                  file->line_ending = (*n == '\n') ? WIKIPAD_EOL_DOS : WIKIPAD_EOL_MAC;

                  break;
                }
            }

          /* get the iter at the beginning of the document */
          gtk_text_buffer_get_start_iter (file->buffer, &start_iter);

          /* insert the file contents in the buffer (for documents with cr line ending) */
          for (n = m = contents; n < end; n = g_utf8_next_char (n))
            {
              if (G_UNLIKELY (*n == '\r'))
                {
                  /* insert the text in the buffer */
                  if (G_LIKELY (n - m > 0))
                    gtk_text_buffer_insert (file->buffer, &start_iter, m, n - m);

                  /* advance the offset */
                  m = g_utf8_next_char (n);

                  /* insert a new line when the document is not cr+lf */
                  if (*m != '\n')
                    gtk_text_buffer_insert (file->buffer, &start_iter, "\n", 1);
                }
            }

          /* insert the remaining part, or everything for lf line ending */
          if (G_LIKELY (n - m > 0))
            gtk_text_buffer_insert (file->buffer, &start_iter, m, n - m);

          /* get the start iter */
          gtk_text_buffer_get_start_iter (file->buffer, &start_iter);

          /* set the cursor to the beginning of the document */
          gtk_text_buffer_place_cursor (file->buffer, &start_iter);
        }

      /* assume everything when file */
      retval = 0;

      /* store the file status */
      if (G_LIKELY (filename != template_filename))
        {
          if (G_LIKELY (g_stat (file->filename, &statb) == 0))
            {
              /* whether the file is readonly (ie. not writable by the user) */
              wikipad_file_set_readonly (file, !((statb.st_mode & S_IWUSR) != 0));

              /* store the file modification time */
              file->mtime = statb.st_mtime;
            }
          else
            {
              /* set return value */
              retval = ERROR_FILE_STATUS_FAILED;
            }
        }
      else
        {
          /* this is a new document with content from a template */
          file->mtime = 0;
          wikipad_file_set_readonly (file, FALSE);
        }

      failed:

      /* make sure the buffer is empty if we did not succeed */
      if (G_UNLIKELY (retval != 0))
        {
          gtk_text_buffer_get_bounds (file->buffer, &start_iter, &end_iter);
          gtk_text_buffer_delete (file->buffer, &start_iter, &end_iter);
        }

      /* cleanup */
      g_free (encoded);

      /* close the mapped file */
#if GLIB_CHECK_VERSION (2, 21, 0)
      g_mapped_file_unref (mapped_file);
#else
      g_mapped_file_free (mapped_file);
#endif

      /* guess and set the file's filetype/language */
      wikipad_file_set_language (file, wikipad_file_guess_language (file));

      /* this does not count as a modified buffer */
      gtk_text_buffer_set_modified (file->buffer, FALSE);
    }

  return retval;
}



gboolean
wikipad_file_save (WikipadFile  *file,
                    GError       **error)
{
  gint          fd;
  gboolean      succeed = FALSE;
  gchar        *contents, *p;
  gchar        *encoded;
  const gchar  *charset;
  GtkTextIter   start_iter, end_iter;
  gint          l, m, length;
  gsize         written;
  struct stat   statb;
  gchar       **chunks;

  g_return_val_if_fail (WIKIPAD_IS_FILE (file), FALSE);
  g_return_val_if_fail (GTK_IS_TEXT_BUFFER (file->buffer), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (file->filename != NULL, FALSE);

  /* open the file */
  fd = g_open (file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (G_LIKELY (fd != -1))
    {
      /* get the buffer bounds */
      gtk_text_buffer_get_bounds (file->buffer, &start_iter, &end_iter);

      /* get the buffer contents */
      contents = gtk_text_buffer_get_slice (file->buffer, &start_iter, &end_iter, TRUE);

      if (G_LIKELY (contents))
        {
          /* get the content length */
          length = strlen (contents);

          /* handle line endings */
          if (file->line_ending == WIKIPAD_EOL_MAC)
            {
              /* replace the unix with a mac line ending */
              for (p = contents; *p != '\0'; p++)
                if (G_UNLIKELY (*p == '\n'))
                  *p = '\r';
            }
          else if (file->line_ending == WIKIPAD_EOL_DOS)
            {
              /* split the contents into chunks */
              chunks = g_strsplit (contents, "\n", -1);

              /* cleanup */
              g_free (contents);

              /* join the chunks with dos line endings in between */
              contents = g_strjoinv ("\r\n", chunks);

              /* cleanup */
              g_strfreev (chunks);

              /* new contents length */
              length = strlen (contents);
            }

          /* add and utf-8 bom at the start of the contents if needed */
          if (file->write_bom && wikipad_encoding_is_unicode (file->encoding))
            {
              /* realloc the contents string */
              contents = g_realloc (contents, length + 4);

              /* move the existing contents 3 bytes */
              g_memmove (contents + 3, contents, length + 1);

              /* write an utf-8 bom at the start of the contents */
              contents[0] = (guchar) 0xef;
              contents[1] = (guchar) 0xbb;
              contents[2] = (guchar) 0xbf;

              /* increase the length */
              length += 3;
            }

          /* convert to the encoding if set */
          if (G_UNLIKELY (file->encoding != WIKIPAD_ENCODING_UTF_8))
            {
              /* get the charset */
              charset = wikipad_encoding_get_charset (file->encoding);
              if (G_UNLIKELY (charset == NULL))
                goto failed;

              /* convert the content to the user encoding */
              encoded = g_convert (contents, length, charset, "UTF-8", NULL, &written, error);

              /* return if nothing was encoded */
              if (G_UNLIKELY (encoded == NULL))
                goto failed;

              /* cleanup */
              g_free (contents);

              /* set the new contents */
              contents = encoded;
              length = written;
            }

          /* write the buffer to the file */
          for (m = 0; m < length;)
            {
              /* write */
              l = write (fd, contents + m, length - m);

              if (G_UNLIKELY (l < 0))
                {
                  /* just try again on EAGAIN/EINTR */
                  if (G_LIKELY (errno != EAGAIN && errno != EINTR))
                    {
                      /* set an error */
                      g_set_error (error, G_FILE_ERROR, g_file_error_from_errno (errno), "%s", g_strerror (errno));

                      /* bail out */
                      goto failed;
                    }
                }
              else
                {
                  /* advance the offset */
                  m += l;
                }
            }

          /* set the new modification time */
          if (G_LIKELY (fstat (fd, &statb) == 0))
            file->mtime = statb.st_mtime;

          /* everything has been saved */
          gtk_text_buffer_set_modified (file->buffer, FALSE);

          /* we saved succesfully */
          wikipad_file_set_readonly (file, FALSE);

          /* if the user hasn't set the filetype, try and re-guess it now
           * that we have a new filename to go by */
          if (! file->user_set_language)
            wikipad_file_set_language (file, wikipad_file_guess_language (file));

          /* everything went file */
          succeed = TRUE;

          failed:

          /* cleanup */
          g_free (contents);

          /* close the file */
          close (fd);
        }
    }
  else
    {
      /* set an error */
      g_set_error (error, G_FILE_ERROR, g_file_error_from_errno (errno), "%s", g_strerror (errno));
    }

  return succeed;
}



gboolean
wikipad_file_reload (WikipadFile  *file,
                      GError       **error)
{
  GtkTextIter start, end;
  gint        succeed;

  g_return_val_if_fail (WIKIPAD_IS_FILE (file), FALSE);
  g_return_val_if_fail (GTK_IS_TEXT_BUFFER (file->buffer), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (file->filename != NULL, FALSE);

  /* simple test if the file has not been removed */
  if (G_UNLIKELY (g_file_test (file->filename, G_FILE_TEST_EXISTS) == FALSE))
    {
      /* set an error */
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_NOENT,
                   _("The file \"%s\" you tried to reload does not exist anymore"), file->filename);

      return FALSE;
    }

  /* clear the buffer */
  gtk_text_buffer_get_bounds (file->buffer, &start, &end);
  gtk_text_buffer_delete (file->buffer, &start, &end);

  /* reload the file */
  succeed = wikipad_file_open (file, NULL, error);

  return (succeed == 0);
}



gboolean
wikipad_file_get_externally_modified (WikipadFile  *file,
                                       GError       **error)
{
  struct stat statb;
  GFileError  error_code;

  g_return_val_if_fail (WIKIPAD_IS_FILE (file), TRUE);
  g_return_val_if_fail (file->filename != NULL, TRUE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  /* check if our modification time differs from the current one */
  if (G_LIKELY (g_stat (file->filename, &statb) == 0))
    return (file->mtime > 0 && statb.st_mtime != file->mtime);

  /* get the error code */
  error_code = g_file_error_from_errno (errno);

  /* file does not exists, nothing wrong with that */
  if (G_LIKELY (error_code == G_FILE_ERROR_NOENT))
    return FALSE;

  /* set an error */
  if (error != NULL)
    g_set_error (error, G_FILE_ERROR, error_code, _("Failed to read the status of \"%s\""), file->filename);

  return TRUE;
}



void
wikipad_file_set_user_set_language (WikipadFile *file,
                                     gboolean      set_by_user)
{
  g_return_if_fail (WIKIPAD_IS_FILE (file));

  file->user_set_language = set_by_user;
}

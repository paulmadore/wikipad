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

#ifndef __WIKIPAD_DOCUMENT_H__
#define __WIKIPAD_DOCUMENT_H__

#include <wikipad/wikipad-util.h>
#include <wikipad/wikipad-file.h>
#include <wikipad/wikipad-view.h>

G_BEGIN_DECLS

typedef struct _WikipadDocumentPrivate WikipadDocumentPrivate;
typedef struct _WikipadDocumentClass   WikipadDocumentClass;
typedef struct _WikipadDocument        WikipadDocument;

#define WIKIPAD_SCROLL_MARGIN 0.02

#define WIKIPAD_TYPE_DOCUMENT            (wikipad_document_get_type ())
#define WIKIPAD_DOCUMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_DOCUMENT, WikipadDocument))
#define WIKIPAD_DOCUMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_DOCUMENT, WikipadDocumentClass))
#define WIKIPAD_IS_DOCUMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_DOCUMENT))
#define WIKIPAD_IS_DOCUMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_DOCUMENT))
#define WIKIPAD_DOCUMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_DOCUMENT, WikipadDocumentClass))

struct _WikipadDocument
{
  GtkScrolledWindow        __parent__;

  /* private structure */
  WikipadDocumentPrivate *priv;

  /* file */
  WikipadFile            *file;

  /* text buffer */
  GtkTextBuffer           *buffer;

  /* text view */
  MousepadView            *textview;

  /* the highlight tag */
  GtkTextTag              *tag;
};

GType             wikipad_document_get_type       (void) G_GNUC_CONST;

WikipadDocument *wikipad_document_new            (void);

void              wikipad_document_set_overwrite  (WikipadDocument *document,
                                                    gboolean          overwrite);

void              wikipad_document_focus_textview (WikipadDocument *document);

void              wikipad_document_send_signals   (WikipadDocument *document);

GtkWidget        *wikipad_document_get_tab_label  (WikipadDocument *document);

const gchar      *wikipad_document_get_basename   (WikipadDocument *document);

const gchar      *wikipad_document_get_filename   (WikipadDocument *document);

gboolean          wikipad_document_get_word_wrap  (WikipadDocument *document);

G_END_DECLS

#endif /* !__WIKIPAD_DOCUMENT_H__ */

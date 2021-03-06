/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * gtksourcestyleschemechooserwidget.h
 * This file is part of gtksourceview
 *
 * Copyright (C) 2014 - Christian Hergert
 *               2014 - Ignacio Casal Quinteiro
 *
 * gtksourceview is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * gtksourceview is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with gtksourceview. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON_H__
#define __GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON            (gtk_source_style_scheme_chooser_button_get_type())
#define GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON, GtkSourceStyleSchemeChooserButton))
#define GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON, GtkSourceStyleSchemeChooserButton const))
#define GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON, GtkSourceStyleSchemeChooserButtonClass))
#define GTK_SOURCE_IS_STYLE_SCHEME_CHOOSER_BUTTON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON))
#define GTK_SOURCE_IS_STYLE_SCHEME_CHOOSER_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON))
#define GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER_BUTTON, GtkSourceStyleSchemeChooserButtonClass))

typedef struct _GtkSourceStyleSchemeChooserButton        GtkSourceStyleSchemeChooserButton;
typedef struct _GtkSourceStyleSchemeChooserButtonClass   GtkSourceStyleSchemeChooserButtonClass;

struct _GtkSourceStyleSchemeChooserButton
{
	GtkButton parent;
};

struct _GtkSourceStyleSchemeChooserButtonClass
{
	GtkButtonClass parent;
};

GType                 gtk_source_style_scheme_chooser_button_get_type              (void);

GtkWidget            *gtk_source_style_scheme_chooser_button_new                   (void);

G_END_DECLS

#endif /* __GTK_SOURCE_STYLE_SCHEME_CHOOSER_BUTTON_H__ */

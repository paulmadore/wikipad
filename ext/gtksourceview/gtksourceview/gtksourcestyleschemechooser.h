/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * gtk_sourcesourcestyleschemechooser.h
 * This file is part of gtk_sourcesourceview
 *
 * Copyright (C) 2014 - Ignacio Casal Quinteiro
 *
 * gtk_sourcesourceview is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * gtk_sourcesourceview is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with gtk_sourcesourceview. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __GTK_SOURCE_STYLE_SCHEME_CHOOSER_H__
#define __GTK_SOURCE_STYLE_SCHEME_CHOOSER_H__

#include <gtksourceview/gtksourcetypes.h>

G_BEGIN_DECLS

#define GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER                  (gtk_source_style_scheme_chooser_get_type ())
#define GTK_SOURCE_STYLE_SCHEME_CHOOSER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER, GtkSourceStyleSchemeChooser))
#define GTK_SOURCE_IS_STYLE_SCHEME_CHOOSER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER))
#define GTK_SOURCE_STYLE_SCHEME_CHOOSER_GET_IFACE(inst)       (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GTK_SOURCE_TYPE_STYLE_SCHEME_CHOOSER, GtkSourceStyleSchemeChooserInterface))

typedef struct _GtkSourceStyleSchemeChooser          GtkSourceStyleSchemeChooser;
typedef struct _GtkSourceStyleSchemeChooserInterface GtkSourceStyleSchemeChooserInterface;

struct _GtkSourceStyleSchemeChooserInterface
{
	GTypeInterface base_interface;

	/* Methods */
	GtkSourceStyleScheme * (* get_style_scheme)       (GtkSourceStyleSchemeChooser *chooser);

	void                   (* set_style_scheme)       (GtkSourceStyleSchemeChooser *chooser,
	                                                   GtkSourceStyleScheme        *scheme);

	/* Padding */
	gpointer padding[12];
};

GType                     gtk_source_style_scheme_chooser_get_type               (void) G_GNUC_CONST;

GtkSourceStyleScheme     *gtk_source_style_scheme_chooser_get_style_scheme       (GtkSourceStyleSchemeChooser *chooser);

void                      gtk_source_style_scheme_chooser_set_style_scheme       (GtkSourceStyleSchemeChooser *chooser,
                                                                                  GtkSourceStyleScheme        *scheme);

G_END_DECLS

#endif /* __GTK_SOURCE_STYLE_SCHEME_CHOOSER_H__ */

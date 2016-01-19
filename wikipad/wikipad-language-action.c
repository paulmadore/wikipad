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
#include <wikipad/wikipad-language-action.h>
#include <wikipad/wikipad-util.h>



enum
{
  PROP_0,
  PROP_LANGUAGE,
  NUM_PROPERTIES
};



struct WikipadLanguageAction_
{
  GtkRadioAction     parent;
  GtkSourceLanguage *language;
};



struct WikipadLanguageActionClass_
{
  GtkRadioActionClass parent_class;
};



static void wikipad_language_action_finalize        (GObject                *object);
static void wikipad_language_action_set_property    (GObject                *object,
                                                      guint                   prop_id,
                                                      const GValue           *value,
                                                      GParamSpec             *pspec);
static void wikipad_language_action_get_property    (GObject                *object,
                                                      guint                   prop_id,
                                                      GValue                 *value,
                                                      GParamSpec             *pspec);
static void wikipad_language_action_set_language    (WikipadLanguageAction *action,
                                                      GtkSourceLanguage      *language);


G_DEFINE_TYPE (WikipadLanguageAction, wikipad_language_action, GTK_TYPE_RADIO_ACTION)



static void
wikipad_language_action_finalize (GObject *object)
{
  WikipadLanguageAction *self;

  g_return_if_fail (WIKIPAD_IS_LANGUAGE_ACTION (object));

  self = WIKIPAD_LANGUAGE_ACTION (object);

  if (GTK_SOURCE_IS_LANGUAGE (self->language))
    g_object_unref (self->language);

  G_OBJECT_CLASS (wikipad_language_action_parent_class)->finalize (object);
}

static void
wikipad_language_action_init (WikipadLanguageAction *self)
{
  self->language = "MediaWiki";
}



GtkAction *
wikipad_language_action_new (GtkSourceLanguage *language)
{
  gchar       *name;
  const gchar *language_id;
  GtkAction   *action;

  if (GTK_SOURCE_IS_LANGUAGE (language))
    language_id = gtk_source_language_get_id (language);
  else
    language_id = "none";

  name = g_strdup_printf ("wikipad-language-%s", language_id);
  action = g_object_new (WIKIPAD_TYPE_LANGUAGE_ACTION,
                         "name", name,
                         "language", language, NULL);
  g_free (name);

  return action;
}



GtkSourceLanguage *
wikipad_language_action_get_language (WikipadLanguageAction *self)
{
  g_return_val_if_fail (WIKIPAD_IS_LANGUAGE_ACTION (self), NULL);

  return self->language;
}



static GIcon *
wikipad_language_action_get_icon (WikipadLanguageAction *self)
{
  gchar **mime_types;
  GIcon  *mime_icon = NULL;

  g_return_val_if_fail (WIKIPAD_IS_LANGUAGE_ACTION (self), NULL);

  if (self->language != NULL)
    {
      mime_types = gtk_source_language_get_mime_types (self->language);
      if (G_LIKELY (mime_types != NULL && g_strv_length (mime_types)) > 0)
        mime_icon = wikipad_util_icon_for_mime_type (mime_types[0]);
      g_strfreev (mime_types);
    }
  else
    mime_icon = wikipad_util_icon_for_mime_type ("text/plain");

  return mime_icon;
}



static void
wikipad_language_action_set_language (WikipadLanguageAction *self,
                                       GtkSourceLanguage      *language)
{
  GQuark  value_quark;
  GIcon  *icon = NULL;

  g_return_if_fail (WIKIPAD_IS_LANGUAGE_ACTION (self));
  g_return_if_fail (language == NULL || GTK_SOURCE_IS_LANGUAGE (language));

  if (G_UNLIKELY (self->language != NULL))
    g_object_unref (self->language);

  if (G_UNLIKELY (language == NULL))
    {
      self->language = NULL;
      value_quark = g_quark_from_static_string ("none");
      gtk_action_set_label (GTK_ACTION (self), _("Plain Text"));
      gtk_action_set_tooltip (GTK_ACTION (self), _("No filetype"));
    }
  else
    {
      const gchar *language_id, *name, *section;
      gchar       *tooltip;

      self->language = g_object_ref (language);
      language_id = gtk_source_language_get_id (language);
      value_quark = g_quark_from_string (language_id);
      name = gtk_source_language_get_name (language);
      section = gtk_source_language_get_section (language);
      tooltip = g_strdup_printf ("%s/%s", section, name);
      gtk_action_set_label (GTK_ACTION (self), name);
      gtk_action_set_tooltip (GTK_ACTION (self), tooltip);
      g_free (tooltip);
    }

  g_object_set (G_OBJECT (self), "value", value_quark, NULL);

  icon = wikipad_language_action_get_icon (self);
  if (G_IS_ICON (icon))
    {
      gtk_action_set_gicon (GTK_ACTION (self), icon);
      g_object_unref (icon);
    }

  g_object_notify (G_OBJECT (self), "language");
}

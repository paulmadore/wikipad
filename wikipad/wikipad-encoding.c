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
#include <wikipad/wikipad-encoding.h>



const WikipadEncodingInfo encoding_infos[] =
{
  /* west european */
  { WIKIPAD_ENCODING_ISO_8859_14,  "ISO-8859-14",      N_("Celtic") },
  { WIKIPAD_ENCODING_ISO_8859_7,   "ISO-8859-7",       N_("Greek") },
  { WIKIPAD_ENCODING_WINDOWS_1253, "WINDOWS-1253",     N_("Greek") },
  { WIKIPAD_ENCODING_ISO_8859_10,  "ISO-8859-10",      N_("Nordic") },
  { WIKIPAD_ENCODING_ISO_8859_3,   "ISO-8859-3",       N_("South European") },
  { WIKIPAD_ENCODING_IBM_850,      "IBM850",           N_("Western") },
  { WIKIPAD_ENCODING_ISO_8859_1,   "ISO-8859-1",       N_("Western") },
  { WIKIPAD_ENCODING_ISO_8859_15,  "ISO-8859-15",      N_("Western") },
  { WIKIPAD_ENCODING_WINDOWS_1252, "WINDOWS-1252",     N_("Western") },

  /* east european */
  { WIKIPAD_ENCODING_ISO_8859_4,   "ISO-8859-4",       N_("Baltic") },
  { WIKIPAD_ENCODING_ISO_8859_13,  "ISO-8859-13",      N_("Baltic") },
  { WIKIPAD_ENCODING_WINDOWS_1257, "WINDOWS-1257",     N_("Baltic") },
  { WIKIPAD_ENCODING_IBM_852,      "IBM852",           N_("Central European") },
  { WIKIPAD_ENCODING_ISO_8859_2,   "ISO-8859-2",       N_("Central European") },
  { WIKIPAD_ENCODING_WINDOWS_1250, "WINDOWS-1250",     N_("Central European") },
  { WIKIPAD_ENCODING_IBM_855,      "IBM855",           N_("Cyrillic") },
  { WIKIPAD_ENCODING_ISO_8859_5,   "ISO-8859-5",       N_("Cyrillic") },
  { WIKIPAD_ENCODING_ISO_IR_111,   "ISO-IR-111",       N_("Cyrillic") },
  { WIKIPAD_ENCODING_KOI8_R,       "KOI8R",            N_("Cyrillic") },
  { WIKIPAD_ENCODING_WINDOWS_1251, "WINDOWS-1251",     N_("Cyrillic") },
  { WIKIPAD_ENCODING_CP_866,       "CP866",            N_("Cyrillic/Russian") },
  { WIKIPAD_ENCODING_KOI8_U,       "KOI8U",            N_("Cyrillic/Ukrainian") },
  { WIKIPAD_ENCODING_ISO_8859_16,  "ISO-8859-16",      N_("Romanian") },

  /* middle eastern */
  { WIKIPAD_ENCODING_IBM_864,      "IBM864",           N_("Arabic") },
  { WIKIPAD_ENCODING_ISO_8859_6,   "ISO-8859-6",       N_("Arabic") },
  { WIKIPAD_ENCODING_WINDOWS_1256, "WINDOWS-1256",     N_("Arabic") },
  { WIKIPAD_ENCODING_IBM_862,      "IBM862",           N_("Hebrew") },
  { WIKIPAD_ENCODING_ISO_8859_8_I, "ISO-8859-8-I",     N_("Hebrew") },
  { WIKIPAD_ENCODING_WINDOWS_1255, "WINDOWS-1255",     N_("Hebrew") },
  { WIKIPAD_ENCODING_ISO_8859_8,   "ISO-8859-8",       N_("Hebrew Visual") },

  /* asian */
  { WIKIPAD_ENCODING_ARMSCII_8,    "ARMSCII-8",        N_("Armenian") },
  { WIKIPAD_ENCODING_GEOSTD8,      "GEORGIAN-ACADEMY", N_("Georgian") },
  { WIKIPAD_ENCODING_TIS_620,      "TIS-620",          N_("Thai") },
  { WIKIPAD_ENCODING_IBM_857,      "IBM857",           N_("Turkish") },
  { WIKIPAD_ENCODING_WINDOWS_1254, "WINDOWS-1254",     N_("Turkish") },
  { WIKIPAD_ENCODING_ISO_8859_9,   "ISO-8859-9",       N_("Turkish") },
  { WIKIPAD_ENCODING_TCVN,         "TCVN",             N_("Vietnamese") },
  { WIKIPAD_ENCODING_VISCII,       "VISCII",           N_("Vietnamese") },
  { WIKIPAD_ENCODING_WINDOWS_1258, "WINDOWS-1258",     N_("Vietnamese") },

  /* unicode */
  { WIKIPAD_ENCODING_UTF_7,        "UTF-7",            N_("Unicode") },
  { WIKIPAD_ENCODING_UTF_8,        "UTF-8",            N_("Unicode") },
  { WIKIPAD_ENCODING_UTF_16LE,     "UTF-16LE",         N_("Unicode") },
  { WIKIPAD_ENCODING_UTF_16BE,     "UTF-16BE",         N_("Unicode") },
  { WIKIPAD_ENCODING_UCS_2LE,      "UCS-2LE",          N_("Unicode") },
  { WIKIPAD_ENCODING_UCS_2BE,      "UCS-2BE",          N_("Unicode") },
  { WIKIPAD_ENCODING_UTF_32LE,     "UTF-32LE",         N_("Unicode") },
  { WIKIPAD_ENCODING_UTF_32BE,     "UTF-32BE",         N_("Unicode") },

  /* east asian */
  { WIKIPAD_ENCODING_GB18030,      "GB18030",          N_("Chinese Simplified") },
  { WIKIPAD_ENCODING_GB2312,       "GB2312",           N_("Chinese Simplified") },
  { WIKIPAD_ENCODING_GBK,          "GBK",              N_("Chinese Simplified") },
  { WIKIPAD_ENCODING_HZ,           "HZ",               N_("Chinese Simplified") },
  { WIKIPAD_ENCODING_BIG5,         "BIG5",             N_("Chinese Traditional") },
  { WIKIPAD_ENCODING_BIG5_HKSCS,   "BIG5-HKSCS",       N_("Chinese Traditional") },
  { WIKIPAD_ENCODING_EUC_TW,       "EUC-TW",           N_("Chinese Traditional") },
  { WIKIPAD_ENCODING_EUC_JP,       "EUC-JP",           N_("Japanese") },
  { WIKIPAD_ENCODING_ISO_2022_JP,  "ISO-2022-JP",      N_("Japanese") },
  { WIKIPAD_ENCODING_SHIFT_JIS,    "SHIFT_JIS",        N_("Japanese") },
  { WIKIPAD_ENCODING_EUC_KR,       "EUC-KR",           N_("Korean") },
  { WIKIPAD_ENCODING_ISO_2022_KR,  "ISO-2022-KR",      N_("Korean") },
  { WIKIPAD_ENCODING_JOHAB,        "JOHAB",            N_("Korean") },
  { WIKIPAD_ENCODING_UHC,          "UHC",              N_("Korean") }
};



guint n_encoding_infos = G_N_ELEMENTS (encoding_infos);



const gchar *
wikipad_encoding_get_charset (WikipadEncoding encoding)
{
  guint i;

  /* try to find and return the charset */
  for (i = 0; i < G_N_ELEMENTS (encoding_infos); i++)
    if (encoding_infos[i].encoding == encoding)
      return encoding_infos[i].charset;

  return NULL;
}



const gchar *
wikipad_encoding_get_name (WikipadEncoding encoding)
{
  guint i;

  /* try to find and return the translated name */
  for (i = 0; i < G_N_ELEMENTS (encoding_infos); i++)
    if (encoding_infos[i].encoding == encoding)
      return _(encoding_infos[i].name);

  return NULL;
}



WikipadEncoding
wikipad_encoding_user (void)
{
  static WikipadEncoding  encoding = WIKIPAD_ENCODING_NONE;
  const gchar             *charset;

  if (G_UNLIKELY (encoding == WIKIPAD_ENCODING_NONE))
    {
      /* try to find the user charset */
      if (g_get_charset (&charset) == FALSE)
        encoding = wikipad_encoding_find (charset);

      /* default to utf-8 */
      if (G_LIKELY (encoding == WIKIPAD_ENCODING_NONE))
        encoding = WIKIPAD_ENCODING_UTF_8;
    }

  return encoding;
}



WikipadEncoding
wikipad_encoding_find (const gchar *charset)
{
  guint i;

  /* check for invalid strings */
  if (G_UNLIKELY (charset == NULL || *charset == '\0'))
    return WIKIPAD_ENCODING_NONE;

  /* find the charset */
  for (i = 0; i < G_N_ELEMENTS (encoding_infos); i++)
    if (strcasecmp (encoding_infos[i].charset, charset) == 0)
      return encoding_infos[i].encoding;

  /* no encoding charset found */
  return WIKIPAD_ENCODING_NONE;
}



gboolean
wikipad_encoding_is_unicode (WikipadEncoding encoding)
{
  const gchar *charset;

  /* get the characterset name */
  charset = wikipad_encoding_get_charset (encoding);

  /* check for unicode charset */
  if (charset != NULL && (strncmp (charset, "UTF", 3) == 0
      || strncmp (charset, "UCS", 3) == 0))
    return TRUE;

  /* not an unicode charset */
  return FALSE;
}

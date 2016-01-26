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

#ifndef __WIKIPAD_ENCODINGS_H__
#define __WIKIPAD_ENCODINGS_H__

G_BEGIN_DECLS

typedef struct _WikipadEncodingInfo WikipadEncodingInfo;

typedef enum
{
  WIKIPAD_ENCODING_NONE,
  WIKIPAD_ENCODING_CUSTOM,

  WIKIPAD_ENCODING_ISO_8859_1,
  WIKIPAD_ENCODING_ISO_8859_2,
  WIKIPAD_ENCODING_ISO_8859_3,
  WIKIPAD_ENCODING_ISO_8859_4,
  WIKIPAD_ENCODING_ISO_8859_5,
  WIKIPAD_ENCODING_ISO_8859_6,
  WIKIPAD_ENCODING_ISO_8859_7,
  WIKIPAD_ENCODING_ISO_8859_8,
  WIKIPAD_ENCODING_ISO_8859_8_I,
  WIKIPAD_ENCODING_ISO_8859_9,
  WIKIPAD_ENCODING_ISO_8859_10,
  WIKIPAD_ENCODING_ISO_8859_13,
  WIKIPAD_ENCODING_ISO_8859_14,
  WIKIPAD_ENCODING_ISO_8859_15,
  WIKIPAD_ENCODING_ISO_8859_16,

  WIKIPAD_ENCODING_UTF_7,
  WIKIPAD_ENCODING_UTF_8,
  WIKIPAD_ENCODING_UTF_16LE,
  WIKIPAD_ENCODING_UTF_16BE,
  WIKIPAD_ENCODING_UCS_2LE,
  WIKIPAD_ENCODING_UCS_2BE,
  WIKIPAD_ENCODING_UTF_32LE,
  WIKIPAD_ENCODING_UTF_32BE,

  WIKIPAD_ENCODING_ARMSCII_8,
  WIKIPAD_ENCODING_BIG5,
  WIKIPAD_ENCODING_BIG5_HKSCS,
  WIKIPAD_ENCODING_CP_866,

  WIKIPAD_ENCODING_EUC_JP,
  WIKIPAD_ENCODING_EUC_KR,
  WIKIPAD_ENCODING_EUC_TW,

  WIKIPAD_ENCODING_GB18030,
  WIKIPAD_ENCODING_GB2312,
  WIKIPAD_ENCODING_GBK,
  WIKIPAD_ENCODING_GEOSTD8,
  WIKIPAD_ENCODING_HZ,

  WIKIPAD_ENCODING_IBM_850,
  WIKIPAD_ENCODING_IBM_852,
  WIKIPAD_ENCODING_IBM_855,
  WIKIPAD_ENCODING_IBM_857,
  WIKIPAD_ENCODING_IBM_862,
  WIKIPAD_ENCODING_IBM_864,

  WIKIPAD_ENCODING_ISO_2022_JP,
  WIKIPAD_ENCODING_ISO_2022_KR,
  WIKIPAD_ENCODING_ISO_IR_111,
  WIKIPAD_ENCODING_JOHAB,
  WIKIPAD_ENCODING_KOI8_R,
  WIKIPAD_ENCODING_KOI8_U,

  WIKIPAD_ENCODING_SHIFT_JIS,
  WIKIPAD_ENCODING_TCVN,
  WIKIPAD_ENCODING_TIS_620,
  WIKIPAD_ENCODING_UHC,
  WIKIPAD_ENCODING_VISCII,

  WIKIPAD_ENCODING_WINDOWS_1250,
  WIKIPAD_ENCODING_WINDOWS_1251,
  WIKIPAD_ENCODING_WINDOWS_1252,
  WIKIPAD_ENCODING_WINDOWS_1253,
  WIKIPAD_ENCODING_WINDOWS_1254,
  WIKIPAD_ENCODING_WINDOWS_1255,
  WIKIPAD_ENCODING_WINDOWS_1256,
  WIKIPAD_ENCODING_WINDOWS_1257,
  WIKIPAD_ENCODING_WINDOWS_1258
}
WikipadEncoding;

struct _WikipadEncodingInfo
{
  WikipadEncoding  encoding;
  const gchar      *charset;
  const gchar      *name;
};


extern const WikipadEncodingInfo encoding_infos[];

extern guint                      n_encoding_infos;

const gchar      *wikipad_encoding_get_charset (WikipadEncoding  encoding);

const gchar      *wikipad_encoding_get_name    (WikipadEncoding  encoding);

WikipadEncoding  wikipad_encoding_user        (void);

WikipadEncoding  wikipad_encoding_find        (const gchar      *charset);

gboolean          wikipad_encoding_is_unicode  (WikipadEncoding  encoding);

G_END_DECLS

#endif /* !__WIKIPAD_ENCODINGS_H__ */

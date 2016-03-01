
#ifndef ___wikipad_marshal_MARSHAL_H__
#define ___wikipad_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:INT,INT,INT (wikipad-marshal.list:1) */
extern void _wikipad_marshal_VOID__INT_INT_INT (GClosure     *closure,
                                                GValue       *return_value,
                                                guint         n_param_values,
                                                const GValue *param_values,
                                                gpointer      invocation_hint,
                                                gpointer      marshal_data);

/* INT:FLAGS,STRING,STRING (wikipad-marshal.list:2) */
extern void _wikipad_marshal_INT__FLAGS_STRING_STRING (GClosure     *closure,
                                                       GValue       *return_value,
                                                       guint         n_param_values,
                                                       const GValue *param_values,
                                                       gpointer      invocation_hint,
                                                       gpointer      marshal_data);

/* VOID:OBJECT,INT,INT (wikipad-marshal.list:3) */
extern void _wikipad_marshal_VOID__OBJECT_INT_INT (GClosure     *closure,
                                                   GValue       *return_value,
                                                   guint         n_param_values,
                                                   const GValue *param_values,
                                                   gpointer      invocation_hint,
                                                   gpointer      marshal_data);

G_END_DECLS

#endif /* ___wikipad_marshal_MARSHAL_H__ */


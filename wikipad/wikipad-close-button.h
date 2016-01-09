#ifndef __WIKIPAD_CLOSE_BUTTON_H__
#define __WIKIPAD_CLOSE_BUTTON_H__ 1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define WIKIPAD_TYPE_CLOSE_BUTTON            (wikipad_close_button_get_type ())
#define WIKIPAD_CLOSE_BUTTON(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WIKIPAD_TYPE_CLOSE_BUTTON, WikipadCloseButton))
#define WIKIPAD_CLOSE_BUTTON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WIKIPAD_TYPE_CLOSE_BUTTON, WikipadCloseButtonClass))
#define WIKIPAD_IS_CLOSE_BUTTON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WIKIPAD_TYPE_CLOSE_BUTTON))
#define WIKIPAD_IS_CLOSE_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WIKIPAD_TYPE_CLOSE_BUTTON))
#define WIKIPAD_CLOSE_BUTTON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), WIKIPAD_TYPE_CLOSE_BUTTON, WikipadCloseButtonClass))

typedef struct WikipadCloseButton_      WikipadCloseButton;
typedef struct WikipadCloseButtonClass_ WikipadCloseButtonClass;

GType      wikipad_close_button_get_type (void);
GtkWidget *wikipad_close_button_new      (void);

G_END_DECLS

#endif /* __WIKIPAD_CLOSE_BUTTON_H__ */

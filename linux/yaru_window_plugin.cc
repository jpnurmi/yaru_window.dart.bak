#include "include/yaru_window/yaru_window_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

struct _YaruWindowPlugin {
  GObject parent_instance;
  FlPluginRegistrar* registrar;
  GHashTable* signals;
};

G_DEFINE_TYPE(YaruWindowPlugin, yaru_window_plugin, g_object_get_type())

static GtkWindow* yaru_window_plugin_get_window(YaruWindowPlugin* self,
                                                gint window_id) {
  FlView* view = fl_plugin_registrar_get_view(self->registrar);
  return GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(view)));
}

static GdkPoint get_cursor_position(GtkWindow* window) {
  GdkWindow* handle = gtk_widget_get_window(GTK_WIDGET(window));
  GdkDisplay* display = gdk_window_get_display(handle);
  GdkSeat* seat = gdk_display_get_default_seat(display);
  GdkDevice* pointer = gdk_seat_get_pointer(seat);
  GdkPoint pos;
  gdk_device_get_position(pointer, nullptr, &pos.x, &pos.y);
  return pos;
}

static GdkPoint get_window_origin(GtkWindow* window) {
  GdkWindow* handle = gtk_widget_get_window(GTK_WIDGET(window));
  GdkPoint pos;
  gdk_window_get_origin(handle, &pos.x, &pos.y);
  return pos;
}

static void move_window(GtkWindow* window) {
  GdkPoint cursor = get_cursor_position(window);
  gtk_window_begin_move_drag(window, GDK_BUTTON_PRIMARY, cursor.x, cursor.y,
                             GDK_CURRENT_TIME);
}

static void show_window_menu(GtkWindow* window) {
  GdkWindow* handle = gtk_widget_get_window(GTK_WIDGET(window));
  GdkDisplay* display = gdk_window_get_display(handle);
  GdkSeat* seat = gdk_display_get_default_seat(display);
  GdkDevice* pointer = gdk_seat_get_pointer(seat);

  GdkPoint cursor = get_cursor_position(window);
  GdkPoint origin = get_window_origin(window);

  g_autoptr(GdkEvent) event = gdk_event_new(GDK_BUTTON_PRESS);
  event->button.button = GDK_BUTTON_SECONDARY;
  event->button.device = pointer;
  event->button.window = handle;
  event->button.x_root = cursor.x;
  event->button.y_root = cursor.y;
  event->button.x = cursor.x - origin.x;
  event->button.y = cursor.y - origin.y;
  gdk_window_show_window_menu(handle, event);
}

static void restore_window(GtkWindow* window) {
  GdkWindow* handle = gtk_widget_get_window(GTK_WIDGET(window));
  GdkWindowState state = gdk_window_get_state(handle);
  if (state & GDK_WINDOW_STATE_FULLSCREEN) {
    gtk_window_unfullscreen(window);
  } else if (state & GDK_WINDOW_STATE_MAXIMIZED) {
    gtk_window_unmaximize(window);
  } else if (state & GDK_WINDOW_STATE_ICONIFIED) {
    gtk_window_deiconify(window);
  }
}

static void yaru_window_plugin_handle_method_call(YaruWindowPlugin* self,
                                                  FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);
  gint window_id = fl_value_get_int(fl_method_call_get_args(method_call));
  GtkWindow* window = yaru_window_plugin_get_window(self, window_id);

  if (strcmp(method, "close") == 0) {
    gtk_window_close(window);
  } else if (strcmp(method, "destroy") == 0) {
    gtk_widget_destroy(GTK_WIDGET(window));
  } else if (strcmp(method, "fullscreen") == 0) {
    gtk_window_fullscreen(window);
  } else if (strcmp(method, "hide") == 0) {
    gtk_widget_hide(GTK_WIDGET(window));
  } else if (strcmp(method, "init") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "maximize") == 0) {
    gtk_window_maximize(window);
  } else if (strcmp(method, "menu") == 0) {
    show_window_menu(window);
  } else if (strcmp(method, "minimize") == 0) {
    gtk_window_iconify(window);
  } else if (strcmp(method, "move") == 0) {
    move_window(window);
  } else if (strcmp(method, "restore") == 0) {
    restore_window(window);
  } else if (strcmp(method, "show") == 0) {
    gtk_widget_show(GTK_WIDGET(window));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  if (!response) {
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void yaru_window_plugin_dispose(GObject* object) {
  YaruWindowPlugin* self = YARU_WINDOW_PLUGIN(object);
  g_clear_object(&self->registrar);
  g_clear_pointer(&self->signals, g_hash_table_unref);
  G_OBJECT_CLASS(yaru_window_plugin_parent_class)->dispose(object);
}

static void yaru_window_plugin_class_init(YaruWindowPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = yaru_window_plugin_dispose;
}

static void yaru_window_plugin_init(YaruWindowPlugin* self) {
  self->signals = g_hash_table_new(g_direct_hash, g_direct_equal);
}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  YaruWindowPlugin* plugin = YARU_WINDOW_PLUGIN(user_data);
  yaru_window_plugin_handle_method_call(plugin, method_call);
}

static FlValue* get_window_state(GtkWindow* window) {
  GdkWindow* handle = gtk_widget_get_window(GTK_WIDGET(window));
  GdkWindowState state = gdk_window_get_state(handle);
  GdkWindowTypeHint type = gdk_window_get_type_hint(handle);

  gboolean is_active = gtk_window_is_active(window);
  gboolean is_closable = gtk_window_get_deletable(window);
  gboolean is_fullscreen = state & GDK_WINDOW_STATE_FULLSCREEN;
  gboolean is_maximized = state & GDK_WINDOW_STATE_MAXIMIZED;
  gboolean is_minimized = state & GDK_WINDOW_STATE_ICONIFIED;
  gboolean is_normal = type == GDK_WINDOW_TYPE_HINT_NORMAL;
  gboolean is_restorable = is_fullscreen || is_maximized || is_minimized;

  FlValue* result = fl_value_new_map();
  fl_value_set_string_take(result, "active", fl_value_new_bool(is_active));
  fl_value_set_string_take(result, "closable", fl_value_new_bool(is_closable));
  fl_value_set_string_take(result, "fullscreen",
                           fl_value_new_bool(is_fullscreen));
  fl_value_set_string_take(result, "maximizable",
                           fl_value_new_bool(is_normal && !is_maximized));
  fl_value_set_string_take(result, "maximized",
                           fl_value_new_bool(is_maximized));
  fl_value_set_string_take(result, "minimizable",
                           fl_value_new_bool(is_normal && !is_minimized));
  fl_value_set_string_take(result, "minimized",
                           fl_value_new_bool(is_minimized));
  fl_value_set_string_take(result, "restorable",
                           fl_value_new_bool(is_restorable));
  return result;
}

static gboolean window_state_cb(GtkWidget* window, GdkEventWindowState* event,
                                gpointer user_data) {
  FlEventChannel* channel = FL_EVENT_CHANNEL(user_data);
  g_autoptr(FlValue) state = get_window_state(GTK_WINDOW(window));
  fl_event_channel_send(channel, state, nullptr, nullptr);
  return false;
}

static FlMethodErrorResponse* listen_state_cb(FlEventChannel* channel,
                                              FlValue* args,
                                              gpointer user_data) {
  YaruWindowPlugin* plugin = YARU_WINDOW_PLUGIN(user_data);
  gint window_id = fl_value_get_int(args);
  GtkWindow* window = yaru_window_plugin_get_window(plugin, window_id);
  if (!g_hash_table_contains(plugin->signals, GINT_TO_POINTER(window_id))) {
    gint handler = g_signal_connect(window, "window-state-event",
                                    G_CALLBACK(window_state_cb), channel);
    g_hash_table_insert(plugin->signals, GINT_TO_POINTER(window_id),
                        GINT_TO_POINTER(handler));
  }
  g_autoptr(FlValue) state = get_window_state(window);
  fl_event_channel_send(channel, state, nullptr, nullptr);
  return nullptr;
}

static FlMethodErrorResponse* cancel_state_cb(FlEventChannel* channel,
                                              FlValue* args,
                                              gpointer user_data) {
  YaruWindowPlugin* plugin = YARU_WINDOW_PLUGIN(user_data);
  gint window_id = fl_value_get_int(args);
  GtkWindow* window = yaru_window_plugin_get_window(plugin, window_id);
  gint handler = GPOINTER_TO_INT(
      g_hash_table_lookup(plugin->signals, GINT_TO_POINTER(window_id)));
  if (handler != 0) {
    g_signal_handler_disconnect(window, handler);
    g_hash_table_remove(plugin->signals, GINT_TO_POINTER(window_id));
  }
  return nullptr;
}

// Returns true if the widget is GtkHeaderBar or HdyHeaderBar from libhandy.
static gboolean is_header_bar(GtkWidget* widget) {
  return widget != nullptr &&
         (GTK_IS_HEADER_BAR(widget) ||
          g_str_has_suffix(G_OBJECT_TYPE_NAME(widget), "HeaderBar"));
}

// Recursively searches for a Gtk/HdyHeaderBar in the widget tree.
static GtkWidget* find_header_bar(GtkWidget* widget) {
  if (is_header_bar(widget)) {
    return widget;
  }

  if (GTK_IS_CONTAINER(widget)) {
    g_autoptr(GList) children =
        gtk_container_get_children(GTK_CONTAINER(widget));
    for (GList* l = children; l != nullptr; l = l->next) {
      GtkWidget* header_bar = find_header_bar(GTK_WIDGET(l->data));
      if (header_bar != nullptr) {
        return header_bar;
      }
    }
  }

  return nullptr;
}

// Returns the window's header bar which is typically a GtkHeaderBar used as
// GtkWindow::titlebar, or a HdyHeaderBar as HdyWindow granchild.
static GtkWidget* get_header_bar(GtkWindow* window) {
  GtkWidget* titlebar = gtk_window_get_titlebar(window);
  if (is_header_bar(titlebar)) {
    return titlebar;
  }
  return find_header_bar(GTK_WIDGET(window));
}

static void init_window(GtkWindow* window) {
  GtkWidget* header_bar = get_header_bar(window);
  if (header_bar != nullptr) {
    gtk_widget_hide(header_bar);
  }
}

void yaru_window_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  g_autoptr(YaruWindowPlugin) plugin =
      YARU_WINDOW_PLUGIN(g_object_new(yaru_window_plugin_get_type(), nullptr));
  plugin->registrar = FL_PLUGIN_REGISTRAR(g_object_ref(registrar));

  GtkWindow* window = yaru_window_plugin_get_window(plugin, 0);
  init_window(window);

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  FlBinaryMessenger* messenger = fl_plugin_registrar_get_messenger(registrar);

  g_autoptr(FlMethodChannel) method_channel =
      fl_method_channel_new(messenger, "yaru_window", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      method_channel, method_call_cb, g_object_ref(plugin), g_object_unref);

  g_autoptr(FlEventChannel) event_channel = fl_event_channel_new(
      messenger, "yaru_window/state", FL_METHOD_CODEC(codec));
  fl_event_channel_set_stream_handlers(event_channel, listen_state_cb,
                                       cancel_state_cb, g_object_ref(plugin),
                                       g_object_unref);
}

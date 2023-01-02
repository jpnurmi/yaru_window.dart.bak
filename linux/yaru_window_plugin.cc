#include "include/yaru_window/yaru_window_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

struct _YaruWindowPlugin {
  GObject parent_instance;
  FlPluginRegistrar* registrar;
  FlMethodChannel* method_channel;
  FlEventChannel* event_channel;
  GHashTable* signals;
};

G_DEFINE_TYPE(YaruWindowPlugin, yaru_window_plugin, g_object_get_type())

static GtkWindow* yaru_window_plugin_get_window(YaruWindowPlugin* self,
                                                gint window_id) {
  FlView* view = fl_plugin_registrar_get_view(self->registrar);
  return GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(view)));
}

static FlValue* get_window_geometry(GtkWindow* window) {
  gint x, y;
  gtk_window_get_position(window, &x, &y);
  gint width, height;
  gtk_window_get_size(window, &width, &height);

  FlValue* result = fl_value_new_map();
  fl_value_set_string_take(result, "id", fl_value_new_int(0));  // TODO
  fl_value_set_string_take(result, "type", fl_value_new_string("geometry"));
  fl_value_set_string_take(result, "x", fl_value_new_int(x));
  fl_value_set_string_take(result, "y", fl_value_new_int(y));
  fl_value_set_string_take(result, "width", fl_value_new_int(width));
  fl_value_set_string_take(result, "height", fl_value_new_int(height));
  return result;
}

static void set_window_geometry(GtkWindow* window, FlValue* geometry) {
  FlValue* x = fl_value_lookup_string(geometry, "x");
  FlValue* y = fl_value_lookup_string(geometry, "y");
  FlValue* width = fl_value_lookup_string(geometry, "width");
  FlValue* height = fl_value_lookup_string(geometry, "height");
  FlValue* maximum_width = fl_value_lookup_string(geometry, "maximumWidth");
  FlValue* maximum_height = fl_value_lookup_string(geometry, "maximumHeight");
  FlValue* minimum_width = fl_value_lookup_string(geometry, "minimumWidth");
  FlValue* minimum_height = fl_value_lookup_string(geometry, "minimumHeight");

  gboolean has_x = fl_value_get_type(x) == FL_VALUE_TYPE_INT;
  gboolean has_y = fl_value_get_type(y) == FL_VALUE_TYPE_INT;
  if (has_x || has_y) {
    GdkPoint pos;
    if (!has_x || !has_y) {
      gtk_window_get_position(window, &pos.x, &pos.y);
    }
    if (has_x) {
      pos.x = fl_value_get_int(x);
    }
    if (has_y) {
      pos.y = fl_value_get_int(y);
    }
    gtk_window_move(window, pos.x, pos.y);
  }

  gboolean has_width = fl_value_get_type(width) == FL_VALUE_TYPE_INT;
  gboolean has_height = fl_value_get_type(height) == FL_VALUE_TYPE_INT;
  if (has_width || has_height) {
    GdkRectangle size;
    if (!has_width || !has_height) {
      gtk_window_get_size(window, &size.width, &size.height);
    }
    if (has_width) {
      size.width = fl_value_get_int(width);
    }
    if (has_height) {
      size.height = fl_value_get_int(height);
    }
    gtk_window_resize(window, size.width, size.height);
  }

  GdkWindowHints mask = GdkWindowHints(0);
  GdkGeometry hints = {
      .min_width = 0,
      .min_height = 0,
      .max_width = G_MAXINT,
      .max_height = G_MAXINT,
  };
  if (fl_value_get_type(maximum_width) == FL_VALUE_TYPE_INT) {
    mask = GdkWindowHints(mask | GDK_HINT_MAX_SIZE);
    hints.max_width = fl_value_get_int(maximum_width);
  }
  if (fl_value_get_type(maximum_height) == FL_VALUE_TYPE_INT) {
    mask = GdkWindowHints(mask | GDK_HINT_MAX_SIZE);
    hints.max_height = fl_value_get_int(maximum_height);
  }
  if (fl_value_get_type(minimum_width) == FL_VALUE_TYPE_INT) {
    mask = GdkWindowHints(mask | GDK_HINT_MIN_SIZE);
    hints.min_width = fl_value_get_int(minimum_width);
  }
  if (fl_value_get_type(minimum_height) == FL_VALUE_TYPE_INT) {
    mask = GdkWindowHints(mask | GDK_HINT_MIN_SIZE);
    hints.min_height = fl_value_get_int(minimum_height);
  }
  if (mask != 0) {
    gtk_window_set_geometry_hints(window, nullptr, &hints, mask);
  }
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
  gboolean is_restorable =
      is_normal && (is_fullscreen || is_maximized || is_minimized);
  gboolean is_visible = gtk_widget_is_visible(GTK_WIDGET(window));

  FlValue* result = fl_value_new_map();
  fl_value_set_string_take(result, "id", fl_value_new_int(0));  // TODO
  fl_value_set_string_take(result, "type", fl_value_new_string("state"));
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
  fl_value_set_string_take(result, "visible", fl_value_new_bool(is_visible));
  return result;
}

static void set_window_state(GtkWindow* window, FlValue* state) {
  FlValue* active = fl_value_lookup_string(state, "active");
  FlValue* closable = fl_value_lookup_string(state, "closable");
  FlValue* fullscreen = fl_value_lookup_string(state, "fullscreen");
  FlValue* maximizable = fl_value_lookup_string(state, "maximizable");
  FlValue* maximized = fl_value_lookup_string(state, "maximized");
  FlValue* minimizable = fl_value_lookup_string(state, "minimizable");
  FlValue* minimized = fl_value_lookup_string(state, "minimized");
  FlValue* restorable = fl_value_lookup_string(state, "restorable");
  FlValue* visible = fl_value_lookup_string(state, "visible");

  if (fl_value_get_type(active) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(active)) {
      gtk_window_present(window);
    }
  }
  if (fl_value_get_type(closable) == FL_VALUE_TYPE_BOOL) {
    gtk_window_set_deletable(window, fl_value_get_bool(closable));
  }
  if (fl_value_get_type(fullscreen) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(fullscreen)) {
      gtk_window_fullscreen(window);
    } else {
      gtk_window_unfullscreen(window);
    }
  }
  if (fl_value_get_type(maximizable) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(maximizable)) {
      gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_NORMAL);
    } else {
      gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DIALOG);
    }
  }
  if (fl_value_get_type(maximized) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(maximized)) {
      gtk_window_maximize(window);
    } else {
      gtk_window_unmaximize(window);
    }
  }
  if (fl_value_get_type(minimizable) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(minimizable)) {
      gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_NORMAL);
    } else {
      gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DIALOG);
    }
  }
  if (fl_value_get_type(minimized) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(minimized)) {
      gtk_window_iconify(window);
    } else {
      gtk_window_deiconify(window);
    }
  }
  if (fl_value_get_type(restorable) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(restorable)) {
      gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_NORMAL);
    } else {
      gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DIALOG);
    }
  }
  if (fl_value_get_type(visible) == FL_VALUE_TYPE_BOOL) {
    if (fl_value_get_bool(visible)) {
      gtk_widget_show(GTK_WIDGET(window));
    } else {
      gtk_widget_hide(GTK_WIDGET(window));
    }
  }
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

static gboolean window_configure_cb(GtkWidget* window, GdkEventConfigure* event,
                                    gpointer user_data) {
  FlEventChannel* channel = FL_EVENT_CHANNEL(user_data);
  g_autoptr(FlValue) geometry = get_window_geometry(GTK_WINDOW(window));
  fl_event_channel_send(channel, geometry, nullptr, nullptr);
  return false;
}

static gboolean window_state_cb(GtkWidget* window, GdkEventWindowState* event,
                                gpointer user_data) {
  FlEventChannel* channel = FL_EVENT_CHANNEL(user_data);
  g_autoptr(FlValue) state = get_window_state(GTK_WINDOW(window));
  fl_event_channel_send(channel, state, nullptr, nullptr);
  return false;
}

static void window_property_cb(GtkWindow* window, GParamSpec*,
                               gpointer user_data) {
  FlEventChannel* channel = FL_EVENT_CHANNEL(user_data);
  g_autoptr(FlValue) state = get_window_state(GTK_WINDOW(window));
  fl_event_channel_send(channel, state, nullptr, nullptr);
}

static void yaru_window_plugin_listen_window(YaruWindowPlugin* self,
                                             gint window_id) {
  GtkWindow* window = yaru_window_plugin_get_window(self, window_id);
  if (!g_hash_table_contains(self->signals, GINT_TO_POINTER(window_id))) {
    g_signal_connect_object(window, "configure-event",
                            G_CALLBACK(window_configure_cb),
                            self->event_channel, G_CONNECT_DEFAULT);
    g_signal_connect_object(window, "window-state-event",
                            G_CALLBACK(window_state_cb), self->event_channel,
                            G_CONNECT_DEFAULT);
    g_signal_connect_object(G_OBJECT(window), "notify::deletable",
                            G_CALLBACK(window_property_cb), self->event_channel,
                            G_CONNECT_DEFAULT);
    g_signal_connect_object(G_OBJECT(window), "notify::type-hint",
                            G_CALLBACK(window_property_cb), self->event_channel,
                            G_CONNECT_DEFAULT);
    g_hash_table_insert(self->signals, GINT_TO_POINTER(window_id), 0);
  }
}

static void yaru_window_plugin_handle_method_call(YaruWindowPlugin* self,
                                                  FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);
  FlValue* args = fl_method_call_get_args(method_call);
  gint window_id = fl_value_get_int(fl_value_get_list_value(args, 0));
  GtkWindow* window = yaru_window_plugin_get_window(self, window_id);

  if (strcmp(method, "close") == 0) {
    gtk_window_close(window);
  } else if (strcmp(method, "destroy") == 0) {
    gtk_widget_destroy(GTK_WIDGET(window));
  } else if (strcmp(method, "fullscreen") == 0) {
    gtk_window_fullscreen(window);
  } else if (strcmp(method, "geometry") == 0) {
    g_autoptr(FlValue) geometry = get_window_geometry(window);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(geometry));
  } else if (strcmp(method, "hide") == 0) {
    gtk_widget_hide(GTK_WIDGET(window));
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
  } else if (strcmp(method, "state") == 0) {
    g_autoptr(FlValue) state = get_window_state(window);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(state));
  } else if (strcmp(method, "setGeometry") == 0) {
    FlValue* geometry = fl_value_get_list_value(args, 1);
    set_window_geometry(window, geometry);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "setState") == 0) {
    FlValue* state = fl_value_get_list_value(args, 1);
    set_window_state(window, state);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
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
  g_clear_object(&self->method_channel);
  g_clear_object(&self->event_channel);
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

static FlMethodErrorResponse* listen_state_cb(FlEventChannel* channel,
                                              FlValue* args,
                                              gpointer user_data) {
  return nullptr;
}

static gboolean remove_signal(gpointer key, gpointer value,
                              gpointer user_data) {
  YaruWindowPlugin* plugin = YARU_WINDOW_PLUGIN(user_data);
  gint window_id = GPOINTER_TO_INT(key);
  gint handler = GPOINTER_TO_INT(value);
  GtkWindow* window = yaru_window_plugin_get_window(plugin, window_id);
  g_signal_handler_disconnect(window, handler);
  return true;
}

static FlMethodErrorResponse* cancel_state_cb(FlEventChannel* channel,
                                              FlValue* args,
                                              gpointer user_data) {
  YaruWindowPlugin* plugin = YARU_WINDOW_PLUGIN(user_data);
  g_hash_table_foreach_remove(plugin->signals, remove_signal, plugin);
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

  plugin->method_channel =
      fl_method_channel_new(messenger, "yaru_window", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      plugin->method_channel, method_call_cb, g_object_ref(plugin),
      g_object_unref);

  plugin->event_channel = fl_event_channel_new(messenger, "yaru_window/events",
                                               FL_METHOD_CODEC(codec));
  fl_event_channel_set_stream_handlers(plugin->event_channel, listen_state_cb,
                                       cancel_state_cb, g_object_ref(plugin),
                                       g_object_unref);

  yaru_window_plugin_listen_window(plugin, 0);
}

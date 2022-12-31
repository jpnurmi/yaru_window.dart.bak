#include "include/yaru_window/yaru_window_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

struct _YaruWindowPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(YaruWindowPlugin, yaru_window_plugin, g_object_get_type())

static void yaru_window_plugin_handle_method_call(YaruWindowPlugin* self,
                                                  FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "close") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "destroy") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "fullscreen") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "hide") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "init") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "maximize") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "menu") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "minimize") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "move") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "restore") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else if (strcmp(method, "show") == 0) {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void yaru_window_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(yaru_window_plugin_parent_class)->dispose(object);
}

static void yaru_window_plugin_class_init(YaruWindowPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = yaru_window_plugin_dispose;
}

static void yaru_window_plugin_init(YaruWindowPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  YaruWindowPlugin* plugin = YARU_WINDOW_PLUGIN(user_data);
  yaru_window_plugin_handle_method_call(plugin, method_call);
}

void yaru_window_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  g_autoptr(YaruWindowPlugin) plugin =
      YARU_WINDOW_PLUGIN(g_object_new(yaru_window_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "yaru_window", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      channel, method_call_cb, g_object_ref(plugin), g_object_unref);
}

//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <yaru_window/yaru_window_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) yaru_window_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "YaruWindowPlugin");
  yaru_window_plugin_register_with_registrar(yaru_window_registrar);
}

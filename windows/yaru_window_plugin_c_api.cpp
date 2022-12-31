#include "include/yaru_window/yaru_window_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "yaru_window_plugin.h"

void YaruWindowPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  yaru_window::YaruWindowPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

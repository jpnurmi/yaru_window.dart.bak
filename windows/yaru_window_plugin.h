#ifndef FLUTTER_PLUGIN_YARU_WINDOW_PLUGIN_H_
#define FLUTTER_PLUGIN_YARU_WINDOW_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace yaru_window {

class YaruWindowPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  YaruWindowPlugin();

  virtual ~YaruWindowPlugin();

  // Disallow copy and assign.
  YaruWindowPlugin(const YaruWindowPlugin&) = delete;
  YaruWindowPlugin& operator=(const YaruWindowPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace yaru_window

#endif  // FLUTTER_PLUGIN_YARU_WINDOW_PLUGIN_H_

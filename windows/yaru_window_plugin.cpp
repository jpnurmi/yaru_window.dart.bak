#include "include/yaru_window/yaru_window_plugin.h"

#include <flutter/event_channel.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <windows.h>

#include "yaru_window.h"

namespace {
typedef flutter::EncodableValue FlValue;
typedef flutter::EventChannel<FlValue> FlEventChannel;
typedef flutter::EventSink<FlValue> FlEventSink;
typedef flutter::MethodCall<FlValue> FlMethodCall;
typedef flutter::MethodResult<FlValue> FlMethodResult;
typedef flutter::MethodChannel<FlValue> FlMethodChannel;
typedef flutter::Plugin FlPlugin;
typedef flutter::PluginRegistrarManager FlPluginRegistrarManager;
typedef flutter::PluginRegistrarWindows FlPluginRegistrar;
typedef flutter::StandardMethodCodec FlStandardMethodCodec;
typedef flutter::StreamHandler<FlValue> FlStreamHandler;
typedef flutter::StreamHandlerError<FlValue> FlStreamHandlerError;
typedef flutter::FlutterView FlView;

YaruWindow GetYaruWindow(FlView *view) {
  HWND hwnd = ::GetAncestor(view->GetNativeWindow(), GA_ROOT);
  return YaruWindow(hwnd);
}

class YaruWindowStateHandler : public FlStreamHandler {
 public:
  YaruWindowStateHandler(FlPluginRegistrar *registrar) : _registrar(registrar) {
    _delegate = _registrar->RegisterTopLevelWindowProcDelegate(
        [this](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
          return ProcessMsg(hwnd, message, wparam, lparam);
        });
  }

  ~YaruWindowStateHandler() {
    _registrar->UnregisterTopLevelWindowProcDelegate(_delegate);
  }

 protected:
  void SendWindowState(YaruWindow window) {
    const std::map<FlValue, FlValue> status = {
        {"active", window.IsActive()},
        {"closable", window.IsClosable()},
        {"fullscreen", window.IsFullscreen()},
        {"maximizable", window.IsMaximizable()},
        {"maximized", window.IsMaximized()},
        {"minimizable", window.IsMinimizable()},
        {"minimized", window.IsMinimized()},
        {"restorable", window.IsRestorable()},
    };
    _sink->Success(status);
  }

  std::unique_ptr<FlStreamHandlerError> OnListenInternal(
      const FlValue *arguments,
      std::unique_ptr<FlEventSink> &&events) override {
    _sink = std::move(events);
    auto window = GetYaruWindow(_registrar->GetView());
    window.Init();
    SendWindowState(window);
    return nullptr;
  }

  std::unique_ptr<FlStreamHandlerError> OnCancelInternal(
      const FlValue *arguments) override {
    _sink = nullptr;
    return nullptr;
  }

  std::optional<HRESULT> ProcessMsg(HWND hwnd, UINT message, WPARAM wparam,
                                    LPARAM lparam) {
    YaruWindow window(hwnd);
    switch (message) {
      case WM_NCACTIVATE:
        SendWindowState(window);
        return true;
      case WM_NCCALCSIZE:
        if (wparam) {
          NCCALCSIZE_PARAMS *sz = reinterpret_cast<NCCALCSIZE_PARAMS *>(lparam);
          sz->rgrc[0].top += window.IsMaximized() ? 8 : 0;
          sz->rgrc[0].right -= 8;
          sz->rgrc[0].bottom -= 8;
          sz->rgrc[0].left -= -8;
          return false;
        }
        break;
      case WM_SIZE:
        if (wparam == SIZE_MAXIMIZED || wparam == SIZE_MINIMIZED ||
            wparam == SIZE_RESTORED) {
          SendWindowState(window);
        }
        break;
      default:
        break;
    }
    return std::nullopt;
  }

 private:
  int _delegate = -1;
  std::unique_ptr<FlEventSink> _sink;
  FlPluginRegistrar *_registrar = nullptr;
};

class YaruWindowPlugin : public FlPlugin {
 public:
  YaruWindowPlugin(FlPluginRegistrar *registrar);

 protected:
  void HandleMethodCall(const FlMethodCall &method_call,
                        std::unique_ptr<FlMethodResult> result);

 private:
  YaruWindow GetWindow(int id);

  FlPluginRegistrar *_registrar = nullptr;
  std::unique_ptr<FlMethodChannel> _method_channel;
  std::unique_ptr<FlEventChannel> _event_channel;
};

YaruWindowPlugin::YaruWindowPlugin(FlPluginRegistrar *registrar)
    : _registrar(registrar) {
  _method_channel =
      std::make_unique<FlMethodChannel>(registrar->messenger(), "yaru_window",
                                        &FlStandardMethodCodec::GetInstance());
  _method_channel->SetMethodCallHandler([this](const auto &call, auto result) {
    HandleMethodCall(call, std::move(result));
  });

  _event_channel = std::make_unique<FlEventChannel>(
      registrar->messenger(), "yaru_window/state",
      &FlStandardMethodCodec::GetInstance());
  _event_channel->SetStreamHandler(
      std::make_unique<YaruWindowStateHandler>(registrar));
}

void YaruWindowPlugin::HandleMethodCall(
    const FlMethodCall &method_call, std::unique_ptr<FlMethodResult> result) {
  const std::string method = method_call.method_name();
  int id = std::get<int>(*method_call.arguments());
  YaruWindow window = GetWindow(id);
  if (method.compare("close") == 0) {
    window.Close();
  } else if (method.compare("destroy") == 0) {
    window.Destroy();
  } else if (method.compare("fullscreen") == 0) {
    window.SetFullscreen(true);
  } else if (method.compare("hide") == 0) {
    window.Hide();
  } else if (method.compare("maximize") == 0) {
    window.Maximize();
  } else if (method.compare("menu") == 0) {
    window.Menu();
  } else if (method.compare("minimize") == 0) {
    window.Minimize();
  } else if (method.compare("move") == 0) {
    window.Move();
  } else if (method.compare("restore") == 0) {
    window.Restore();
  } else if (method.compare("show") == 0) {
    window.Show();
  } else {
    result->NotImplemented();
    return;
  }
  result->Success();
}

YaruWindow YaruWindowPlugin::GetWindow(int id) {
  return GetYaruWindow(_registrar->GetView());
}
}  // namespace

void YaruWindowPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef ref) {
  FlPluginRegistrar *registrar =
      FlPluginRegistrarManager::GetInstance()->GetRegistrar<FlPluginRegistrar>(
          ref);
  registrar->AddPlugin(std::make_unique<YaruWindowPlugin>(registrar));
}

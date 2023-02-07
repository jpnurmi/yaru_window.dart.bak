#include "include/yaru_window/yaru_window_plugin.h"

#include "yaru_flutter.h"
#include "yaru_window.h"

namespace {
YaruWindow GetYaruWindow(FlView *view) {
  HWND hwnd = ::GetAncestor(view->GetNativeWindow(), GA_ROOT);
  return YaruWindow(hwnd);
}

class YaruWindowEventHandler : public FlStreamHandler {
 public:
  YaruWindowEventHandler(FlPluginRegistrar *registrar) : _registrar(registrar) {
    _delegate = _registrar->RegisterTopLevelWindowProcDelegate(
        [this](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
          return ProcessMsg(hwnd, message, wparam, lparam);
        });
  }

  ~YaruWindowEventHandler() {
    _registrar->UnregisterTopLevelWindowProcDelegate(_delegate);
  }

 protected:
  void SendWindowGeometry(YaruWindow window) {
    if (_sink) {
      _sink->Success(window.GetGeometry());
    }
  }

  void SendWindowState(YaruWindow window) {
    if (_sink) {
      _sink->Success(window.GetState());
    }
  }

  std::unique_ptr<FlStreamHandlerError> OnListenInternal(
      const FlValue *arguments,
      std::unique_ptr<FlEventSink> &&events) override {
    _sink = std::move(events);
    // TODO: find a better place for initialization
    auto window = GetYaruWindow(_registrar->GetView());
    window.Init();
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
      case WM_MOVE:
        SendWindowGeometry(window);
        break;
      case WM_SIZE:
        if (wparam == SIZE_MAXIMIZED || wparam == SIZE_MINIMIZED ||
            wparam == SIZE_RESTORED) {
          SendWindowState(window);
        }
        SendWindowGeometry(window);
        break;
      case WM_SHOWWINDOW:
        SendWindowState(window);
        break;
      case WM_STYLECHANGED:
        if (wparam == GWL_STYLE) {
          SendWindowState(window);
        }
        break;
      case WM_USER:
        SendWindowState(window);
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
      registrar->messenger(), "yaru_window/events",
      &FlStandardMethodCodec::GetInstance());
  _event_channel->SetStreamHandler(
      std::make_unique<YaruWindowEventHandler>(registrar));
}

void YaruWindowPlugin::HandleMethodCall(
    const FlMethodCall &method_call, std::unique_ptr<FlMethodResult> result) {
  const std::string method = method_call.method_name();
  const std::vector<FlValue> args =
      std::get<std::vector<FlValue>>(*method_call.arguments());
  int id = std::get<int>(args[0]);
  YaruWindow window = GetWindow(id);
  if (method.compare("close") == 0) {
    window.Close();
  } else if (method.compare("destroy") == 0) {
    window.Destroy();
  } else if (method.compare("drag") == 0) {
    window.Drag();
  } else if (method.compare("fullscreen") == 0) {
    window.SetFullscreen(true);
  } else if (method.compare("geometry") == 0) {
    result->Success(window.GetGeometry());
    return;
  } else if (method.compare("hide") == 0) {
    window.Hide();
  } else if (method.compare("maximize") == 0) {
    window.Maximize();
  } else if (method.compare("minimize") == 0) {
    window.Minimize();
  } else if (method.compare("restore") == 0) {
    window.Restore();
  } else if (method.compare("show") == 0) {
    window.Show();
  } else if (method.compare("showMenu") == 0) {
    window.ShowMenu();
  } else if (method.compare("state") == 0) {
    result->Success(window.GetState());
    return;
  } else if (method.compare("setState") == 0) {
    window.SetState(std::get<std::map<FlValue, FlValue>>(args[1]));
  } else if (method.compare("setGeometry") == 0) {
    window.SetGeometry(std::get<std::map<FlValue, FlValue>>(args[1]));
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

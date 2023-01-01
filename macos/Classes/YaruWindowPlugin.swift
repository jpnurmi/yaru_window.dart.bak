import Cocoa
import FlutterMacOS

public class YaruWindowPlugin: NSObject, NSWindowDelegate, FlutterPlugin, FlutterStreamHandler {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "yaru_window", binaryMessenger: registrar.messenger)
    let events = FlutterEventChannel(name: "yaru_window/state", binaryMessenger: registrar.messenger)

    let plugin = YaruWindowPlugin(registrar: registrar, channel: channel, events: events)
    registrar.addMethodCallDelegate(plugin, channel: channel)
    events.setStreamHandler(plugin)

    let window = NSApp.windows.first
    window?.delegate = plugin
    window?.titleVisibility = .hidden
    window?.titlebarAppearsTransparent = true
    window?.styleMask.insert(.fullSizeContentView)
  }

  let registrar: FlutterPluginRegistrar
  let channel: FlutterMethodChannel
  let events: FlutterEventChannel
  var sinks: [Int: FlutterEventSink] = [:]

  init(registrar: FlutterPluginRegistrar, channel: FlutterMethodChannel, events: FlutterEventChannel) {
    self.registrar = registrar
    self.channel = channel
    self.events = events
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    let window = self.getWindow(id: call.arguments as! Int)
    switch call.method {
    case "close":
      window.performClose(nil)
      result(nil)
    case "destroy":
      window.close()
      result(nil)
    case "fullscreen":
      if (!window.styleMask.contains(.fullScreen)) {
        window.toggleFullScreen(nil)
      }
      result(nil)
    case "hide":
      window.orderOut(nil)
      result(nil)
    case "maximize":
      window.zoom(nil)
      result(nil)
    case "menu":
      result(false)
    case "minimize":
      window.miniaturize(nil)
      result(nil)
    case "move":
      if (window.currentEvent != nil) {
        window.performDrag(with: window.currentEvent!)
      }
      result(nil)
    case "restore":
      if (window.styleMask.contains(.fullScreen)) {
        window.toggleFullScreen(nil)
      }
      if (window.isMiniaturized) {
        window.deminiaturize(nil)
      }
      if (window.isZoomed) {
        window.zoom(nil)
      }
      result(nil)
    case "show":
      window.makeKeyAndOrderFront(nil)
      result(nil)
    default:
      result(FlutterMethodNotImplemented)
    }
  }

  public func onListen(withArguments arguments: Any?, eventSink events: @escaping FlutterEventSink) -> FlutterError? {
    let windowId = arguments as! Int
    sinks[windowId] = events
    let window = self.getWindow(id: windowId)
    self.sendWindowState(window: window)
    return nil
  }

  public func onCancel(withArguments arguments: Any?) -> FlutterError? {
    let windowId = arguments as? Int
    if (windowId != nil) {
      sinks.removeValue(forKey: windowId!)
    }
    return nil
  }

  private func getWindow(id: Int) -> NSWindow {
    let window = registrar.view!.window!
    assert(window.delegate === self)
    return window
  }

  private func getWindowState(window: NSWindow) -> NSDictionary {
    let isActive = window.isKeyWindow
    let isClosable = window.styleMask.contains(.closable)
    let isFullscreen = window.styleMask.contains(.fullScreen)
    let isMaximized = window.isZoomed
    let isMinimizable = window.styleMask.contains(.miniaturizable)
    let isMinimized = window.isMiniaturized
    let isRestorable = isFullscreen || isMinimized || isMaximized
    return [
      "active": isActive,
      "closable": isClosable,
      "fullscreen": isFullscreen,
      "maximizable": !isMaximized,
      "maximized": isMaximized,
      "minimizable": isMinimizable,
      "minimized": isMinimized,
      "restorable": isRestorable,
    ]
  }

  private func sendWindowState(window: NSWindow) {
    for (windowId, sink) in sinks {
      if (window == self.getWindow(id: windowId)) {
        sink(self.getWindowState(window: window))
      }
    }
  }

  public func windowDidBecomeKey(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowDidResignKey(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowDidMiniaturize(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowDidDeminiaturize(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowDidEnterFullScreen(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowDidExitFullScreen(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowDidResize(_ notification: Notification) {
    let window = notification.object as! NSWindow
    if (window.isZoomed) {
      self.sendWindowState(window: window)
    }
  }
}

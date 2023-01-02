import Cocoa
import FlutterMacOS

public class YaruWindowPlugin: NSObject, NSWindowDelegate, FlutterPlugin, FlutterStreamHandler {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "yaru_window", binaryMessenger: registrar.messenger)
    let events = FlutterEventChannel(name: "yaru_window/events", binaryMessenger: registrar.messenger)

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
  var sink: FlutterEventSink?

  init(registrar: FlutterPluginRegistrar, channel: FlutterMethodChannel, events: FlutterEventChannel) {
    self.registrar = registrar
    self.channel = channel
    self.events = events
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    let args = call.arguments as! [Any]
    let window = self.getWindow(id: args[0] as! Int)
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
    case "geometry":
      result(self.getWindowGeometry(window: window))
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
    case "state":
      result(self.getWindowState(window: window))
    case "setGeometry":
      self.setWindowGeometry(window: window, geometry: args[1] as! NSDictionary)
      result(nil)
    case "setState":
      self.setWindowState(window: window, state: args[1] as! NSDictionary)
      result(nil)
    default:
      result(FlutterMethodNotImplemented)
    }
  }

  public func onListen(withArguments arguments: Any?, eventSink events: @escaping FlutterEventSink) -> FlutterError? {
    self.sink = events
    return nil
  }

  public func onCancel(withArguments arguments: Any?) -> FlutterError? {
    self.sink = nil
    return nil
  }

  private func getWindow(id: Int) -> NSWindow {
    let window = registrar.view!.window!
    assert(window.delegate === self)
    return window
  }

  private func getWindowGeometry(window: NSWindow) -> NSDictionary {
    return [
      "id": 0, // TODO
      "type": "geometry",
      "x": window.frame.origin.x,
      "y": window.frame.origin.y,
      "width": window.frame.size.width,
      "height": window.frame.size.height,
      "maximumWidth": window.maxSize.width,
      "maximumHeight": window.maxSize.height,
      "minimumWidth": window.minSize.width,
      "minimumHeight": window.minSize.height,
    ]
  }

  private func setWindowGeometry(window: NSWindow, geometry: NSDictionary) {
    var frame = window.frame
    if let x = geometry["x"] as? Int {
      frame.origin.x = CGFloat(x)
    }
    if let y = geometry["y"] as? Int {
      frame.origin.y = CGFloat(y)
    }
    if let width = geometry["width"] as? Int {
      frame.size.width = CGFloat(width)
    }
    if let height = geometry["height"] as? Int {
      frame.size.height = CGFloat(height)
    }
    window.setFrame(frame, display: true)

    var minSize = window.minSize
    if let minimumWidth = geometry["minimumWidth"] as? Int {
      minSize.width = CGFloat(minimumWidth)
    }
    if let minimumHeight = geometry["minimumHeight"] as? Int {
      minSize.height = CGFloat(minimumHeight)
    }
    window.minSize = minSize

    var maxSize = window.maxSize
    if let maximumWidth = geometry["maximumWidth"] as? Int {
      maxSize.width = CGFloat(maximumWidth)
    }
    if let maximumHeight = geometry["maximumHeight"] as? Int {
      maxSize.height = CGFloat(maximumHeight)
    }
    window.maxSize = maxSize
  }

  private func getWindowState(window: NSWindow) -> NSDictionary {
    let isActive = window.isKeyWindow
    let isClosable = window.styleMask.contains(.closable)
    let isFullscreen = window.styleMask.contains(.fullScreen)
    let isMaximizable = !window.isZoomed && window.standardWindowButton(.zoomButton)?.isEnabled ?? false
    let isMaximized = window.isZoomed
    let isMinimizable = window.styleMask.contains(.miniaturizable)
    let isMinimized = window.isMiniaturized
    let isRestorable = isFullscreen || isMinimized || isMaximized
    let isVisible = window.isVisible
    return [
      "id": 0, // TODO
      "type": "state",
      "active": isActive,
      "closable": isClosable,
      "fullscreen": isFullscreen,
      "maximizable": isMaximizable,
      "maximized": isMaximized,
      "minimizable": isMinimizable,
      "minimized": isMinimized,
      "restorable": isRestorable,
      "visible": isVisible,
    ]
  }

  private func setWindowState(window: NSWindow, state: NSDictionary) {
    if let closable = state["closable"] as? Bool {
      if (closable) {
        window.styleMask.insert(.closable)
      } else {
        window.styleMask.remove(.closable)
      }
      self.sendWindowState(window: window)
    }
    if let fullscreen = state["fullscreen"] as? Bool {
      if (window.styleMask.contains(.fullScreen) != fullscreen) {
        window.toggleFullScreen(nil)
      }
    }
    if let maximizable = state["maximizable"] as? Bool {
      window.standardWindowButton(.zoomButton)?.isEnabled = maximizable
      self.sendWindowState(window: window)
    }
    if let maximized = state["maximized"] as? Bool {
      if (window.isZoomed != maximized) {
        window.zoom(nil)
      }
    }
    if let minimizable = state["minimizable"] as? Bool {
      if (minimizable) {
        window.styleMask.insert(.miniaturizable)
      } else {
        window.styleMask.remove(.miniaturizable)
      }
      self.sendWindowState(window: window)
    }
    if let minimized = state["minimized"] as? Bool {
      if (minimized) {
        window.miniaturize(nil)
      } else {
        window.deminiaturize(nil)
      }
    }
    if let visible = state["visible"] as? Bool {
      if (visible) {
        window.makeKeyAndOrderFront(nil)
      } else {
        window.orderOut(nil)
      }
    }
  }

  private func sendWindowGeometry(window: NSWindow) {
    sink?(self.getWindowGeometry(window: window))
  }

  private func sendWindowState(window: NSWindow) {
    sink?(self.getWindowState(window: window))
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
    self.sendWindowGeometry(window: window)
  }

  public func windowDidExpose(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }

  public func windowWillClose(_ notification: Notification) {
    self.sendWindowState(window: notification.object as! NSWindow)
  }
}

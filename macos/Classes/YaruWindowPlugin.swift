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
    case "drag":
      if (window.currentEvent != nil) {
        window.performDrag(with: window.currentEvent!)
      }
      result(nil)
    case "fullscreen":
      if (!window.styleMask.contains(.fullScreen)) {
        window.toggleFullScreen(nil)
      }
      result(nil)
    case "geometry":
      if (args.count == 2) {
        self.setWindowGeometry(window, geometry: args[1] as! NSDictionary)
        result(nil)
      } else {
        result(self.getWindowGeometry(window))
      }
    case "hide":
      window.orderOut(nil)
      result(nil)
    case "hideTitle":
      window.titleVisibility = .hidden
      window.titlebarAppearsTransparent = true
      window.styleMask.insert(.fullSizeContentView)
      result(nil)
    case "maximize":
      window.zoom(nil)
      result(nil)
    case "minimize":
      window.miniaturize(nil)
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
    case "showMenu":
      result(false)
    case "state":
      if (args.count == 2) {
        self.setWindowState(window, state: args[1] as! NSDictionary)
        result(nil)
      } else {
        result(self.getWindowState(window))
      }
    case "style":
      if (args.count == 2) {
        self.setWindowStyle(window, style: args[1] as! NSDictionary)
        result(nil)
      } else {
        result(self.getWindowStyle(window))
      }
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

  private func getWindowGeometry(_ window: NSWindow) -> NSDictionary {
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

  private func setWindowGeometry(_ window: NSWindow, geometry: NSDictionary) {
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

  private func getWindowState(_ window: NSWindow) -> NSDictionary {
    let isActive = window.isKeyWindow
    let isClosable = window.styleMask.contains(.closable)
    let isFullscreen = window.styleMask.contains(.fullScreen)
    let isMaximizable = !window.isZoomed && window.standardWindowButton(.zoomButton)?.isEnabled ?? false
    let isMaximized = window.isZoomed
    let isMinimizable = window.styleMask.contains(.miniaturizable)
    let isMinimized = window.isMiniaturized
    let isMovable = window.isMovable;
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
      "movable": isMovable,
      "restorable": isRestorable,
      "visible": isVisible,
    ]
  }

  private func setWindowState(_ window: NSWindow, state: NSDictionary) {
    if let closable = state["closable"] as? Bool {
      if (closable) {
        window.styleMask.insert(.closable)
      } else {
        window.styleMask.remove(.closable)
      }
      self.sendWindowState(window)
    }
    if let fullscreen = state["fullscreen"] as? Bool {
      if (window.styleMask.contains(.fullScreen) != fullscreen) {
        window.toggleFullScreen(nil)
      }
    }
    if let maximizable = state["maximizable"] as? Bool {
      window.standardWindowButton(.zoomButton)?.isEnabled = maximizable
      self.sendWindowState(window)
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
      self.sendWindowState(window)
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

  private func getWindowStyle(_ window: NSWindow) -> NSDictionary {
    return [
      "id": 0, // TODO
      "type": "style",
      "background": window.backgroundColor.argb,
      "opacity": window.alphaValue,
    ]
  }

  private func setWindowStyle(_ window: NSWindow, style: NSDictionary) {
    if let background = style["background"] as? Int {
      window.backgroundColor = NSColor(argb: background)
    }
    if let opacity = style["opacity"] as? CGFloat {
      window.alphaValue = opacity
    }
  }

  private func sendWindowGeometry(_ window: NSWindow) {
    sink?(self.getWindowGeometry(window))
  }

  private func sendWindowState(_ window: NSWindow) {
    sink?(self.getWindowState(window))
  }

  private func sendWindowClose(_ window: NSWindow) {
    DispatchQueue.main.async {
      self.channel.invokeMethod("onClose", arguments: 0) { result in
        print(String(describing: result))
        if (result as? Bool == true) {
          window.close()
        }
      }
    }
  }

  public func windowDidBecomeKey(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowDidResignKey(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowDidMiniaturize(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowDidDeminiaturize(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowDidEnterFullScreen(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowDidExitFullScreen(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowDidResize(_ notification: Notification) {
    let window = notification.object as! NSWindow
    if (window.isZoomed) {
      self.sendWindowState(window)
    }
    self.sendWindowGeometry(window)
  }

  public func windowDidExpose(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }

  public func windowShouldClose(_ sender: NSWindow) -> Bool {
    self.sendWindowClose(sender)
    return false
  }

  public func windowWillClose(_ notification: Notification) {
    self.sendWindowState(notification.object as! NSWindow)
  }
}

public extension NSColor {
    convenience init(argb: Int) {
        self.init(deviceRed: CGFloat((argb >> 16) & 0xff) / 255.0,
                  green: CGFloat((argb >> 8) & 0xff) / 255.0,
                  blue: CGFloat(argb & 0xff) / 255.0,
                  alpha: CGFloat((argb >> 24) & 0xff) / 255.0)
    }

    var argb: Int {
        let color = self.usingColorSpaceName(NSColorSpaceName.deviceRGB)!
        var r: CGFloat = 0.0
        var g: CGFloat = 0.0
        var b: CGFloat = 0.0
        var a: CGFloat = 0.0
        color.getRed(&r, green: &g, blue: &b, alpha: &a)
        return (Int(a * 255.0) << 24) | (Int(r * 255.0) << 16) | (Int(g * 255.0) << 8) | Int(b * 255.0)
    }
}

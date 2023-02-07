import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'platform_interface.dart';

class YaruWindowMethodChannel extends YaruWindowPlatform {
  @visibleForTesting
  final channel = const MethodChannel('yaru_window');
  @visibleForTesting
  final events = const EventChannel('yaru_window/events');

  Future<T?> _invokeMethod<T>(String method, [dynamic arguments]) {
    return channel.invokeMethod<T>(method, arguments);
  }

  Future<Map<K, V>> _invokeMapMethod<K, V>(String method, [dynamic arguments]) {
    return channel.invokeMapMethod<K, V>(method, arguments).then((v) => v!);
  }

  Stream<Map>? _events;
  Stream<JsonObject> _receiveEvents(int id, String type) {
    _events ??= events.receiveBroadcastStream().cast<Map>();
    return _events!
        .where((event) => event['id'] == id && event['type'] == type)
        .map((event) => event.cast<String, dynamic>());
  }

  @override
  Future<void> close(int id) => _invokeMethod('close', [id]);
  @override
  Future<void> destroy(int id) => _invokeMethod('destroy', [id]);
  @override
  Future<void> drag(int id) => _invokeMethod('drag', [id]);
  @override
  Future<void> fullscreen(int id) => _invokeMethod('fullscreen', [id]);
  @override
  Future<JsonObject> geometry(int id) => _invokeMapMethod('geometry', [id]);
  @override
  Future<void> hide(int id) => _invokeMethod('hide', [id]);
  @override
  Future<void> hideTitle(int id) => _invokeMethod('hideTitle', [id]);
  @override
  Future<void> init(int id) => _invokeMethod('init', [id]);
  @override
  Future<void> maximize(int id) => _invokeMethod('maximize', [id]);
  @override
  Future<void> minimize(int id) => _invokeMethod('minimize', [id]);
  @override
  Future<void> restore(int id) => _invokeMethod('restore', [id]);
  @override
  Future<void> show(int id) => _invokeMethod('show', [id]);
  @override
  Future<void> showMenu(int id) => _invokeMethod('showMenu', [id]);
  @override
  Future<JsonObject> state(int id) => _invokeMapMethod('state', [id]);
  @override
  Future<JsonObject> style(int id) => _invokeMapMethod('style', [id]);

  @override
  Stream<JsonObject> geometries(int id) => _receiveEvents(id, 'geometry');
  @override
  Stream<JsonObject> states(int id) => _receiveEvents(id, 'state');

  @override
  Future<void> setGeometry(int id, JsonObject geometry) {
    return _invokeMethod('seometry', [id, geometry]);
  }

  @override
  Future<void> setState(int id, JsonObject state) {
    return _invokeMethod('state', [id, state]);
  }

  @override
  Future<void> setStyle(int id, JsonObject style) {
    return _invokeMethod('style', [id, style]);
  }
}

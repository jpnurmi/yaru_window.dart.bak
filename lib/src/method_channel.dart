import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'platform_interface.dart';

class YaruWindowMethodChannel extends YaruWindowPlatform {
  @visibleForTesting
  final channel = const MethodChannel('yaru_window');
  @visibleForTesting
  final events = const EventChannel('yaru_window/state');
  @override
  Future<void> close(int id) => channel.invokeMethod('close', id);
  @override
  Future<void> destroy(int id) => channel.invokeMethod('destroy', id);
  @override
  Future<void> fullscreen(int id) => channel.invokeMethod('fullscreen', id);
  @override
  Future<void> hide(int id) => channel.invokeMethod('hide', id);
  @override
  Future<void> init(int id) => channel.invokeMethod('init', id);
  @override
  Future<void> maximize(int id) => channel.invokeMethod('maximize', id);
  @override
  Future<void> menu(int id) => channel.invokeMethod('menu', id);
  @override
  Future<void> minimize(int id) => channel.invokeMethod('minimize', id);
  @override
  Future<void> move(int id) => channel.invokeMethod('move', id);
  @override
  Future<void> restore(int id) => channel.invokeMethod('restore', id);
  @override
  Future<void> show(int id) => channel.invokeMethod('show', id);
  @override
  Stream<Map> state(int id) => events.receiveBroadcastStream(id).cast<Map>();
}

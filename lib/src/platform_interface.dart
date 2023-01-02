import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'method_channel.dart';

typedef JsonArray = List<dynamic>;
typedef JsonObject = Map<String, dynamic>;

abstract class YaruWindowPlatform extends PlatformInterface {
  YaruWindowPlatform() : super(token: _token);

  static final Object _token = Object();

  static YaruWindowPlatform get instance => _instance;
  static YaruWindowPlatform _instance = YaruWindowMethodChannel();
  static set instance(YaruWindowPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<void> close(int id) => throw UnimplementedError('close');
  Future<void> destroy(int id) => throw UnimplementedError('destroy');
  Future<void> fullscreen(int id) => throw UnimplementedError('fullscreen');
  Future<JsonObject> geometry(int id) => throw UnimplementedError('geometry');
  Future<void> hide(int id) => throw UnimplementedError('hide');
  Future<void> init(int id) => throw UnimplementedError('init');
  Future<void> maximize(int id) => throw UnimplementedError('maximize');
  Future<void> menu(int id) => throw UnimplementedError('menu');
  Future<void> minimize(int id) => throw UnimplementedError('minimize');
  Future<void> move(int id) => throw UnimplementedError('move');
  Future<void> restore(int id) => throw UnimplementedError('restore');
  Future<void> show(int id) => throw UnimplementedError('show');
  Future<JsonObject> state(int id) => throw UnimplementedError('state');
  Future<JsonObject> style(int id) => throw UnimplementedError('style');

  Stream<JsonObject> geometries(int id) {
    throw UnimplementedError('geometryChanged');
  }

  Stream<JsonObject> states(int id) {
    throw UnimplementedError('stateChanged');
  }

  Future<void> setGeometry(int id, JsonObject geometry) {
    throw UnimplementedError('setGeometry');
  }

  Future<void> setState(int id, JsonObject state) {
    throw UnimplementedError('setState');
  }

  Future<void> setStyle(int id, JsonObject style) {
    throw UnimplementedError('setStyle');
  }
}

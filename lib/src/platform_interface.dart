import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'method_channel.dart';

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
  Future<void> hide(int id) => throw UnimplementedError('hide');
  Future<void> init(int id) => throw UnimplementedError('init');
  Future<void> maximize(int id) => throw UnimplementedError('maximize');
  Future<void> menu(int id) => throw UnimplementedError('menu');
  Future<void> minimize(int id) => throw UnimplementedError('minimize');
  Future<void> move(int id) => throw UnimplementedError('move');
  Future<void> restore(int id) => throw UnimplementedError('restore');
  Future<void> show(int id) => throw UnimplementedError('show');
  Stream<Map> state(int id) => throw UnimplementedError('state');
  Future<void> setState(int id, Map state) =>
      throw UnimplementedError('setState');
}

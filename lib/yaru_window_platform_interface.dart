import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'yaru_window_method_channel.dart';

abstract class YaruWindowPlatform extends PlatformInterface {
  /// Constructs a YaruWindowPlatform.
  YaruWindowPlatform() : super(token: _token);

  static final Object _token = Object();

  static YaruWindowPlatform _instance = MethodChannelYaruWindow();

  /// The default instance of [YaruWindowPlatform] to use.
  ///
  /// Defaults to [MethodChannelYaruWindow].
  static YaruWindowPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [YaruWindowPlatform] when
  /// they register themselves.
  static set instance(YaruWindowPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}

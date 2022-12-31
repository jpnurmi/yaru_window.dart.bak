import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'yaru_window_platform_interface.dart';

/// An implementation of [YaruWindowPlatform] that uses method channels.
class MethodChannelYaruWindow extends YaruWindowPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('yaru_window');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}

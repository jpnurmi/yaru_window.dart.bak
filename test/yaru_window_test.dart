import 'package:flutter_test/flutter_test.dart';
import 'package:yaru_window/yaru_window.dart';
import 'package:yaru_window/src/platform_interface.dart';
import 'package:yaru_window/src/method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockYaruWindowPlatform
    with MockPlatformInterfaceMixin
    implements YaruWindowPlatform {
  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final YaruWindowPlatform initialPlatform = YaruWindowPlatform.instance;

  test('$MethodChannelYaruWindow is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelYaruWindow>());
  });

  test('getPlatformVersion', () async {
    YaruWindow yaruWindowPlugin = YaruWindow();
    MockYaruWindowPlatform fakePlatform = MockYaruWindowPlatform();
    YaruWindowPlatform.instance = fakePlatform;

    expect(await yaruWindowPlugin.getPlatformVersion(), '42');
  });
}

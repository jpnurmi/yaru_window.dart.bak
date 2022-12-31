import 'src/platform_interface.dart';

class YaruWindow {
  Future<String?> getPlatformVersion() {
    return YaruWindowPlatform.instance.getPlatformVersion();
  }
}

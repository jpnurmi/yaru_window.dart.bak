import 'package:flutter/widgets.dart';

import 'platform_interface.dart';
import 'window_state.dart';

class YaruWindow {
  const YaruWindow._(this._id);
  factory YaruWindow([int id = 0]) => _windows[id] ??= YaruWindow._(id);

  final int _id;
  static final Map<int, YaruWindow> _windows = {};

  static YaruWindow of(BuildContext context) {
    const id = 0; // View.of(context).windowId;
    return YaruWindow(id);
  }

  Future<void> close() => YaruWindowPlatform.instance.close(_id);
  Future<void> destroy() => YaruWindowPlatform.instance.destroy(_id);
  Future<void> fullscreen() => YaruWindowPlatform.instance.fullscreen(_id);
  Future<void> hide() => YaruWindowPlatform.instance.hide(_id);
  Future<void> maximize() => YaruWindowPlatform.instance.maximize(_id);
  Future<void> menu() => YaruWindowPlatform.instance.menu(_id);
  Future<void> minimize() => YaruWindowPlatform.instance.minimize(_id);
  Future<void> move() => YaruWindowPlatform.instance.move(_id);
  Future<void> restore() => YaruWindowPlatform.instance.restore(_id);
  Future<void> show() => YaruWindowPlatform.instance.show(_id);
  Stream<YaruWindowState> state() => YaruWindowPlatform.instance
      .state(_id)
      .map((json) => YaruWindowState.fromJson(json.cast<String, dynamic>()));
  Future<void> setState(YaruWindowState state) =>
      YaruWindowPlatform.instance.setState(_id, state.toJson());
}

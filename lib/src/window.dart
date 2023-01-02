import 'package:flutter/widgets.dart';

import 'geometry.dart';
import 'platform_interface.dart';
import 'state.dart';
import 'style.dart';

class YaruWindow {
  const YaruWindow._(this._id);
  factory YaruWindow([int id = 0]) => _windows[id] ??= YaruWindow._(id);

  final int _id;
  static final Map<int, YaruWindow> _windows = {};

  static YaruWindow of(BuildContext context) {
    const id = 0; // View.of(context).windowId;
    return YaruWindow(id);
  }

  YaruWindowPlatform get _platform => YaruWindowPlatform.instance;

  Future<void> close() => _platform.close(_id);
  Future<void> destroy() => _platform.destroy(_id);
  Future<void> fullscreen() => _platform.fullscreen(_id);
  Future<YaruWindowGeometry> geometry() =>
      _platform.geometry(_id).then(YaruWindowGeometry.fromJson);
  Future<void> hide() => _platform.hide(_id);
  Future<void> maximize() => _platform.maximize(_id);
  Future<void> menu() => _platform.menu(_id);
  Future<void> minimize() => _platform.minimize(_id);
  Future<void> move() => _platform.move(_id);
  Future<void> restore() => _platform.restore(_id);
  Future<void> show() => _platform.show(_id);
  Future<YaruWindowState> state() =>
      _platform.state(_id).then(YaruWindowState.fromJson);
  Future<YaruWindowStyle> style() =>
      _platform.style(_id).then(YaruWindowStyle.fromJson);

  Stream<YaruWindowGeometry> geometries() async* {
    yield await geometry();
    yield* _platform.geometries(_id).map(YaruWindowGeometry.fromJson);
  }

  Stream<YaruWindowState> states() async* {
    yield await state();
    yield* _platform.states(_id).map(YaruWindowState.fromJson);
  }

  Future<void> setGeometry(YaruWindowGeometry geometry) {
    return _platform.setState(_id, geometry.toJson());
  }

  Future<void> setState(YaruWindowState state) {
    return _platform.setState(_id, state.toJson());
  }

  Future<void> setStyle(YaruWindowStyle style) {
    return _platform.setStyle(_id, style.toJson());
  }
}

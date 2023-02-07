import 'package:flutter/widgets.dart';

import 'geometry.dart';
import 'platform_interface.dart';
import 'state.dart';
import 'style.dart';

class YaruWindow {
  static final Map<int, YaruWindowInstance> _windows = {};

  static YaruWindowInstance of(BuildContext context) {
    const id = 0; // View.of(context).windowId;
    return _windows[id] ??= const YaruWindowInstance._(id);
  }

  static Future<void> close(BuildContext context) {
    return YaruWindow.of(context).close();
  }

  static Future<void> destroy(BuildContext context) {
    return YaruWindow.of(context).destroy();
  }

  static Future<void> drag(BuildContext context) {
    return YaruWindow.of(context).drag();
  }

  static Future<void> fullscreen(BuildContext context) {
    return YaruWindow.of(context).fullscreen();
  }

  static Future<YaruWindowGeometry> geometry(BuildContext context) {
    return YaruWindow.of(context).geometry();
  }

  static Future<void> hide(BuildContext context) {
    return YaruWindow.of(context).hide();
  }

  static Future<void> hideTitle(BuildContext context) {
    return YaruWindow.of(context).hideTitle();
  }

  static Future<void> maximize(BuildContext context) {
    return YaruWindow.of(context).maximize();
  }

  static Future<void> minimize(BuildContext context) {
    return YaruWindow.of(context).minimize();
  }

  static Future<void> restore(BuildContext context) {
    return YaruWindow.of(context).restore();
  }

  static Future<void> show(BuildContext context) {
    return YaruWindow.of(context).show();
  }

  static Future<void> showMenu(BuildContext context) {
    return YaruWindow.of(context).showMenu();
  }

  static Future<YaruWindowState> state(BuildContext context) {
    return YaruWindow.of(context).state();
  }

  static Future<YaruWindowStyle> style(BuildContext context) {
    return YaruWindow.of(context).style();
  }

  static Stream<YaruWindowGeometry> geometries(BuildContext context) {
    return YaruWindow.of(context).geometries();
  }

  static Stream<YaruWindowState> states(BuildContext context) {
    return YaruWindow.of(context).states();
  }

  static Future<void> setGeometry(
      BuildContext context, YaruWindowGeometry geometry) {
    return YaruWindow.of(context).setGeometry(geometry);
  }

  static Future<void> setState(BuildContext context, YaruWindowState state) {
    return YaruWindow.of(context).setState(state);
  }

  static Future<void> setClosable(BuildContext context, bool closable) {
    return YaruWindow.of(context).setClosable(closable);
  }

  static Future<void> setTitle(BuildContext context, String title) {
    return YaruWindow.of(context).setTitle(title);
  }

  static Future<void> setStyle(BuildContext context, YaruWindowStyle style) {
    return YaruWindow.of(context).setStyle(style);
  }

  static Future<void> setBackground(BuildContext context, Color background) {
    return YaruWindow.of(context).setBackground(background);
  }
}

class YaruWindowInstance {
  const YaruWindowInstance._(this._id);

  final int _id;

  YaruWindowPlatform get _platform => YaruWindowPlatform.instance;

  Future<void> close() => _platform.close(_id);
  Future<void> destroy() => _platform.destroy(_id);
  Future<void> drag() => _platform.drag(_id);
  Future<void> fullscreen() => _platform.fullscreen(_id);
  Future<YaruWindowGeometry> geometry() =>
      _platform.geometry(_id).then(YaruWindowGeometry.fromJson);
  Future<void> hide() => _platform.hide(_id);
  Future<void> hideTitle() => _platform.hideTitle(_id);
  Future<void> maximize() => _platform.maximize(_id);
  Future<void> minimize() => _platform.minimize(_id);
  Future<void> restore() => _platform.restore(_id);
  Future<void> show() => _platform.show(_id);
  Future<void> showMenu() => _platform.showMenu(_id);
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

  Future<void> setClosable(bool closable) {
    return setState(YaruWindowState(closable: closable));
  }

  Future<void> setTitle(String title) {
    return setState(YaruWindowState(title: title));
  }

  Future<void> setStyle(YaruWindowStyle style) {
    return _platform.setStyle(_id, style.toJson());
  }

  Future<void> setBackground(Color background) {
    return setStyle(YaruWindowStyle(background: background));
  }
}

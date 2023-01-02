import 'package:meta/meta.dart';

@immutable
class YaruWindowState {
  const YaruWindowState({
    this.active,
    this.closable,
    this.fullscreen,
    this.maximizable,
    this.maximized,
    this.minimizable,
    this.minimized,
    this.restorable,
    this.visible,
  });

  factory YaruWindowState.fromJson(Map<String, dynamic> json) {
    return YaruWindowState(
      active: json['active'],
      closable: json['closable'],
      fullscreen: json['fullscreen'],
      maximizable: json['maximizable'],
      maximized: json['maximized'],
      minimizable: json['minimizable'],
      minimized: json['minimized'],
      restorable: json['restorable'],
      visible: json['visible'],
    );
  }

  final bool? active;
  final bool? closable;
  final bool? fullscreen;
  final bool? maximizable;
  final bool? maximized;
  final bool? minimizable;
  final bool? minimized;
  final bool? restorable;
  final bool? visible;

  Map<String, dynamic> toJson() {
    return {
      'active': active,
      'closable': closable,
      'fullscreen': fullscreen,
      'maximizable': maximizable,
      'maximized': maximized,
      'minimizable': minimizable,
      'minimized': minimized,
      'restorable': restorable,
      'visible': visible,
    };
  }

  YaruWindowState copyWith({
    bool? active,
    bool? closable,
    bool? fullscreen,
    bool? maximizable,
    bool? maximized,
    bool? minimizable,
    bool? minimized,
    bool? restorable,
    bool? visible,
  }) {
    return YaruWindowState(
      active: active ?? this.active,
      closable: closable ?? this.closable,
      fullscreen: fullscreen ?? this.fullscreen,
      maximizable: maximizable ?? this.maximizable,
      maximized: maximized ?? this.maximized,
      minimizable: minimizable ?? this.minimizable,
      minimized: minimized ?? this.minimized,
      restorable: restorable ?? this.restorable,
      visible: visible ?? this.visible,
    );
  }

  YaruWindowState merge(YaruWindowState? other) {
    return copyWith(
      active: other?.active,
      closable: other?.closable,
      fullscreen: other?.fullscreen,
      maximizable: other?.maximizable,
      maximized: other?.maximized,
      minimizable: other?.minimizable,
      minimized: other?.minimized,
      restorable: other?.restorable,
      visible: other?.visible,
    );
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;
    return other is YaruWindowState &&
        other.active == active &&
        other.closable == closable &&
        other.fullscreen == fullscreen &&
        other.maximizable == maximizable &&
        other.maximized == maximized &&
        other.minimizable == minimizable &&
        other.minimized == minimized &&
        other.restorable == restorable &&
        other.visible == visible;
  }

  @override
  int get hashCode {
    return Object.hash(
      active,
      closable,
      fullscreen,
      maximizable,
      maximized,
      minimizable,
      minimized,
      restorable,
      visible,
    );
  }

  @override
  String toString() {
    return 'YaruWindowState(active: $active, closable: $closable, fullscreen: $fullscreen, maximizable: $maximizable, maximized: $maximized, minimizable: $minimizable, minimized: $minimized, restorable: $restorable, visible: $visible)';
  }
}

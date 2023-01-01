import 'package:meta/meta.dart';

@immutable
class YaruWindowState {
  const YaruWindowState({
    this.isActive,
    this.isClosable,
    this.isFullscreen,
    this.isMaximizable,
    this.isMaximized,
    this.isMinimizable,
    this.isMinimized,
    this.isRestorable,
    this.isVisible,
  });

  factory YaruWindowState.fromJson(Map<String, dynamic> json) {
    return YaruWindowState(
      isActive: json['active'] == true,
      isClosable: json['closable'] == true,
      isFullscreen: json['fullscreen'] == true,
      isMaximizable: json['maximizable'] == true,
      isMaximized: json['maximized'] == true,
      isMinimizable: json['minimizable'] == true,
      isMinimized: json['minimized'] == true,
      isRestorable: json['restorable'] == true,
      isVisible: json['visible'] == true,
    );
  }

  final bool? isActive;
  final bool? isClosable;
  final bool? isFullscreen;
  final bool? isMaximizable;
  final bool? isMaximized;
  final bool? isMinimizable;
  final bool? isMinimized;
  final bool? isRestorable;
  final bool? isVisible;

  Map<String, dynamic> toJson() {
    return {
      'active': isActive,
      'closable': isClosable,
      'fullscreen': isFullscreen,
      'maximizable': isMaximizable,
      'maximized': isMaximized,
      'minimizable': isMinimizable,
      'minimized': isMinimized,
      'restorable': isRestorable,
      'visible': isVisible,
    };
  }

  YaruWindowState copyWith({
    bool? isActive,
    bool? isClosable,
    bool? isFullscreen,
    bool? isMaximizable,
    bool? isMaximized,
    bool? isMinimizable,
    bool? isMinimized,
    bool? isRestorable,
    bool? isVisible,
  }) {
    return YaruWindowState(
      isActive: isActive ?? this.isActive,
      isClosable: isClosable ?? this.isClosable,
      isFullscreen: isFullscreen ?? this.isFullscreen,
      isMaximizable: isMaximizable ?? this.isMaximizable,
      isMaximized: isMaximized ?? this.isMaximized,
      isMinimizable: isMinimizable ?? this.isMinimizable,
      isMinimized: isMinimized ?? this.isMinimized,
      isRestorable: isRestorable ?? this.isRestorable,
      isVisible: isVisible ?? this.isVisible,
    );
  }

  YaruWindowState merge(YaruWindowState? other) {
    return copyWith(
      isActive: other?.isActive,
      isClosable: other?.isClosable,
      isFullscreen: other?.isFullscreen,
      isMaximizable: other?.isMaximizable,
      isMaximized: other?.isMaximized,
      isMinimizable: other?.isMinimizable,
      isMinimized: other?.isMinimized,
      isRestorable: other?.isRestorable,
      isVisible: other?.isVisible,
    );
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;
    return other is YaruWindowState &&
        other.isActive == isActive &&
        other.isClosable == isClosable &&
        other.isFullscreen == isFullscreen &&
        other.isMaximizable == isMaximizable &&
        other.isMaximized == isMaximized &&
        other.isMinimizable == isMinimizable &&
        other.isMinimized == isMinimized &&
        other.isRestorable == isRestorable &&
        other.isVisible == isVisible;
  }

  @override
  int get hashCode {
    return Object.hash(
      isActive,
      isClosable,
      isFullscreen,
      isMaximizable,
      isMaximized,
      isMinimizable,
      isMinimized,
      isRestorable,
      isVisible,
    );
  }

  @override
  String toString() {
    return 'YaruWindowState(isActive: $isActive, isClosable: $isClosable, isFullscreen: $isFullscreen, isMaximizable: $isMaximizable, isMaximized: $isMaximized, isMinimizable: $isMinimizable, isMinimized: $isMinimized, isRestorable: $isRestorable, isVisible: $isVisible)';
  }
}

import 'dart:ui';

import 'package:meta/meta.dart';

@immutable
class YaruWindowStyle {
  const YaruWindowStyle({
    this.background,
    this.opacity,
  });

  factory YaruWindowStyle.fromJson(Map<String, dynamic> json) {
    Color? toColor(int? value) => value == null ? null : Color(value);
    return YaruWindowStyle(
      background: toColor(json['background']),
      opacity: json['opacity'],
    );
  }

  final Color? background;
  final double? opacity;

  Map<String, dynamic> toJson() {
    return {
      'background': background?.value,
      'opacity': opacity,
    };
  }

  YaruWindowStyle copyWith({
    Color? background,
    double? opacity,
  }) {
    return YaruWindowStyle(
      background: background ?? this.background,
      opacity: opacity ?? this.opacity,
    );
  }

  YaruWindowStyle merge(YaruWindowStyle? other) {
    return copyWith(
      background: other?.background,
      opacity: other?.opacity,
    );
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;
    return other is YaruWindowStyle &&
        other.background == background &&
        other.opacity == opacity;
  }

  @override
  int get hashCode {
    return Object.hash(
      background,
      opacity,
    );
  }

  @override
  String toString() {
    return 'YaruWindowStyle(background: $background, opacity: $opacity)';
  }
}

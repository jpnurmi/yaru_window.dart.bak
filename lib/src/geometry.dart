import 'package:meta/meta.dart';

@immutable
class YaruWindowGeometry {
  const YaruWindowGeometry({
    this.x,
    this.y,
    this.width,
    this.height,
    this.maximumWidth,
    this.maximumHeight,
    this.minimumWidth,
    this.minimumHeight,
  });

  factory YaruWindowGeometry.fromJson(Map<String, dynamic> json) {
    return YaruWindowGeometry(
      x: json['x'],
      y: json['y'],
      width: json['width'],
      height: json['height'],
      maximumWidth: json['maximumWidth'],
      maximumHeight: json['maximumHeight'],
      minimumWidth: json['minimumWidth'],
      minimumHeight: json['minimumHeight'],
    );
  }

  final int? x;
  final int? y;
  final int? width;
  final int? height;

  final int? maximumWidth;
  final int? maximumHeight;

  final int? minimumWidth;
  final int? minimumHeight;

  Map<String, dynamic> toJson() {
    return {
      'x': x,
      'y': y,
      'width': width,
      'height': height,
      'maximumWidth': maximumWidth,
      'maximumHeight': maximumHeight,
      'minimumWidth': minimumWidth,
      'minimumHeight': minimumHeight,
    };
  }

  YaruWindowGeometry copyWith({
    int? x,
    int? y,
    int? width,
    int? height,
    int? maximumWidth,
    int? maximumHeight,
    int? minimumWidth,
    int? minimumHeight,
  }) {
    return YaruWindowGeometry(
      x: x ?? this.x,
      y: y ?? this.y,
      width: width ?? this.width,
      height: height ?? this.height,
      maximumWidth: maximumWidth ?? this.maximumWidth,
      maximumHeight: maximumHeight ?? this.maximumHeight,
      minimumWidth: minimumWidth ?? this.minimumWidth,
      minimumHeight: minimumHeight ?? this.minimumHeight,
    );
  }

  YaruWindowGeometry merge(YaruWindowGeometry? other) {
    return copyWith(
      x: other?.x,
      y: other?.y,
      width: other?.width,
      height: other?.height,
      maximumWidth: other?.maximumWidth,
      maximumHeight: other?.maximumHeight,
      minimumWidth: other?.minimumWidth,
      minimumHeight: other?.minimumHeight,
    );
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;
    return other is YaruWindowGeometry &&
        other.x == x &&
        other.y == y &&
        other.width == width &&
        other.height == height &&
        other.maximumWidth == maximumWidth &&
        other.maximumHeight == maximumHeight &&
        other.minimumWidth == minimumWidth &&
        other.minimumHeight == minimumHeight;
  }

  @override
  int get hashCode {
    return Object.hash(
      x,
      y,
      width,
      height,
      maximumWidth,
      maximumHeight,
      minimumWidth,
      minimumHeight,
    );
  }

  @override
  String toString() {
    return 'YaruWindowGeometry(x: $x, y: $y, width: $width, height: $height, maximumWidth: $maximumWidth, maximumHeight: $maximumHeight, minimumWidth: $minimumWidth, minimumHeight: $minimumHeight)';
  }
}

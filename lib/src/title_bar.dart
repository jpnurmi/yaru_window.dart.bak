import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/widgets.dart';
import 'package:yaru_widgets/yaru_widgets.dart';

import 'window.dart';
import 'state.dart';

class YaruWindowTitleBar extends StatefulWidget implements PreferredSizeWidget {
  const YaruWindowTitleBar({
    super.key,
    this.leading,
    this.title,
    this.trailing,
    this.centerTitle,
    this.backgroundColor,
  });

  /// The primary title widget.
  final Widget? title;

  /// A widget to display before the [title] widget.
  final Widget? leading;

  /// A widget to display after the [title] widget.
  final Widget? trailing;

  /// Whether the title should be centered.
  final bool? centerTitle;

  /// The background color. Defaults to [Colors.transparent].
  final Color? backgroundColor;

  @override
  Size get preferredSize => const Size(0, kIsWeb ? 0 : kYaruTitleBarHeight);

  @override
  State<YaruWindowTitleBar> createState() => _YaruWindowTitleBarState();
}

class _YaruWindowTitleBarState extends State<YaruWindowTitleBar> {
  YaruWindowState get defaultState => Platform.isMacOS
      ? const YaruWindowState(
          closable: false,
          maximizable: false,
          minimizable: false,
        )
      : const YaruWindowState();

  void onClose() => YaruWindow.of(context).close();
  void onDrag() => YaruWindow.of(context).drag();
  void onMaximize() => YaruWindow.of(context).maximize();
  void onMenu() => YaruWindow.of(context).menu();
  void onMinimize() => YaruWindow.of(context).minimize();
  void onRestore() => YaruWindow.of(context).restore();

  @override
  Widget build(BuildContext context) {
    if (kIsWeb) return const SizedBox.shrink();
    final window = YaruWindow.of(context);
    return StreamBuilder<YaruWindowState>(
      stream: window.states(),
      builder: (context, snapshot) {
        final state = snapshot.data?.merge(defaultState) ?? defaultState;
        return YaruTitleBar(
          leading: widget.leading,
          title: widget.title,
          trailing: widget.trailing,
          centerTitle: widget.centerTitle,
          backgroundColor: widget.backgroundColor,
          isActive: state.active,
          isClosable: state.closable,
          isMaximizable: state.maximizable,
          isMinimizable: state.minimizable,
          isRestorable: state.restorable,
          onClose: (_) => onClose(),
          onMove: (_) => onDrag(),
          onMaximize: (_) => onMaximize(),
          onMinimize: (_) => onMinimize(),
          onRestore: (_) => onRestore(),
          onShowMenu: (_) => onMenu(),
        );
      },
    );
  }
}

class YaruDialogTitleBar extends YaruWindowTitleBar {
  const YaruDialogTitleBar({
    super.key,
    super.leading,
    super.title,
    super.trailing,
    super.centerTitle,
    super.backgroundColor,
  });

  @override
  State<YaruWindowTitleBar> createState() => _YaruDialogTitleBarState();
}

class _YaruDialogTitleBarState extends _YaruWindowTitleBarState {
  @override
  YaruWindowState get defaultState =>
      const YaruWindowState(maximizable: false, minimizable: false);

  @override
  void onClose() => Navigator.of(context).maybePop();
}

import 'package:flex_color_picker/flex_color_picker.dart';
import 'package:flutter/material.dart';
import 'package:yaru/yaru.dart';
import 'package:yaru_widgets/yaru_widgets.dart' hide YaruWindow;
import 'package:yaru_window/yaru_window.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    final window = YaruWindow.of(context);
    return YaruTheme(
      builder: (context, yaru, child) => MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: yaru.theme,
        darkTheme: yaru.darkTheme,
        home: StreamBuilder<YaruWindowState>(
          stream: window.states(),
          builder: (context, snapshot) {
            final state = snapshot.data;
            print(state);
            return const Scaffold(
              backgroundColor: Colors.transparent,
              appBar: YaruWindowTitleBar(),
              body: ColorSelector(),
            );
          },
        ),
      ),
    );
  }
}

class ColorSelector extends StatefulWidget {
  const ColorSelector({super.key});

  @override
  State<ColorSelector> createState() => _ColorSelectorState();
}

class _ColorSelectorState extends State<ColorSelector> {
  Color? _color;

  @override
  Widget build(BuildContext context) {
    _color ??= Theme.of(context).colorScheme.background;
    return Column(
      children: [
        const Spacer(),
        ColorPicker(
          color: _color!,
          enableShadesSelection: false,
          pickersEnabled: const {
            ColorPickerType.accent: false,
            ColorPickerType.primary: false,
            ColorPickerType.custom: false,
            ColorPickerType.wheel: true,
          },
          onColorChanged: (color) {
            setState(() => _color = color.withOpacity(_color!.opacity));
            YaruWindow.of(context)
                .setStyle(YaruWindowStyle(background: _color));
          },
        ),
        const Spacer(),
        Slider(
          value: 1.0,
          onChanged: (value) {
            setState(() => _color = _color!.withOpacity(value));
            YaruWindow.of(context)
                .setStyle(YaruWindowStyle(background: _color));
          },
        ),
        const Spacer(),
      ],
    );
  }
}

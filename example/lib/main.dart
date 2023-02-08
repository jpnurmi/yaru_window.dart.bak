import 'package:flex_color_picker/flex_color_picker.dart';
import 'package:flutter/material.dart';
import 'package:yaru/yaru.dart';
import 'package:yaru_window/yaru_window.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return YaruTheme(
      builder: (context, yaru, child) => MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: yaru.theme,
        darkTheme: yaru.darkTheme,
        home: Scaffold(
          backgroundColor: Colors.transparent,
          body: Row(
            children: const [
              Expanded(flex: 3, child: ColorSelector()),
              Expanded(flex: 2, child: StateView()),
            ],
          ),
        ),
      ),
    );
  }
}

class StateView extends StatelessWidget {
  const StateView({super.key});

  @override
  Widget build(BuildContext context) {
    final window = YaruWindow.of(context);
    return StreamBuilder<YaruWindowState>(
      stream: window.states(),
      builder: (context, snapshot) {
        final state = snapshot.data;
        print(state);
        return ListView(
          padding: const EdgeInsets.all(20),
          children: [
            ListTile(
              title: const Text('Active'),
              subtitle: Text('${state?.isActive}'),
            ),
            ListTile(
              title: const Text('Closable'),
              subtitle: Text('${state?.isClosable}'),
              trailing: ElevatedButton(
                onPressed: state?.isClosable == true ? window.close : null,
                child: const Text('Close'),
              ),
            ),
            ListTile(
              title: const Text('Fullscreen'),
              subtitle: Text('${state?.isFullscreen}'),
              trailing: ElevatedButton(
                onPressed:
                    state?.isFullscreen != true ? window.fullscreen : null,
                child: const Text('Fullscreen'),
              ),
            ),
            ListTile(
              title: Text(
                  'Maximizable ${state?.isMaximized == true ? '(maximized)' : ''}'),
              subtitle: Text('${state?.isMaximizable}'),
              trailing: ElevatedButton(
                onPressed:
                    state?.isMaximizable == true ? window.maximize : null,
                child: const Text('Maximize'),
              ),
            ),
            ListTile(
              title: Text(
                  'Minimizable ${state?.isMinimized == true ? '(minimized)' : ''}'),
              subtitle: Text('${state?.isMinimizable}'),
              trailing: ElevatedButton(
                onPressed:
                    state?.isMinimizable == true ? window.minimize : null,
                child: const Text('Minimize'),
              ),
            ),
            ListTile(
              title: const Text('Movable'),
              subtitle: Text('${state?.isMovable}'),
              trailing: ElevatedButton(
                onPressed: state?.isMovable == true ? window.drag : null,
                child: const Text('Drag'),
              ),
            ),
            ListTile(
              title: const Text('Restorable'),
              subtitle: Text('${state?.isRestorable}'),
              trailing: ElevatedButton(
                onPressed: state?.isRestorable == true ? window.restore : null,
                child: const Text('Restore'),
              ),
            ),
            ListTile(
              title: const Text('Title'),
              subtitle: Text('${state?.title}'),
              trailing: ElevatedButton(
                onPressed: () {
                  showDialog(
                    context: context,
                    builder: (context) => AlertDialog(
                      title: const Text('Title'),
                      content: TextFormField(
                        autofocus: true,
                        initialValue: state?.title,
                        onFieldSubmitted: Navigator.of(context).pop,
                        onChanged: window.setTitle,
                      ),
                    ),
                  );
                },
                child: const Text('Edit'),
              ),
            ),
            ListTile(
              title: const Text('Visible'),
              subtitle: Text('${state?.isVisible}'),
              trailing: ElevatedButton(
                onPressed: () => window
                    .hide()
                    .then((_) => Future.delayed(const Duration(seconds: 2)))
                    .then((_) => window.show()),
                child: const Text('Hide & Show'),
              ),
            ),
          ],
        );
      },
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
        const Padding(
          padding: EdgeInsets.all(20),
          child: Align(
            alignment: Alignment.topLeft,
            child: Text('Color'),
          ),
        ),
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
            YaruWindow.of(context).setBackground(_color!);
          },
        ),
        const Spacer(),
        const Padding(
          padding: EdgeInsets.all(20),
          child: Align(
            alignment: Alignment.topLeft,
            child: Text('Opacity'),
          ),
        ),
        Slider(
          value: _color!.opacity,
          onChanged: (value) {
            setState(() => _color = _color!.withOpacity(value));
            YaruWindow.of(context).setBackground(_color!);
          },
        ),
        const Spacer(),
      ],
    );
  }
}

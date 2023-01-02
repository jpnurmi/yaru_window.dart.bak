import 'package:flutter/material.dart';
import 'package:yaru/yaru.dart';
import 'package:yaru_widgets/yaru_widgets.dart';
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
        home: Scaffold(
          appBar: const YaruWindowTitleBar(title: Text('YaruWindow')),
          body: StreamBuilder<YaruWindowState>(
            stream: window.state(),
            builder: (context, snapshot) {
              final state = snapshot.data;
              print(state);
              return ListView(
                children: [
                  YaruCheckboxListTile(
                    title: const Text('Active'),
                    value: state?.isActive == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Closable'),
                    value: state?.isClosable == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Fullscreen'),
                    value: state?.isFullscreen == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Maximizable'),
                    value: state?.isMaximizable == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Maximized'),
                    value: state?.isMaximized == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Minimizable'),
                    value: state?.isMinimizable == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Minimized'),
                    value: state?.isMinimized == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Restorable'),
                    value: state?.isRestorable == true,
                    onChanged: null,
                  ),
                  YaruCheckboxListTile(
                    title: const Text('Visible'),
                    value: state?.isVisible == true,
                    onChanged: null,
                  ),
                ],
              );
            },
          ),
        ),
      ),
    );
  }
}

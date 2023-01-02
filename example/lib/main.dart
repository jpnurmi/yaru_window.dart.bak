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
          body: StreamBuilder<YaruWindowState>(
            stream: window.states(),
            builder: (context, snapshot) {
              final state = snapshot.data;
              print(state);
              return Scaffold(
                appBar: YaruWindowTitleBar(title: Text(state?.title ?? '')),
                body: ListView(
                  children: [
                    YaruCheckboxListTile(
                      title: const Text('Active'),
                      value: state?.active == true,
                      onChanged: null,
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Closable'),
                      value: state?.closable == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(closable: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Fullscreen'),
                      value: state?.fullscreen == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(fullscreen: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Maximizable'),
                      value: state?.maximizable == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(maximizable: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Maximized'),
                      value: state?.maximized == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(maximized: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Minimizable'),
                      value: state?.minimizable == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(minimizable: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Minimized'),
                      value: state?.minimized == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(minimized: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Restorable'),
                      value: state?.restorable == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(restorable: value),
                      ),
                    ),
                    YaruCheckboxListTile(
                      title: const Text('Visible'),
                      value: state?.visible == true,
                      onChanged: (value) => window.setState(
                        YaruWindowState(visible: value),
                      ),
                    ),
                  ],
                ),
              );
            },
          ),
        ),
      ),
    );
  }
}

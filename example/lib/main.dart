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
        theme: yaru.theme,
        darkTheme: yaru.darkTheme,
        home: Scaffold(
          appBar: AppBar(title: const Text('YaruWindow')),
          body: StreamBuilder(
            stream: YaruWindow.of(context).state(),
            builder: (context, snapshot) => Text('${snapshot.data}'),
          ),
        ),
      ),
    );
  }
}

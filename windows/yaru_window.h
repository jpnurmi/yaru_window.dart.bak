#ifndef _YARU_WINDOW_H_
#define _YARU_WINDOW_H_

#include <windows.h>

enum Brightness { Light, Dark };

class YaruWindow {
 public:
  YaruWindow(HWND hwnd);

  void Init();

  bool IsActive() const;

  bool IsClosable() const;
  void SetClosable(bool closable = true);

  bool IsVisible() const;
  void Show();
  void Hide();

  bool IsMinimized() const;
  bool IsMinimizable() const;
  void Minimize();

  bool IsMaximized() const;
  bool IsMaximizable() const;
  void Maximize();

  bool IsFullscreen() const;
  void SetFullscreen(bool fullscreen);

  bool IsRestorable() const;
  void Restore();

  void Menu();
  void Move();

  void Close();
  void Destroy();

 private:
  HWND hwnd = nullptr;
  WINDOWPLACEMENT placement = {sizeof(WINDOWPLACEMENT)};
};

#endif  // _YARU_WINDOW_H_
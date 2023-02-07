#ifndef _YARU_WINDOW_H_
#define _YARU_WINDOW_H_

#include "yaru_flutter.h"

enum Brightness { Light, Dark };

class YaruWindow {
 public:
  YaruWindow(HWND hwnd);

  bool IsActive() const;
  void Activate(bool active = true);

  bool IsClosable() const;
  void SetClosable(bool closable = true);

  bool IsVisible() const;
  void SetVisible(bool visible = true);
  void Show();
  void Hide();

  bool IsMinimized() const;
  void Minimize(bool minimize = true);

  bool IsMinimizable() const;
  void SetMinimizable(bool minimizable);

  bool IsMaximized() const;
  void Maximize(bool maximize = true);

  bool IsMaximizable() const;
  void SetMaximizable(bool maximizable);

  bool IsFullscreen() const;
  void SetFullscreen(bool fullscreen);

  bool IsRestorable() const;
  void Restore();

  void Drag();
  void HideTitle();
  void ShowMenu();

  void Close();
  void Destroy();

  std::map<FlValue, FlValue> GetGeometry() const;
  void SetGeometry(const std::map<FlValue, FlValue>& geometry);

  std::map<FlValue, FlValue> GetState() const;
  void SetState(const std::map<FlValue, FlValue>& state);

 private:
  HWND hwnd = nullptr;
  WINDOWPLACEMENT placement = {sizeof(WINDOWPLACEMENT)};
};

#endif  // _YARU_WINDOW_H_

#include "yaru_window.h"

#include <dwmapi.h>

YaruWindow::YaruWindow(HWND hwnd) : hwnd(hwnd) {}

bool YaruWindow::IsActive() const { return ::GetForegroundWindow() == hwnd; }

void YaruWindow::Activate(bool active) {
  if (active) {
    ::SetForegroundWindow(hwnd);
  } else {
    ::SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  }
}

bool YaruWindow::IsClosable() const {
  HMENU menu = ::GetSystemMenu(hwnd, false);
  MENUITEMINFO info;
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  ::GetMenuItemInfo(menu, SC_CLOSE, false, &info);
  return !(info.fState & MFS_DISABLED);
}

void YaruWindow::SetClosable(bool closable) {
  HMENU menu = ::GetSystemMenu(hwnd, true);
  UINT flags = closable ? MF_ENABLED : MF_DISABLED | MF_GRAYED;
  ::EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND | flags);
}

bool YaruWindow::IsVisible() const { return ::IsWindowVisible(hwnd); }

void YaruWindow::SetVisible(bool visible) {
  ::ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
}

void YaruWindow::Show() { ::ShowWindow(hwnd, SW_SHOW); }

void YaruWindow::Hide() { ::ShowWindow(hwnd, SW_HIDE); }

bool YaruWindow::IsMinimized() const {
  return ::GetWindowLong(hwnd, GWL_STYLE) & WS_MINIMIZE;
}

void YaruWindow::Minimize(bool minimize) {
  ::ShowWindow(hwnd, minimize ? SW_MINIMIZE : SW_SHOW);
}

bool YaruWindow::IsMinimizable() const {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  return (style & WS_MINIMIZEBOX) && !(style & WS_MINIMIZE);
}

void YaruWindow::SetMinimizable(bool minimizable) {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  if (minimizable) {
    style |= WS_MINIMIZEBOX;
  } else {
    style &= ~WS_MINIMIZEBOX;
  }
  ::SetWindowLong(hwnd, GWL_STYLE, style);
}

bool YaruWindow::IsMaximized() const {
  return ::GetWindowLong(hwnd, GWL_STYLE) & WS_MAXIMIZE;
}

void YaruWindow::Maximize(bool maximize) {
  ::ShowWindow(hwnd, maximize ? SW_MAXIMIZE : SW_SHOW);
}

bool YaruWindow::IsMaximizable() const {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  return (style & WS_MAXIMIZEBOX) && !(style & WS_MAXIMIZE);
}

void YaruWindow::SetMaximizable(bool maximizable) {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  if (maximizable) {
    style |= WS_MAXIMIZEBOX;
  } else {
    style &= ~WS_MAXIMIZEBOX;
  }
  ::SetWindowLong(hwnd, GWL_STYLE, style);
}

bool YaruWindow::IsFullscreen() const {
  return (::GetWindowLong(hwnd, GWL_STYLE) & WS_OVERLAPPEDWINDOW) == 0;
}

void YaruWindow::SetFullscreen(bool fullscreen) {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  if (!fullscreen) {
    MONITORINFO mi = {sizeof(mi)};
    if (::GetWindowPlacement(hwnd, &placement) &&
        ::GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY),
                         &mi)) {
      ::SetWindowLong(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
      ::SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                     mi.rcMonitor.right - mi.rcMonitor.left,
                     mi.rcMonitor.bottom - mi.rcMonitor.top,
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
  } else {
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
    ::SetWindowPlacement(hwnd, &placement);
    ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                       SWP_FRAMECHANGED);
  }
}

bool YaruWindow::IsRestorable() const {
  return IsMaximized() || IsMinimized() || IsFullscreen();
}

void YaruWindow::Restore() { ::ShowWindow(hwnd, SW_RESTORE); }

void YaruWindow::Drag() {
  ::ReleaseCapture();
  ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
}

void YaruWindow::HideTitle() {
  MARGINS margins = {0, 0, 0, 0};
  ::DwmExtendFrameIntoClientArea(hwnd, &margins);
  ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

void YaruWindow::ShowMenu() {
  POINT cursor;
  ::GetCursorPos(&cursor);
  HMENU menu = ::GetSystemMenu(hwnd, false);
  BOOL cmd = ::TrackPopupMenu(menu, TPM_RIGHTBUTTON | TPM_RETURNCMD, cursor.x,
                              cursor.y, 0, hwnd, nullptr);
  if (cmd) {
    ::PostMessage(hwnd, WM_SYSCOMMAND, cmd, 0);
  }
}

void YaruWindow::Close() { ::SendMessage(hwnd, WM_CLOSE, 0, 0); }

void YaruWindow::Destroy() { ::DestroyWindow(hwnd); }

std::map<FlValue, FlValue> YaruWindow::GetGeometry() const {
  RECT rect;
  ::GetWindowRect(hwnd, &rect);
  return {
      {"id", 0},  // TODO
      {"type", "geometry"},
      {"x", rect.left},
      {"y", rect.top},
      {"width", rect.right - rect.left},
      {"height", rect.bottom - rect.top},
  };
}

void YaruWindow::SetGeometry(const std::map<FlValue, FlValue>& geometry) {
  FlValue x = geometry.at(FlValue("x"));
  FlValue y = geometry.at(FlValue("y"));
  FlValue width = geometry.at(FlValue("width"));
  FlValue height = geometry.at(FlValue("height"));

  RECT rect;
  ::GetWindowRect(hwnd, &rect);

  UINT flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER;
  if (std::get_if<int>(&x)) {
    flags &= ~SWP_NOMOVE;
    rect.left = std::get<int>(x);
  }
  if (std::get_if<int>(&y)) {
    flags &= ~SWP_NOMOVE;
    rect.top = std::get<int>(y);
  }
  if (std::get_if<int>(&width)) {
    flags &= ~SWP_NOSIZE;
    rect.right = rect.left + std::get<int>(width);
  }
  if (std::get_if<int>(&height)) {
    flags &= ~SWP_NOSIZE;
    rect.bottom = rect.top + std::get<int>(height);
  }
  ::SetWindowPos(hwnd, nullptr, rect.left, rect.top, rect.right - rect.left,
                 rect.bottom - rect.top, flags);
}

std::map<FlValue, FlValue> YaruWindow::GetState() const {
  return {
      {"id", 0},  // TODO
      {"type", "state"},
      {"active", IsActive()},
      {"closable", IsClosable()},
      {"fullscreen", IsFullscreen()},
      {"maximizable", IsMaximizable()},
      {"maximized", IsMaximized()},
      {"minimizable", IsMinimizable()},
      {"minimized", IsMinimized()},
      {"movable", true},
      {"restorable", IsRestorable()},
      {"visible", IsVisible()},
  };
}

void YaruWindow::SetState(const std::map<FlValue, FlValue>& state) {
  FlValue active = state.at(FlValue("active"));
  if (std::get_if<bool>(&active)) {
    Activate(std::get<bool>(active));
  }

  FlValue closable = state.at(FlValue("closable"));
  if (std::get_if<bool>(&closable)) {
    SetClosable(std::get<bool>(closable));
  }

  FlValue fullscreen = state.at(FlValue("fullscreen"));
  if (std::get_if<bool>(&fullscreen)) {
    SetFullscreen(std::get<bool>(fullscreen));
  }

  FlValue maximizable = state.at(FlValue("maximizable"));
  if (std::get_if<bool>(&maximizable)) {
    SetMaximizable(std::get<bool>(maximizable));
  }

  FlValue maximized = state.at(FlValue("maximized"));
  if (std::get_if<bool>(&maximized)) {
    Maximize(std::get<bool>(maximized));
  }

  FlValue minimizable = state.at(FlValue("minimizable"));
  if (std::get_if<bool>(&minimizable)) {
    SetMinimizable(std::get<bool>(minimizable));
  }

  FlValue minimized = state.at(FlValue("minimized"));
  if (std::get_if<bool>(&minimized)) {
    Minimize(std::get<bool>(minimized));
  }

  FlValue visible = state.at(FlValue("visible"));
  if (std::get_if<bool>(&visible)) {
    SetVisible(std::get<bool>(visible));
  }

  ::SendMessage(hwnd, WM_USER, 0, 0);
}

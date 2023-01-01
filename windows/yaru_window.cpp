#include "yaru_window.h"

#include <dwmapi.h>

YaruWindow::YaruWindow(HWND hwnd) : hwnd(hwnd) {}

void YaruWindow::Init() {
  MARGINS margins = {0, 0, 0, 0};
  ::DwmExtendFrameIntoClientArea(hwnd, &margins);
  ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

bool YaruWindow::IsActive() const { return ::GetForegroundWindow() == hwnd; }

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

void YaruWindow::Show() { ::ShowWindow(hwnd, SW_SHOW); }

void YaruWindow::Hide() { ::ShowWindow(hwnd, SW_HIDE); }

bool YaruWindow::IsMinimized() const {
  return ::GetWindowLong(hwnd, GWL_STYLE) & WS_MINIMIZE;
}

bool YaruWindow::IsMinimizable() const {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  return (style & WS_MINIMIZEBOX) && !(style & WS_MINIMIZE);
}

void YaruWindow::Minimize() { ::ShowWindow(hwnd, SW_MINIMIZE); }

bool YaruWindow::IsMaximized() const {
  return ::GetWindowLong(hwnd, GWL_STYLE) & WS_MAXIMIZE;
}

bool YaruWindow::IsMaximizable() const {
  DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
  return (style & WS_MAXIMIZEBOX) && !(style & WS_MAXIMIZE);
}

void YaruWindow::Maximize() { ::ShowWindow(hwnd, SW_MAXIMIZE); }

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

void YaruWindow::Menu() {
  POINT cursor;
  ::GetCursorPos(&cursor);
  HMENU menu = ::GetSystemMenu(hwnd, false);
  BOOL cmd = ::TrackPopupMenu(menu, TPM_RIGHTBUTTON | TPM_RETURNCMD, cursor.x,
                              cursor.y, 0, hwnd, nullptr);
  if (cmd) {
    ::PostMessage(hwnd, WM_SYSCOMMAND, cmd, 0);
  }
}

void YaruWindow::Move() {
  ::ReleaseCapture();
  ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
}

void YaruWindow::Close() { ::SendMessage(hwnd, WM_CLOSE, 0, 0); }

void YaruWindow::Destroy() { ::DestroyWindow(hwnd); }

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <string>

#include "window.h"
#include "button.h"
#include "GUI.h"

#include "Grid.h"
#include "Config.h"

enum ButtonEvents{
    Pause,
    Clear,
    Random_fill,
    Chess_fill,
    Increase_ratio,
    Decrease_ratio
};

enum WindowState{
    Unpaused,
    Paused
};
WindowState& operator++(WindowState& st, int);

class MainWindow : public Window
{
public:
    MainWindow();
    ~MainWindow();

    void OnCreate();
    void OnSizeChanged();

    void DrawStats(Context *cr);
    void DrawHelp(Context *cr);

    void DrawSquare(Context *cr, int i, int j);
    void DrawGrid(Context *cr);
    void OnDraw(Context *cr);

    void OnNotify(Window *child, uint32_t type, const Point &position);
    bool OnLeftMouseButtonClick(const Point &position);
    bool OnRightMouseButtonClick(const Point &position);
    bool OnKeyPress(uint64_t value);

    bool OnTimeout();

    std::string get_str_pause();

private:
    RGB Colors[3];
    Grid* Grid_;
    uint16_t upper_padding, lower_padding, side_padding, sq_side;
    WindowState pause;
    int tolerance;
};

#endif // MAINWINDOW_H

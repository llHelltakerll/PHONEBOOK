#include "include/OFLWindow.h"

OFLWindow::OFLWindow(int x, int y, int w, int h, const char* l)
    : Fl_Window(x, y, w, h, l)
{
    callback(windowCallback);
}

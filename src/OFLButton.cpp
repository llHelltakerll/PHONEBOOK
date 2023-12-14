#include "include/OFLButton.h"

OFLButton::OFLButton(int x, int y, int w, int h, const char* l)
    : Fl_Button(x, y, w, h, l)
{
    callback(CallbackFunc, 0);
    clear_visible_focus();
};

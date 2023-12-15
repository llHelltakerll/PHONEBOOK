#pragma once

#include "OFLButton.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <iostream>

class ConfirmButton : public OFLButton {
public:
    ConfirmButton(Fl_Window* win, int x, int y, int w, int h,
                  const char* l = "");
    void Press();
    void Confirm();
private:
    Fl_Window* win;
};

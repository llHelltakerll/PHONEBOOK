#pragma once

#include "ConfirmButton.h"
#include "OFLWindow.h"
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <string>

enum { err_win_w = 350, err_win_h = 150 };

class ErrorOkWindow : public OFLWindow {
public:
    ErrorOkWindow(int x, int y, const char* err_mesg, const char* l = "");
    ~ErrorOkWindow();
    void Interaction() override;
private:
    ConfirmButton* conf_but;
    Fl_Box* label;
};

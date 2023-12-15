#pragma once

#include "ConfirmButton.h"
#include "OFLWindow.h"
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <string>

class ErrorOkWindow : public OFLWindow {
public:
    ErrorOkWindow(int x, int y, int w, int h, const char* err_mesg,
                  const char* l = "");
    ~ErrorOkWindow();
    void Interaction() override;
private:
    ConfirmButton* conf_but;
    Fl_Box* label;
};

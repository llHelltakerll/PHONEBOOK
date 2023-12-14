#pragma once

#include <FL/Fl_Button.H>

class OFLButton : public Fl_Button {
public:
    OFLButton(int x, int y, int w, int h, const char* l = "");
    virtual void Press() = 0;
protected:
    static void CallbackFunc(Fl_Widget* w, void* user)
    {
        OFLButton* a_w = reinterpret_cast<OFLButton*>(w);
        a_w->Press();
    }
};

#pragma once
#include "DataTable.h"
#include <FL/Fl_Check_Button.H>

enum { check_b_label_fontsz = 17 };

class OFLCheckButton : public Fl_Check_Button {
public:
    OFLCheckButton(int x, int y, int w, int h, const char* l = "");
    virtual void CheckBox() = 0;
private:
    static void CallBackFunction(Fl_Widget* w, void* user)
    {
        OFLCheckButton* a_w = reinterpret_cast<OFLCheckButton*>(w);
        a_w->CheckBox();
    }
};

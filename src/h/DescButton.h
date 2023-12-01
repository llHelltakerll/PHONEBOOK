#pragma once
#include "DataTable.h"
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Widget.H>

enum { fontsz = 17 };

class OFLCheckButton : public Fl_Check_Button {
public:
    OFLCheckButton(int x, int y, int w, int h, const char* l = "")
        : Fl_Check_Button(x, y, w, h, l)
    {
        box(FL_NO_BOX);
        labelsize(fontsz);
        align(FL_ALIGN_RIGHT);
        callback(CallBackFunction, 0);
    }
    virtual void CheckBox() = 0;
private:
    static void CallBackFunction(Fl_Widget* w, void* user)
    {
        OFLCheckButton* a_w = reinterpret_cast<OFLCheckButton*>(w);
        a_w->CheckBox();
    }
};

class DescButton : public OFLCheckButton {
public:
    DescButton(DataTable*& table, int x, int y, int w, int h, const char* l)
        : OFLCheckButton(x, y, w, h, l), table(table)
    {}
private:
    virtual void Desc() { table->setInvert(value()); }
    void CheckBox() override { Desc(); }
private:
    DataTable* table;
};

// class IncompMatchButton

#pragma once
#include "DataTable.h"
#include <FL/Enumerations.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Input_Choice.H>

class OFLInput : public Fl_Input {
public:
    OFLInput(int x, int y, int w, int h, const char* lb)
        : Fl_Input(x, y, w, h, lb)
    {
        textsize(label_fontsz);
        align(FL_ALIGN_TOP_LEFT);
        labelsize(header_c_font);
        when(FL_WHEN_CHANGED);
        callback(CallBackFunction, 0);
    }
    virtual ~OFLInput() {}
    virtual void Input() {}
private:
    static void CallBackFunction(Fl_Widget* w, void* user)
    {
        OFLInput* a_w = reinterpret_cast<OFLInput*>(w);
        a_w->Input();
    }
};

class SortByInput : public OFLInput {
    int sort;
    DataTable* table;
public:
    SortByInput(DataTable*& table, int x, int y, int w, int h,
                const char* lb = "", int sort = 0)
        : OFLInput(x + spacing_x, y + spacing_y, w, h, lb), table(table),
          sort(sort)
    {}
private:
    virtual void SortByVal() { table->sort(sort, value(), true); }
    virtual void Input() { SortByVal(); }
};

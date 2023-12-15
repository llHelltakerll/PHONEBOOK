#pragma once

#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <iostream>

class OFLTable : public Fl_Table {
public:
    OFLTable(int x, int y, int w, int h, const char* l = "", int c = 0,
             int r = 0);

    virtual ~OFLTable() = default;
protected:
    virtual void DrawData(const char* s, int X, int Y, int W, int H) = 0;
    virtual void DrawHeaderCol(std::string s, int X, int Y, int W, int H,
                               bool is_pressed)
        = 0;
    virtual void DrawHeaderRow(const char* s, int X, int Y, int W, int H) = 0;
};

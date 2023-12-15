#pragma once

#include "InsertWindow.h"
#include "OFLButton.h"

class InsertButton : public OFLButton {
public:
    InsertButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "");

    void Press();

    void CreateWin();
private:
    InsertWindow* win;
    DataTable*& table;
    int active_row;
};

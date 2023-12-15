#pragma once

#include "ConfirmButton.h"
#include "DataTable.h"
#include "ErrorOkWindow.h"
#include "OFLWindow.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>

class InsertWindow : public OFLWindow {
public:
    InsertWindow(DataTable*& table, int x, int y, int w, int h, const char* l);

    virtual ~InsertWindow();

    void insertInpValues();
private:
    void Interaction() override;
protected:
    Fl_Input* number_inp;
    Fl_Input* full_name_inp;
    Fl_Input* street_inp;
    Fl_Input* house_inp;
    Fl_Input* flat_inp;
    DataTable*& table;
    ConfirmButton* conf_but;
    ErrorOkWindow* err_win;
};

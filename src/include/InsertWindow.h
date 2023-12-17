#pragma once

#include "ConfirmButton.h"
#include "DataTable.h"
#include "ErrorOkWindow.h"
#include "OFLWindow.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>

enum {
    ins_win_h = 200,
    ins_win_w = 450,
};

enum {
    flat_inp_w = 80,
    house_inp_w = flat_inp_w,
    conf_button_w = flat_inp_w + 10,
    street_inp_w = 3 * house_inp_w,
    full_name_inp_w = street_inp_w + flat_inp_w + house_inp_w + spacing_x * 2,
    number_inp_w = flat_inp_w + street_inp_w

};

class InsertWindow : public OFLWindow {
public:
    InsertWindow(DataTable*& table, int x, int y, const char* l);

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

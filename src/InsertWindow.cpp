#include "include/InsertWindow.h"

InsertWindow::InsertWindow(DataTable*& table, int x, int y, int w, int h,
                           const char* l)
    : OFLWindow(x, y, w, h, l), table(table)
{
    begin();
    conf_but = new ConfirmButton(this, 150, 10, 80, 30, "Подтвердить");
    number_inp = new Fl_Input(0, 10, 100, 30, "телефон:");
    number_inp->value("+7 ");
    full_name_inp = new Fl_Input(0, 50, 100, 30, "фио:");
    street_inp = new Fl_Input(0, 90, 100, 30, "улица:");
    house_inp = new Fl_Input(0, 130, 100, 30, "дом:");
    flat_inp = new Fl_Input(0, 170, 100, 30, "квартира:");
    set_modal();
    show();
    end();
}

InsertWindow::~InsertWindow()
{
    delete conf_but;
    delete number_inp;
    delete full_name_inp;
    delete street_inp;
    delete house_inp;
    delete flat_inp;
}

void InsertWindow::insertInpValues()
{
    std::vector<std::string> val_vec(table->sw->getRowCount());
    val_vec[1] = number_inp->value();
    val_vec[2] = full_name_inp->value();
    val_vec[3] = street_inp->value();
    val_vec[4] = house_inp->value();
    val_vec[5] = flat_inp->value();

    if (table->sw->insert(val_vec) == SQLITE_CONSTRAINT) {
        err_win = new ErrorOkWindow(0, 0, 400, 150,
                                    "Ошибка! Введен дубликат номера телефона",
                                    "Окно ошибок");
        return;
    }
    table->undoTable();
}
void InsertWindow::Interaction()
{
    std::string number_inp_str = number_inp->value();
    std::string full_name_inp_str = full_name_inp->value();
    std::string street_inp_str = street_inp->value();
    std::string house_inp_str = house_inp->value();
    std::string flat_inp_str = flat_inp->value();
    hide();
    if (Fl::event() == FL_CLOSE) { Fl::delete_widget(this); }
    else {
        insertInpValues();
    }
}

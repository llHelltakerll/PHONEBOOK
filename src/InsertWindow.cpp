#include "include/InsertWindow.h"

InsertWindow::InsertWindow(DataTable*& table, int x, int y, const char* l)
    : OFLWindow(x, y, ins_win_w, ins_win_h, l), table(table)
{
    begin();
    number_inp
        = new Fl_Input(spacing_x, button_h, number_inp_w, button_h, "Телефон:");
    number_inp->align(FL_ALIGN_TOP_LEFT);

    number_inp->value("+7 ");
    full_name_inp = new Fl_Input(spacing_x, button_h * 3, full_name_inp_w,
                                 button_h, "Фио:");
    full_name_inp->align(FL_ALIGN_TOP_LEFT);

    street_inp = new Fl_Input(spacing_x, button_h * 5, street_inp_w, button_h,
                              "Улица:");
    street_inp->align(FL_ALIGN_TOP_LEFT);

    house_inp = new Fl_Input(street_inp_w + spacing_x * 2, button_h * 5,
                             house_inp_w, button_h, "Дом:");
    house_inp->align(FL_ALIGN_TOP_LEFT);

    flat_inp = new Fl_Input(street_inp_w + house_inp_w + spacing_x * 3,
                            button_h * 5, flat_inp_w, button_h, "Квартира:");
    flat_inp->align(FL_ALIGN_TOP_LEFT);

    conf_but = new ConfirmButton(this, number_inp_w + spacing_x * 2, button_h,
                                 conf_button_w, button_h, "Подтвердить");
    conf_but->labelsize(13);
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
        err_win = new ErrorOkWindow(0, 0, "Введен дубликат номера телефона!",
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

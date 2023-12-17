#include "include/UpdateWindow.h"

UpdateWindow::UpdateWindow(DataTable*& table, int x, int y, const char* l)
    : InsertWindow(table, x, y, l)
{
    number_inp->value(table->getNumberByRow(table->getActiveRow()).c_str());
    full_name_inp->value(table->getFNByRow(table->getActiveRow()).c_str());
    street_inp->value(table->getStreetByRow(table->getActiveRow()).c_str());
    house_inp->value(table->getHNByRow(table->getActiveRow()).c_str());
    flat_inp->value(table->getFlatByRow(table->getActiveRow()).c_str());
}

void UpdateWindow::updateInpValues()
{
    std::vector<std::string> val_vec(table->sw->getRowCount());
    val_vec[1] = number_inp->value();
    val_vec[2] = full_name_inp->value();
    val_vec[3] = street_inp->value();
    val_vec[4] = house_inp->value();
    val_vec[5] = flat_inp->value();
    if (table->updateField(val_vec) == SQLITE_CONSTRAINT) {
        err_win = new ErrorOkWindow(0, 0, "Введен дубликат номера телефона",
                                    "Окно ошибок");
        return;
    }
    table->undoTable();
}

void UpdateWindow::Interaction()
{
    hide();
    if (Fl::event() == FL_CLOSE) { Fl::delete_widget(this); }
    else {
        updateInpValues();
    }
}

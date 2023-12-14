#include "include/ClearInputButton.h"
ClearInputButton::ClearInputButton(std::vector<Fl_Input*>& inp_vec,
                                   DataTable*& table, int x, int y, int w,
                                   int h, const char* l)
    : OFLButton(x, y, w, h, l), inp_vec(inp_vec), table(table)
{}

void ClearInputButton::Press()
{
    clearInputs();
};

void ClearInputButton::clearInputs()
{
    for (Fl_Input* input: inp_vec) { input->value(""); }
    table->refreshTable();
}

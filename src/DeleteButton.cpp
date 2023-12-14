#include "include/DeleteButton.h"

DeleteButton::DeleteButton(DataTable*& table, int x, int y, int w, int h,
                           const char* l)
    : OFLButton(x, y, w, h, l), table(table)

{}

void DeleteButton::Press()
{
    deleteField();
};
void DeleteButton::deleteField()
{
    if (table->getActiveRow() == -1) return;
    table->deleteById();
}

#include "include/DeleteButton.h"
#include "include/ErrorOkWindow.h"

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
    if (table->getActiveRow() == -1) {
        err_win = new ErrorOkWindow(
            0, 0, 400, 200, "Не выбран гражданин для изменения", "Окно ошибок");
        return;
    }
    table->deleteById();
    table->undoTable();
}

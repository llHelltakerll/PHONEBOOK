#include "include/InsertButton.h"

InsertButton::InsertButton(DataTable*& table, int x, int y, int w, int h,
                           const char* l)
    : OFLButton(x, y, w, h, l), table(table)
{}

void InsertButton::Press()
{
    CreateWin();
};

void InsertButton::CreateWin()
{
    std::cout << active_row << "\n";
    win = new InsertWindow(table, 0, 0, 500, 200, "Маленькое окно");
}

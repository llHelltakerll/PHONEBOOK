#include "h/DescButton.h"

DescButton::DescButton(DataTable*& table, int x, int y, int w, int h,
                       const char* l)
    : OFLCheckButton(x, y, w, h, l), table(table)
{
    clear_visible_focus();
}
void DescButton::Desc() const
{
    table->setInvert(value());
}
void DescButton::CheckBox()
{
    Desc();
}

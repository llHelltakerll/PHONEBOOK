#include "h/DescButton.h"

DescButton::DescButton(DataTable*& table, int x, int y, int w, int h,
                       const char* l)
    : OFLCheckButton(x, y, w, h, l), table(table)
{}
void DescButton::Desc() const
{
    table->setInvert(value());
}
void DescButton::CheckBox()
{
    Desc();
}

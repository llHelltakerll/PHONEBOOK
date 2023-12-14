#include "include/TableInputSort.h"

TableInputSort::TableInputSort(DataTable& table, int x, int y, int w, int h,
                               const char* lb, int sort)
    : OFLInput(x + spacing_x, y + spacing_y, w, h, lb), table(table), sort(sort)
{}
void TableInputSort::SortByVal() const
{
    table.sort(sort, value(), true);
}
void TableInputSort::Input()
{
    SortByVal();
}

void TableInputSort::clear()
{
    value("");
}

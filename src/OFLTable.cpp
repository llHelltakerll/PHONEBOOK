#include "include/OFLTable.h"

OFLTable::OFLTable(int x, int y, int w, int h, const char* l, int c, int r)
    : Fl_Table(x, y, w, h, l)
{
    begin();
    rows(r);
    row_header(true); // enable row headers (along left)

    cols(c);
    col_header(true); // enable column headers (along top)
    end();
};

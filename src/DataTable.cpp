#include "h/DataTable.h"
#include "h/Button.h"
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

DataTable::DataTable(int x, int y, int w, int h, SQLiteWrapper*& sw,
                     const char* l)
    : OFLTable(x, y, w, h, l, sw->getColCount(), sw->getRowCount()), sw(sw),
      active_cols(new bool[sw->getColCount()]),
      active_rows(new bool[sw->getRowCount()]{}),
      input_values(sw->getColCount() + 1)
{
    row_resize(false); // disable row resizing
    row_height_all(button_h);
    row_header_width(header_row_w);

    col_resize(false); // enable column resizing
    col_header_height(header_col_h);
    col_resize(false); // enable column resizing

    init_table_info();
    sort(1);

    for (int c = 0; c < sw->getColCount(); c++) { col_width(c, set_col_w(c)); }
    this->callback(&event_callback, (void*)this); // setup callback
    this->when(FL_WHEN_CHANGED);
}

DataTable::~DataTable()
{
    delete[] active_cols;
    delete[] active_rows;
}

void DataTable::sort(int c, std::string val, bool input, bool clear_prev)
{
    int active_col{};

    if (input) {
        inp_val.val = val;
        inp_val.col = c;
        input_values[c - 1] = val;
    }

    for (int i = 0; i < sw->getColCount(); i++) {
        std::cout << "VALUES: " << input_values[i];
    }
    std::cout << "\nEND\n";

    for (int i = 0; i < sw->getColCount(); i++) {
        if (active_cols[i] == true) { active_col = i + 1; }
    }

    if (val.empty() && inp_val.val.empty()) {
        if (active_col == 0 && c == 0) {
            sw->sortBy(0, val, active_col, invert_temp, clear_prev);
        }
        else {
            sw->sortBy(c, input_values[c - 1], active_col, invert_temp,
                       clear_prev);
        }
    }
    else if (!inp_val.val.empty()) {
        if (active_col == 0 && c == 0) {
            sw->sortBy(c, input_values[c - 1], active_col, invert_temp,
                       clear_prev);
        }
        else {
            sw->sortBy(c, input_values[c - 1], active_col, invert_temp,
                       clear_prev);
        }
    }
    init_table_info();
    redraw();
    return;
}

void DataTable::setInvert(bool invert)
{
    invert_temp = invert;
    sort(inp_val.col, inp_val.val);
}
void DataTable::init_table_info()
{
    info_rows.clear();
    for (rows_count = 0; sw->step(); rows_count++) {
        db_rows temp;
        temp.id += sw->getColText(0);
        temp.number += sw->getColText(1);
        temp.full_name += sw->getColText(2);
        temp.street += sw->getColText(3);
        temp.house_number += sw->getColText(4);
        temp.flat += sw->getColText(5);
        info_rows.push_back(temp);
    }
    rows(rows_count);
}

void DataTable::push_col_callback()
{
    if (Fl::event() == FL_PUSH && callback_context() == CONTEXT_COL_HEADER) {
        int C = callback_col();
        if (active_cols[C] == false) {
            active_cols[C] = true;
            for (int i = 0; i < sw->getColCount(); i++) {
                if (i != C) active_cols[i] = false;
            }
            sort(C + 1, "");
        }
        else {
            active_cols[C] = false;
            sort(0, "");
        }
        DrawHeaderCol(sw->getColName(C), 0, 0, 0, 0, true);
        redraw();
    }
}
void DataTable::push_row_callback()
{
    if (Fl::event() == FL_PUSH && callback_context() == CONTEXT_ROW_HEADER) {
        std::string symbol;
        int R = callback_row();
        if (active_rows[R] == false) {
            active_rows[R] = true;
            symbol = "@>";
            for (int i = 0; i < sw->getRowCount(); i++) {
                if (i != R) active_rows[i] = false;
            }
        }
        else {
            symbol = "";
            active_rows[R] = false;
        }

        DrawHeaderRow(symbol.c_str(), 0, 0, 0, 0);
    }
}

void DataTable::DrawHeaderCol(std::string s, int X, int Y, int W, int H,
                              bool is_pressed)
{
    Fl_Color darkGrayColor = fl_rgb_color(160, 160, 160);
    if (is_pressed) {
        fl_draw_box(FL_THIN_DOWN_BOX, X, Y, W, H, darkGrayColor);
    }
    else {
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, darkGrayColor);
    }

    redraw();

    fl_push_clip(X, Y, W, H);
    fl_color(FL_BLACK);
    fl_draw(s.c_str(), X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

void DataTable::DrawData(const char* s, int X, int Y, int W, int H)
{
    fl_push_clip(X, Y, W, H);
    fl_color(FL_WHITE);
    fl_rectf(X, Y, W, H);
    fl_color(FL_GRAY0);
    fl_draw(s, X + 5, Y, W, H, FL_ALIGN_LEFT);
    fl_color(color());
    fl_rect(X, Y, W, H);
    fl_pop_clip();
}

void DataTable::set_col(int COL, int ROW, int X, int Y, int W, int H)
{
    switch (COL) {
    case number_col:
        DrawData(info_rows[ROW].number.c_str(), X, Y, W, H);
        break;
    case fullname_col:
        DrawData(info_rows[ROW].full_name.c_str(), X, Y, W, H);
        break;
    case street_col:
        DrawData(info_rows[ROW].street.c_str(), X, Y, W, H);
        break;
    case house_col:
        DrawData(info_rows[ROW].house_number.c_str(), X, Y, W, H);
        break;
    case flat_col:
        DrawData(info_rows[ROW].flat.c_str(), X, Y, W, H);
        break;
    }
}
void DataTable::insertField(std::vector<std::string> insert_val)
{
    if (sw->insert(insert_val) == 1) {
        ErrorOkWindow* err_win
            = new ErrorOkWindow(0, 0, 500, 200, "Error window");
    }
    sw->doPrevQuary();
    active_rows[getActiveRow()] = false;
    init_table_info();
    redraw();
}

void DataTable::updateField(std::vector<std::string> upd_vals)
{
    for (int i = 1; i <= sw->getColCount(); i++) {
        if (sw->update(std::atoi(info_rows[getActiveRow()].id.c_str()), i,
                       upd_vals[i])
            == 1) {
            ErrorOkWindow* err_win
                = new ErrorOkWindow(0, 0, 500, 200, "Error window");
        };
    }
    sw->doPrevQuary();
    active_rows[getActiveRow()] = false;
    init_table_info();
    redraw();
}
void DataTable::DrawHeaderRow(const char* s, int X, int Y, int W, int H)
{
    redraw(); // for more accurate title images and automaticly set
              // coordinates
    fl_push_clip(X, Y, W, H);
    fl_color(FL_BLACK);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

int DataTable::set_col_w(int COL) const
{
    switch (COL) {
    case number_col:
        return static_cast<int>(w() * 0.18);
    case fullname_col:
        return static_cast<int>(w() * 0.448);
    case street_col:
        return static_cast<int>(w() * 0.15);
    case house_col:
        return static_cast<int>(w() * 0.1);
    case flat_col:
        return static_cast<int>(w() * 0.098);
    default:
        return 0;
    }
}

std::string DataTable::getHeaderColName(int C) const
{
    switch (C) {
    case number_col:
        return "Телефон";
    case fullname_col:
        return "ФИО";
    case street_col:
        return "Улица";
    case house_col:
        return "Номер дома";
    case flat_col:
        return "Квартира";
    }
    return "";
}

Fl_Color darkGrayColor = fl_rgb_color(160, 160, 160);
void DataTable::draw_cell(TableContext context, int ROW, int COL, int X, int Y,
                          int W, int H)
{
    switch (context) {
    case CONTEXT_COL_HEADER:
        fl_font(FL_SCREEN_BOLD, header_c_font);
        DrawHeaderCol(getHeaderColName(COL).c_str(), X, Y, W, H,
                      active_cols[COL]);
        return;

    case CONTEXT_ROW_HEADER:
        fl_font(FL_COURIER_BOLD, header_r_font);
        if (active_rows[ROW] == false) {
            fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, darkGrayColor);
            DrawHeaderRow("", X, Y, W, H);
        }
        else {
            fl_draw_box(FL_THIN_DOWN_BOX, X, Y, W, H, darkGrayColor);
            DrawHeaderRow("@>", X, Y, W, H);
        }
        return;
    case CONTEXT_CELL:
        fl_font(FL_HELVETICA, cell_font);
        set_col(COL, ROW, X + 1, Y, W, H);
        return;

    case CONTEXT_RC_RESIZE:
        for (int c = 0; c < sw->getColCount(); c++) {
            col_width(c, set_col_w(c));
        }
        return;
    default:
        return;
    }
}

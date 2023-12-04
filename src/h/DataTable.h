#pragma once

#include "MainTable.h"
#include "SQLiteWrapper.h"
#include <FL/Enumerations.H>
#include <string>

enum { label_fontsz = 18 };

enum {
    big_button_w = 770,
    small_buttow_w = 180,
    mid_button_w = 270,
    mid_big_button_w = 480,
    button_h = 32,
    spacing_x = 10,
    spacing_y = button_h,
};

enum { number_col = 0, fullname_col, street_col, house_col, flat_col, id_col };

enum {
    number_col_w = 700,
    fullname_col_w = 550,
    street_col_w = 750,
    house_col_w = 810,
    flat_col_w = 755,
};

enum { header_r_font = 25, header_c_font = 16, cell_font = 15 };
class DataTable : public MainTable {
public:
    DataTable(int x, int y, int w, int h, SQLiteWrapper*& sw,
              const char* l = "")
        : MainTable(x, y, w, h, l, sw->getColCount()), sw(sw),
          active_cols(new bool[sw->getColCount()]),
          active_rows(new bool[sw->getRowCount()]{}),
          input_values(sw->getColCount() + 1)
    {
        for (int c = 0; c < sw->getColCount(); c++) {
            col_width(c, set_col_w(c));
        }
        rows(sw->getRowCount());
        this->callback(&event_callback, (void*)this); // setup callback
        this->when(FL_WHEN_CHANGED);
    }

    ~DataTable()
    {
        delete[] active_cols;
        delete[] active_rows;
    }

    void sort(int c, std::string val = "", bool input = false)
    {
        int active_col{};

        if (input) {
            inp_val.val = val;
            inp_val.col = c;
            input_values[c - 1] = val;
        }

        std::cout << sw->getColCount() << "\n";
        for (int i = 0; i < sw->getColCount(); i++) {
            std::cout << i << ":  " << input_values[i] << "\n";
        }

        for (int i = 0; i < sw->getColCount(); i++) {
            if (active_cols[i] == true) { active_col = i + 1; }
        }

        if (val.empty() && inp_val.val.empty()) {
            if (active_col == 0 && c == 0) { sw->sortBy(0, val, active_col); }
            else {
                sw->sortBy(c, val, active_col, invert_temp);
            }
        }
        else if (!inp_val.val.empty()) {
            if (active_col == 0 && c == 0) {
                sw->sortBy(inp_val.col, inp_val.val, active_col, invert_temp);
            }
            else {
                sw->sortBy(inp_val.col, inp_val.val, active_col, invert_temp);
            }
        }
        else {
            inp_val.val = val;
            inp_val.col = c;
        }
        init_table_info();
        redraw();
        return;
    }

    void setInvert(bool invert)
    {
        invert_temp = invert;
        sort(inp_val.col, inp_val.val);
    }
private:
    void init_table_info()
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

    void push_col_callback()
    {
        if (Fl::event() == FL_PUSH
            && callback_context() == CONTEXT_COL_HEADER) {
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
    void push_row_callback()
    {
        if (Fl::event() == FL_PUSH
            && callback_context() == CONTEXT_ROW_HEADER) {
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
    static void event_callback(Fl_Widget*, void* data)
    {
        DataTable* table = (DataTable*)data;
        table->push_col_callback();
        table->push_row_callback();
    }
    void DrawHeaderCol(std::string s, int X, int Y, int W, int H,
                       bool is_pressed)
    {
        if (is_pressed) {
            fl_draw_box(FL_THIN_DOWN_BOX, X, Y, W, H, row_header_color());
        }
        else {
            fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        }

        redraw();

        fl_push_clip(X, Y, W, H);
        fl_color(FL_BLACK);
        fl_draw(s.c_str(), X, Y, W, H, FL_ALIGN_CENTER);
        fl_pop_clip();
    }

    void DrawData(const char* s, int X, int Y, int W, int H)
    {
        fl_push_clip(X, Y, W, H);
        fl_color(FL_WHITE);
        fl_rectf(X, Y, W, H);
        fl_color(FL_GRAY0);
        fl_draw(s, X, Y, W, H, FL_ALIGN_LEFT);
        fl_color(color());
        fl_rect(X, Y, W, H);
        fl_pop_clip();
    }

    void set_col(int COL, int ROW, int X, int Y, int W, int H)
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

    void DrawHeaderRow(const char* s, int X, int Y, int W, int H)
    {
        redraw(); // for more accurate title images and automaticly set
                  // coordinates
        fl_push_clip(X, Y, W, H);
        fl_color(FL_BLACK);
        fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
        fl_pop_clip();
    }

    int set_col_w(int COL)
    {
        switch (COL) {
        case number_col:
            return static_cast<int>(w() * 0.18);
        case fullname_col:
            return static_cast<int>(w() * 0.45);
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

    std::string getHeaderColName(int C)
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

    void draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W,
                   int H)
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
                fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
                DrawHeaderRow("", X, Y, W, H);
            }
            else {
                fl_draw_box(FL_THIN_DOWN_BOX, X, Y, W, H, row_header_color());
                DrawHeaderRow("@>", X, Y, W, H);
            }
            return;
        case CONTEXT_CELL:
            fl_font(FL_HELVETICA, cell_font);
            init_table_info();
            set_col(COL, ROW, X + 1, Y, W, H);
            return;

        case CONTEXT_RC_RESIZE:
            for (int c = 0; c < sw->getColCount(); c++) {
                col_width(c, set_col_w(c));
            }
            return;
        case CONTEXT_ENDPAGE:
        default:
            return;
        }
    }
private:
    struct db_rows {
        std::string id = " ";
        std::string number = " ";
        std::string full_name = " ";
        std::string street = " ";
        std::string house_number = " ";
        std::string flat = " ";
    };

    bool* active_cols;
    bool* active_rows;
    int rows_count;
    std::vector<db_rows> info_rows;
    std::vector<std::string> input_values;
    struct input_value {
        int col;
        std::string val;
    };
    std::vector<input_value> inp_val_vec;
    input_value inp_val;
    bool invert_temp;
public:
    SQLiteWrapper* sw;
};

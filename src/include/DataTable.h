#pragma once

#include "OFLTable.h"
#include "SQLiteWrapper.h"
#include <string>

enum {
    big_button_w = 770,
    small_buttow_w = 180,
    mid_button_w = 270,
    mid_big_button_w = 480,
    button_h = 28,
    spacing_x = 10,
    spacing_y = button_h + 12,
};

enum { number_col = 0, fullname_col, street_col, house_col, flat_col, id_col };

enum { header_row_w = 30, header_col_h = 24 };

enum {
    number_col_w = 700,
    fullname_col_w = 550,
    street_col_w = 750,
    house_col_w = 810,
    flat_col_w = 755,
};

enum { label_fontsz = 18 };
enum { header_r_font = 25, header_c_font = 16, cell_font = 15 };

class DataTable : public OFLTable {
public:
    DataTable(int x, int y, int w, int h, SQLiteWrapper*& sw,
              const char* l = "");

    ~DataTable();

    void sort(int c, std::string val = "", bool input = false,
              bool clear_prev = false);

    void setInvert(bool invert);

    int getActiveRow();

    void undoTable();

    void deleteById();

    void refreshTable();

    int updateField(std::vector<std::string> upd_vals);

    inline std::string getNumberByRow(int r) { return info_rows[r].number; }
    inline std::string getFNByRow(int r) { return info_rows[r].full_name; }
    inline std::string getStreetByRow(int r) { return info_rows[r].street; }
    inline std::string getHNByRow(int r) { return info_rows[r].house_number; }
    inline std::string getFlatByRow(int r) { return info_rows[r].flat; }
private:
    void init_table_info();

    void push_col_callback();
    void push_row_callback();

    static void event_callback(Fl_Widget*, void* data)
    {
        DataTable* table = (DataTable*)data;
        table->push_col_callback();
        table->push_row_callback();
    }
    void DrawHeaderCol(std::string s, int X, int Y, int W, int H,
                       bool is_pressed) override;

    void DrawData(const char* s, int X, int Y, int W, int H) override;

    void set_col(int COL, int ROW, int X, int Y, int W, int H);

    void DrawHeaderRow(const char* s, int X, int Y, int W, int H) override;

    int set_col_w(int COL) const;
    std::string getHeaderColName(int C) const;

    void draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W,
                   int H) override;
private:
    struct db_rows {
        std::string id;
        std::string number;
        std::string full_name;
        std::string street;
        std::string house_number;
        std::string flat;
    };

    bool* active_rows;
    bool* active_cols;
    int rows_count;
    std::vector<db_rows> info_rows;
    std::vector<std::string> input_values;
    struct input_value {
        int col = 1;
        std::string val;
    };
    std::vector<input_value> inp_val_vec;
    input_value inp_val;
    bool invert_temp;
public:
    SQLiteWrapper* sw;
};

#pragma once

#include <FL/Enumerations.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <iostream>

class MainTable : public Fl_Table {
public:
    MainTable(int x, int y, int w, int h, const char* l = "", int c = 0)
        : Fl_Table(x, y, w, h, l)
    {
        begin();
        const int col_h = (30);
        row_header(true); // enable row headers (along left)
        col_header_height(24);
        row_header_width(25);
        row_height_all(col_h); // default height of rows
        row_resize(false); // disable row resizing

        cols(c); // how many columns
        col_header(true); // enable column headers (along top)
        col_resize(false); // enable column resizing

        end(); // end the Fl_Table group
    };
private:
    void DrawHeader(const char* label, int X, int Y, int W, int H)
    {
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        fl_color(FL_BLACK);
        fl_draw(label, X, Y, W, H, FL_ALIGN_CENTER);
        fl_pop_clip();
    }

    void DrawData(const char* label, int X, int Y, int W, int H)
    {
        fl_push_clip(X, Y, W, H);

        // draw background
        fl_color(FL_WHITE);
        fl_rectf(X, Y, W, H);

        // draw data
        fl_color(FL_WHITE);
        fl_rectf(X, Y, W, H);

        // Draw box border
        fl_color(color());
        fl_rect(X, Y, W, H);

        fl_draw(label, X, Y, W, H, FL_ALIGN_CENTER);
        fl_pop_clip();
    }

    void draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W,
                   int H) override
    {
        static char label[40];
        switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, 16);
            return;
        case CONTEXT_COL_HEADER:
            DrawHeader(label, X, Y, W, H);
            return;
        case CONTEXT_ROW_HEADER:
            DrawHeader(label, X, Y, W, H);
            return;
        case CONTEXT_CELL:
            DrawData(label, X, Y, W, H);
            return;
        default:
            return;
        }
    }
};

#include "h/DataTable.h"
#include "h/Input.h"
#include "h/SQLiteWrapper.h"

#define DB_DIR "/home/nix/code/phonebook2/data/sqlitedb.db"

enum {
    win_w = 1400,
    win_h = 650,
    win_x = 50,
    win_y = 300,
};

enum { table_w = 1380, table_h = 480, table_x = 10, table_y = win_y - 150 };

int main()
{
    SQLiteWrapper* sw;
    try {
        sw = new SQLiteWrapper(DB_DIR, "PHONEBOOK");
        Fl_Double_Window* win
            = new Fl_Double_Window(win_x, win_y, win_w, win_h);

        DataTable* table
            = new DataTable(table_x, table_y, table_w, table_h, sw);

        SortByInput* inp_p_number = new SortByInput(
            table, spacing_x, 0, mid_button_w, button_h, "Телефон:", 1);

        SortByInput* inp_full_name = new SortByInput(
            table, spacing_x, button_h * 2, big_button_w, button_h, "ФИО:", 2);

        SortByInput* inp_street
            = new SortByInput(table, mid_button_w + spacing_x * 2, 0,
                              mid_big_button_w, button_h, "Улица:", 3);

        SortByInput* inp_house_n
            = new SortByInput(table, big_button_w + spacing_x * 2, button_h * 2,
                              small_buttow_w, button_h, "Номер дома:", 4);

        SortByInput* inp_flat = new SortByInput(
            table, big_button_w + spacing_x * 3 + small_buttow_w, button_h * 2,
            small_buttow_w, button_h, "Квартира: ", 5);

        win->resizable(win);
        win->end();
        win->show();
    }
    catch (std::runtime_error& err) {
        std::cout << "Exception: " << err.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown " << sw->errMesg();
    }
    return Fl::run();
}

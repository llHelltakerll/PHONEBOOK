#include "include/ClearInputButton.h"
#include "include/DataTable.h"
#include "include/DeleteButton.h"
#include "include/DescButton.h"
#include "include/InsertButton.h"
#include "include/SQLiteWrapper.h"
#include "include/TableInputSort.h"
#include "include/UpdateButton.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include <vector>

#define DB_DIR "/home/nix/code/phonebookCurs/data/sqlitedb.db"

enum {
    win_w = 1400,
    win_h = 650,
    win_x = 50,
    win_y = 300,
};

enum { table_w = 1380, table_h = 480, table_x = 10, table_y = win_y - 150 };

int main()
{
    SQLiteWrapper* sw = nullptr;
    try {
        sw = new SQLiteWrapper(DB_DIR, "PHONEBOOK");

        Fl_Window* win = new Fl_Window(win_x, win_y, win_w, win_h,
                                       "Телефонный справочник");

        DataTable* table
            = new DataTable(table_x, table_y, table_w, table_h, sw);

        TableInputSort* inp_p_number = new TableInputSort(
            *table, spacing_x, 0, mid_button_w, button_h, "Телефон:", 1);

        TableInputSort* inp_full_name = new TableInputSort(
            *table, spacing_x, button_h * 2, big_button_w, button_h, "ФИО:", 2);

        TableInputSort* inp_street
            = new TableInputSort(*table, mid_button_w + spacing_x * 2, 0,
                                 mid_big_button_w, button_h, "Улица:", 3);

        TableInputSort* inp_house_n = new TableInputSort(
            *table, big_button_w + spacing_x * 2, button_h * 2,
            small_buttow_w - 50, button_h, "Номер дома:", 4);

        TableInputSort* inp_flat = new TableInputSort(
            *table, big_button_w + spacing_x * 3 + small_buttow_w - 50,
            button_h * 2, small_buttow_w - 50, button_h, "Квартира: ", 5);

        std::vector<Fl_Input*> inp_vec
            = {inp_p_number, inp_full_name, inp_street, inp_house_n, inp_flat};

        ClearInputButton* cbut = new ClearInputButton(
            inp_vec, table, big_button_w + spacing_x * 3,
            button_h + button_h / 2 - 3, small_buttow_w / 1.5, button_h,
            "Очистить ввод");

        InsertButton* ibut = new InsertButton(
            table, big_button_w + small_buttow_w / 1.5 + spacing_x * 4,
            button_h + button_h / 2 - 3, small_buttow_w / 2, button_h,
            "Добавить");

        DeleteButton* dbut = new DeleteButton(
            table,
            big_button_w + (small_buttow_w / 1.5) * 1.7 + spacing_x * 5.5,
            button_h + button_h / 2 - 3, small_buttow_w / 2, button_h,
            "Удалить");

        UpdateButton* ubut = new UpdateButton(
            table, big_button_w + (small_buttow_w / 1.5) * 2.4 + spacing_x * 7,
            button_h + button_h / 2 - 3, small_buttow_w / 2, button_h,
            "Изменить");

        DescButton* descB = new DescButton(
            table, big_button_w + spacing_x * 4 + 195 * 2, button_h * 3.5,
            small_buttow_w / 8, button_h, "По убыванию");

        Fl_PNG_Image image("/home/nix/Изображения/m_Pn7.png");

        Fl_Box box(win_w - 150, button_h, 100, 100);
        box.image(image.copy());

        win->resizable(win);
        win->show();
        win->end();
        return Fl::run();
    }
    catch (std::runtime_error& err) {
        std::cout << "Exception: " << err.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception: " << sw->errMesg();
    }
}

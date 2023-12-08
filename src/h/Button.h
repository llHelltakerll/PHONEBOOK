#pragma once

#include "TableInputSort.h"
#include <FL/Fl_Button.H>
#include <vector>

class OFLButton : public Fl_Button {
public:
    OFLButton(int x, int y, int w, int h, const char* l = "")
        : Fl_Button(x, y, w, h, l)
    {
        callback(CallbackFunc, 0);
    };
    virtual void Press() = 0;
protected:
    static void CallbackFunc(Fl_Widget* w, void* user)
    {
        OFLButton* a_w = reinterpret_cast<OFLButton*>(w);
        a_w->Press();
    }
};

class ClearInputButton : public OFLButton {
public:
    ClearInputButton(std::vector<Fl_Input*>& inp_vec, int x, int y, int w,
                     int h, const char* l = "")
        : OFLButton(x, y, w, h, l), inp_vec(inp_vec)
    {}

    void Press() override { clearInputs(); };
    void clearInputs()
    {
        for (Fl_Input* input: inp_vec) { input->value(""); }
    }
private:
    std::vector<Fl_Input*>& inp_vec;
};

class ConfirmButton : public OFLButton {
public:
    ConfirmButton(Fl_Window* win, DataTable*& table, int x, int y, int w, int h,
                  const char* l = "")
        : OFLButton(x, y, w, h, l), win(win), table(table)
    {}
    void Press() override { Confirm(); };
    void Confirm()
    {
        win->do_callback();
        Fl::delete_widget(win);
    }
private:
    Fl_Window* win;
    DataTable*& table;
};

class ChangeWindow : Fl_Window {
public:
    ChangeWindow(DataTable*& table, int x, int y, int w, int h, const char* l)
        : Fl_Window(x, y, w, h, l), table(table)
    {
        callback(windowCallback);
        begin();
        conf_but
            = new ConfirmButton(this, table, 150, 10, 80, 30, "Подтвердить");
        number_inp = new Fl_Input(0, 10, 100, 30, "телефон:");
        full_name_inp = new Fl_Input(0, 50, 100, 30, "фио:");
        street_inp = new Fl_Input(0, 90, 100, 30, "улица:");
        house_inp = new Fl_Input(0, 130, 100, 30, "дом:");
        flat_inp = new Fl_Input(0, 170, 100, 30, "квартира:");
        set_modal();
        show();
        end();
    }
    ~ChangeWindow()
    {
        delete conf_but;
        delete number_inp;
        delete full_name_inp;
        delete street_inp;
        delete house_inp;
        delete flat_inp;
    }

    void insertInpValues()
    {
        std::string insert_val;
        insert_val += number_inp->value();
        insert_val += " ";
        insert_val += full_name_inp->value();
        insert_val += " ";
        insert_val += street_inp->value();
        insert_val += " ";
        insert_val += house_inp->value();
        insert_val += " ";
        insert_val += flat_inp->value();
        table->insert(insert_val);
    }
private:
    static void windowCallback(Fl_Widget* widget, void* data)
    {
        ChangeWindow* window = static_cast<ChangeWindow*>(widget);
        window->hide();
        window->insertInpValues();

        if (Fl::event() == FL_CLOSE) { Fl::delete_widget(window); }
    }
public:
    Fl_Input* number_inp;
    Fl_Input* full_name_inp;
    Fl_Input* street_inp;
    Fl_Input* house_inp;
    Fl_Input* flat_inp;
    DataTable*& table;
    ConfirmButton* conf_but;
};
class ChangeButton : public OFLButton {
public:
    ChangeButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "")
        : OFLButton(x, y, w, h, l), table(table)
    {}

    void Press() override { CreateWin(); };
    void CreateWin()
    {
        if ((active_row = table->getActiveRow()) == -1) return;
        std::cout << active_row << "\n";
        win = new ChangeWindow(table, 0, 0, 500, 200, "Маленькое окно");
    }
private:
private:
    ChangeWindow* win;
    DataTable*& table;
    int active_row;
};

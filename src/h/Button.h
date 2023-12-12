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
    {
        labelsize(11);
    }

    void Press() override { clearInputs(); };
    void clearInputs()
    {
        for (Fl_Input* input: inp_vec) { input->value(""); }
    }
private:
    std::vector<Fl_Input*>& inp_vec;
};

class DeleteButton : public OFLButton {
public:
    DeleteButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "")
        : OFLButton(x, y, w, h, l), table(table)

    {}

    void Press() override { deleteField(); };
    void deleteField() { table->deleteById(); }
private:
    DataTable*& table;
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

class OFLWindow : public Fl_Window {
public:
    OFLWindow(int x, int y, int w, int h, const char* l)
        : Fl_Window(x, y, w, h, l)
    {
        callback(windowCallback);
    }
    virtual ~OFLWindow(){};
    virtual void Interaction() = 0;
private:
    static void windowCallback(Fl_Widget* w, void* data)
    {
        OFLWindow* a_w = reinterpret_cast<OFLWindow*>(w);
        a_w->Interaction();
    }
};

class InsertWindow : public OFLWindow {
public:
    InsertWindow(DataTable*& table, int x, int y, int w, int h, const char* l)
        : OFLWindow(x, y, w, h, l), table(table)
    {
        begin();
        conf_but
            = new ConfirmButton(this, table, 150, 10, 80, 30, "Подтвердить");
        number_inp = new Fl_Input(0, 10, 100, 30, "телефон:");
        number_inp->value("+ 7 ");
        full_name_inp = new Fl_Input(0, 50, 100, 30, "фио:");
        street_inp = new Fl_Input(0, 90, 100, 30, "улица:");
        house_inp = new Fl_Input(0, 130, 100, 30, "дом:");
        flat_inp = new Fl_Input(0, 170, 100, 30, "квартира:");
        set_modal();
        show();
        end();
    }
    virtual ~InsertWindow()
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
        std::vector<std::string> val_vec;
        val_vec[0] = number_inp->value();
        val_vec[1] += full_name_inp->value();
        val_vec[2] += street_inp->value();
        val_vec[3] += house_inp->value();
        val_vec[4] += flat_inp->value();
        table->update(val_vec);
    }
private:
    void Interaction() override
    {
        hide();
        if (Fl::event() == FL_CLOSE) { Fl::delete_widget(this); }
        else {
            insertInpValues();
        }
    }
protected:
    Fl_Input* number_inp;
    Fl_Input* full_name_inp;
    Fl_Input* street_inp;
    Fl_Input* house_inp;
    Fl_Input* flat_inp;
    DataTable*& table;
    ConfirmButton* conf_but;
};

class UpdateWindow : public InsertWindow {
public:
    UpdateWindow(DataTable*& table, int x, int y, int w, int h, const char* l)
        : InsertWindow(table, x, y, w, h, l)
    {
        number_inp->value(table->getNumberByRow(table->getActiveRow()).c_str());
        full_name_inp->value(table->getFNByRow(table->getActiveRow()).c_str());
        street_inp->value(table->getStreetByRow(table->getActiveRow()).c_str());
        house_inp->value(table->getHNByRow(table->getActiveRow()).c_str());
        flat_inp->value(table->getFlatByRow(table->getActiveRow()).c_str());
    }

    void updateInpValues()
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
        std::cout << insert_val << "\n";
    }
private:
    void Interaction() override
    {
        hide();
        if (Fl::event() == FL_CLOSE) { Fl::delete_widget(this); }
        else {
            insertInpValues();
        }
    }
};

class InsertButton : public OFLButton {
public:
    InsertButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "")
        : OFLButton(x, y, w, h, l), table(table)
    {}

    void Press() override { CreateWin(); };
    void CreateWin()
    {
        std::cout << active_row << "\n";
        win = new InsertWindow(table, 0, 0, 500, 200, "Маленькое окно");
    }
private:
private:
    InsertWindow* win;
    DataTable*& table;
    int active_row;
};

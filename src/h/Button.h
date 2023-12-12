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
        clear_visible_focus();
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
    ClearInputButton(std::vector<Fl_Input*>& inp_vec, DataTable*& table, int x,
                     int y, int w, int h, const char* l = "")
        : OFLButton(x, y, w, h, l), inp_vec(inp_vec), table(table)
    {}

    void Press() override { clearInputs(); };
    void clearInputs()
    {
        for (Fl_Input* input: inp_vec) { input->value(""); }
        table->refreshTable();
    }
private:
    DataTable*& table;
    std::vector<Fl_Input*>& inp_vec;
};

class DeleteButton : public OFLButton {
public:
    DeleteButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "")
        : OFLButton(x, y, w, h, l), table(table)

    {}

    void Press() override { deleteField(); };
    void deleteField()
    {
        if (table->getActiveRow() == -1) return;
        table->deleteById();
    }
private:
    DataTable*& table;
};

class ConfirmButton : public OFLButton {
public:
    ConfirmButton(Fl_Window* win, int x, int y, int w, int h,
                  const char* l = "")
        : OFLButton(x, y, w, h, l), win(win)
    {}
    void Press() override { Confirm(); };
    void Confirm()
    {
        win->do_callback();
        std::cout << "DELETE WIN\n\n";
        Fl::delete_widget(win);
    }
private:
    Fl_Window* win;
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

class ErrorOkWindow : public OFLWindow {
public:
    ErrorOkWindow(int x, int y, int w, int h, const char* l)
        : OFLWindow(x, y, w, h, l)
    {
        begin();
        label = new Fl_Box(150, 50, 260, 100,
                           "Ошибка! Введен дубликат номера телефона");
        conf_but = new ConfirmButton(this, 150, 10, 80, 30, "Ok");
        set_modal();
        show();
        end();
    };

    ~ErrorOkWindow()
    {
        std::cout << "DELETE WIN\n";
        delete conf_but;
        delete label;
    }
    void Interaction() override
    {
        hide();
        if (Fl::event() == FL_CLOSE) {
            std::cout << "DEL BY CLOSE\n";
            Fl::delete_widget(this);
        }
    }
private:
    ConfirmButton* conf_but;
    Fl_Box* label;
};

class InsertWindow : public OFLWindow {
public:
    InsertWindow(DataTable*& table, int x, int y, int w, int h, const char* l)
        : OFLWindow(x, y, w, h, l), table(table)
    {
        begin();
        conf_but = new ConfirmButton(this, 150, 10, 80, 30, "Подтвердить");
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
        std::vector<std::string> val_vec(table->sw->getRowCount());
        val_vec[1] = number_inp->value();
        val_vec[2] = full_name_inp->value();
        val_vec[3] = street_inp->value();
        val_vec[4] = house_inp->value();
        val_vec[5] = flat_inp->value();
        table->insertField(val_vec);
    }
private:
    void Interaction() override
    {
        std::string number_inp_str = number_inp->value();
        std::string full_name_inp_str = full_name_inp->value();
        std::string street_inp_str = street_inp->value();
        std::string house_inp_str = house_inp->value();
        std::string flat_inp_str = flat_inp->value();
        // if (number_inp_str.empty() || full_name_inp_str.empty()
        //     || street_inp_str.empty() || house_inp_str.empty()
        //     || flat_inp_str.empty()) {
        //     return;
        // };
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
        std::vector<std::string> val_vec(table->sw->getRowCount());
        val_vec[1] = number_inp->value();
        val_vec[2] = full_name_inp->value();
        val_vec[3] = street_inp->value();
        val_vec[4] = house_inp->value();
        val_vec[5] = flat_inp->value();
        table->updateField(val_vec);
    }
private:
    void Interaction() override
    {
        hide();
        if (Fl::event() == FL_CLOSE) { Fl::delete_widget(this); }
        else {
            updateInpValues();
        }
    }
};
class UpdateButton : public OFLButton {
public:
    UpdateButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "")
        : OFLButton(x, y, w, h, l), table(table)
    {}

    void Press() override { CreateWin(); };
    void CreateWin()
    {
        if (table->getActiveRow() == -1) { return; }
        std::cout << active_row << "\n";
        win = new UpdateWindow(table, 0, 0, 500, 200, "Изменение");
    }
private:
private:
    InsertWindow* win;
    DataTable*& table;
    int active_row;
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

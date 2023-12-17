#include "include/ErrorOkWindow.h"
#include "include/InsertWindow.h"

ErrorOkWindow::ErrorOkWindow(int x, int y, const char* err_mesg, const char* l)
    : OFLWindow(x, y, err_win_w, err_win_h, l)
{
    begin();
    label = new Fl_Box(180, 50, 0, 0, err_mesg);
    conf_but = new ConfirmButton(this, err_win_w / 2 - conf_button_w / 2,
                                 err_win_h / 3 + button_h, conf_button_w,
                                 button_h, "Ok");
    set_modal();
    show();
    end();
};

ErrorOkWindow::~ErrorOkWindow()
{
    std::cout << "DELETE WIN\n";
    delete conf_but;
    delete label;
}
void ErrorOkWindow::Interaction()
{
    hide();
    if (Fl::event() == FL_CLOSE) {
        std::cout << "DEL BY CLOSE\n";
        Fl::delete_widget(this);
    }
}

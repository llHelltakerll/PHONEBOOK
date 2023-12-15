#include "include/ErrorOkWindow.h"

ErrorOkWindow::ErrorOkWindow(int x, int y, int w, int h, const char* err_mesg,
                             const char* l)
    : OFLWindow(x, y, w, h, l)
{
    begin();
    label = new Fl_Box(50, 20, 260, 0, err_mesg);
    conf_but = new ConfirmButton(this, 170, 60, 80, 30, "Ok");
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

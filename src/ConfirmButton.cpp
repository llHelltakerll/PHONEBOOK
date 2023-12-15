#include "include/ConfirmButton.h"
ConfirmButton::ConfirmButton(Fl_Window* win, int x, int y, int w, int h,
                             const char* l)
    : OFLButton(x, y, w, h, l), win(win)
{}
void ConfirmButton::Press()
{
    Confirm();
};
void ConfirmButton::Confirm()
{
    win->do_callback();
    std::cout << "DELETE WIN\n\n";
    Fl::delete_widget(win);
}

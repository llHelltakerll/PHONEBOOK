#include "h/OFLCheckButton.h"

OFLCheckButton::OFLCheckButton(int x, int y, int w, int h, const char* l)
    : Fl_Check_Button(x, y, w, h, l)
{
    box(FL_NO_BOX);
    labelsize(check_b_label_fontsz);
    align(FL_ALIGN_RIGHT);
    callback(CallBackFunction, 0);
}

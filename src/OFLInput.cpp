#include "h/OFLInput.h"

OFLInput::OFLInput(int x, int y, int w, int h, const char* lb)
    : Fl_Input(x, y, w, h, lb)
{
    textsize(text_fontsz);
    align(FL_ALIGN_TOP_LEFT);
    labelsize(input_label_fontsz);
    when(FL_WHEN_CHANGED);
    callback(CallBackFunction, 0);
}

OFLInput ::~OFLInput(){};

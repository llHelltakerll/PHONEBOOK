#pragma once
#include <FL/Fl_Input.H>

enum { text_fontsz = 18 };
enum { input_label_fontsz = 16 };

class OFLInput : public Fl_Input {
public:
    OFLInput(int x, int y, int w, int h, const char* lb);

    virtual ~OFLInput();

    virtual void Input() = 0;
private:
    static void CallBackFunction(Fl_Widget* w, void* user)
    {
        OFLInput* a_w = reinterpret_cast<OFLInput*>(w);
        a_w->Input();
    }
};

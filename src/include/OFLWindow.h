#pragma once
#include <FL/Fl_Window.H>

class OFLWindow : public Fl_Window {
public:
    OFLWindow(int x, int y, int w, int h, const char* l);
    virtual ~OFLWindow() = default;
    virtual void Interaction() = 0;
private:
    static void windowCallback(Fl_Widget* w, void* data)
    {
        OFLWindow* a_w = reinterpret_cast<OFLWindow*>(w);
        a_w->Interaction();
    }
};

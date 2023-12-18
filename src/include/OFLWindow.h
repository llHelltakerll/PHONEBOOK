#pragma once
#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <iostream>

class OFLWindow : public Fl_Window {
public:
    OFLWindow(int x, int y, int w, int h, const char* l);
    virtual ~OFLWindow() = default;
    virtual void Interaction() = 0;
private:
    static void windowCallback(Fl_Widget* w, void* data)
    {
        if (Fl::event_key() == FL_Escape && Fl::event() != FL_CLOSE) { return; }

        OFLWindow* a_w = reinterpret_cast<OFLWindow*>(w);
        a_w->Interaction();
    }
};

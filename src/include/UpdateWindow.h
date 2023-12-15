#pragma once

#include "InsertWindow.h"

class UpdateWindow : public InsertWindow {
public:
    UpdateWindow(DataTable*& table, int x, int y, int w, int h, const char* l);
    void updateInpValues();
private:
    void Interaction() override;
};

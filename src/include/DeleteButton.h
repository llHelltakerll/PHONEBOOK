#pragma once

#include "DataTable.h"
#include "OFLButton.h"

class DeleteButton : public OFLButton {
public:
    DeleteButton(DataTable*& table, int x, int y, int w, int h,
                 const char* l = "");

    void Press() override;
    void deleteField();
private:
    DataTable*& table;
};

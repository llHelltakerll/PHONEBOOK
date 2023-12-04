#pragma once
#include "DataTable.h"
#include "OFLCheckButton.h"

class DescButton : public OFLCheckButton {
public:
    DescButton(DataTable*& table, int x, int y, int w, int h, const char* l);
private:
    virtual void Desc() const;
    void CheckBox() override;
private:
    DataTable* table;
};

#pragma once
#include "DataTable.h"
#include "OFLInput.h"

class TableInputSort : public OFLInput {
public:
    TableInputSort(DataTable& table, int x, int y, int w, int h,
                   const char* lb = "", int sort = 0);
private:
    virtual void SortByVal() const;
    void Input() override;
private:
    const int sort;
    DataTable& table;
};

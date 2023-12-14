#include "DataTable.h"
#include "OFLButton.h"
#include <FL/Fl_Input.H>
#include <vector>

class ClearInputButton : public OFLButton {
public:
    ClearInputButton(std::vector<Fl_Input*>& inp_vec, DataTable*& table, int x,
                     int y, int w, int h, const char* l = "");

    void Press() override;
    void clearInputs();
private:
    DataTable*& table;
    std::vector<Fl_Input*>& inp_vec;
};

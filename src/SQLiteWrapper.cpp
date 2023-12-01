#include "h/SQLiteWrapper.h"

SQLiteWrapper::SQLiteWrapper(const char* dir, std::string table_name,
                             int flags, //
                             const char* vfsName)
    : table_name(table_name)
{
    int rc = sqlite3_open_v2(dir, &db, flags, vfsName); // открывает базу данных
    if (rc) { throw std::runtime_error(errMesg()); }
    setRowCount();
    setColNames();
    col_count = col_name_vec.size() - 1;
    std::string default_sort_query = "SELECT * FROM \"main\".\"" + table_name
                                     + "\"  ORDER BY "
                                       "\""
                                     + col_name_vec[0]
                                     + "\" ASC LIMIT 0, 49999";
    prepare(default_sort_query);
}

SQLiteWrapper::~SQLiteWrapper()
{
    final();
    sqlite3_close(db);
}

const char* SQLiteWrapper::errMesg()
{
    return sqlite3_errmsg(db);
}

void SQLiteWrapper::prepare(std::string comd)
{
    static int i;
    if (i == 0) {
        final();
        i++;
    }
    int rc = sqlite3_prepare_v2(db, comd.c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) { throw std::runtime_error(errMesg()); }
}

int SQLiteWrapper::step() const
{
    return sqlite3_step(stmt) == SQLITE_ROW;
}

void SQLiteWrapper::final() const
{
    sqlite3_finalize(stmt);
}

int SQLiteWrapper::copyColInt(int col) const
{
    return sqlite3_column_int(stmt, col);
}

std::string SQLiteWrapper::getColText(int col) const
{
    return reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
}

void SQLiteWrapper::sortBy(int where_col, std::string val, int order_col,
                           bool invert)
{
    std::string invert_comd = "";
    if (invert) { invert_comd = "DESC"; }

    sort_query = "SELECT * FROM main." + table_name + " WHERE "
                 + col_name_vec[where_col] + " LIKE '" + val
                 + "%' "
                   "ORDER BY "
                 + col_name_vec[order_col] + " " + invert_comd
                 + " LIMIT 0, 49999;";
    // }
    // else {
    //     sort_query = "SELECT * FROM \"main\".\"" + table_name
    //                  + "\"  ORDER BY "
    //                    "\""
    //                  + col_name_vec[order_col] + "\" ASC LIMIT 0, 49999";
    // }
    std::cout << sort_query << "\n";
    prepare(sort_query);
}
void SQLiteWrapper::setRowCount()
{
    prepare(count_query);
    if (step()) { row_count = copyColInt(0); }
}

void SQLiteWrapper::setColNames()
{
    const std::string pragmaQuery
        = "PRAGMA table_info(" + std::string(table_name) + ")";
    prepare(pragmaQuery);

    while (step()) { col_name_vec.push_back(getColText(1)); }

    final();
}
std::string SQLiteWrapper::addQuotesAndCommas(const std::string& input)
{
    std::istringstream stream(input);
    std::string word;
    std::string result;

    while (stream >> word) {
        result += "'" + word + "'";
        if (stream.peek() != EOF) { result += ", "; }
    }

    return result;
}

void SQLiteWrapper::insert(std::string cells_name)
{
    cells_name = addQuotesAndCommas(cells_name);
    std::string cols_name;
    for (int i = 1; i < col_name_vec.size(); i++) {
        cols_name += col_name_vec[i] + ", ";
    }
    cols_name[cols_name.size() - 2] = ' ';
    std::cout << cols_name;
    std::string insert_data_query
        = "INSERT INTO " + table_name + " (ID, " + cols_name + ") VALUES ("
          + std::to_string(getRowCount() + 1) + ", " + cells_name
          + ""
            ");";
    std::cout << insert_data_query << "\n";
    exec(insert_data_query);
    setRowCount();
}

void SQLiteWrapper::update(int id, int col, std::string str)
{
    if (col == 0) {
        std::cerr << "Can`t change id column\n";
        return;
    }
    if (id > col_name_vec.size()) {
        std::cerr << "No such id\n";
        return;
    }
    std::string update_query = "UPDATE " + table_name + " SET "
                               + col_name_vec[col] + " = '" + str
                               + "' WHERE "
                                 "ID = '"
                               + std::to_string(id) + "';";
    exec(update_query);
}

void SQLiteWrapper::exec(std::string query)
{
    int rc = sqlite3_exec(db, query.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) throw std::runtime_error(errMesg());
}

void SQLiteWrapper::deleteById(int id)
{
    std::string delete_data_query = "DELETE FROM " + table_name
                                    + " WHERE ID = " + std::to_string(id) + ";";

    exec(delete_data_query);
    setRowCount();
}

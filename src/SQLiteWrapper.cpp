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
    std::string invert_comd;
    const char* previousLocale = std::setlocale(LC_CTYPE, nullptr);
    std::setlocale(LC_CTYPE, "ru_RU.UTF-8");

    std::size_t len = std::mbstowcs(nullptr, val.c_str(), 0);
    wchar_t* wbuffer = new wchar_t[len + 1];
    std::mbstowcs(wbuffer, val.c_str(), len);
    wbuffer[len] = L'\0';
    std::wstring wstr(wbuffer);

    // Используйте другую строку для хранения результата
    std::wstring result;

    for (wchar_t ch: wstr) {
        // Добавление символа в квадратные скобки
        result += L"[";
        result += std::towupper(ch); // Приведение к нижнему регистру
        result += std::tolower(ch); // Приведение к верхнему регистру
        result += L"]";
    }

    // Освобождение буфера
    delete[] wbuffer;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    val = converter.to_bytes(result);
    // std::cout << val << std::endl;

    if (invert) { invert_comd = "DESC"; }
    input_values[where_col] = val;
    std::string where = " WHERE ";
    for (int i = 1; i <= getColCount(); i++) {
        where += col_name_vec[i];
        where += " GLOB '*";
        where += input_values[i];
        where += "*'";
        if (i == getColCount()) break;
        where += " AND ";
    };
    sort_query = "SELECT * FROM main." + table_name + where
                 + " COLLATE UNICODE "
                   "ORDER BY "
                 + col_name_vec[order_col] + " " + invert_comd
                 + " LIMIT 0, 49999;";
    std::setlocale(LC_CTYPE, previousLocale);
    // std::cout << sort_query << "\n";
    prepare(sort_query);
    setPrevQuery(sort_query);
}

void SQLiteWrapper::doPrevQuary()
{
    if (prev_query.empty()) { sortBy(0); }
    prepare(prev_query);
}
void SQLiteWrapper::setRowCount()

{
    prepare(count_query);
    if (step()) { row_count = copyColInt(0); }
    input_values.resize(row_count);
}

void SQLiteWrapper::setColNames()
{
    const std::string pragmaQuery
        = "PRAGMA table_info(" + std::string(table_name) + ")";
    prepare(pragmaQuery);

    for (int i = 0; step();) { col_name_vec.push_back(getColText(1)); }

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
    std::string insert_data_query = "INSERT INTO " + table_name + " ("
                                    + cols_name + " ) VALUES (" + cells_name
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

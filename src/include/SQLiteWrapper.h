#pragma once
#include <codecvt>
#include <iostream>
#include <locale>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <vector>

class SQLiteWrapper { // Класс оболчка для базы данных SQLite3
public:
    SQLiteWrapper(const char* dir, std::string table_name,
                  int flags = SQLITE_OPEN_READWRITE,
                  const char* vfsName = "unix-excl");

    ~SQLiteWrapper();

    const char* errMesg();

    int insert(std::vector<std::string> vec_str);

    bool recordExists(std::string& query);

    int update(int id, int col, std::string str);

    int exec(const std::string& query);

    void doPrevQuary();

    void deleteById(int id);

    void prepare(std::string comd);

    int step() const;

    void final() const;

    int copyColInt(int col) const;

    std::string getColText(int col) const;

    inline int getRowCount() const { return row_count; }

    inline int getColCount() const { return col_count; }

    inline std::string getColName(int c) { return col_name_vec[c]; }

    void sortBy(int order_col = 0, std::string val = "", int where_col = 0,
                bool invert = false, bool clear = false,
                bool cast_to_int = false);
private:
    void setRowCount();

    void setPrevQuery(std::string query) { prev_query = query; }

    std::string addQuotesAndCommas(const std::string& input);

    void setColNames();
private:
    sqlite3* db = nullptr; // дескриптор для работы с базой данных
    sqlite3_stmt* stmt;

    int row_count = 0;
    int col_count = 0;

    std::vector<std::string> col_name_vec;
    std::vector<std::string> input_values;

    std::string prev_query;
    std::string table_name;
    std::string sort_query;
    std::string count_query = "SELECT COUNT(*) FROM main." + table_name;
};

#include "sql/Strings.hpp"

#include "Util/Strcat.hpp"

BEGIN_PYRAMID_SQL_NAMESPACE


/*****************************************************/
/****************DB-Specific Methods******************/
/*****************************************************/

std::string Strings::CreateTables::indexType() const
{
    return "INTEGER PRIMARY KEY AUTOINCREMENT";
}

std::string Strings::SQLiteCreateTables::indexType() const
{
    //don't necessarily need an autoincrementing rowid in sqlite
    //not making the rowid autoincrement allows previous ids to be reused
    //see https://www.sqlite.org/autoinc.html
    return "INTEGER PRIMARY KEY";
}

std::string Strings::PostgresCreateTables::indexType() const
{
    return "SERIAL PRIMARY KEY";
}

/*****************************************************/
/******************Table Methods**********************/
/*****************************************************/

IMPLEMENT_TABLE_METHOD(Ticks, 
        STRCAT("CREATE TABLE Ticks(tick", indexType(), ")"))


//TODO
IMPLEMENT_TABLE_METHOD(Config, 
        "")

IMPLEMENT_TABLE_METHOD(CapitalHolder, 
        STRCAT("CREATE TABLE CapitalHolder(id", indexType(), ","
            ""))

IMPLEMENT_TABLE_METHOD(Inventory, 
        STRCAT("CREATE TABLE Inventory(id", indexType(), ",",
            "inventory INTEGER NOT NULL, ",
            "FOREIGN KEY(id) REFERENCES CapitalHolder(id))"))

END_PYRAMID_SQL_NAMESPACE

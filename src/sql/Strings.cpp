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

std::string Strings::CreateTables::moneyType() const
{
    return "REAL";
}

std::string Strings::PostgresCreateTables::moneyType() const
{
    return "NUMERIC(15, 6)";
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
        STRCAT("CREATE TABLE CapitalHolder(id", indexType(), ", "
            "guid CHAR(16) UNIQUE NOT NULL)"))

IMPLEMENT_TABLE_METHOD(Inventory, 
        STRCAT("CREATE TABLE Inventory(",
            "id", indexType(), ",",
            "inventory INTEGER NOT NULL, ",
            "FOREIGN KEY(id) REFERENCES CapitalHolder(id))"))

IMPLEMENT_TABLE_METHOD(StartingDistributors, 
        STRCAT("CREATE TABLE StartingDistributors(",
            "id INTEGER UNIQUE NOT NULL, ",
            "FOREIGN KEY(id) REFERENCES CapitalHolder(id))"))

IMPLEMENT_TABLE_METHOD(Sales, 
        STRCAT("CREATE TABLE Sales(",
            "id ", indexType(), ", ",
            "buyer INTEGER NOT NULL, ",
            "seller INTEGER NOT NULL, ",
            "when INTEGER NOT NULL, ",
            "FOREIGN KEY(buyer) REFERENCES CapitalHolder(id), ",
            "FOREIGN KEY(seller) REFERENCES CapitalHolder(id), ",
            "FOREIGN KEY(when) REFERENCES Ticks(tick), ",
            "CHECK(buyer <> seller))"))

IMPLEMENT_TABLE_METHOD(Conversions, 
        STRCAT("CREATE TABLE Conversions(",
            "id ", indexType(), ", ",
            "converted INTEGER NOT NULL, ",
            "convertedBy INTEGER NOT NULL, ",
            "when INTEGER NOT NULL, ",
            "FOREIGN KEY(converted) REFERENCES CapitalHolder(id), ",
            //TODO: consider a distributors table or view
            "FOREIGN KEY(convertedBy) REFERENCES CapitalHolder(id), ",
            "FOREIGN KEY(when) REFERENCES Ticks(tick), ",
            "CHECK(converted <> convertedBy))"))

END_PYRAMID_SQL_NAMESPACE

#pragma once

#include "NamespaceDefines.hpp"

#include <string>

BEGIN_PYRAMID_SQL_NAMESPACE

#define TABLE_METHOD(name) virtual std::string create ## name ## Table() const;
#define IMPLEMENT_TABLE_METHOD(name, body) \
    std::string Strings::CreateTables::create ## name ## Table() const { return std::string(body); }

class Strings 
{
public:
    class CreateTables
    {
    protected:
        virtual std::string indexType() const;
    public:
        TABLE_METHOD(Ticks);
        TABLE_METHOD(Config);
        TABLE_METHOD(CapitalHolder);
        TABLE_METHOD(Inventory);
        TABLE_METHOD(StartingDistributors);
        TABLE_METHOD(Sales);

        virtual ~CreateTables() {}
    };

    class SQLiteCreateTables : public CreateTables
    {
    protected:
        virtual std::string indexType() const override;
    };

    class PostgresCreateTables : public CreateTables
    {
    protected:
        virtual std::string indexType() const override;
    };

    virtual ~Strings() {}
};

END_PYRAMID_SQL_NAMESPACE

#pragma once

#include "NamespaceDefines.hpp"

#include <string>

BEGIN_PYRAMID_SQL_NAMESPACE

#define TABLE_METHOD(name) virtual std::string nameTable() const;

class Strings 
{
public:
    class CreateTables
    {
    public:
        virtual std::string createTicksTable() const;
        virtual std::string createConfigTable() const;
        virtual std::string createCapitalHolderTable() const;
        virtual std::string createInventoryTable() const;
        virtual std::string createStartingDistributorsTable() const;
        virtual std::string createSalesTable() const;

        virtual ~CreateTables() {}
    };

    virtual ~Strings() {}
};

END_PYRAMID_SQL_NAMESPACE

#pragma once

#include "Util/PrettyPrintable.hpp"

#include "Guid.hpp"
#include "Types.hpp"

#include <unordered_set>
#include <initializer_list>
#include <vector>
#include <string>
#include <array>

static bool operator<(const xg::Guid& lhs, const xg::Guid& rhs) 
{
    return lhs.str() < rhs.str();
}

namespace pyramid_scheme_simulator {
class UniqueSet;
class Unique;
class Uniqueable;
}

//forward declare hash template specialization for Unique
namespace std {
    template <>
    struct hash<pyramid_scheme_simulator::Unique>
    {
    public:
        size_t operator()(pyramid_scheme_simulator::Unique const &) const;
    };
}

namespace pyramid_scheme_simulator {

class UniqueSet
{
    std::unordered_set<xg::Guid> transformInitializerList(std::initializer_list<UniqueSet> us)
    {
        std::unordered_set<xg::Guid> acc;
        //insert all of the ids from each of the UniqueSets
        for(auto i : us)
        {
            acc.insert(i.guids.begin(), i.guids.end());
        }
        return acc;
    }

protected:
    std::unordered_set<xg::Guid> guids;

    //make sure the set is always initialized with the Compare
    //object
    UniqueSet(): guids({})
    {}

public:
    UniqueSet(std::initializer_list<xg::Guid> us) : guids({})
    {
        guids.insert(us.begin(), us.end());
    }

    UniqueSet(std::initializer_list<UniqueSet> us) 
        : guids(transformInitializerList(us))
    {}

    UniqueSet(xg::Guid guid): UniqueSet()
    {
        guids.insert(guid);
    }


    template <typename InputIt>
    UniqueSet(InputIt begin, InputIt end)
    : UniqueSet()
    {
        guids.insert(begin, end);
    }

    virtual ~UniqueSet() {}

    /** the comparison functions will work equally well when the 
     * set only has one element*/
    virtual bool operator==(const UniqueSet &other) const
    {
        return guids == other.guids;
    }
    virtual bool operator!=(const UniqueSet &other) const
    {
        return guids != other.guids;
    }

    static UniqueSet newUniqueSet() { 
        return UniqueSet(xg::newGuid()); 
    }
    static const UniqueSet& emptyUniqueSet;
};

class Unique : public UniqueSet, public PrettyPrintable
{
public:
    const xg::Guid id;

    static const Unique emptyUnique;
    static Unique newUnique() { 
        return Unique(xg::newGuid()); 
    }

    Unique(xg::Guid guid): UniqueSet(guid), id(guid)
    {}

    Unique(const std::array<unsigned char, 16> values): id(values)
    {}
    
    Unique(const Unique& u): UniqueSet(u.id), id(u.id)
    {}

    Unique(Unique&& u)
        : Unique(std::move(u.id))
    {}

    Unique(): UniqueSet()
    {}

    virtual ~Unique() {}

    std::string str() const {
        return id.str();
    }

    //a somewhat shorter but still identifying string
    std::string print() const;

    bool operator<(const Unique&) const;

    virtual std::string prettyPrintImpl() const override;
};

std::ostream& operator<<(std::ostream& os, const Unique& res);

namespace {

    UniqueSet uniqueSetFromUniqueInitializerList(
            std::initializer_list<Unique> us)
    {
        std::vector<xg::Guid> ids;
        ids.reserve(us.size());

        for(auto i : us)
        {
            ids.push_back(i.id);
        }

        return UniqueSet(ids.begin(), ids.end());
    }
}


/**
 * a trait for classes with Unique IDs
 */
class Uniqueable
{
public:
    const Unique id;
    Uniqueable() : id(xg::newGuid()) {}
    Uniqueable(Unique x) : id(x) {}
    virtual ~Uniqueable() {}

    std::string prettyPrintId() const
    {
        return id.print();
    }
};

}


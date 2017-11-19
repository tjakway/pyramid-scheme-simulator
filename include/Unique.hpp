#pragma once

#include "Guid.hpp"
#include "Types.hpp"

#include <set>
#include <initializer_list>

namespace pyramid_scheme_simulator {

class Uniqueable;

class UniqueSet
{
    class GuidComparator
    {
        bool operator()(xg::Guid& lhs, xg::Guid& rhs)
        {
            return lhs.str() < rhs.str();
        }
    } guidComparator;

    std::set<xg::Guid> transformInitializerList(std::initializer_list<UniqueSet> us)
    {
        std::set<xg::Guid> acc({}, guidComparator);
        //insert all of the ids from each of the UniqueSets
        for(auto i : us)
        {
            acc.insert(i.guids.begin(), i.guids.end());
        }
    }

protected:
    std::set<xg::Guid> guids;

    //make sure the set is always initialized with the Compare
    //object
    UniqueSet(): guids({}, guidComparator)
    {}

public:
    UniqueSet(std::initializer_list<xg::Guid> us) : guids(us, guidComparator)
    {}

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


    virtual bool operator==(const UniqueSet &other) const
    {
        return guids == other.guids;
    }
    virtual bool operator!=(const UniqueSet &other) const
    {
        return guids != other.guids;
    }

    virtual bool operator<(const UniqueSet &other) const
    {
        return guids < other.guids;
    }
    virtual bool operator<=(const UniqueSet &other) const
    {
        return guids <= other.guids;
    }
    virtual bool operator>(const UniqueSet &other) const
    {
        return guids > other.guids;
    }
    virtual bool operator>=(const UniqueSet &other) const
    {
        return guids >= other.guids;
    }

    static UniqueSet newUniqueSet() { 
        return UniqueSet(xg::newGuid()); 
    }
    static const UniqueSet emptyUniqueSet = UniqueSet(xg::Guid());
};

class Unique : public UniqueSet
{
public:
    Unique(xg::Guid guid): UniqueSet(guid), id(guid)
    {}

    const xg::Guid id;

    /** the comparison functions will work equally well when the 
     * set only has one element*/
};


/**
 * a trait for classes with Unique IDs
 */
class Uniqueable 
{
public:
    const UniqueSet id;
    Uniqueable() : id(xg::newGuid()) {}
    Uniqueable(Unique x) : id(x) {}
    Uniqueable(std::initializer_list<Unique> xs)
        : id(xs)
    {}
};

}

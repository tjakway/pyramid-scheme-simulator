#pragma once

#include "Guid.hpp"
#include "Types.hpp"

#include <set>
#include <initializer_list>
#include <vector>

namespace std
{
    template<>
    struct less<xg::Guid>
    {
    public:
        bool operator()(const xg::Guid& lhs,  const xg::Guid& rhs) const
        {
            return lhs.str() < rhs.str();
        }
    };
}

namespace pyramid_scheme_simulator {

class UniqueSet;
class Unique;
class Uniqueable;

class UniqueSet
{
    class GuidComparator
    {
    public:
        bool operator()(xg::Guid& lhs, xg::Guid& rhs)
        {
            return lhs.str() < rhs.str();
        }
        bool operator()(const xg::Guid& lhs, const xg::Guid& rhs)
        {
            return lhs.str() < rhs.str();
        }
    };



    using UniqueSetCollection = std::set<xg::Guid>;


    UniqueSetCollection transformInitializerList(std::initializer_list<UniqueSet> us)
    {
        UniqueSetCollection acc;
        //insert all of the ids from each of the UniqueSets
        for(auto i : us)
        {
            acc.insert(i.guids.begin(), i.guids.end());
        }
        return acc;
    }

protected:
    UniqueSetCollection guids;

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
    static const UniqueSet& emptyUniqueSet;
};

class Unique : public UniqueSet
{
public:
    const xg::Guid id;

    Unique(xg::Guid guid): UniqueSet(guid), id(guid)
    {}
    
    Unique(const Unique& u): UniqueSet(u.id), id(u.id)
    {}

    /** the comparison functions will work equally well when the 
     * set only has one element*/


    static const Unique emptyUnique;
};

namespace {

    UniqueSet uniqueSetFromUniqueInitializerList(
            std::initializer_list<Unique> us)
    {
        std::vector<xg::Guid> ids(us.size());
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
    /*Uniqueable(std::initializer_list<Unique> xs)
        : id(uniqueSetFromUniqueInitializerList(xs))
    {}*/
};

}

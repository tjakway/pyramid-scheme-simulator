#pragma once

#include "Util/PrettyPrintUniqueable.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class ChanceContributor : public PrettyPrintUniqueable
{
public:
    virtual double getChance(rd_ptr) = 0;

    /**
     * convenience method that returns whether or not the chance "happened"
     * according to the distribution governed by the ChanceContributor
     */
    bool sampleFrom(rd_ptr);

    virtual std::unique_ptr<ChanceContributor> clone() const = 0;

    std::unique_ptr<ChanceContributor> operator+(const ChanceContributor& other);

    virtual ~ChanceContributor() {}

private:
    class Intersection;
};

//see 
//https://stackoverflow.com/questions/1485985/can-a-nested-c-class-inherit-its-enclosing-class
//for inner classes inheriting from outer
/**
 * combining 2 ChanceContributors gives the intersection of their chances
 */
class ChanceContributor::Intersection : public ChanceContributor
{
    std::unique_ptr<ChanceContributor> firstSource, secondSource; 
public:
    Intersection(std::unique_ptr<ChanceContributor> a, 
            std::unique_ptr<ChanceContributor> b)
        : firstSource(std::move(a)), secondSource(std::move(b)) {}

    virtual double getChance(rd_ptr) override;
    virtual std::unique_ptr<ChanceContributor> clone() const override;

    virtual ~Intersection() {}
};

/**
 * a ChanceContributor that doesn't select from any distribution
 * but just uses the chance passed to its constructor
 */
class StaticChanceContributor : public ChanceContributor
{
    double chance;
public:
    StaticChanceContributor(double staticChance);

    virtual double getChance(rd_ptr) override;
    virtual std::unique_ptr<ChanceContributor> clone() const override;

    virtual ~StaticChanceContributor() {}
};

/**
 * most trivial instance of a ChanceContributor: has no state, just rolls the dice
 */
class RandomChanceContributor : public ChanceContributor
{
public:
    RandomChanceContributor() {}
    virtual double getChance(rd_ptr) override;

    virtual std::unique_ptr<ChanceContributor> clone() const override;
    virtual ~RandomChanceContributor() {}
};

}

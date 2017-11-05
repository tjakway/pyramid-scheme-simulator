#pragma once

#include <memory>
#include <random>
#include <functional>

/**
 * simulation parameters
 * see doc for explanation
 *
 * TODO: CTOR with preamble
 * TODO: for C++ random number distributions, see  C++ concepts: RandomNumberDistribution
 *  [numeric/random/*]
 */
class Config
{
//private internal variables
    const int randomSeed;

//public config values
public:
    const std::unique_ptr<std::mt19937_64> randomGen;

    const unsigned long graphSize;

    //how likely are people to have connections to other people?
    const double graphConnectedness;

    const bool allowUnconnectedSubgraphs;

    //corresponding to the maximum number of people someone
    //can maintain social relationships with
    //(hard to have 1000 best friends)
    const unsigned int maxEdgesPerNode;

    //how are sales skills distributed throughout the population?
    //random guess: normal distribution?
    const std::function<double()> salesSkillDistribution;
    
    /**
     * Distribution of funds among the general population
     * reflecting “capitalization”: how much capital can someone wield, i.e. assets + loans
     * people whose capitalization is lower than:
     *  the amount needed to become consumers can’t become consumers
     *  the amount needed to become distributors can’t become distributors
    */
    //const ??? fundsDistribution;
    

    /**
     * function of capitalization to sales
     *  presumably if people are poorer they’re less likely to buy the product…
     *  but also test the reverse: if people are poorer they’re more likely to buy the product
     */
    //const ??? salesFunction;
    
    //how much does a distributor get from converting someone else into a distributor?
    //const ??? downstreamEffects;
    

    //capitalization change per tick
    //const ??? capitalizationChange;

};

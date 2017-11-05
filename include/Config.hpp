#pragma once

/**
 * simulation parameters
 * see doc for explanation
 */
class Config
{
    const unsigned long graphSize;

    //how likely are people to have connections to other people?
    const double graphConnectedness;

    const bool allowUnconnectedSubgraphs;

    //corresponding to the maximum number of people someone
    //can maintain social relationships with
    //(hard to have 1000 best friends)
    const unsigned int maxEdgesPerNode;

    //const ??? salesSkillDistribution;
    
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

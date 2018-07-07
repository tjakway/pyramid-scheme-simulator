#include "ChanceContributor.hpp"

#include "Types.hpp"
#include "Util/Util.hpp"

namespace pyramid_scheme_simulator {

std::unique_ptr<ChanceContributor> 
    ChanceContributor::operator+(const ChanceContributor& other)
{
    return make_unique<Intersection>(clone(),
            other.clone());
}

double ChanceContributor::Intersection::getChance(rd_ptr rd)
{
    return firstSource->getChance(rd) * secondSource->getChance(rd);
}

std::unique_ptr<ChanceContributor> ChanceContributor::Intersection::clone() const
{
    return make_unique<Intersection>(
            firstSource->clone(), 
            secondSource->clone());
}

bool ChanceContributor::sampleFrom(rd_ptr rd)
{
    return Util::sampleFrom(rd, getChance(rd));
}

StaticChanceContributor::StaticChanceContributor(double staticChance)
    : chance(staticChance) {}


std::string ChanceContributor::Intersection::prettyPrintImpl() const
{
    return STRCAT("intersection of ", PrettyPrintable::getClassNameOrNull(firstSource), 
            " and ", PrettyPrintable::getClassNameOrNull(secondSource));
}


std::string StaticChanceContributor::prettyPrintImpl() const
{
    return STRCAT("chance=", chance);
}

/**
 * ignore the passed random device and return the chance we got in the CTOR
 */
double StaticChanceContributor::getChance(rd_ptr)
{
    return chance;
}

std::unique_ptr<ChanceContributor> StaticChanceContributor::clone() const
{
    return make_unique<StaticChanceContributor>(chance);
}

double RandomChanceContributor::getChance(rd_ptr rd)
{
    return Util::sampleUniformDistributionZeroToOne(rd);
}

std::unique_ptr<ChanceContributor> RandomChanceContributor::clone() const 
{
    return make_unique<RandomChanceContributor>();
}


}

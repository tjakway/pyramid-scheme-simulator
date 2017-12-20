#pragma once

#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {

void applySalesSideEffects(
        PopulationGraph&,
        SaleHandler::RecordType);

}

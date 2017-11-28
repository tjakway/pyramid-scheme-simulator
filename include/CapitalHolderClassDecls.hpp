#pragma once

#include <memory>
#include <functional>

namespace pyramid_scheme_simulator {

class CapitalHolder;
class Consumer;
class Distributor;

using NewDistributorFunction = 
    std::function<std::shared_ptr<Distributor>(Consumer&, std::shared_ptr<Distributor>)>;
}

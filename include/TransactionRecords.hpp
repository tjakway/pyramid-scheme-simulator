#include "Util/Monoid.hpp"
#include "CapitalHolder.hpp"

namespace pyramid_scheme_simulator {

template <typename T> 
using TransactionRecord = Monoid<T>;

template <typename T, typename ...Us>
class Transaction
{
    virtual TransactionRecord<T> operator()(Us... args) = 0;
};

template <typename T>
class VertexTransaction
{
    virtual TransactionRecord<T> operator()(CapitalHolder&) = 0;
};

template <typename T>
class EdgeTransaction
{
    virtual TransactionRecord<T> operator()(CapitalHolder&,
                                            CapitalHolder&) = 0;
};

}

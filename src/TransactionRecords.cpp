#include "TransactionRecords.hpp"

#include <memory>

namespace pyramid_scheme_simulator {



bool compareUniqueables(std::unique_ptr<Uniqueable>& fst,
        std::unique_ptr<Uniqueable>& snd)
{
    const bool fstEmpty = fst.get() == nullptr;
    const bool sndEmpty = snd.get() == nullptr;

    //check for null pointers
    if(fstEmpty && sndEmpty) {
        return false;
    }
    else if(!fstEmpty && sndEmpty) {
        return false;
    }
    else if(fstEmpty && !sndEmpty) {
        return true;
    }
    //neither pointer is null, dereference & compare
    else {
        return fst->id.str() < snd->id.str();
    }
}

}

#include "TransactionRecords.hpp"

#include <memory>
#include <functional>
#include <string>

namespace pyramid_scheme_simulator {



bool compareUniqueables(Uniqueable* fst,
        Uniqueable* snd) {
    const bool fstEmpty = fst == nullptr;
    const bool sndEmpty = snd == nullptr;

    //check for null pointers
    if(fstEmpty && sndEmpty) {
        return false;
    }
    else if(!fstEmpty && sndEmpty) {
        return false;
    }
    else if(fstEmpty && !sndEmpty) {
        return true;
    } else {

        return compareUniques(&fst->id, &snd->id);
    }
}
 
bool compareUniques(Unique* fst, Unique* snd)
{
    const bool fstEmpty = fst == nullptr;
    const bool sndEmpty = snd == nullptr;

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

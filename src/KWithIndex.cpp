// KWithIndex class created to keep track of the position (index inside the intermediate_results vector) of K value 
// being the final result of test01 analysis even after sorting
// The KWithIndex::median() method was introduced since MathHelper::median() does not support such inputs

#include "KWithIndex.h"

#include <algorithm>
#include <vector>

bool KWithIndex::operator<(const KWithIndex& other) const {
    return (K < other.K);
}

KWithIndex KWithIndex::mean(const KWithIndex& KWI1, const KWithIndex& KWI2) {
    KWithIndex mean_result = {
        .K = (KWI1.K + KWI2.K) / 2.0,
        .index = KWI1.index 
    };

    return mean_result;
} 

KWithIndex KWithIndex::median(std::vector<KWithIndex> KWI_vec) {
    std::sort(KWI_vec.begin(), KWI_vec.end());

    size_t n = KWI_vec.size();

    if (n % 2 == 0) {
        return KWithIndex::mean(KWI_vec[n/2 - 1], KWI_vec[n/2]);
    } else {
        return KWI_vec[n/2];
    }
}



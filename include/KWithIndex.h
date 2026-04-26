#ifndef K_WITH_INDEX_H
#define K_WITH_INDEX_H

#include <algorithm>
#include <vector>

// KWithIndex class created to keep track of the position (index inside the intermediate_results vector) of K value 
// being the final result of test01 analysis even after sorting
// The KWithIndex::median() method was introduced since MathHelper::median() does not support such inputs
class KWithIndex {
public:
    double K = 0.0;
    size_t index = 0;

    bool operator<(const KWithIndex& other) const;

    // Returns KWithIndex object with .K being mean value of both inputs K values
    // but .index is set to the index of first input (since we still need to navigate
    // inside of the intermediate_results vector). Choosing .index equal to the index
    // of second output would also be valid
    static KWithIndex mean(const KWithIndex& KWI1, const KWithIndex& KWI2);

    static KWithIndex median(std::vector<KWithIndex> KWI_vec);
};//class KWithIndex

#endif
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <vector>

namespace MathHelper {

    constexpr double pi = 3.14159265358979323846;

    double mean(const std::vector<double> &data);

    double min(const std::vector<double> &data);

    double max(const std::vector<double> &data);

    double median(std::vector<double> data); // Passed by value on purpose (sorting using std::sort() modifies contents)

    double var(const std::vector<double> &data);
    
    double cov(const std::vector<double> &data1, const std::vector<double> &data2);

    double corr(const std::vector<double> &data1, const std::vector<double> &data2);
}

#endif
#include "MathHelper.h"

#include <algorithm>
#include <cmath>
#include <numeric>

double MathHelper::mean(const std::vector<double> &data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
} // MathHelper::mean()

double MathHelper::min(const std::vector<double> &data) {
    return *std::min_element(data.begin(), data.end());
    //      ^-- dereferencing cuz min_element() return an iterator to the smallest element
} // MathHelper::min()

double MathHelper::max(const std::vector<double> &data) {
    return *std::max_element(data.begin(), data.end());
    //      ^-- dereferencing cuz max_element() return an iterator to the smallest element
} // MathHelper::max()

double MathHelper::median(std::vector<double> data) {
    std::sort(data.begin(), data.end());

    size_t n = data.size();

    if (n % 2 == 0) {
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    } else {
        return data[n/2];
    }
} // MathHelper::median()

double MathHelper::var(const std::vector<double> &data) {
    double mean = MathHelper::mean(data);

    double sum = 0.0;
    for(double val : data) {
        double diff = val - mean;
        sum += diff * diff;
    }

    return sum / data.size();
} // MathHelper::var()

double MathHelper::cov(const std::vector<double> &data1, const std::vector<double> &data2) {

    double m1 = MathHelper::mean(data1);
    double m2 = MathHelper::mean(data2);

    double sum = 0.0;

    for (size_t i = 0; i < data1.size(); ++i) {
        sum += (data1[i] - m1) * (data2[i] - m2);
    }

    return sum / data1.size();
} // MathHelper::cov()

double MathHelper::corr(const std::vector<double> &data1, const std::vector<double> &data2) {
    double cov = MathHelper::cov(data1, data2);
    double var1 = MathHelper::var(data1);
    double var2 = MathHelper::var(data2);

    return cov / std::sqrt(var1 * var2);
} // MathHelper::corr()
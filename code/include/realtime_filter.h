#ifndef REALTIME_FILTER_H
#define REALTIME_FILTER_H

#include <vector>


std::vector<double> realtime_filter(std::vector<double> x, std::vector<double> b, std::vector<double> a) {
    int len_a = a.size();
    int len_b = b.size();
    int len_x = x.size();
    std::vector<double> y(len_x);
    static std::vector<double> zi(len_b);
    for (int i = 0; i < len_x; i++) {
        y[i] = b[0] * x[i] + zi[0];
        for (int j = 1; j < len_b; j++) {
            zi[j - 1] = b[j] * x[i] - a[j] * y[i] + zi[j];
        }
    } 
    return y;
}

#endif
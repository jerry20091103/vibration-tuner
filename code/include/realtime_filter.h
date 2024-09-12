#ifndef REALTIME_FILTER_H
#define REALTIME_FILTER_H

#include <vector>
#include <esp_dsp.h>


std::vector<double> realtime_filter(std::vector<double> x, std::vector<double> b, std::vector<double> a, int stringId) {
    int len_a = a.size();
    int len_b = b.size();
    int len_x = x.size();
    std::vector<double> y(len_x);
    // create 6 zi vectors of length len_b
    static std::vector<std::vector<double>> zi(6, std::vector<double>(len_b));

    for (int i = 0; i < len_x; i++) {
        y[i] = b[0] * x[i] + zi[stringId][0];
        for (int j = 1; j < len_b; j++) {
            zi[stringId][j - 1] = b[j] * x[i] - a[j] * y[i] + zi[stringId][j];
        }
    } 
    return y;
}

#endif
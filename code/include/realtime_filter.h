#ifndef REALTIME_FILTER_H
#define REALTIME_FILTER_H

#include <vector>

using namespace std;

vector<double> realtime_filter(vector<double> x, vector<double> b, vector<double> a) {
    vector<double> y;
    int len_a = a.size();
    int len_b = b.size();
    int len_x = x.size();
    static vector<double> zi(len_b);
    for (int i = 0; i < len_x; i++) {
        y[i] = b[0] * x[i] + zi[0];
        for (int j = 1; j < len_b; j++) {
            zi[j - 1] = b[j] * x[i] - a[j] * y[i] + zi[j];
        }
    } 
    return y;
}

#endif
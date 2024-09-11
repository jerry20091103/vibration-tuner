#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include<iostream>
#include<vector>
#include<complex>
#include<fstream>
#include<math.h>

using namespace std;

vector<double> ComputeDenCoeffs(int FilterOrder, double Lcutoff, double Ucutoff);

vector<double> TrinomialMultiply(int FilterOrder, vector<double> b, vector<double> c);

vector<double> ComputeNumCoeffs(int FilterOrder, double Lcutoff, double Ucutoff, vector<double> DenC);

vector<double> ComputeLP(int FilterOrder);

vector<double> ComputeHP(int FilterOrder);

#endif
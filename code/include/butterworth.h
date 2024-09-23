#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include<iostream>
#include<vector>
#include<complex>
#include<fstream>
#include<math.h>


std::vector<double> ComputeDenCoeffs(int FilterOrder, double Lcutoff, double Ucutoff);

std::vector<double> TrinomialMultiply(int FilterOrder, std::vector<double> b, std::vector<double> c);

std::vector<double> ComputeNumCoeffs(int FilterOrder, double Lcutoff, double Ucutoff, std::vector<double> DenC);

std::vector<double> ComputeLP(int FilterOrder);

std::vector<double> ComputeHP(int FilterOrder);

#endif
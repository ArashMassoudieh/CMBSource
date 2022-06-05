#ifndef CONCENTRATIONSET_H
#define CONCENTRATIONSET_H

#include "vector"
#include "distribution.h"


using namespace std;



class ConcentrationSet : public vector<double>
{
public:
    ConcentrationSet();
    ConcentrationSet(const ConcentrationSet& cs);
    ConcentrationSet(int n);
    ConcentrationSet& operator=(const ConcentrationSet &cs);
    double mean();
    double stdev();
    double stdevln();
    double meanln();
    double norm(const double &x);
    double normln(const double &x);
    double min();
    double max();
    ConcentrationSet ln();
    void Append(const double &x);
    void Append(const ConcentrationSet &x);
    void Append(ConcentrationSet *x);
    vector<double> EstimateParameters(distribution_type disttype=distribution_type::none);
    double LogLikelihood(const vector<double> &params=vector<double>(),distribution_type disttype=distribution_type::none);
    distribution_type SelectBestDistribution();
    Distribution* FittedDistribution() {return &FittedDist;}

private:
    Distribution FittedDist;
};

#endif // CONCENTRATIONSET_H

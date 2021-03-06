#include "concentrationset.h"
#include "math.h"
#include "Utilities.h"

ConcentrationSet::ConcentrationSet():vector<double>()
{

}

ConcentrationSet::ConcentrationSet(const ConcentrationSet &cs):vector<double>(cs)
{

}

ConcentrationSet::ConcentrationSet(int n):vector<double>(n)
{

}

ConcentrationSet& ConcentrationSet::operator=(const ConcentrationSet &cs)
{
    vector<double>::operator=(cs);
    return *this;
}

double ConcentrationSet::mean()
{
    double sum=0;
    for (unsigned int i=0; i<size(); i++)
    {
        sum += at(i);
    }
    return sum/double(size());
}
double ConcentrationSet::stdev()
{
    return sqrt((norm(2)/double(size())-pow(mean(),2)));
}

double ConcentrationSet::stdevln()
{
    return sqrt(normln(2)/double(size())-pow(meanln(),2));
}

double ConcentrationSet::meanln()
{
    double sum=0;
    for (unsigned int i=0; i<size(); i++)
    {
        sum += log(at(i));
    }
    return sum/double(size());
}
double ConcentrationSet::norm(const double &x)
{
    double sum=0;
    for (unsigned int i=0; i<size(); i++)
    {
        sum += pow(at(i),x);
    }
    return sum;
}
double ConcentrationSet::normln(const double &x)
{
    double sum=0;
    for (unsigned int i=0; i<size(); i++)
    {
        sum += pow(log(at(i)),x);
    }
    return sum;
}
ConcentrationSet ConcentrationSet::ln()
{
    ConcentrationSet out(size());;
    for (unsigned int i=0; i<size(); i++)
    {
        out[i] = log(at(i));
    }
    return out;
}

double ConcentrationSet::min()
{
    return aquiutils::Min(*this);
}
double ConcentrationSet::max()
{
    return aquiutils::Max(*this);
}

void ConcentrationSet::Append(const double &x)
{
    push_back(x);
}
void ConcentrationSet::Append(const ConcentrationSet &x)
{
    insert( end(), x.begin(), x.end());
}

void ConcentrationSet::Append(ConcentrationSet *x)
{
    insert( end(), x->begin(), x->end());
}

vector<double> ConcentrationSet::EstimateParameters(distribution_type disttype)
{
    vector<double> out;
    if (disttype==distribution_type::none)
        disttype = FittedDist.distribution;
    if (disttype==distribution_type::normal)
    {
        out.push_back(mean());
        out.push_back(stdev());
    }
    else if (disttype==distribution_type::lognormal)
    {
        out.push_back(meanln());
        out.push_back(stdevln());
    }
    return out;
}

double ConcentrationSet::LogLikelihood(const vector<double> &params,distribution_type disttype)
{
    double loglikelihood=0;
    if (params.size()==0 && disttype==distribution_type::none)
    {
        for (unsigned int i=0; i<size(); i++)
            loglikelihood += log(FittedDist.Eval(at(i)));
    }
    if (params.size()!=0 && disttype!=distribution_type::none)
    {
        Distribution dist;
        dist.parameters = params;
        dist.distribution = disttype;
        for (unsigned int i=0; i<size(); i++)
            loglikelihood += log(dist.Eval(at(i)));
    }
    return loglikelihood;
}

distribution_type ConcentrationSet::SelectBestDistribution()
{
    if (this->min()<=0)
    {
        return distribution_type::normal;
    }
    double loglikelihood_normal = 0;
    double loglikelihood_lognormal = 0;

    vector<double> parameters_normal = EstimateParameters(distribution_type::normal);
    loglikelihood_normal = LogLikelihood(parameters_normal,distribution_type::normal);

    vector<double> parameters_lognormal = EstimateParameters(distribution_type::lognormal);
    loglikelihood_normal = LogLikelihood(parameters_normal,distribution_type::lognormal);

    if (loglikelihood_normal>loglikelihood_lognormal) return distribution_type::normal; else return distribution_type::lognormal;
}


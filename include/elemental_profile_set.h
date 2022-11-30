#ifndef ELEMENTAL_PROFILE_SET_H
#define ELEMENTAL_PROFILE_SET_H

#include <elemental_profile.h>
#include <map>
#include <string>
#include <vector>
#include "concentrationset.h"
#include "interface.h"

using namespace std;



class Elemental_Profile_Set: public map<string, Elemental_Profile>, public Interface
{
public:
    Elemental_Profile_Set();
    Elemental_Profile_Set(const Elemental_Profile_Set& mp);
    Elemental_Profile_Set& operator=(const Elemental_Profile_Set &mp);
    Elemental_Profile *Profile(const string &name);
    Elemental_Profile Profile(const string &name) const;
    Elemental_Profile *Profile(unsigned int i);
    Elemental_Profile Profile(unsigned int i) const;
    vector<double> GetAllConcentrationsFor(const string &element_name);
    vector<double> GetProfileForSample(const string &source_name);
    Elemental_Profile *Append_Profile(const string &name, const Elemental_Profile &profile=Elemental_Profile());
   
    vector<string> SampleNames(); // Return the list of the name of samples
    ConcentrationSet *ElementalDistribution(const string &element_name)
    {
        return &element_distributions[element_name];
    }
    ConcentrationSet ElementalDistribution(const string &element_name) const {
        return element_distributions.at(element_name);
    }
    distribution_type DistributionAssigned(const string &element_name)
    {
        if (element_distributions.count(element_name)==0)
            return distribution_type::none;

        return element_distributions[element_name].FittedDistribution()->distribution;
    }
    double Estimated_mu(const string &element) //return the estimated mean for an element
    {
        if (element_distributions.count(element)>0)
            return element_distributions[element].EstimatedMu();
        else
            return 0;
    }
    double Estimated_sigma(const string &element)
    {
        if (element_distributions.count(element)>0)
            return element_distributions[element].EstimatedSigma();
        else
            return 0;
    }
    bool Set_Estimated_mu(const string &element, const double &value)
    {
        if (element_distributions.count(element)>0)
        {   element_distributions[element].SetEstimatedMu(value);
            return true;
        }
        else
            return false;

    }
    bool Set_Estimated_sigma(const string &element, const double &value)
    {
        if (element_distributions.count(element)>0)
        {   element_distributions[element].SetEstimatedSigma(value);
            return true;
        }
        else
            return false;

    }
    bool SetContribution(const double &x)
    {
        contribution = x;
        return true;
    }

    bool SetContribution_softmax(const double &x)
    {
        contribution_softmax = x;
        return true;
    }
    double Contribution() {return contribution; }
    double Contribution_softmax() {return contribution_softmax; }
    Distribution* GetEstimatedDistribution(const string &element_name )
    {
        if (element_distributions.count(element_name)>0)
            return element_distributions[element_name].GetEstimatedDistribution();
        else
            return nullptr;

    }

    Distribution* GetFittedDistribution(const string& element_name)
    {
        if (element_distributions.count(element_name) > 0)
            return element_distributions[element_name].FittedDistribution();
        else
            return nullptr;

    }
    string ToString() override;
    vector<string> ElementNames();
    double max();
    double min(); 
    bool writetofile(QFile*) override;
    bool Read(const QStringList &strlist) override;

private:
    map<string,ConcentrationSet> element_distributions; // concentrations for each element
    double contribution = 0;
    double contribution_softmax = 0;

};

#endif // ELEMENTAL_PROFILE_SET_H

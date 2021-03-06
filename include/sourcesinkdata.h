#ifndef SOURCESINKDATA_H
#define SOURCESINKDATA_H

#include <elemental_profile_set.h>
#include "vector"
#include "parameter.h"
#include "observation.h"

struct profiles_data
{
    vector<string> element_names;
    vector<string> sample_names;
    vector<vector<double>> values;
};

struct element_data
{
    string group_name;
    vector<string> sample_names;
    vector<double> values;
};

struct element_data_groups
{
    vector<string> group_names;
    vector<string> sample_names;
    vector<vector<double>> values;
};

struct element_information
{
    enum class role {do_not_include, isotope, particle_size, element} Role = role::element;
    double standard_ratio;
    string base_element;

};

class SourceSinkData
{
public:
    SourceSinkData();
    SourceSinkData(const SourceSinkData& mp);
    SourceSinkData& operator=(const SourceSinkData &mp);
    Elemental_Profile_Set* AppendSampleSet(const string &name, const Elemental_Profile_Set &elemental_profile_set=Elemental_Profile_Set());
    Elemental_Profile_Set *sample_set(const string &name);
    vector<string> GroupNames();
    vector<string> ElementNames();
    vector<string> SampleNames(const string groupname);
    profiles_data ExtractData(const vector<vector<string>> &indicators);
    element_data ExtractElementData(const string &element, const string &group);
    map<string,vector<double>> ExtractElementData(const string &element);
    void PopulateElementDistributions();
    void AssignAllDistributions();
    Distribution *FittedDistribution(const string &element_name);
    map<string,Elemental_Profile_Set>::iterator begin() {return sample_sets.begin();}
    map<string,Elemental_Profile_Set>::iterator end() {return sample_sets.end();}
    map<string,Elemental_Profile_Set>::const_iterator cbegin() {return sample_sets.begin();}
    map<string,Elemental_Profile_Set>::const_iterator cend() {return sample_sets.end();}
    element_information* GetElementInformation(const string &element_name)
    {
        if (ElementInformation.count(element_name))
            return  &ElementInformation.at(element_name);
        else
            return nullptr;
    }
    ConcentrationSet* GetElementDistribution(const string &element_name)
    {
        if (element_distributions.count(element_name))
            return  &element_distributions.at(element_name);
        else
            return nullptr;
    }
    ConcentrationSet* GetElementDistribution(const string &element_name, const string &sample_group)
    {
        if (!sample_set(sample_group))
        {
            cout<<"Sample Group '" + sample_group +"' does not exist!"<<endl;
            return nullptr;

        }
        if (!sample_set(sample_group)->ElementalDistribution(element_name))
        {
            cout<<"Element '" + element_name +"' does not exist!"<<endl;
            return nullptr;
        }

        return  sample_set(sample_group)->ElementalDistribution(element_name);

    }
    void PopulateElementInformation();
    bool Execute(const string &command, const map<string,string> &arguments);
    string OutputPath();
    bool SetOutputPath(const string &oppath);
    vector<Parameter> &Parameters() {return parameters;}
    size_t ParametersCount() {return parameters.size(); }
    size_t ObservationsCount() {return observations.size(); }
    Parameter *parameter(size_t i)
    {
        if (i>=0 && i<parameters.size())
            return &parameters[i];
        else
            return nullptr;
    }
    Observation *observation(size_t i)
    {
        if (i>=0 && i<observations.size())
            return &observations[i];
        else
            return nullptr;
    }
    string GetNameForParameterID(int i);
    bool SetTargetGroup(const string &targroup) {target_group = targroup;}
    string TargetGroup() {return target_group;}
    bool InitializeParametersObservations(const string &targetsamplename="");
    bool SetParameterValue(unsigned int i, double value);
    CVector PredictTarget();
    CMatrix SourceMeanMatrix();
    CVector ContributionVector();
    CVector ObservedDataforSelectedSample(const string &SelectedTargetSample="");
    double GetObjectiveFunctionValue();
    double LogLikelihood();
    vector<string> ElementsToBeUsedInCMB();
    vector<string> SourceGroupNames();
    bool SetSelectedTargetSample(const string &sample_name);
    string SelectedTargetSample();

private:
    map<string,Elemental_Profile_Set> sample_sets;
    map<string,ConcentrationSet> element_distributions;
    map<string, element_information> ElementInformation;
    string outputpath;
    string target_group;
    string selected_target_sample;
    vector<Parameter> parameters;
    vector<Observation> observations;
    double LogPriorContributions();
    double LogLikelihoodSourceElementalDistributions();
    double LogLikelihoodModelvsMeasured();
    CVector GetSourceContributions();
    Parameter* ElementalContent_mu(int element_iterator, int source_iterator);
    Parameter* ElementalContent_sigma(int element_iterator, int source_iterator);
    double ElementalContent_mu_value(int element_iterator, int source_iterator);
    double ElementalContent_sigma_value(int element_iterator, int source_iterator);
    unsigned int numberofelements = 0;
    unsigned int numberofsourcesamplesets = 0;
    vector<string> samplesetsorder;
    vector<string> elementorder;
    double error_stdev = 0;


};

#endif // SOURCESINKDATA_H

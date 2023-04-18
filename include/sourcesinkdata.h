#ifndef SOURCESINKDATA_H
#define SOURCESINKDATA_H

#include <elemental_profile_set.h>
#include "vector"
#include "parameter.h"
#include "observation.h"
#include "contribution.h"
#include "results.h"
#include "ProgressWindow.h"
#include "fstream"
#include "qjsonobject.h"
#include "cmbvector.h"


enum class transformation {linear, softmax};

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


enum class estimation_mode {only_contributions, elemental_profile_and_contribution, source_elemental_profiles_based_on_source_data};

class SourceSinkData: public map<string, Elemental_Profile_Set>
{
public:
    SourceSinkData();
    SourceSinkData(const SourceSinkData& mp);
    SourceSinkData& operator=(const SourceSinkData &mp);
    SourceSinkData Corrected(const string &target_sample, bool omnsizecorrect=true, map<string, element_information> *elementinfo=nullptr);
    void Clear();
    Elemental_Profile_Set* AppendSampleSet(const string &name, const Elemental_Profile_Set &elemental_profile_set=Elemental_Profile_Set());
    Elemental_Profile_Set *sample_set(const string &name);
    vector<string> GroupNames();
    vector<string> ElementNames();
    vector<string> SampleNames(const string groupname); // List of sample names for a particular group
    profiles_data ExtractData(const vector<vector<string>> &indicators);
    element_data ExtractElementData(const string &element, const string &group);
    map<string,vector<double>> ExtractElementData(const string &element);
    Elemental_Profile *GetElementalProfile(const string sample_name); //Get Elemental Profile for a particular sample
    void PopulateElementDistributions();
    void AssignAllDistributions();
    Distribution *FittedDistribution(const string &element_name);
    static QString Role(const element_information::role &rl);
    static element_information::role Role(const QString &rl);
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
            cout<<"Sample Group '" + sample_group +"' does not exist!"<<std::endl;
            return nullptr;

        }
        if (!sample_set(sample_group)->ElementalDistribution(element_name))
        {
            cout<<"Element '" + element_name +"' does not exist!"<<std::endl;
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
    bool SetTargetGroup(const string &targroup)
    {
	    target_group = targroup;
        return true; 
    }
    string TargetGroup() {return target_group;}
    bool InitializeParametersObservations(const string &targetsamplename="",estimation_mode est_mode = estimation_mode::elemental_profile_and_contribution);
    bool InitializeContributionsRandomly(); //initializes source contributions randomly
    bool InitializeContributionsRandomly_softmax(); //initializes source contributions randomly for softmax transformation
    bool SetParameterValue(unsigned int i, double value); //set the parameter values for estimation
    CVector PredictTarget(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    CVector PredictTarget_Isotope(parameter_mode param_mode= parameter_mode::based_on_fitted_distribution);
    CVector PredictTarget_Isotope_delta(parameter_mode param_mode= parameter_mode::based_on_fitted_distribution);
    CMatrix SourceMeanMatrix(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    CMatrix SourceMeanMatrix_Isotopes(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    CVector ContributionVector(bool full=true);
    CVector ContributionVector_softmax();
    void SetContribution(int i, double value);
    void SetContribution_softmax(int i, double value);
    void SetContribution(const CVector &X);
    void SetContribution_softmax(const CVector &X); //set source contribution softmax transforations
    CVector ObservedDataforSelectedSample(const string &SelectedTargetSample="");
    CVector ObservedDataforSelectedSample_Isotope(const string &SelectedTargetSample="");//return observed isotopes transformed into isotope contents
    CVector ObservedDataforSelectedSample_Isotope_delta(const string &SelectedTargetSample="");//return delta values of observed isotopes
    double GetObjectiveFunctionValue();
    double LogLikelihood(estimation_mode est_mode = estimation_mode::elemental_profile_and_contribution);
    vector<string> ElementsToBeUsedInCMB();
    vector<string> IsotopesToBeUsedInCMB();
    vector<string> SourceGroupNames();
    bool SetSelectedTargetSample(const string &sample_name);
    string SelectedTargetSample();
    vector<string> SourceOrder() {return samplesetsorder;}
    vector<string> SamplesetsOrder() {return samplesetsorder;}
    vector<string> ConstituentOrder() {return constituent_order;}
    vector<string> ElementOrder() {return element_order;}
    vector<string> IsotopeOrder() {return isotope_order;}
    vector<string> SizeOMOrder() {return size_om_order;}
    ResultItem GetContribution();
    ResultItem GetPredictedElementalProfile(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    CVector GetPredictedValues(); // copied the predicted constituents and isotopes from observations into a vector
    ResultItem GetPredictedElementalProfile_Isotope(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    ResultItem GetObservedElementalProfile();
    ResultItem GetObservedElementalProfile_Isotope();
    ResultItem GetObservedvsModeledElementalProfile(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    ResultItem GetObservedvsModeledElementalProfile_Isotope(parameter_mode param_mode = parameter_mode::based_on_fitted_distribution);
    vector<ResultItem> GetMLRResults();
    ResultItem GetCalculatedElementMeans();
    vector<ResultItem> GetSourceProfiles();
    ResultItem GetCalculatedElementStandardDev();
    ResultItem GetCalculatedElementMu();
    ResultItem GetEstimatedElementMu();
    ResultItem GetEstimatedElementMean();
    ResultItem GetEstimatedElementSigma();
    CVector ResidualVector();
    CVector_arma ResidualVector_arma();
    CMatrix ResidualJacobian();
    CMatrix ResidualJacobian_softmax();
    CMatrix_arma ResidualJacobian_arma();
    CVector OneStepLevenBerg_Marquardt(double lambda = 0);
    CVector OneStepLevenBerg_Marquardt_softmax(double lambda);
    bool SolveLevenBerg_Marquardt(transformation trans = transformation::linear );
    void SetProgressWindow(ProgressWindow *_rtw) {rtw = _rtw;}
    void SetParameterEstimationMode(estimation_mode est_mode) {parameter_estimation_mode = est_mode;}
    estimation_mode ParameterEstimationMode() {return parameter_estimation_mode;}
    bool WriteToFile(QFile *fil);
    bool ReadFromFile(QFile *fil);
    bool WriteElementInformationToFile(QFile *fil);
    bool WriteDataToFile(QFile *file);
    QJsonObject ElementInformationToJsonObject();
    QJsonObject ElementDataToJsonObject();
    bool ReadElementInformationfromJsonObject(const QJsonObject &jsonobject);
    bool ReadElementDatafromJsonObject(const QJsonObject &jsonobject);
    bool Perform_Regression_vs_om_size(const string &om, const string &d, regression_form form=regression_form::linear);
    CMBVector DiscriminantFunctionAnalysis(const string &source1, const string &source2);
    CMBVector DiscriminantFunctionAnalysis(const string &source1);
    CMBMatrix DiscriminantFunctionAnalysis();
    CMBVector DFATransformed(const CMBVector &eigenvector, const string &source);
    Elemental_Profile_Set TheRest(const string &source);
    CMBVector BracketTest(const string &target_sample);
private:

    map<string,ConcentrationSet> element_distributions;
    map<string, element_information> ElementInformation;
    string outputpath;
    string target_group;
    string selected_target_sample;
    vector<Parameter> parameters;
    vector<Observation> observations;
    double LogPriorContributions();
    double LogLikelihoodSourceElementalDistributions();
    double LogLikelihoodModelvsMeasured(estimation_mode est_mode = estimation_mode::elemental_profile_and_contribution);
    double LogLikelihoodModelvsMeasured_Isotope(estimation_mode est_mode = estimation_mode::elemental_profile_and_contribution);
    CVector GetSourceContributions();
    Parameter* ElementalContent_mu(int element_iterator, int source_iterator);
    Parameter* ElementalContent_sigma(int element_iterator, int source_iterator);
    double ElementalContent_mu_value(int element_iterator, int source_iterator);
    double ElementalContent_sigma_value(int element_iterator, int source_iterator);
    unsigned int numberofconstituents = 0;
    unsigned int numberofisotopes = 0;
    unsigned int numberofsourcesamplesets = 0;
    vector<string> samplesetsorder;
    vector<string> constituent_order;
    vector<string> element_order;
    vector<string> isotope_order;
    vector<string> size_om_order;
    void populate_constituent_orders();
    double error_stdev = 0;
    double error_stdev_isotope = 0;
    ProgressWindow *rtw = nullptr;
    estimation_mode parameter_estimation_mode = estimation_mode::elemental_profile_and_contribution;
    string omconstituent;
    string sizeconsituent;



};

#endif // SOURCESINKDATA_H

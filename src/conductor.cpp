#include "conductor.h"
#include "ProgressWindow.h"
#include "results.h"
#include "contribution.h"
#include "resultitem.h"



Conductor::Conductor()
{

}

bool Conductor::Execute(const string &command, map<string,string> arguments)
{
    results.clear();
    if (command == "GA")
    {
        if (GA!=nullptr) delete GA;
        ProgressWindow *rtw = new ProgressWindow();
        rtw->show();
        Data()->InitializeParametersObservations(arguments["Sample"]);
        GA = new CGA<SourceSinkData>(Data());
        GA->filenames.pathname = workingfolder.toStdString() + "/";
        GA->SetRunTimeWindow(rtw);
        GA->SetProperties(arguments);
        GA->InitiatePopulation();
        GA->optimize();

        results.SetName("GA " + arguments["Sample"]);

        ResultItem result_cont = GA->Model_out.GetContribution();
        results.Append(result_cont);

        ResultItem result_calculated_means = GA->Model_out.GetCalculatedElementMeans();
        results.Append(result_calculated_means);

        ResultItem result_estimated_means = GA->Model_out.GetEstimatedElementMean();
        results.Append(result_estimated_means);

        ResultItem result_modeled_vs_measured = GA->Model_out.GetObservedvsModeledElementalProfile();
        results.Append(result_modeled_vs_measured);

        ResultItem result_modeled_vs_measured_isotope = GA->Model_out.GetObservedvsModeledElementalProfile_Isotope();
        results.Append(result_modeled_vs_measured_isotope);

    }
    if (command == "GA (fixed elemental contribution)")
    {
        if (GA!=nullptr) delete GA;
        ProgressWindow *rtw = new ProgressWindow();
        rtw->SetTitle("Fitness",0);
        rtw->SetYAxisTitle("Fitness",0);

        rtw->show();
        Data()->InitializeParametersObservations(arguments["Sample"],estimation_mode::only_contributions);
        Data()->SetParameterEstimationMode(estimation_mode::only_contributions);
        GA = new CGA<SourceSinkData>(Data());
        GA->filenames.pathname = workingfolder.toStdString() + "/";
        GA->SetRunTimeWindow(rtw);
        GA->SetProperties(arguments);
        GA->InitiatePopulation();
        GA->optimize();
        results.SetName("GA (fixed profile) " + arguments["Sample"]);
        ResultItem result_cont = GA->Model_out.GetContribution();
        results.Append(result_cont);

        ResultItem result_modeled_vs_measured = GA->Model_out.GetObservedvsModeledElementalProfile(parameter_mode::direct);
        results.Append(result_modeled_vs_measured);

        ResultItem result_modeled_vs_measured_isotope = GA->Model_out.GetObservedvsModeledElementalProfile_Isotope(parameter_mode::direct);
        results.Append(result_modeled_vs_measured_isotope);

    }
    if (command == "GA (disregarding targets)")
    {
        if (GA!=nullptr) delete GA;
        ProgressWindow *rtw = new ProgressWindow();
        rtw->show();
        Data()->InitializeParametersObservations(arguments["Sample"],estimation_mode::source_elemental_profiles_based_on_source_data);
        Data()->SetParameterEstimationMode(estimation_mode::source_elemental_profiles_based_on_source_data);
        GA = new CGA<SourceSinkData>(Data());
        GA->filenames.pathname = workingfolder.toStdString() + "/";
        GA->SetRunTimeWindow(rtw);
        GA->SetProperties(arguments);
        GA->InitiatePopulation();
        GA->optimize();

        results.SetName("GA (no targets) " + arguments["Sample"]);
        ResultItem result_calculated_means = GA->Model_out.GetCalculatedElementMeans();
        results.Append(result_calculated_means);

        ResultItem result_estimated_means = GA->Model_out.GetEstimatedElementMean();
        results.Append(result_estimated_means);

        ResultItem result_calculated_stds = GA->Model_out.GetCalculatedElementStandardDev();
        results.Append(result_calculated_stds);

        ResultItem result_estimated_stds = GA->Model_out.GetEstimatedElementSigma();
        results.Append(result_estimated_stds);



    }
    if (command == "Levenberg-Marquardt")
    {
        ProgressWindow* rtw = new ProgressWindow();
        rtw->show();
        Data()->InitializeParametersObservations(arguments["Sample"]);
        Data()->SetProgressWindow(rtw);
        if (arguments["Softmax transformation"]=="true")
            Data()->SolveLevenBerg_Marquardt(transformation::softmax);
        else
            Data()->SolveLevenBerg_Marquardt(transformation::linear);

        results.SetName("LM " + arguments["Sample"]);
        ResultItem result_cont = Data()->GetContribution();
        results.Append(result_cont);

        ResultItem result_modeled = Data()->GetPredictedElementalProfile(parameter_mode::direct);
        results.Append(result_modeled);

        ResultItem result_modeled_vs_measured = Data()->GetObservedvsModeledElementalProfile();
        results.Append(result_modeled_vs_measured);

        ResultItem result_modeled_vs_measured_isotope = Data()->GetObservedvsModeledElementalProfile_Isotope(parameter_mode::direct);
        results.Append(result_modeled_vs_measured_isotope);

    }

    if (command == "MLR")
    {
        results.SetName("MLR_vs_OM&Size ");
        Data()->Perform_Regression_vs_om_size(arguments["Organic Matter constituent"],arguments["Particla Size constituent"]);
        vector<ResultItem> regression_result = Data()->GetMLRResults();
        for (unsigned int i=0; i<regression_result.size(); i++)
        {
            results.Append(regression_result[i]);
        }
    }
    if (command == "CovMat")
    {
        results.SetName("Covariance Matrix for " + arguments["Source/Target group"] );
        ResultItem covMatResItem;
        covMatResItem.SetName("Covariance Matrix for " + arguments["Source/Target group"] );
        covMatResItem.SetType(result_type::matrix);

        CMBMatrix *covmatr = new CMBMatrix(Data()->at(arguments["Source/Target group"]).CovarianceMatrix());
        covMatResItem.SetResult(covmatr);
        results.Append(covMatResItem);

    }
    if (command == "CorMat")
    {
        results.SetName("Correlation Matrix for " + arguments["Source/Target group"] );
        ResultItem corMatResItem;
        corMatResItem.SetName("Correlation Matrix for " + arguments["Source/Target group"] );
        corMatResItem.SetType(result_type::matrix);

        CMBMatrix *cormatr = new CMBMatrix(Data()->at(arguments["Source/Target group"]).CorrelationMatrix());
        corMatResItem.SetResult(cormatr);
        results.Append(corMatResItem);

    }
    if (command == "DFA")
    {
        results.SetName("DFA coefficients between " + arguments["Source/Target group I"] + "&" + arguments["Source/Target group II"] );
        ResultItem DFAResItem;
        DFAResItem.SetName("DFA coefficients between " + arguments["Source/Target group I"] + "&" + arguments["Source/Target group II"]  );
        DFAResItem.SetType(result_type::vector);
        DFAResItem.SetYAxisMode(yaxis_mode::normal);
        CMBVector *dfaeigenvector = new CMBVector(Data()->DiscriminantFunctionAnalysis(arguments["Source/Target group I"],arguments["Source/Target group II"]));
        DFAResItem.SetResult(dfaeigenvector);
        results.Append(DFAResItem);

    }
    if (command == "KS")
    {
        results.SetName("Kolmogorov–Smirnov statististics for " + arguments["Source/Target group"] );
        ResultItem KSItem;
        KSItem.SetName("Kolmogorov–Smirnov statististics for " + arguments["Source/Target group"] );
        KSItem.SetType(result_type::vector);
        KSItem.SetYAxisMode(yaxis_mode::normal);
        distribution_type dist;
        if (arguments["Distribution"]=="Normal")
            dist = distribution_type::normal;
        else if (arguments["Distribution"]=="Lognormal")
            dist = distribution_type::lognormal;
        CMBVector *ksoutput = new CMBVector(Data()->at(arguments["Source/Target group"]).KolmogorovSmirnovStat(dist));
        KSItem.SetResult(ksoutput);
        results.Append(KSItem);
    }
    if (command == "KS-individual")
    {
        results.SetName("Kolmogorov–Smirnov statististics for constituent " +arguments["Constituent"] + " in group " + arguments["Source/Target group"]);
        ResultItem KSItem;
        KSItem.SetName("Kolmogorov–Smirnov statististics for constituent " +arguments["Constituent"] + " in group " + arguments["Source/Target group"]);
        KSItem.SetType(result_type::timeseries_set);
        distribution_type dist;
        if (arguments["Distribution"]=="Normal")
            dist = distribution_type::normal;
        else if (arguments["Distribution"]=="Lognormal")
            dist = distribution_type::lognormal;
        CMBTimeSeriesSet *ksoutput = new CMBTimeSeriesSet(Data()->at(arguments["Source/Target group"]).ElementalDistribution(arguments["Constituent"])->DataCDFnFitted(dist));
        KSItem.SetResult(ksoutput);
        results.Append(KSItem);
    }
    if (command == "CMB Bayesian")
    {
        results.SetName("MCMC results for '" +arguments["Sample"] + "'");
        ResultItem MCMC_samples;
        MCMC_samples.SetShowAsString(false);
        MCMC_samples.SetType(result_type::mcmc_samples);
        MCMC_samples.SetName("MCMC samples for '" +arguments["Sample"] + "'");
        CMBTimeSeriesSet *samples = new CMBTimeSeriesSet();

        if (MCMC!=nullptr) delete MCMC;
        MCMC = new CMCMC<SourceSinkData>();
        MCMC->Model = Data();
        ProgressWindow *rtw = new ProgressWindow(nullptr,3);
        rtw->SetTitle("Acceptance Rate",0);
        rtw->SetTitle("Purturbation Factor",1);
        rtw->SetTitle("Log posterior value",2);
        rtw->SetYAxisTitle("Acceptance Rate",0);
        rtw->SetYAxisTitle("Purturbation Factor",1);
        rtw->SetYAxisTitle("Log posterior value",2);
        rtw->show();

        Data()->InitializeParametersObservations(arguments["Sample"]);
        MCMC->SetProperty("number_of_samples",arguments["Number of samples"]);
        MCMC->SetProperty("number_of_chains",arguments["Number of chains"]);
        MCMC->SetProperty("number_of_burnout_samples",arguments["Samples to be discarded (burnout)"]);
        MCMC->initialize(samples,true);
        MCMC->step(QString::fromStdString(arguments["Number of chains"]).toInt(), QString::fromStdString(arguments["Number of samples"]).toInt(), arguments["samples_file_name"], samples, rtw);
        MCMC_samples.SetResult(samples);
        results.Append(MCMC_samples);
        ResultItem distribution_res_item;
        CMBTimeSeriesSet *dists = new CMBTimeSeriesSet();
        *dists = samples->distribution(100,0,QString::fromStdString(arguments["Samples to be discarded (burnout)"]).toInt());
        distribution_res_item.SetName("Posterior Distributions");
        distribution_res_item.SetShowAsString(false);
        distribution_res_item.SetType(result_type::distribution);
        distribution_res_item.SetResult(dists);
        results.Append(distribution_res_item);
    }
    return true;
}

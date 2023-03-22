#include "results.h"
#include "resultitem.h"
#include "contribution.h"
#include "elemental_profile_set.h"
#include "rangeset.h"

Results::Results()
{

}

Results::Results(const Results &rhs): map<string, ResultItem>(rhs)
{
    name = rhs.name;
}
Results& Results::operator = (const Results &rhs)
{
    map<string, ResultItem>::operator=(rhs);
    name = rhs.name;
    return *this;
}

void Results::Append(const ResultItem &ritem)
{
    operator[](ritem.Name()) = ritem;
}

QJsonObject Results::toJsonObject()
{
    QJsonObject out;
    for (map<string,ResultItem>::iterator it = begin(); it!=end(); it++)
    {
        out[QString::fromStdString(it->first)] = it->second.Result()->toJsonObject();
    }
    return out;

}

bool Results::ReadFromJson(const QJsonObject &jsonobject)
{
    for(QString key: jsonobject.keys() ) {

        if (key=="Contributions")
        {
            Contribution *contribution = new Contribution();
            contribution->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetName(key.toStdString());
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetType(result_type::contribution);
            res_item.SetResult(contribution);
            operator[](key.toStdString()) = res_item;
        }
        else if (key=="Modeled Elemental Profile")
        {
            Elemental_Profile *modeled = new Elemental_Profile();
            modeled->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetName(key.toStdString());
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetType(result_type::predicted_concentration);
            res_item.SetResult(modeled);
            operator[](key.toStdString()) = res_item;
        }
        else if (key=="Observed vs Modeled Elemental Profile")
        {
            Elemental_Profile_Set *modeled_vs_measured = new Elemental_Profile_Set();
            modeled_vs_measured->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetName(key.toStdString());
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetType(result_type::elemental_profile_set);
            res_item.SetResult(modeled_vs_measured);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("OM & Size MLR for "))
        {
            MultipleLinearRegressionSet* mlrset = new MultipleLinearRegressionSet(); 
            mlrset->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetName(key.toStdString());
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetType(result_type::elemental_profile_set);
            res_item.SetResult(mlrset);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("MCMC samples"))
        {
            CMBTimeSeriesSet *samples = new CMBTimeSeriesSet();
            samples->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetName(key.toStdString());
            res_item.SetShowAsString(false);
            res_item.SetType(result_type::mcmc_samples);
            res_item.SetResult(samples);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("Posterior Distributions"))
        {
            CMBTimeSeriesSet* samples = new CMBTimeSeriesSet();
            samples->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetShowAsString(false);
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetName(key.toStdString());
            res_item.SetType(result_type::distribution);
            res_item.SetResult(samples);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("Source Contribution Credible Intervals"))
        {
            RangeSet* rangeset = new RangeSet(); 
            rangeset->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetShowAsString(true);
            res_item.SetType(result_type::rangeset);
            res_item.SetName(key.toStdString());
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetResult(rangeset);
            res_item.SetYAxisMode(yaxis_mode::log);
            res_item.SetYLimit(_range::high, 1.0);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("Posterior Predicted Constituents"))
        {
            CMBTimeSeriesSet* posterior_predicted_distribution = new CMBTimeSeriesSet();
            posterior_predicted_distribution->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetShowAsString(false);
            res_item.SetType(result_type::distribution_with_observed);
            res_item.SetName(key.toStdString());
            res_item.SetResult(posterior_predicted_distribution);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("Predicted Samples Credible Intervals"))
        {
            RangeSet* rangeset = new RangeSet();
            rangeset->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetShowAsString(true);
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetType(result_type::rangeset_with_observed);
            res_item.SetName(key.toStdString());
            res_item.SetYAxisMode(yaxis_mode::log);
            res_item.SetResult(rangeset);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("DFA coefficients"))
        {
            CMBVector* dfaresults = new CMBVector();
            dfaresults->ReadFromJsonObject(jsonobject[key].toObject());
            ResultItem res_item;
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetShowGraph(true);
            res_item.SetType(result_type::vector);
            res_item.SetName(key.toStdString());
            res_item.SetYAxisMode(yaxis_mode::normal);
            res_item.SetResult(dfaresults);
            operator[](key.toStdString()) = res_item;
        }
        else if (key.contains("Bracketing results"))
        {
            CMBVector* bracketingresults = new CMBVector();
            bracketingresults->ReadFromJsonObject(jsonobject[key].toObject());
            bracketingresults->SetBooleanValue(true);
            ResultItem res_item;
            res_item.SetShowAsString(true);
            res_item.SetShowTable(true);
            res_item.SetShowGraph(false);
            res_item.SetType(result_type::vector);
            res_item.SetName(key.toStdString());
            res_item.SetYAxisMode(yaxis_mode::normal);
            res_item.SetResult(bracketingresults);
            operator[](key.toStdString()) = res_item;
        }


     }
    return true;

}

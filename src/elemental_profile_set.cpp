#include "elemental_profile_set.h"
#include "iostream"

using namespace std;

Elemental_Profile_Set::Elemental_Profile_Set() :map<string, Elemental_Profile>(), Interface()
{

}

Elemental_Profile_Set::Elemental_Profile_Set(const Elemental_Profile_Set& mp) :map<string, Elemental_Profile>(mp), Interface()
{
    element_distributions = mp.element_distributions;
    contribution = mp.contribution;
}

Elemental_Profile_Set& Elemental_Profile_Set::operator=(const Elemental_Profile_Set &mp)
{
    map<string, Elemental_Profile>::operator=(mp);
    Interface::operator=(mp);
    contribution = mp.contribution;
    element_distributions = mp.element_distributions;
    return *this;
}

Elemental_Profile *Elemental_Profile_Set::Append_Profile(const string &name, const Elemental_Profile &profile)
{
    if (count(name)>0)
    {
        cout<<"Profile '" + name + "' already exists!"<<std::endl;
        return nullptr;
    }
    else
    {
        operator[](name) = profile;
    }
    for (map<string,double>::const_iterator it=profile.begin(); it!=profile.end(); it++)
    {
        element_distributions[it->first].push_back(it->second);
    }
    return &operator[](name);
}

vector<string> Elemental_Profile_Set::SampleNames()
{
    vector<string> out;
    for (map<string,Elemental_Profile>::iterator it=begin(); it!=end(); it++)
    {
        out.push_back(it->first);
    }
    return out;
}

string Elemental_Profile_Set::ToString()
{
    string out; 
    if (size() == 0) return string(); 
    out += "Element name \t";
    for (map<string, Elemental_Profile>::iterator it = begin(); it != end(); it++)
    {
        out += it->first + "\t";
    }
    out += "\n";
    vector<string> elements = ElementNames(); 
    for (int i = 0; i < elements.size(); i++)
    {
        out += elements[i];
        for (map<string, Elemental_Profile>::iterator it = begin(); it != end(); it++)
        {
            out += "\t" + aquiutils::numbertostring(it->second.at(elements[i])); 
        }
        out += "\n";
    }
    return out; 
}

bool Elemental_Profile_Set::writetofile(QFile* file)
{
    file->write(QString::fromStdString(ToString()).toUtf8());
    return 0;
}

QJsonObject Elemental_Profile_Set::toJsonObject()
{
    string out;
    QJsonObject json_object;
    if (size() == 0) return QJsonObject();
    for (map<string, Elemental_Profile>::iterator it = begin(); it != end(); it++)
    {
        json_object[QString::fromStdString(it->first)] = it->second.toJsonObject(); 
    }
    return json_object;
    
}

bool Elemental_Profile_Set::ReadFromJsonObject(const QJsonObject &jsonobject)
{
    clear();
    for(QString key: jsonobject.keys() ) {
        Elemental_Profile elemental_profile;
        elemental_profile.ReadFromJsonObject(jsonobject[key].toObject());
        Append_Profile(key.toStdString(), elemental_profile);
    }
    return true;
}

bool Elemental_Profile_Set::Read(const QStringList &strlist)
{


    return true;
}

bool Elemental_Profile_Set::ContainsElement(const string &elementname)
{
    if (size()==0)
        return false;

    bool out=true;
    for (map<string, Elemental_Profile>::iterator it = begin(); it != end(); it++)
    {
        out &= it->second.contains(elementname);

    }
    return out;

}

vector<string> Elemental_Profile_Set::ElementNames()
{
    vector<string> out; 
    if (size() == 0)
        return vector<string>(); 
    for (map<string, double>::iterator it = operator[](SampleNames()[0]).begin(); it != operator[](SampleNames()[0]).end(); it++)
        out.push_back(it->first);
    return out; 
}


Elemental_Profile *Elemental_Profile_Set::Profile(const string &name)
{
    if (count(name)==0)
    {
        cout<<"Sample '" + name + "' does not exist!"<<std::endl;
        return nullptr;
    }
    else
        return &operator[](name);

}

Elemental_Profile *Elemental_Profile_Set::Profile(unsigned int i)
{
    if (i>=size())
        return nullptr;
    else
    {
        map<string,Elemental_Profile>::iterator it=begin();
        for (int ii=0; ii<i; ii++)
            it++;
        return &it->second;
    }

}

Elemental_Profile Elemental_Profile_Set::Profile(const string &name) const
{
    if (count(name)==0)
    {
        cout<<"Sample '" + name + "' does not exist!"<<std::endl;
        return Elemental_Profile();
    }
    else
        return at(name);

}

Elemental_Profile Elemental_Profile_Set::Profile(unsigned int i) const
{
    if (i>=size())
        return Elemental_Profile();
    else
    {
        map<string,Elemental_Profile>::const_iterator it=begin();
        for (int ii=0; ii<i; ii++)
            it++;
        return it->second;
    }

}

vector<double> Elemental_Profile_Set::GetProfileForSample(const string &sample_name)
{
    if (!Profile(sample_name))
        return vector<double>();

    return Profile(sample_name)->Vals();
}

double Elemental_Profile_Set::max()
{
    double _max = -1e12;
    for (map<string, Elemental_Profile>::iterator it = begin(); it != end(); it++)
    {
        if (it->second.max() > _max) _max = it->second.max(); 
    }
    return _max; 
}

double Elemental_Profile_Set::min()
{
    double _min = 1e12;
    for (map<string, Elemental_Profile>::iterator it = begin(); it != end(); it++)
    {
        if (it->second.min() < _min) _min = it->second.min();
    }
    return _min;
}

MultipleLinearRegressionSet Elemental_Profile_Set::regress_vs_size_OM(const string &om, const string &d)
{
    MultipleLinearRegressionSet out;
    vector<string> elementnames = ElementNames();
    for (unsigned int i=0; i<elementnames.size(); i++)
    {
        out[elementnames[i]] = regress_vs_size_OM(elementnames[i],om,d);
    }
    return out;
}

MultipleLinearRegression Elemental_Profile_Set::regress_vs_size_OM(const string &element, const string &om, const string &d)
{
    MultipleLinearRegression out;
    vector<double> dependent = GetProfileForSample(element);
    vector<vector<double>> independents;
    independents.push_back(GetProfileForSample(om));
    independents.push_back(GetProfileForSample(d));
    out.Regress(independents,dependent);
    return out;
}

void Elemental_Profile_Set::SetRegression(const string &om, const string &d)
{
    mlr_vs_om_size = regress_vs_size_OM(om,d);
}

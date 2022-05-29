#include "elemental_profile_set.h"
#include "iostream"

using namespace std;

Elemental_Profile_Set::Elemental_Profile_Set()
{

}

Elemental_Profile_Set::Elemental_Profile_Set(const Elemental_Profile_Set& mp)
{
    elemental_profiles = mp.elemental_profiles;
}

Elemental_Profile_Set& Elemental_Profile_Set::operator=(const Elemental_Profile_Set &mp)
{
    elemental_profiles = mp.elemental_profiles;
    return *this;
}

Elemental_Profile *Elemental_Profile_Set::Append_Profile(const string &name, const Elemental_Profile &profile)
{
    if (elemental_profiles.count(name)>0)
    {
        cout<<"Profile '" + name + "' already exists!"<<endl;
        return nullptr;
    }
    else
    {
        elemental_profiles[name] = profile;
    }
    return &elemental_profiles[name];
}

vector<string> Elemental_Profile_Set::SampleNames()
{
    vector<string> out;
    for (map<string,Elemental_Profile>::iterator it=elemental_profiles.begin(); it!=elemental_profiles.end(); it++)
    {
        out.push_back(it->first);
    }
    return out;
}

Elemental_Profile *Elemental_Profile_Set::Profile(const string &name)
{
    if (elemental_profiles.count(name)==0)
    {
        cout<<"Sample '" + name + "' does not exist!"<<endl;
        return nullptr;
    }
    else
        return &elemental_profiles[name];

}

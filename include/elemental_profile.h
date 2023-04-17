#ifndef ELEMENTAL_PROFILE_H
#define ELEMENTAL_PROFILE_H

#include <map>
#include <string>
#include <vector>
#include "interface.h"
#include "multiplelinearregressionset.h"

using namespace std;
class Elemental_Profile : public map<string,double>, public Interface
{
public:
    Elemental_Profile();
    Elemental_Profile(const Elemental_Profile& mp);
    Elemental_Profile& operator=(const Elemental_Profile &mp);
    double Val(const string &name) const; // returns the value of elemental content for a specific element/constituent
    bool SetVal(const string &name, const double &val); //Sets the value of elemental content for a specific element/constituent
    bool SetMark(const string &name, bool val); //mark specific element/constituent
    bool Mark(const string &name);
    bool AppendElement(const string &name,const double &val=0); //Append an element and it's value to the list of elements
    vector<double> Vals(); //Returns a vector containing the values of all emenents in the profile
    string ToString() override;
    QTableWidget *ToTable() override;
    bool Read(const QStringList &strlist) override;
    QJsonObject toJsonObject() override;
    bool ReadFromJsonObject(const QJsonObject &jsonobject) override;
    double max(); 
    double min(); 
    vector<string> ElementNames();
    bool writetofile(QFile* file) override;
    bool contains(const string &element);
    bool IncludedInAnalysis() {return included_in_analysis;}
    void SetIncluded(bool value) {included_in_analysis = value;}
    Elemental_Profile OrganicandSizeCorrect(const double &size, const double &om, MultipleLinearRegressionSet *mlr);
    double DotProduct(const CMBVector &vector);
private:
    bool included_in_analysis = true;
    map<string,bool> marked;
};


#endif // ELEMENTAL_PROFILE_H

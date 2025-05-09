#ifndef ELEMENTAL_PROFILE_H
#define ELEMENTAL_PROFILE_H

#include <map>
#include <string>
#include <vector>
#include "interface.h"
#include "multiplelinearregressionset.h"

struct element_information
{
    enum class role {do_not_include, isotope, particle_size, element, organic_carbon} Role = role::element;
    double standard_ratio;
    string base_element;
    bool include_in_analysis = true;

};


//using namespace std;
class Elemental_Profile : public map<string,double>, public Interface
{
public:
    Elemental_Profile();
    Elemental_Profile(const Elemental_Profile& mp);
    Elemental_Profile& operator=(const Elemental_Profile &mp);
    Elemental_Profile CopyIncluded(map<string,element_information> *elementinfo=nullptr) const;
    Elemental_Profile CopyandCorrect(bool exclude_elements, bool omnsizecorrect, const vector<double> &om_size, const MultipleLinearRegressionSet *mlr=nullptr, const map<string, element_information> *elementinfo=nullptr) const;
    Elemental_Profile Extract(const vector<string> &element_list) const; //extract the elements included in the element_list
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
    bool IncludedInAnalysis() const {return included_in_analysis;}
    void SetIncluded(bool value) {included_in_analysis = value;}
    Elemental_Profile OrganicandSizeCorrect(const vector<double> &om_size, const MultipleLinearRegressionSet *mlr, const map<string, element_information> *elementinfo=nullptr) const;
    double DotProduct(const CMBVector &vector) const;
    CMBVector SortByValue(bool ascending = true) const;
    vector<string> SelectTop(int n, bool ascending = true) const;
    Elemental_Profile ExtractElementsOnly(const map<string, element_information> *ElementInformation, bool isotopes=false) const;
private:
    bool included_in_analysis = true;
    map<string,bool> marked;
};


#endif // ELEMENTAL_PROFILE_H

#ifndef CMBVector_H
#define CMBVector_H

#include "Vector.h"
#include "interface.h"
#include "range.h"

class CMBVector : public CVector, public Interface
{
public:
    CMBVector();
    CMBVector(int n);
    CMBVector(const CMBVector& mp);
    CMBVector(const CVector_arma& mp);
    CMBVector& operator=(const CMBVector &mp);
    CMBVector(const CVector& mp);

    CMBVector& operator=(const CVector &mp);
    CMBVector& operator=(const CVector_arma &mp);

    QJsonObject toJsonObject() override;
    bool ReadFromJsonObject(const QJsonObject &jsonobject) override;
    string ToString() override;
    QTableWidget *ToTable() override;
    bool writetofile(QFile* file) override;
    double valueAt(int i) const;
    string Label(int i) const {return labels[i];}
    double valueAt(const string &label ) const;
    int LookupLabel(const string &label ) const;
    vector<string> Labels() const {return labels;}
    void SetLabel(int i, const string &label) {labels[i]=label;}
    void SetLabels(const vector<string> &label) {labels=label;}
    void SetBooleanValue(bool val) {boolean_values = val;}
    CVector toVector() const;
    CMBVector Sort(const CMBVector &sortvector=CMBVector()) const;
    CMBVector AbsSort(const CMBVector &sortvector=CMBVector()) const;
    string MaxElement() const;
    string MaxAbsElement()const ;
    CMBVector Eliminate(const string &element) const;
    void append(const string &label, const double &val);
    CMBVector Extract(int start, int end) const;
    int size() const {return num;}
    CMBVector ExtractWithinRange(const double &lowval, const double &highval) const; //Extract element names with values within lowval and highval
    CMBVector ExtractUpToMinimum() const; //Extrac the element names up to the minimum value

private:
    vector<string> labels;
    bool boolean_values = false;



};

CMBVector operator+(const CMBVector&, const CMBVector&);
CMBVector operator+(double, const CMBVector&);
CMBVector operator+(const CMBVector&, double);
CMBVector operator-(const CMBVector&, const CMBVector&);
CMBVector operator-(double, const CMBVector&);
CMBVector operator-(const CMBVector&, double);
CMBVector operator*(const CMBVector&, const CMBVector&);
CMBVector operator*(double, const CMBVector&);
CMBVector operator/(const CMBVector&, double);


#endif // CMBVector_H

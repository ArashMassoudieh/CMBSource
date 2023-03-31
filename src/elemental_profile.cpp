#include "elemental_profile.h"
#include "iostream"
#include "Utilities.h"
#include <QFile>

using namespace std;

Elemental_Profile::Elemental_Profile() :map<string, double>(), Interface()
{

}

Elemental_Profile::Elemental_Profile(const Elemental_Profile& mp):map<string,double>(mp),Interface()
{
    marked = mp.marked;
}

Elemental_Profile& Elemental_Profile::operator=(const Elemental_Profile &mp)
{
    Interface::operator=(mp);
    marked = mp.marked;
    map<string,double>::operator=(mp);
    return *this;
}



double Elemental_Profile::Val(const string &name) const
{
    if (count(name)==0)
    {
        cout<<"Element '" + name + "' does not exist!"<<std::endl;
        return -1;
    }
    else
        return at(name);
}


bool Elemental_Profile::SetVal(const string &name, const double &val)
{
    if (count(name)==0)
    {
        cout<<"Element '" + name + "' does not exist!"<<std::endl;
        return false;
    }
    else
    {   operator[](name)=val;
        return true;
    }
}

bool Elemental_Profile::SetMark(const string &name, bool val)
{
    if (count(name)==0)
    {
        cout<<"Element '" + name + "' does not exist!"<<std::endl;
        return false;
    }
    else
    {   marked[name]=val;
        return true;
    }
}

bool Elemental_Profile::Mark(const string &name)
{
    if (count(name)==0)
    {
        cout<<"Element '" + name + "' does not exist!"<<std::endl;
        return false;
    }
    else
    {   return marked[name];

    }
}

bool Elemental_Profile::AppendElement(const string &name,const double &val)
{
    if (count(name)==0)
    {
        operator[](name) =val;
        marked[name] = false;
        return true;
    }
    else
    {   cout<<"Element '" + name + "' already exists";
        return false;
    }
}

vector<double> Elemental_Profile::Vals()
{
    vector<double> vals;
    for (map<string,double>::iterator it=begin(); it!=end(); it++)
        vals.push_back(it->second);

    return vals;
}

string Elemental_Profile::ToString()
{
    string out;
    for (map<string,double>::iterator it=begin(); it!=end(); it++)
    {
        out += it->first + ":" + aquiutils::numbertostring(it->second) + "\n";
    }
    return out;
}

QTableWidget *Elemental_Profile::ToTable()
{
    QTableWidget *tablewidget = new QTableWidget();
    tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablewidget->setColumnCount(1);
    tablewidget->setRowCount(size());
    QStringList headers;
    QStringList constituents;
    int i=0;
    for (map<string,double>::const_iterator it=cbegin(); it!=cend(); it++ )
    {
        constituents<<QString::fromStdString(it->first);
        tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(it->second)));
        i++;
    }
    headers << "Value";
    tablewidget->setHorizontalHeaderLabels(headers);
    tablewidget->setVerticalHeaderLabels(constituents);
    return tablewidget;
}

QJsonObject Elemental_Profile::toJsonObject()
{
    QJsonObject json_object; 
    for (map<string, double>::iterator it = begin(); it != end(); it++)
    {
        json_object[QString::fromStdString(it->first)] = it->second;
    }
    return json_object;
}

bool Elemental_Profile::ReadFromJsonObject(const QJsonObject &jsonobject)
{
    clear();
    for(QString key: jsonobject.keys() ) {
        operator[](key.toStdString()) = jsonobject[key].toDouble();
    }
    return true;
}

bool Elemental_Profile::Read(const QStringList &strlist)
{
    clear();
    for (int i=0; i<strlist.size(); i++)
    {
        if (strlist[i].split(":").size()>1)
        {
            AppendElement(strlist[i].split(":")[0].toStdString(), strlist[i].split(":")[1].toDouble());
        }
    }
    return true;
}

bool Elemental_Profile::writetofile(QFile* file)
{
    file->write(QString::fromStdString(ToString()).toUtf8());
    return true;
}

double Elemental_Profile::max()
{
    double _max = -1e12; 
    for (map<string, double>::iterator it = begin(); it != end(); it++)
    {
        if (it->second > _max) _max = it->second; 
    }
    return _max; 
}
double Elemental_Profile::min()
{
    double _min = 1e12;
    for (map<string, double>::iterator it = begin(); it != end(); it++)
    {
        if (it->second < _min) _min = it->second;
    }
    return _min;
}

vector<string> Elemental_Profile::ElementNames()
{
    vector<string> out;
    for (map<string, double>::iterator it = begin(); it != end(); it++)
    {
        out.push_back(it->first);
    }
    return out; 
}

bool Elemental_Profile::contains(const string &element)
{
    return (map<string,double>::count(element)!=0);
}

Elemental_Profile Elemental_Profile::OrganicandSizeCorrect(const double &size, const double &om, MultipleLinearRegressionSet *mlrset)
{
    Elemental_Profile out;
    for (map<string,double>::iterator it=begin(); it!=end(); it++)
    {
        out[it->first]=it->second;

        if (mlrset->count(it->first)==1)
        {   MultipleLinearRegression *mlr = &mlrset->operator[](it->first);

            if (mlr->Effective(0) && mlr->GetIndependentVariableNames()[0]!=it->first)
            {
                qDebug()<<om<<","<<this->at(mlr->GetIndependentVariableNames()[0])<<","<<mlr->CoefficientsIntercept()[1];
                if (mlr->Equation()==regression_form::linear)
                    out[it->first] += (om-this->at(mlr->GetIndependentVariableNames()[0]))*mlr->CoefficientsIntercept()[1];
                else
                    out[it->first] *= pow(om/this->at(mlr->GetIndependentVariableNames()[0]),mlr->CoefficientsIntercept()[1]);
            }
            if (mlr->Effective(1) && mlr->GetIndependentVariableNames()[1]!=it->first)
            {
                qDebug()<<size<<","<<this->at(mlr->GetIndependentVariableNames()[1])<<","<<mlr->CoefficientsIntercept()[2];
                if (mlr->Equation()==regression_form::linear)
                    out[it->first] += (size-this->at(mlr->GetIndependentVariableNames()[1]))*mlr->CoefficientsIntercept()[2];
                else
                    out[it->first] += pow(size/this->at(mlr->GetIndependentVariableNames()[1]),mlr->CoefficientsIntercept()[2]);

            }
        }
        if (out[it->first]<0)
        {
            cout<<"corrected value is negative!";
        }
    }
    return out;
}

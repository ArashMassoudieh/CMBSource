#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "BTC.h"

class Observation
{
public:
    Observation();
    Observation(const Observation &obs);
    Observation& operator=(const Observation &obs);
    void SetName(const string &nam) {name = nam;}
    string Name() {return name;}
    string GetName() { return name; }
    void SetValues(const CTimeSeries<double> &values);
    void AppendValues(const double &t, const double &val);
private:
    CTimeSeries<double> values;
    string name;
};

#endif // OBSERVATION_H
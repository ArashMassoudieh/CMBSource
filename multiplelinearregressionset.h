#ifndef MULTIPLELINEARREGRESSIONSET_H
#define MULTIPLELINEARREGRESSIONSET_H

#include "interface.h"
#include "multiplelinearregression.h"

class MultipleLinearRegressionSet: public map<string,MultipleLinearRegression>, public Interface
{
public:
    MultipleLinearRegressionSet();
    MultipleLinearRegressionSet(const MultipleLinearRegressionSet& mp);
    MultipleLinearRegressionSet& operator=(const MultipleLinearRegressionSet &mp);
    string ToString() override;
    QJsonObject toJsonObject() override;
    bool ReadFromJsonObject(const QJsonObject &jsonobject) override;
    bool Append(QString key, const MultipleLinearRegression &MLR);
    void SetSource(const string& source) { Source = source; }
    QTableWidget *ToTable() override;
    string Key(int i);

private:
    string Source; 
};

#endif // MULTIPLELINEARREGRESSIONSET_H

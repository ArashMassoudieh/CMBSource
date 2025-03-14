#ifndef RESULTS_H
#define RESULTS_H
#include <map>
#include <unordered_map>
#include <interface.h>
#include <string>

//using namespace std;

class QJsonObject;
class ResultItem;


class Results: public map<string,ResultItem>
{
public:
    Results();
    Results(const Results &rhs);
    Results& operator = (const Results &rhs);
    void Append(const ResultItem&);
    void SetName(const string &_name) {name = _name;}
    bool ReadFromJson(const QJsonObject &jsonobject);
    string GetName() {return name;}
    QJsonObject toJsonObject();
    void SetError(const string &_error) {error = _error;}
    void AppendError(const string &_error) {error = error + _error;}
    string Error() {return error;}
private:
    string name;
    string error;

};

#endif // RESULTS_H

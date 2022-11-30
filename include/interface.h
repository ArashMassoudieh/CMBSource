#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <QStringList>

class QFile; 

using namespace std;

class Interface
{
public:
    Interface();
    Interface(const Interface &intf);
    Interface& operator=(const Interface &intf);
    virtual string ToString();
    virtual bool writetofile(QFile* file);
    virtual bool Read(const QStringList &strlist);

};

#endif // INTERFACE_H

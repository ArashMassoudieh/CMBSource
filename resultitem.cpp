#include "resultitem.h"

ResultItem::ResultItem():Interface()
{
    YLimits.resize(2);
}

ResultItem::ResultItem(const ResultItem &rhs): Interface(rhs)
{
    name = rhs.name;
    type = rhs.type;
    result = rhs.result;
    y_axis_mode = rhs.y_axis_mode;
    x_axis_mode = rhs.x_axis_mode;
    showasstring = rhs.showasstring;
    fixYlimit = rhs.fixYlimit;
    YLimits = rhs.YLimits;
    showTable = rhs.showTable;
    showGraph = rhs.showGraph;
    showabsvalue = rhs.showabsvalue;
}
ResultItem& ResultItem::operator = (const ResultItem &rhs)
{
    Interface::operator=(rhs);
    name = rhs.name;
    type = rhs.type;
    result = rhs.result;
    y_axis_mode = rhs.y_axis_mode;
    x_axis_mode = rhs.x_axis_mode;
    showasstring = rhs.showasstring;
    fixYlimit = rhs.fixYlimit;
    YLimits = rhs.YLimits;
    showTable = rhs.showTable;
    showGraph = rhs.showGraph;
    showabsvalue = rhs.showabsvalue;
    return *this;
}

#include <QRubberBand>
#include "generalplotter.h"
#include "Utilities.h"

template<typename T>
static inline QVector<T> fromStdVector(const std::vector<T> &vector)
{
   return QVector<T>(vector.begin(), vector.end());
}

QVector<QCPScatterStyle::ScatterShape> GeneralPlotter::shapes = Make_Shapes();

GeneralPlotter::GeneralPlotter(QWidget * parent)
    : QCustomPlot(parent)
    , mZoomMode(true)
    , mRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{
    x_max_min_range.push_back(1e12);x_max_min_range.push_back(-1e12);
    y_max_min_range.push_back(1e12);y_max_min_range.push_back(-1e12);


}

GeneralPlotter::~GeneralPlotter()
{
    delete mRubberBand;
}

void GeneralPlotter::setZoomMode(bool mode)
{
    mZoomMode = mode;
}

void GeneralPlotter::mousePressEvent(QMouseEvent * event)
{
    if (mZoomMode)
    {
        if (event->button() == Qt::LeftButton)
        {
            mOrigin = event->pos();
            mRubberBand->setGeometry(QRect(mOrigin, QSize()));
            mRubberBand->show();
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void GeneralPlotter::mouseMoveEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
    }
    //QCustomPlot::mouseMoveEvent(event);
}
void GeneralPlotter::wheelEvent(QWheelEvent *event)
{
    double scale = pow((double)2, event->angleDelta().y() / 360.0);
    xAxis->scaleRange(scale, xAxis->range().center());
    yAxis->scaleRange(scale, yAxis->range().center());
    replot();

}
void GeneralPlotter::mouseReleaseEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        if (mRubberBand->geometry().height() > 5 && mRubberBand->geometry().width() > 5)
        {
            const QRect & zoomRect = mRubberBand->geometry();
            int xp1, yp1, xp2, yp2;
            zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
            auto x1 = xAxis->pixelToCoord(xp1);
            auto x2 = xAxis->pixelToCoord(xp2);
            auto y1 = yAxis->pixelToCoord(yp1);
            auto y2 = yAxis->pixelToCoord(yp2);

            xAxis->setRange(x1, x2);
            yAxis->setRange(y1, y2);
        }
        mRubberBand->hide();
        replot();
    }

    QCustomPlot::mouseReleaseEvent(event);
}

void GeneralPlotter::axisDoubleClick(QCPAxis * axis, QCPAxis::SelectablePart part, QMouseEvent * event)
{
    rescaleAxes(true);
    /*
    if (!graphCount())
        return;
    if (axis == xAxis)
    {
        qreal min = *std::min_element(graph(0)->data()->keys().begin(), graph(0)->data()->keys().end());
        qreal max = *std::max_element(graph(0)->data()->keys().begin(), graph(0)->data()->keys().end());
        for (int i = 0; i < graphCount(); i++)
        {
            if (min > *std::min_element(graph(i)->data()->keys().begin(), graph(i)->data()->keys().end()))
                min = *std::min_element(graph(i)->data()->keys().begin(), graph(i)->data()->keys().end());
            if (max < *std::max_element(graph(i)->data()->keys().begin(), graph(i)->data()->keys().end()))
                max = *std::max_element(graph(i)->data()->keys().begin(), graph(i)->data()->keys().end());
        }
        axis->setRange(min, max);
    }
    else if (axis == yAxis)
    {
        qreal min = *std::min_element(graph(0)->data()->values().begin(), graph(0)->data()->values().end());
        qreal max = *std::max_element(graph(0)->data()->values().begin(), graph(0)->data()->values().end());
        for (int i = 0; i < graphCount(); i++)
        {
            if (min > *std::min_element(graph(i)->data() .begin(), graph(i)->data()->values().end()))
                min = *std::min_element(graph(i)->data()->values().begin(), graph(i)->data()->values().end());
            if (max < *std::max_element(graph(i)->data()->values().begin(), graph(i)->data()->values().end()))
                max = *std::max_element(graph(i)->data()->values().begin(), graph(i)->data()->values().end());
        }
        axis->setRange(min, max);
    }
    axis->rang*/
}

bool GeneralPlotter::AddScatter(const string &name, const vector<double> &x, const vector<double> &y, const QCPScatterStyle &symbol)
{
    x_max_min_range[0] = min(x_max_min_range[0],aquiutils::Min(x));
    x_max_min_range[1] = max(x_max_min_range[1],aquiutils::Max(x));
    y_max_min_range[0] = min(y_max_min_range[0],aquiutils::Min(y));
    y_max_min_range[1] = max(y_max_min_range[1],aquiutils::Max(y));
    if (x.size()!=y.size())
        return false;

    QCPGraph *graph = new QCPGraph(xAxis, yAxis);

    graph->setName(QString::fromStdString(name));
    graph->setPen(QPen(QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256))));
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(symbol);

    QVector<double> xvals = fromStdVector<double>(x);
    QVector<double> yvals = fromStdVector<double>(y);

    graph->setData(xvals, yvals);
    SetRange(x_max_min_range,Axis::x);
    SetRange(y_max_min_range,Axis::y);
    replot();
    return true;
}
bool GeneralPlotter::AddScatter(const string &name, const vector<string> &x, const vector<double> &y, const QCPScatterStyle &symbol)
{
    x_max_min_range[0] = min(x_max_min_range[0],double(0));
    x_max_min_range[1] = max(x_max_min_range[1],double(x.size())*1.1);
    y_max_min_range[0] = min(y_max_min_range[0],aquiutils::Min(y));
    y_max_min_range[1] = max(y_max_min_range[1],aquiutils::Max(y));
    if (x.size()!=y.size())
        return false;

    QCPGraph *graph = new QCPGraph(xAxis, yAxis);

    graph->setName(QString::fromStdString(name));
    graph->setPen(QPen(QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256)), 2));
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(symbol);

    QVector<double> yvals = fromStdVector<double>(y);

    QVector<double> ticks;
    QVector<QString> labels;
    QVector<double> qvals = fromStdVector<double>(y);
    for (int i=0; i<y.size(); i++)
    {   ticks << i+1;
        labels << QString::fromStdString(x[i]);
    }
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    xAxis->setTicker(textTicker);
    xAxis->setTickLabelRotation(60);
    xAxis->setSubTicks(false);
    xAxis->setTickLength(0, 4);

    graph->setData(ticks, yvals);
    SetRange(x_max_min_range,Axis::x);
    SetRange(y_max_min_range,Axis::y);
    replot();
    return true;
}

void GeneralPlotter::ZoomExtends()
{
    SetRange(x_max_min_range,Axis::x);
    SetRange(y_max_min_range,Axis::y);
    replot();
}

bool GeneralPlotter::AddScatters(const vector<string> names, const vector<vector<double>> &x,const vector<vector<double>> &y)
{
    if (names.size()!=x.size())
        return false;
    if (names.size()!=y.size())
        return false;
    for (int i=0; i<names.size(); i++)
    {
        AddScatter(names[i],x[i],y[i],shapes[i%shapes.size()]);
    }
    replot();
    return true;
}
bool GeneralPlotter::AddScatters(const vector<string> names, const vector<string> &x,const vector<vector<double>> &y)
{
    if (names.size()!=y.size())
        return false;
    for (int i=0; i<names.size(); i++)
    {
        if (y[i].size()!=x.size())
            return false;
        AddScatter(names[i],x,y[i],shapes[i%shapes.size()]);
    }
    replot();
    return true;
}
bool GeneralPlotter::AddTimeSeries(const string &name, const vector<double> &x, const vector<double> &y)
{
    x_max_min_range[0] = min(x_max_min_range[0],aquiutils::Min(x));
    x_max_min_range[1] = max(x_max_min_range[1],aquiutils::Max(x)*1.1);
    y_max_min_range[0] = min(y_max_min_range[0],aquiutils::Min(y));
    y_max_min_range[1] = max(y_max_min_range[1],aquiutils::Max(y));
    if (x.size()!=y.size())
        return false;

    QCPGraph *graph = new QCPGraph(xAxis, yAxis);

    graph->setName(QString::fromStdString(name));
    QPen Pen = QPen(QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256)));
    Pen.setWidth(3);
    graph->setPen(Pen);
    graph->setLineStyle(QCPGraph::lsLine);

    QVector<double> yvals = fromStdVector<double>(y);
    QVector<double> xvals = fromStdVector<double>(x);

    xAxis->setTickLabelRotation(60);
    xAxis->setSubTicks(false);
    xAxis->setTickLength(0, 4);

    graph->setData(xvals, yvals);
    SetRange(x_max_min_range,Axis::x);
    SetRange(y_max_min_range,Axis::y);
    replot();
    return true;

}
bool GeneralPlotter::AddTimeSeriesSet(const string &name, const vector<vector<double>> &x, const vector<vector<double>> &y)
{
    return true;
}
bool GeneralPlotter::SetYAxisScaleType(AxisScale axisscale)
{
    if (axisscale==AxisScale::log)
    {
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        logTicker->setLogBase(10);
        logTicker->setSubTickCount(10);
        yAxis->setTicker(logTicker);
        yAxis->setScaleType(QCPAxis::ScaleType::stLogarithmic);
    }
    else
    {
        yAxis->setScaleType(QCPAxis::ScaleType::stLinear);
    }
    return true;
}

void GeneralPlotter::SetRange(const vector<double> &range, const Axis &whichaxis)
{
    if (range.size()!=2)
    {
        return;
    }
    if (whichaxis==Axis::x)
    {
        xAxis->setRange(range[0],range[1]);
    }
    if (whichaxis==Axis::y)
    {
        yAxis->setRange(range[0],range[1]);
    }
}

void GeneralPlotter::SetLegend(bool onoff)
{
    legend->setVisible(onoff);
    if (onoff)
    {   axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
        legend->setBrush(QColor(255, 255, 255, 100));
        legend->setBorderPen(Qt::NoPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        legend->setFont(legendFont);
        setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }
}

void GeneralPlotter::Clear()
{
 clearPlottables();
 clearGraphs();
}

bool GeneralPlotter::AddNoneUniformScatter(const map<string,vector<double>> &data, int shape_counter)
{
    x_max_min_range[0] = double(0);
    x_max_min_range[1] = double(data.size()*1.1);


    QCPGraph *points = new QCPGraph(xAxis, yAxis);
    points->setLineStyle(QCPGraph::lsNone);
    points->setScatterStyle(shapes[shape_counter]);
    points->setPen(QPen(QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256)),2));
    int x_val=1;
    QMap<double,QString> ticks;
    for (map<string,vector<double>>::const_iterator set=data.begin(); set!=data.end(); set++ )
    {
        for (int i=0; i<set->second.size(); i++)
            points->addData(x_val,set->second[i]);

        ticks[x_val]=QString::fromStdString(set->first);
        x_val++;
        y_max_min_range[0] = min(y_max_min_range[0],aquiutils::Min(set->second));
        y_max_min_range[1] = max(y_max_min_range[1],aquiutils::Max(set->second));
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks);
    xAxis->setTicker(textTicker);
    xAxis->setTickLabelRotation(60);
    xAxis->setSubTicks(false);
    xAxis->setTickLength(0, 4);

    SetRange(x_max_min_range,Axis::x);
    SetRange(y_max_min_range,Axis::y);
    replot();
    return true;
}


QVector<QCPScatterStyle::ScatterShape> Make_Shapes()
{
    QVector<QCPScatterStyle::ScatterShape> shapes;
    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
    shapes << QCPScatterStyle::ssStar;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;
    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
    shapes << QCPScatterStyle::ssStar;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;
    return shapes;
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sourcesinkdata.h"
#include "generalplotter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetSinkSheet(int i) {Sink_Sheet = i;}
    void WriteMessageOnScreen(const QString &text, QColor color=Qt::black);
    SourceSinkData *Data() {return &data;}
private:
    Ui::MainWindow *ui;
    int Sink_Sheet=-1;
    bool ReadExcel(const QString &filename);
    SourceSinkData data;
    GeneralPlotter *plotter = nullptr;
    QStandardItemModel* ToQStandardItemModel(const SourceSinkData* srcsinkdata);
    QStandardItem* ToQStandardItem(const QString &key, const QJsonObject &json);
    QStandardItemModel* ToQStandardItemModel(const QJsonDocument &jsondocument);
    QStandardItemModel *columnviewmodel = nullptr;
    QJsonDocument loadJson(const QString &fileName);
    void saveJson(const QJsonDocument &document, const QString &fileName);
private slots:
    void on_import_excel();
    void on_plot_raw_elemental_profiles();
    void on_test_plot();
    void on_tree_selectionChanged(const QItemSelection &changed);



};
#endif // MAINWINDOW_H

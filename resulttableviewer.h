#ifndef RESULTTABLEVIEWER_H
#define RESULTTABLEVIEWER_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class ResultTableViewer;
}

class ResultTableViewer : public QDialog
{
    Q_OBJECT

public:
    explicit ResultTableViewer(QWidget *parent = nullptr);
    void SetTable(QTableWidget *tablewidget);
    ~ResultTableViewer();

private:
    Ui::ResultTableViewer *ui;
    QTableWidget *tablewidget;
public slots:
    void on_ExportToCSV();
};

#endif // RESULTTABLEVIEWER_H

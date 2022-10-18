#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QDialog>
#include "string"
#include "results.h"

using namespace std;

namespace Ui {
class ResultsWindow;
}

class ResultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsWindow(QWidget *parent = nullptr);
    void AppendText(const string &text);
    void AppendResult(const result_item &resultitem);
    ~ResultsWindow();
    void SetResults(Results* res) { results = res;  }

private:
    Ui::ResultsWindow *ui;
    Results* results = nullptr; 

public slots:
    void on_result_clicked();
};

#endif // RESULTSWINDOW_H
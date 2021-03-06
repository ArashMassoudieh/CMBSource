#ifndef GENERICFORM_H
#define GENERICFORM_H

#include <QWidget>
#include "QJsonObject"
#include "QPushButton"
#include "QComboBox"
#include "QLineEdit"
#include "QCheckBox"
#include "QSpinBox"

class MainWindow;

enum class delegate_type {LineEdit, ComboBox, CheckBox, SpinBox};
struct parameter_property
{
    QString Discription;
    delegate_type Type = delegate_type::LineEdit;
    QWidget *InputWidget=nullptr;
    QString DefaultValue;
    QString value()
    {
        if (Type==delegate_type::LineEdit)
        {
            return dynamic_cast<QLineEdit*>(InputWidget)->text();
        }
        if (Type==delegate_type::ComboBox)
        {
            return dynamic_cast<QComboBox*>(InputWidget)->currentText();
        }
        if (Type==delegate_type::CheckBox)
        {
            if (dynamic_cast<QCheckBox*>(InputWidget)->checkState()==Qt::CheckState::Checked)
                return "true";
            else
                return "false";
        }
        if (Type==delegate_type::SpinBox)
        {
            return QString::number(dynamic_cast<QSpinBox*>(InputWidget)->value());
        }
        return "";
    }
};

namespace Ui {
class GenericForm;
}

class GenericForm : public QWidget
{
    Q_OBJECT

public:
    explicit GenericForm(QJsonObject *formdata, QWidget *parent = nullptr);
    QVector<parameter_property> Parameter_Properties;
    ~GenericForm();
    bool SetCommand(const QString &cmd);

private:
    QPushButton *buttonOk = nullptr;
    QPushButton *buttonCancel = nullptr;
    Ui::GenericForm *ui;
    MainWindow *mainwindow();
    QString command;
public slots:
    void onProceed();
    void onCancel();
};

#endif // GENERICFORM_H

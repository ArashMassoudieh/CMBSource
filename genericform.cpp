#include "genericform.h"
#include "ui_genericform.h"
#include "QLabel"
#include "mainwindow.h"

GenericForm::GenericForm(QJsonObject *formdata, QWidget *parent, MainWindow *_mainwindow) :
    QWidget(parent),
    ui(new Ui::GenericForm)
{
    mainWindow = _mainwindow; 
    ui->setupUi(this);

    for (int i=0; i<formdata->keys().size(); i++)
    {
        if (formdata->value(formdata->keys()[i]).isObject())
        {
            QJsonObject object = formdata->value(formdata->keys()[i]).toObject();
            formdata->value(formdata->keys()[i]).toObject();
            parameter_property parameter_prop;
            parameter_prop.Discription = formdata->keys()[i];
            parameter_prop.DefaultValue = object.value("default").toString();
            if (object.contains("type"))
            {
                if (object["type"].toString()!="Description")
                {
                    QLabel *label = new QLabel(parameter_prop.Discription, this);
                    QString typeval = object.value("type").toString();
                    if (object.value("type").toString()=="spinBox")
                    {
                        parameter_prop.Type = delegate_type::SpinBox;
                        QSpinBox *spinbox = new QSpinBox(this);

                        if (object.contains("minimum"))
                            spinbox->setMinimum(object.value("minimum").toString().toInt());
                        if (object.contains("maximum"))
                        spinbox->setMaximum(object.value("maximum").toString().toInt());
                        spinbox->setValue(object.value("default").toString().toInt());
                        ui->formLayout->addRow(label,spinbox);
                        parameter_prop.InputWidget = spinbox;
                    }
                    if (object.value("type").toString()=="lineEdit")
                    {
                        parameter_prop.Type = delegate_type::LineEdit;
                        QLineEdit *lineedit = new QLineEdit(this);
                        lineedit->setText(object.value("default").toString());
                        ui->formLayout->addRow(label,lineedit);
                        parameter_prop.InputWidget = lineedit;
                    }
                    if (object.value("type").toString()=="comboBox")
                    {
                        parameter_prop.Type = delegate_type::ComboBox;
                        QComboBox *combobox = new QComboBox(this);
                        combobox->setCurrentText(object.value("default").toString());
                        ui->formLayout->addRow(label,combobox);
                        parameter_prop.InputWidget = combobox;
                        {
                            if (object.value("source").toString().contains("Blank"))
                            {
                                combobox->addItem("");
                            }
                            if (object.value("source").toString().contains("TargetSamplesList"))
                            {
                                vector<string> names = mainwindow()->Data()->SampleNames(mainwindow()->Data()->TargetGroup());
                                for (unsigned int i=0; i<names.size(); i++)
                                    combobox->addItem(QString::fromStdString(names[i]));
                            }
                            if (object.value("source").toString().contains("ElementsList"))
                            {
                                vector<string> names = mainwindow()->Data()->ElementNames();
                                for (unsigned int i=0; i<names.size(); i++)
                                    combobox->addItem(QString::fromStdString(names[i]),QString::fromStdString(names[i]));

                            }
                            if (object.value("source").toString().contains("SourceList"))
                            {
                                vector<string> names = mainwindow()->Data()->SourceGroupNames();
                                for (unsigned int i=0; i<names.size(); i++)
                                    combobox->addItem(QString::fromStdString(names[i]),QString::fromStdString(names[i]));

                            }
                            if (object.value("source").toString().contains("Items:"))
                            {
                                QStringList items = object.value("source").toString().split(":")[1].split(",");
                                combobox->addItems(items);
                            }
                            if (object.value("source").toString().contains("OM"))
                            {
                                QString element = QString::fromStdString(mainwindow()->Data()->FirstOMConstituent());
                                if (element!="")
                                    combobox->setCurrentIndex(combobox->findText(element));
                                int i = combobox->findText(element);
                                qDebug() << i;
                            }
                            if (object.value("source").toString().contains("Size"))
                            {
                                QString element = QString::fromStdString(mainwindow()->Data()->FirstSizeConstituent());
                                if (element!="")
                                    combobox->setCurrentIndex(combobox->findText(element));
                                int i = combobox->findText(element);
                                qDebug() << i;
                            }
                        }
                    }
                    if (object.value("type").toString()=="fileBrowser_Save")
                    {
                        parameter_prop.Type = delegate_type::FileBrowser;
                        FileBrowserPushButtom *filebrowserpushbuttom = new FileBrowserPushButtom(this);
                        filebrowserpushbuttom->dialog_use = save_open::save;
                        filebrowserpushbuttom->setText(object.value("default").toString());
                        ui->formLayout->addRow(label,filebrowserpushbuttom);
                        parameter_prop.InputWidget = filebrowserpushbuttom;

                    }
                    if (object.value("type").toString()=="fileBrowser_Open")
                    {
                        parameter_prop.Type = delegate_type::FileBrowser;
                        FileBrowserPushButtom *filebrowserpushbuttom = new FileBrowserPushButtom(this);
                        filebrowserpushbuttom->dialog_use = save_open::open;
                        filebrowserpushbuttom->setText(object.value("default").toString());
                        ui->formLayout->addRow(label,filebrowserpushbuttom);
                        parameter_prop.InputWidget = filebrowserpushbuttom;

                    }
                    if (object.value("type").toString()=="checkBox")
                    {
                        parameter_prop.Type = delegate_type::CheckBox;
                        QCheckBox *checkbox = new QCheckBox(this);
                        if (object.value("default").toString().toLower()=="true")
                            checkbox->setCheckState(Qt::CheckState::Checked);
                        else
                            checkbox->setCheckState(Qt::CheckState::Unchecked);
                        ui->formLayout->addRow(label,checkbox);
                        parameter_prop.InputWidget = checkbox;
                    }
                    Parameter_Properties.append(parameter_prop);
                }
            }
        }
    }

    for (int i=0; i<formdata->keys().size(); i++)
    {
        if (formdata->value(formdata->keys()[i]).isObject())
        {
            QJsonObject object = formdata->value(formdata->keys()[i]).toObject();
            formdata->value(formdata->keys()[i]).toObject();
            parameter_property parameter_prop;
            parameter_prop.Discription = formdata->keys()[i];
            parameter_prop.DefaultValue = object.value("default").toString();
            if (object.contains("type"))
            {

                QLabel *label;
                if (object["type"].toString()=="Description")
                    label = new QLabel(parameter_prop.Discription, this);
                QString typeval = object.value("type").toString();
                if (object.value("type").toString()=="Description")
                {
                    parameter_prop.Type = delegate_type::Description;
                    QTextBrowser *textBrowswer = new QTextBrowser(this);
                    textBrowswer->insertHtml(object.value("default").toString());
                    ui->formLayout->addRow(label,textBrowswer);
                    parameter_prop.InputWidget = textBrowswer;
                }
            }
        }
    }

    buttonOk = new QPushButton("Ok",this);
    QIcon iconOk = QIcon(qApp->applicationDirPath()+"/../../resources/Icons/Proceed.png");
    buttonOk->setIcon(iconOk);
    ui->horizontalLayout->addWidget(buttonOk);
    connect(buttonOk,SIGNAL(clicked()),this,SLOT(onProceed()));
    buttonCancel = new QPushButton("Cancel",this);
    QIcon iconCancel = QIcon(qApp->applicationDirPath()+"/../../resources/Icons/Cancel.png");
    buttonCancel->setIcon(iconCancel);
    ui->horizontalLayout->addWidget(buttonCancel);
    connect(buttonCancel,SIGNAL(clicked()),this,SLOT(onCancel()));

}

GenericForm::~GenericForm()
{
    Parameter_Properties.clear();
    delete ui;
}

void GenericForm::onProceed()
{
    map<string,string> arguments;
    for (int i=0; i<Parameter_Properties.count(); i++)
    {
        arguments[Parameter_Properties[i].Discription.toStdString()] = Parameter_Properties[i].value().toStdString();
    }
    mainwindow()->Execute(command.toStdString(),arguments);

}
void GenericForm::onCancel()
{
    this->close();
}

MainWindow *GenericForm::mainwindow()
{
    if (mainWindow!=nullptr)
    return mainWindow;
}

bool GenericForm::SetCommand(const QString &cmd)
{
    command = cmd;
    return true;
}

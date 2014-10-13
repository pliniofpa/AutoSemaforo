#include "configuration.h"
#include "ui_configuration.h"

Configuration::Configuration(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Configuration)
{
    ui->setupUi(this);
    //Removes 'What's it?' button in this dialog window.
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->ui->amaToVer_timeEdit->setTime(QTime(0,10));//Dez Minutos
    this->ui->verToPulso_timeEdit->setTime(QTime(0,5));//5 Minutos
    this->ui->pulso_timeEdit->setTime(QTime(0,0,0,300));//300 micro-segundos
    this->ui->sireneLigada_timeEdit->setTime(QTime(0,0,5));//5 Segundos
    this->ui->sireneDesligada_timeEdit->setTime(QTime(0,0,2));//2 Segundos

}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::on_restore_pushButton_clicked()
{
    this->ui->amaToVer_timeEdit->setTime(QTime(0,10));//Dez Minutos
    this->ui->verToPulso_timeEdit->setTime(QTime(0,5));//5 Minutos
    this->ui->pulso_timeEdit->setTime(QTime(0,0,0,300));//300 micro-segundos
    this->ui->sireneLigada_timeEdit->setTime(QTime(0,0,5));//5 Segundos
    this->ui->sireneDesligada_timeEdit->setTime(QTime(0,0,2));//2 Segundos
}

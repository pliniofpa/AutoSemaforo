#include "configuracao.h"
#include "ui_configuracao.h"

Configuracao::Configuracao(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Configuracao)
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

Configuracao::~Configuracao()
{
    delete ui;
}

void Configuracao::on_pushButton_3_clicked()
{
    this->ui->amaToVer_timeEdit->setTime(QTime(0,10));//Dez Minutos
    this->ui->verToPulso_timeEdit->setTime(QTime(0,5));//5 Minutos
    this->ui->pulso_timeEdit->setTime(QTime(0,0,0,300));//300 micro-segundos
    this->ui->sireneLigada_timeEdit->setTime(QTime(0,0,5));//5 Segundos
    this->ui->sireneDesligada_timeEdit->setTime(QTime(0,0,2));//2 Segundos
}

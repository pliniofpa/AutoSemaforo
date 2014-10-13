#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tporta.h"
#include <QTimer>
#include <QMessageBox>
#include "ui_configuration.h"
#include <QThread>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QSettings>
#include "version.h"
#include <QLineEdit>
//#include <QDebug>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "mytimer.h"
#include <QTimer>

void MainWindow::addTimer(const QTime &time, const bool active)
{

    QHBoxLayout *horizontalLayout_Timer = new QHBoxLayout();
    horizontalLayout_Timer->setSpacing(6);
    //horizontal_Layout->setObjectName(QStringLiteral("horizontalLayout_Timer_1"));
    QLabel *timerName_Label = new QLabel(this->ui->scrollAreaWidgetContents);
    timerName_Label->setAttribute(Qt::WA_DeleteOnClose);
    //timerLabel->setObjectName(QStringLiteral("timerName_Label"));
    timerName_Label->setText(QString("Término de Aula #%1: ").arg(++this->timerNumber));
    timerName_Label->setMaximumSize(this->ui->timerName_Label->maximumSize());
    timerName_Label->setFont(this->ui->timerName_Label->font());

    horizontalLayout_Timer->addWidget(timerName_Label, 0, Qt::AlignLeft);

    QSpacerItem *LabelTimer_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_Timer->addItem(LabelTimer_horizontalSpacer);

    QTimeEdit *timeEdit = new QTimeEdit(time,this->ui->scrollAreaWidgetContents);
    timeEdit->setAttribute(Qt::WA_DeleteOnClose);
    timeEdit->setDisplayFormat("hh:mm:ss");
    //timeEdit->setObjectName(QStringLiteral("timeEdit"));
    timeEdit->setFont(this->ui->timeEdit->font());
    //timeEdit->setCalendarPopup(true);

    horizontalLayout_Timer->addWidget(timeEdit, 0, Qt::AlignRight);

    QCheckBox *enabled_checkBox_Timer = new QCheckBox(this->ui->scrollAreaWidgetContents);
    enabled_checkBox_Timer->setAttribute(Qt::WA_DeleteOnClose);
    //enabled_checkBox_Timer->setObjectName(QStringLiteral("checkBox"));

    horizontalLayout_Timer->addWidget(enabled_checkBox_Timer, 0, Qt::AlignRight);

    //this->scrollArea_verticalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    //horizontalLayout_Timer->addItem(scrollArea_verticalSpacer);

    QPushButton *deleteTimer_pushButton = new QPushButton(this->ui->scrollAreaWidgetContents);
    deleteTimer_pushButton->setAttribute(Qt::WA_DeleteOnClose);
    //deleteTimer_pushButton->setObjectName(QStringLiteral("deleteTimer_pushButton"));
    deleteTimer_pushButton->setMaximumSize(QSize(25, 25));
    deleteTimer_pushButton->setFont(this->ui->deleteTimer_pushButton->font());
    QIcon icon;
    icon.addFile(QStringLiteral(":/imagens/delete.ico"), QSize(), QIcon::Normal, QIcon::Off);
    deleteTimer_pushButton->setIcon(icon);
    deleteTimer_pushButton->setFlat(true);

    if(this->deleteButtons.size())
        {
            this->deleteButtons.back()->setEnabled(false);
        }
    MyTimer *timer = new MyTimer(timeEdit);
    QObject::connect(enabled_checkBox_Timer,SIGNAL(toggled(bool)),timer,SLOT(activeTimer(bool)));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(inicio_ciclo()));
    QObject::connect(deleteTimer_pushButton,SIGNAL(clicked()),timerName_Label,SLOT(close()));
    QObject::connect(deleteTimer_pushButton,SIGNAL(clicked()),timeEdit,SLOT(close()));
    QObject::connect(deleteTimer_pushButton,SIGNAL(clicked()),enabled_checkBox_Timer,SLOT(close()));
    QObject::connect(deleteTimer_pushButton,SIGNAL(clicked()),this,SLOT(timerDeleted()));
    QObject::connect(deleteTimer_pushButton,SIGNAL(clicked()),horizontalLayout_Timer,SLOT(deleteLater()));

    horizontalLayout_Timer->addWidget(deleteTimer_pushButton, 0, Qt::AlignRight);

    this->ui->verticalLayout_ScrollArea->removeItem(this->ui->scrollArea_verticalSpacer);
    this->ui->verticalLayout_ScrollArea->addLayout(horizontalLayout_Timer);
    this->ui->verticalLayout_ScrollArea->addItem(this->ui->scrollArea_verticalSpacer);
    //qDebug()<<"Item adicionado";
    this->deleteButtons.append(deleteTimer_pushButton);
    enabled_checkBox_Timer->setChecked(active);
}
void MainWindow::timerDeleted()
{
    this->timerNumber--;
    this->deleteButtons.takeLast()->close();
    if(this->deleteButtons.size())
        {
            this->deleteButtons.last()->setEnabled(true);
        }
    this->ui->verticalLayout_ScrollArea->update();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->serial_on = true;
    cont = 0;
    this->ui->label_sirene->clear();
    this->aux = new char();
    this->config = new Configuration(this); //Dialogo de configuração
#ifndef PARALELA_ATIVADA
    this->paralela_on = false;
    this->config->ui->checkBox->setChecked(false);
    this->config->ui->checkBox->setVisible(false);
#endif
    this->serialport = NULL;
    this->portas_disponiveis(); //Enumera as portas Disponíveis
    sucesso = this->cria_portaserial(); //Cria porta serial usando a primeira porta enumerada
    if(sucesso)
        {
            if(!this->serialport->open(QIODevice::ReadWrite))
                {
                    QMessageBox::critical(this,"Erro ao abrir porta",QString("Código de Erro: %1./nVerifique se %2 esta sendo usado por outro programa ou recurso.").arg(this->serialport->errorString()).arg(this->config->ui->comboBox->currentText()),QMessageBox::Ignore | QMessageBox::Close,QMessageBox::Close);
                    sucesso = false;
                }
            else
                {
                    serialport->setBaudRate(QSerialPort::Baud9600);
                }
        }
    this->ui->pushButton_2->setDisabled(true);
#ifdef PARALELA_ATIVADA
    this->port = 0x378;
    this->porta = new TPorta;
#endif
    //this->int_verifica = new QTimer();
    this->int_a = new QTimer();
    this->int_b = new QTimer();
    this->pulso_on = new QTimer();
    this->pulso_off = new QTimer();
    this->sirene_on = new QTimer();
    this->sirene_off = new QTimer();
    this->all_off = new QTimer();
    this->all_off->setInterval(2000);
    QObject::connect(this->all_off,SIGNAL(timeout()),this,SLOT(All_off()));
    QObject::connect(pulso_on,SIGNAL(timeout()),this,SLOT(pulso_ligado())); //Conecta Timer Para pulso_on
    QObject::connect(pulso_off,SIGNAL(timeout()),this,SLOT(pulso_desligado())); //Conecta Timer Para pulso_off
    QObject::connect(int_a,SIGNAL(timeout()),this,SLOT(ativa_D1()));
    QObject::connect(int_b,SIGNAL(timeout()),this,SLOT(pulso_ligado()));
    QObject::connect(this->ui->actionIntervalos,SIGNAL(triggered()),this,SLOT(abre_config()));
    QObject::connect(this->ui->actionSobreQt,SIGNAL(triggered()),this,SLOT(sobreqt()));
    QObject::connect(this->ui->actionSobre,SIGNAL(triggered()),this,SLOT(sobre()));
    QObject::connect(this->ui->pushButton_14,SIGNAL(clicked()),this,SLOT(inicio_ciclo()));
    QObject::connect(this->ui->pushButton_11,SIGNAL(clicked()),this,SLOT(liga_sirene()));
    QObject::connect(this->sirene_on,SIGNAL(timeout()),this,SLOT(desliga_sirene()));
    QObject::connect(this->sirene_off,SIGNAL(timeout()),this,SLOT(liga_sirene()));
    //Delete Button
    QObject::connect(this->ui->deleteTimer_pushButton,SIGNAL(clicked()),this->ui->timerName_Label,SLOT(close()));
    QObject::connect(this->ui->deleteTimer_pushButton,SIGNAL(clicked()),this->ui->timeEdit,SLOT(close()));
    QObject::connect(this->ui->deleteTimer_pushButton,SIGNAL(clicked()),this->ui->enabled_checkBox_Timer,SLOT(close()));
    QObject::connect(this->ui->deleteTimer_pushButton,SIGNAL(clicked()),this->ui->deleteTimer_pushButton,SLOT(close()));
    //
    this->timerNumber=1;
    this->deleteButtons.append(this->ui->deleteTimer_pushButton);
    QObject::connect(this->ui->deleteTimer_pushButton,SIGNAL(clicked()),this,SLOT(timerDeleted()));
    MyTimer *timer = new MyTimer(this->ui->timeEdit);
    QObject::connect(this->ui->enabled_checkBox_Timer,SIGNAL(toggled(bool)),timer,SLOT(activeTimer(bool)));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(inicio_ciclo()));
    QObject::connect(this->ui->deleteTimer_pushButton,SIGNAL(clicked()),this->ui->horizontalLayout_Timer_1,SLOT(deleteLater()));
    this->loadUerPreferences();
}

MainWindow::~MainWindow()
{
    delete ui;

}
void MainWindow::sobreqt()
{
    QMessageBox::aboutQt(this);
}
void MainWindow::inicio_ciclo()
{
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        porta->outportb(port,0x01); //Ativa o pino D0 do Registro de Dados da porta paralela.
#endif
    if(this->serial_on)
        {
            *aux = *aux|0b00000001;           //Ativa o pino D0 do Registro de Dados da porta serial.
            this->serialport->write(aux,1);
        }
    this->ui->semaforo->setPixmap(QPixmap(":/imagens/offonoff.png"));
    //Para todos os timer auxiliares para Não dar conflito.
    this->int_a->stop();
    this->int_b->stop();
    this->pulso_off->stop();
    this->pulso_on->stop();
    int_a->start(); //Starta timer para D1
    this->ui->pushButton_2->setEnabled(true); //Ativa Botão
    this->ui->pushButton_14->setDisabled(true); //Desativa Ligar Semáforo
}
void MainWindow::All_off()
{
    this->all_off->stop();
    delete this->all_off;
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        this->porta->outportb(port,0x00); //Desativa Todos os pinos da porta paralela
#endif
    if(this->serial_on)
        {
            *aux = 0b00000000;           //Desativa Todos os pinos da porta serial.
            this->serialport->write(aux,1);
        }
}

void MainWindow::liga_sirene()
{
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        porta->outportb(port,0x04); //Ativa o pino D2 do Registro de Dados da porta paralela.
#endif
    if(this->serial_on)
        {
            *aux = *aux|0b00000100;           //Ativa o pino D2 do Registro de Dados da porta serial.
            this->serialport->write(aux,1);
        }
    this->sirene_on->start(); //Ativa Timer para desliga sirene
    this->sirene_off->stop(); //Para Timer para liga sirene
    this->ui->pushButton_11->setDisabled(true); //Deativa botão liga sirene
    this->ui->label_sirene->setPixmap(QPixmap(":/imagens/sireneon.png"));
    this->ui->pushButton_2->setEnabled(true);
}
void MainWindow::desliga_sirene()
{
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        porta->outportb(port,0x00); //Desativa o pino D2 de Dados da porta paralela.
#endif
    if(this->serial_on)
        {
            *aux = *aux&0b11111011;           //Desativa o pino D2 de Dados da porta serial.
            this->serialport->write(aux,1);
        }
    this->sirene_on->stop(); //Para timer para desliga sirene
    if(cont)
        {
            cont=0;
            this->ui->pushButton_11->setEnabled(true);
            //this->ui->label_sirene->setPixmap(QPixmap(":/imagens/sireneoff.png"));
            this->ui->label_sirene->clear();
            this->ui->pushButton_2->setEnabled(!this->ui->pushButton_14->isEnabled());
            return;
        }
    this->sirene_off->start(); //Inicia Timer para liga sirene
    this->ui->pushButton_11->setEnabled(true); //Deativa botão liga sirene
    cont++;
    //this->ui->label_sirene->setPixmap(QPixmap(":/imagens/sireneoff.png"));
    this->ui->label_sirene->clear();
    this->ui->pushButton_2->setEnabled(!this->ui->pushButton_14->isEnabled());

}


bool MainWindow::desativar()
{
    this->ui->pushButton_2->setDisabled(true); //Desativa Botão
    for(int i=0; i<this->checks.size(); i++)
        if((this->times.at(i)->time().hour() == QTime::currentTime().hour()) && (this->times.at(i)->time().minute() == QTime::currentTime().minute()) && (this->checks.at(i)->isChecked()))
            this->checks.at(i)->setChecked(false);
    this->int_a->stop(); //Para Timer para D1
    this->int_b->stop(); //Para Timer para Pulso
    this->pulso_on->stop(); //Para Timer Pulso_on
    this->pulso_off->stop(); //Para Timer Pulso_off
    this->sirene_on->stop(); //Para Timer para desliga sirene
    this->sirene_off->stop(); //Para Timer para liga sirene
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        porta->outportb(port,0x00); //Desativa todos os pinos do Registro de Dados da porta paralela.
#endif
    if(this->serial_on)
        {
            *aux = 0b00000000;           //Desativa todos os pinos do Registro de Dados da porta serial.
            this->serialport->write(aux,1);
        }
    this->ui->semaforo->setPixmap(QPixmap(":/imagens/offoffon.png"));
    //this->ui->semaforo->clear();
    return true;
}
bool MainWindow::ativa_D1()
{
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        this->porta->outportb(port,0x02);
#endif
    if(this->serial_on)
        {
            *aux = *aux|0b00000010;
            this->serialport->write(aux,1);
        }
    this->ui->semaforo->setPixmap(QPixmap(":/imagens/onoffoff.png"));
    this->int_a->stop(); //Para timer para D1
    this->int_b->start(); //Starta timer para Pulso
    return true;
}

bool MainWindow::pulso_ligado()
{
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        this->porta->outportb(port,0x02); //Ativa Porta D1
#endif
    if(this->serial_on)
        {
            *aux = *aux|0b00000010;
            this->serialport->write(aux,1);
        }
    this->ui->semaforo->setPixmap(QPixmap(":/imagens/onoffoff.png"));
    this->int_b->stop(); //Para Timer para Pulso
    this->pulso_off->start(); //Starta Timer para Pulso_off
    this->pulso_on->stop(); //Para Timer para Pulso_on
    return true;
}
bool MainWindow::pulso_desligado()
{
#ifdef PARALELA_ATIVADA
    if(this->paralela_on)
        this->porta->outportb(port,0x00); //Desativa D1
#endif
    if(this->serial_on)
        {
            *aux = *aux&0b11111101;
            this->serialport->write(aux,1);
        }
    this->ui->semaforo->setPixmap(QPixmap(":/imagens/offoffon.png"));
    this->pulso_on->start(); //Starta Timer para Pulso_on
    this->pulso_off->stop(); //Para pulso para Pulso_off
    return true;
}
void MainWindow::abre_config()
{
    this->config->ui->amaToVer_timeEdit->setTime(QTime().addMSecs(this->int_a->interval()));
    this->config->ui->verToPulso_timeEdit->setTime(QTime().addMSecs(this->int_b->interval()));
    this->config->ui->pulso_timeEdit->setTime(QTime().addMSecs(this->pulso_on->interval()));
    this->config->ui->sireneLigada_timeEdit->setTime(QTime().addMSecs(this->sirene_on->interval()));
    this->config->ui->sireneDesligada_timeEdit->setTime(QTime().addMSecs(this->sirene_off->interval()));
    if(this->config->ui->comboBox->currentText()=="")
        this->portas_disponiveis();
    this->config->ui->checkBox->setChecked(this->paralela_on);
    this->config->ui->checkBox_2->setChecked(this->serial_on);
    if(config->exec())
        {
            activeConfigurations();
        }
}
void MainWindow::activeConfigurations()
{
    //qDebug()<<"OK";
    this->int_a->setInterval(this->config->ui->amaToVer_timeEdit->time().msecsSinceStartOfDay());
    this->int_b->setInterval(this->config->ui->verToPulso_timeEdit->time().msecsSinceStartOfDay());
    this->pulso_on->setInterval(this->config->ui->pulso_timeEdit->time().msecsSinceStartOfDay());
    this->pulso_off->setInterval(this->config->ui->pulso_timeEdit->time().msecsSinceStartOfDay());
    this->sirene_on->setInterval(this->config->ui->sireneLigada_timeEdit->time().msecsSinceStartOfDay());
    this->sirene_off->setInterval(this->config->ui->sireneDesligada_timeEdit->time().msecsSinceStartOfDay());
    this->paralela_on = this->config->ui->checkBox->isChecked();
    this->serial_on = this->config->ui->checkBox_2->isChecked();
    if(this->serial_on && this->serialport!=NULL)
        {
            this->serialport->close();
            QSerialPortInfo portInfo = this->serialPorts.at(this->config->ui->comboBox->currentIndex());
            this->serialport->setPort(portInfo);
            if(!this->serialport->open(QIODevice::ReadWrite))
                {
                    QMessageBox::critical(this,"Erro ao abrir porta",QString("Código de Erro: %1./nVerifique se %2 esta sendo usado por outro programa ou recurso.").arg(this->serialport->errorString()).arg(this->config->ui->comboBox->currentText()),QMessageBox::Ignore | QMessageBox::Close,QMessageBox::Close);
                    sucesso = false;
                }
            else
                {
                    this->serialport->setBaudRate(QSerialPort::Baud9600);
                    this->ui->label_porta->setText(this->config->ui->comboBox->currentText());
                    sucesso = true;
                }

        }
    if(!this->serial_on)
        this->ui->label_porta->setText("");

}

void MainWindow::on_pushButton_2_clicked()
{

    this->ui->pushButton_14->setEnabled(true); //Ativa Botão Ligar Semáforo
    this->ui->pushButton_11->setEnabled(true);  //Ativa Botão Ligar Sirena
    this->ui->label_sirene->clear();//Remove icone de sirene
    emit this->desativar();
}
void MainWindow::sobre()
{
    About *sobre = new About(this);
    sobre->exec();
}
//Parte do Tray Icon
/*
void MainWindow::createActions()
{
    openAction = new QAction(tr("&Abrir"), this);
    connect(openAction, SIGNAL(triggered()), this, SLOT(show()));

    closeAction = new QAction(tr("&Sair"), this);
    connect(closeAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    trayIconMenu->addAction(openAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(closeAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(
                trayIcon,
                SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this,
                SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
                );
}
void MainWindow::setIcon()
{
    trayIcon->setIcon(QIcon(":/imagens/icon.ico"));
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        this->show();
}
*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    /*
    if (trayIcon->isVisible()) {
        trayIcon->showMessage(tr("AutoSemáforo ainda está em execução!"),
                              tr("AutoSemáforo ainda está sendo executado. Para sair por favor clique neste ícone e selecione Sair"));
        int ret = QMessageBox::information(this,this->windowTitle(),"Deseja realmente sair?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        if(ret == QMessageBox::Yes){
            saveUserPreferences();
            event->accept();
        }
        else{
            this->hide();
            event->ignore(); // Don't let the event propagate to the base class
        }}
    */
    saveUserPreferences();
    event->accept();
}

bool MainWindow::cria_portaserial()
{
    if(this->config->ui->comboBox->count())
        {
            QSerialPortInfo portInfo = this->serialPorts.at(this->config->ui->comboBox->currentIndex());
            this->serialport = new QSerialPort(portInfo);
            this->serialport->setBaudRate(QSerialPort::Baud9600);
            this->serialport->setDataBits(QSerialPort::Data8);
            this->serialport->setParity(QSerialPort::NoParity);
            this->serialport->setStopBits(QSerialPort::OneStop);
            this->serialport->setFlowControl(QSerialPort::NoFlowControl);
            this->ui->label_porta->setText(this->config->ui->comboBox->currentText());
            return true;
        }
    //Desativa ComboBox
    this->config->ui->checkBox->setChecked(false);
    this->config->ui->checkBox->setDisabled(true);
    this->serial_on = false;
    int ret = QMessageBox::critical(this,"Porta serial não encontrada.","Verifique se o cabo de comunicação serial está conectado. Verifique se o driver de sua porta serial está instalado e configurado corretamente.",QMessageBox::Ignore | QMessageBox::Close,QMessageBox::Close);
    if(ret==QMessageBox::Close)
        {
            QTimer *closeTimer = new QTimer(this);
            QObject::connect(closeTimer,SIGNAL(timeout()),qApp,SLOT(quit()));
            closeTimer->start(0);
        }
    return false;
}
void MainWindow::portas_disponiveis()
{
    this->serialPorts = QSerialPortInfo::availablePorts();
    for(int i=0; i<this->serialPorts.size(); i++)
        this->config->ui->comboBox->addItem(QString("%1 (%2)").arg(this->serialPorts.at(i).description()).arg(this->serialPorts.at(i).portName()));
}
void MainWindow::saveUserPreferences()
{
    QSettings toSave(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    //Windows Preferences
    toSave.beginGroup("MainWindow");
    if(!this->isMaximized())
        {
            toSave.setValue("size",this->size());
            toSave.setValue("pos",this->pos());
        }
    toSave.setValue("maximized",this->isMaximized());
    toSave.endGroup();
    //Alarms Preferences
    toSave.beginGroup("Alarms");
    toSave.beginWriteArray("times");//Begin times Array
    QList<QTimeEdit *> allTimes = this->ui->groupBox->findChildren<QTimeEdit *>();
    QList<QCheckBox *> allCheckBoxs = this->ui->groupBox->findChildren<QCheckBox *>();
    for(int i=0; i<allTimes.count(); i++)
        {
            toSave.setArrayIndex(i);
            toSave.setValue("value",allTimes.at(i)->time().toString("hh:mm:ss:zzz"));
            toSave.setValue("active",allCheckBoxs.at(i)->isChecked());
        }
    toSave.endArray();//End the times Array
    toSave.endGroup();
    //Configuration Preferences
    toSave.beginGroup("Preferences");
    toSave.beginWriteArray("intervals");//Begin times Array
    QList<QTimeEdit *> allLineEdits = this->config->ui->groupBox->findChildren<QTimeEdit *>();
    for(int i=0; i<allLineEdits.count(); i++)
        {
            toSave.setArrayIndex(i);
            toSave.setValue("value",allLineEdits.at(i)->time().toString("hh:mm:ss:zzz"));
        }
    toSave.endArray();//End the times Array
    toSave.endGroup();
    //double key = QDateTime::fromString(datetimestring,"hh:mm:ss").toMSecsSinceEpoch()/1000.0;
}
void MainWindow::loadUerPreferences()
{
    QSettings toLoad(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toLoad.beginGroup("MainWindow");
    this->resize(toLoad.value("size", QSize(600, 600)).toSize());
    this->move(toLoad.value("pos", QPoint(200, 200)).toPoint());
    if(toLoad.value("maximized", false).toBool())
        this->showMaximized();
    toLoad.endGroup();
    //Alarms Preferences
    toLoad.beginGroup("Alarms");
    int size = toLoad.beginReadArray("times");//Begin times Array
    if(size)
        {
            toLoad.setArrayIndex(0);
            this->ui->timeEdit->setTime(QTime::fromString(toLoad.value("value","00:00:00:000").toString(),"hh:mm:ss:zzz"));
            this->ui->enabled_checkBox_Timer->setChecked(toLoad.value("active",false).toBool());
        }
    for(int i=1; i<size; i++)
        {
            toLoad.setArrayIndex(i);
            //allTimes.at(i)->setTime(QTime::fromString(toLoad.value("value","").toString(),"hh:mm:ss:zzz"));
            this->addTimer(QTime::fromString(toLoad.value("value","00:00:00:000").toString(),"hh:mm:ss:zzz"),toLoad.value("active",false).toBool());
        }
    toLoad.endArray();//End the times Array
    toLoad.endGroup();//End the Alarms Group;
    //Configuration Preferences
    toLoad.beginGroup("Preferences");
    size = toLoad.beginReadArray("intervals");//Begin times Array
    QList<QTimeEdit *> allLineEdits = this->config->ui->groupBox->findChildren<QTimeEdit *>();
    for(int i=0; i<size; i++)
        {
            toLoad.setArrayIndex(i);
            allLineEdits.at(i)->setTime(QTime::fromString(toLoad.value("value","").toString(),"hh:mm:ss:zzz"));
        }
    toLoad.endArray();//End the intervals Array
    toLoad.endGroup();//End the Preferences Group;
    activeConfigurations();
}

void MainWindow::on_addTimer_pushButton_clicked()
{
    emit this->addTimer();
}

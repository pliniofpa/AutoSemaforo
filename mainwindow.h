#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVector>
#include <QCheckBox>
#include <QTimeEdit>
#include "configuration.h"
#include "about.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QTime>
class TPorta;
class QTimer;
class QSerialPort;
class QHBoxLayout;
class QLabel;
class QTimeEdit;
class QCheckBox;
class QPushButton;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void activeConfigurations();
    bool toClose;
    Ui::MainWindow *ui;
    QList<QHBoxLayout*> horizontalLayouts;
    QList<QLabel*> labels;
    QList<QTimeEdit*> timerEdits;
    QList<QCheckBox*> checkboxes;
    QList<QPushButton*> pushBottons;
    int timerNumber;
    QList<QPushButton*> deleteButtons;
    int cont;
    //Parte do Tray Icon
    //void createActions();
    //void createTrayIcon();
    //void setIcon();
    void closeEvent(QCloseEvent *); // Overriding the window's close event
    //QSystemTrayIcon *trayIcon;
    //QMenu *trayIconMenu;
    //QAction *openAction;
    //QAction *closeAction;
    bool paralela_on, serial_on;

    char *aux;
    TPorta *porta;
    //QTimer *int_verifica;
    QTimer *int_a;
    QTimer *int_b;
    QTimer *pulso_on;
    QTimer *pulso_off;
    QTimer *sirene_on;
    QTimer *sirene_off;
    QTimer *all_off;
    QVector<QCheckBox*> checks;
    QVector<QTimeEdit*> times;
    Configuration *config;
    QSerialPort *serialport;
    short port;
    bool cria_portaserial();
    void portas_disponiveis();
    QList<QSerialPortInfo> serialPorts;
    bool sucesso;
    void saveUserPreferences();
    void loadUerPreferences();
public slots:
    void inicio_ciclo();
    bool ativa_D1();
    //bool verifica();
    bool desativar();
    bool pulso_ligado();
    bool pulso_desligado();
    void abre_config();
    void sobreqt();
    void sobre();
    void liga_sirene();
    void desliga_sirene();
    void All_off();
    void addTimer(const QTime &time=QTime(), const bool active=false);
    void timerDeleted();

private slots:
    //Sobre Tray Icon
    //void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void on_pushButton_2_clicked();
    void on_addTimer_pushButton_clicked();
};

#endif // MAINWINDOW_H

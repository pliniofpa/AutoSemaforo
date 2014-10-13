#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = 0);
    ~Configuration();
    Ui::Configuration *ui;
private slots:
    void on_restore_pushButton_clicked();
};

#endif // CONFIGURATION_H

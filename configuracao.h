#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <QDialog>

namespace Ui {
    class Configuracao;
}

class Configuracao : public QDialog
{
    Q_OBJECT

public:
    explicit Configuracao(QWidget *parent = 0);
    ~Configuracao();

public:
    Ui::Configuracao *ui;

private slots:
    void on_pushButton_3_clicked();
};

#endif // CONFIGURACAO_H

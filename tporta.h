#include <QLibrary>
#ifndef TPORTA_H
#define TPORTA_H
class QWidget;
//Declaração dos ponteiros para função.
typedef short (*PtrInp)(short EndPorta);
typedef void (*PtrOut)(short EndPorta, short valor);
class TPorta
{
private:
    int ret;
    QLibrary *lib;
public:
    PtrInp inportb;
    PtrOut outportb;

    TPorta();
    int erro(){
        return ret;
    }
};

#endif // TPORTA_H

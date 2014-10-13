#include "tporta.h"
#include <QMessageBox>

//Declaração dos ponteiros para função.
typedef short (*PtrInp)(short EndPorta);
typedef void (*PtrOut)(short EndPorta, short valor);

TPorta::TPorta()
{
    ret = 0;
    this->lib = new QLibrary("inpout32");
    if(!lib->load())
        ret = QMessageBox::critical(0,"Não encontrou inpout32.dll",lib->errorString(),QMessageBox::Ignore | QMessageBox::Close,QMessageBox::Close);

    inportb = (PtrInp) lib->resolve("Inp32");
    outportb = (PtrOut) lib->resolve("Out32");
}

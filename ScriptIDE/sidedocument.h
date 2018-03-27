#ifndef SIDEDOCUMENT_H
#define SIDEDOCUMENT_H

#include <ScintillaDocument.h>



class sIDEDocument : public ScintillaDocument
{
public:
    explicit sIDEDocument(QObject *parent = 0, void *pdoc_=0);
    QString filespec;
};

#endif // SIDEDOCUMENT_H

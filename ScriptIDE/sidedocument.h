#ifndef SIDEDOCUMENT_H
#define SIDEDOCUMENT_H

#include "fileinterfaces.h"

#include <QObject>

class sIDEDocument : public QObject
{
	Q_OBJECT
public:
	explicit sIDEDocument(QObject *parent = nullptr);

	Q_PROPERTY(QString filespec MEMBER filespec)
	QString filespec;

	int refCount = 0;

	void read(IPipeInput<QByteArray> *target);
	IPipeInput<QByteArray> *write();
	IPipeInput<QByteArray> *append();

signals:

public slots:

private:
};

#endif // SIDEDOCUMENT_H

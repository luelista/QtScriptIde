#include "sidedocument.h"

#include <QFile>
#include <QIODevice>
#include <QSaveFile>

class PFileWriter : public IPipeInput<QByteArray>, public QFile {
public:
	PFileWriter(QString &filename) : QFile(filename) {
		open(QIODevice::Append | QIODevice::WriteOnly);
	}
	void onData(const QByteArray &bytes){
		write(bytes);
	}
	void onEnd(){
		close();
	}
};
class PQSaveFileWriter : public IPipeInput<QByteArray>, public QSaveFile {
public:
	using QSaveFile::QSaveFile;
	void onData(const QByteArray &bytes){
		write(bytes);
	}
	void onEnd(){
		commit();

	}
};
class PQIODeviceReader : public IPipeOutput<QByteArray> {
	QIODevice *t;
public:
	PQIODeviceReader (QIODevice *target) : t(target) {

	}
	void readAll() {
		onOutData(t->readAll());
		onOutEnd();
	}
};

sIDEDocument::sIDEDocument(QObject *parent) : QObject(parent)
{

}

void sIDEDocument::read(IPipeInput<QByteArray> *target)
{
	QFile file (filespec);
	file.open(QIODevice::ReadOnly);
	PQIODeviceReader reader(&file);
	reader.pipeInto(target);
	reader.readAll();
}

IPipeInput<QByteArray> *sIDEDocument::write()
{
	return new PQSaveFileWriter(filespec);
}

IPipeInput<QByteArray> *sIDEDocument::append()
{
	return new PFileWriter(filespec);
}


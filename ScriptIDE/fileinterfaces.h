#ifndef FILEINTERFACES_H
#define FILEINTERFACES_H

#include <QByteArray>
#include <QTextCodec>



template <typename T> class IPipeInput {
public:
	virtual void onData(const T &bytes) = 0;
	virtual void onEnd() = 0;
	virtual ~IPipeInput() {}
};

template <typename T> class IPipeOutput {
public:
	virtual void pipeInto(IPipeInput<T> *input) {
		pipeOutput = input;
		pipeEnded = false;
	}
	virtual ~IPipeOutput() {}
private:
	IPipeInput<T> *pipeOutput;
	bool pipeEnded = false;
protected:
	void onOutData(const T &bytes) {
		pipeOutput->onData(bytes);
	}
	void onOutEnd() {
		pipeOutput->onEnd();
		pipeEnded = true;
	}
};

template <typename T> class IReadSave : public IPipeInput<T>, public IPipeOutput<T> {
	virtual ~IReadSave() = 0;
};

using IBinReadSave = IReadSave<QByteArray>;
using IStringReadSave = IReadSave<QString>;


class PTextEncoder : public IPipeInput<QString>, public IPipeOutput<QByteArray> {
	QTextEncoder enc;

public:

	PTextEncoder(const QTextCodec *qtc)
	:	enc(qtc, QTextCodec::DefaultConversion){
	}

	virtual ~PTextEncoder(){}

	// IPipeInput interface
private:
	void onData(const QString &bytes);
	void onEnd(){
		onOutEnd();
	}
};


class PTextDecoder : public IPipeInput<QByteArray>, public IPipeOutput<QString> {
	QTextDecoder dec;

public:

	PTextDecoder(const QTextCodec *qtc)
	  :  dec(qtc, QTextCodec::DefaultConversion){

	}

	virtual ~PTextDecoder(){}

	// IPipeInput interface
public:
	void onData(const QByteArray &bytes);
	void onEnd();
};


class PHEXEncoder : public IPipeInput<QByteArray>, public IPipeOutput<QString> {
public:

	PHEXEncoder(){
	}

	virtual ~PHEXEncoder(){}

	// IPipeInput interface
private:
	void onData(const QByteArray &bytes);
	void onEnd(){
		onOutEnd();
	}
};




#endif // FILEINTERFACES_H

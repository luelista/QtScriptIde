#include "fileinterfaces.h"

void PTextEncoder::onData(const QString &bytes){
	onOutData(enc.fromUnicode(bytes));
}

void PTextDecoder::onData(const QByteArray &bytes)
{
	onOutData(dec.toUnicode(bytes));
}

void PHEXEncoder::onData(const QByteArray &bytes)
{
	onOutData(bytes.toHex());
}

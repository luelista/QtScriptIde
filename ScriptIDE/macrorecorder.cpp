#include "macrorecorder.h"

#include <QString>

MacroRecorder::MacroRecorder()
{

}

static inline uchar hexdig(uint u)
{
    return (u < 0xa ? '0' + u : 'a' + u - 0xa);
}
QByteArray MacroRecorder::escapedString(const QString &s)
{
    const uchar replacement = '?';
    QByteArray ba(s.length(), Qt::Uninitialized);

    uchar *cursor = reinterpret_cast<uchar *>(const_cast<char *>(ba.constData()));
    const uchar *ba_end = cursor + ba.length();
    const ushort *src = reinterpret_cast<const ushort *>(s.constBegin());
    const ushort *const end = reinterpret_cast<const ushort *>(s.constEnd());

    while (src != end) {
        if (cursor >= ba_end - 6) {
            // ensure we have enough space
            int pos = cursor - (const uchar *)ba.constData();
            ba.resize(ba.size()*2);
            cursor = (uchar *)ba.data() + pos;
            ba_end = (const uchar *)ba.constData() + ba.length();
        }

        uint u = *src++;
        if (u < 0x80) {
            if (u < 0x20 || u == 0x22 || u == 0x5c) {
                *cursor++ = '\\';
                switch (u) {
                case 0x22:
                    *cursor++ = '"';
                    break;
                case 0x5c:
                    *cursor++ = '\\';
                    break;
                case 0x8:
                    *cursor++ = 'b';
                    break;
                case 0xc:
                    *cursor++ = 'f';
                    break;
                case 0xa:
                    *cursor++ = 'n';
                    break;
                case 0xd:
                    *cursor++ = 'r';
                    break;
                case 0x9:
                    *cursor++ = 't';
                    break;
                default:
                    *cursor++ = 'u';
                    *cursor++ = '0';
                    *cursor++ = '0';
                    *cursor++ = hexdig(u>>4);
                    *cursor++ = hexdig(u & 0xf);
               }
            } else {
                *cursor++ = (uchar)u;
            }
        } else {
            //if (QUtf8Functions::toUtf8<QUtf8BaseTraits>(u, cursor, src, end) < 0)
                *cursor++ = replacement;
        }
    }

    ba.resize(cursor - (const uchar *)ba.constData());
    return ba;
}

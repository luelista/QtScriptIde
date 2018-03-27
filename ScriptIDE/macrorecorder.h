#ifndef MACRORECORDER_H
#define MACRORECORDER_H

#include <QByteArray>



class MacroRecorder
{
public:
    MacroRecorder();
    static QByteArray escapedString(const QString &s);
};

#endif // MACRORECORDER_H

#ifndef SERIALIZEREXCEPTION_H
#define SERIALIZEREXCEPTION_H

#include <qserializerlib_global.h>
#include <QString>
#include <exception>
#include <map>

enum QSError
{
    JsonArrayExpected,
    JsonObjectExpected,
    InvalidQObject,


    UndefinedQSError = 0xFFFF,
};

class QSERIALIZER_EXPORT QSException : public std::exception
{
public:
    QSException(QSError e = QSError::UndefinedQSError);
    const char * what() const noexcept;

protected:
    QSError e;
};
#endif // SERIALIZEREXCEPTION_H

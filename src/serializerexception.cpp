#include "serializerexception.h"


static std::map<QSError,const char*> descriptions = {
    {JsonArrayExpected, "Expected JSON array"},
    {JsonObjectExpected, "Expected JSON object"},
    {InvalidQObject, "Invalid cast to QObject type"},
    {UnsupportedPropertyType, "Use of unsupported type QMetaProperty object"},

    {UndefinedQSError, "Undefined QSerializer Exception"},
};

QSException::QSException(QSError e)
    : std::exception()
{
    this->e = e;
}

const char *QSException::what() const noexcept
{
    try {
        return descriptions.at(e);
    } catch (...) {
        return "Undefined QSerializerException::NotFoundExDescription";
    }
}

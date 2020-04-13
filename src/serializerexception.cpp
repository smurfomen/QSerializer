#include "serializerexception.h"


static std::map<QSError,QString> descriptions = {
    {JsonArrayExpected, "Expected JSON array"},
    {JsonObjectExpected, "Expected JSON object"},
    {InvalidQObject, "Invalid cast to QObject type"},


    {UndefinedQSError, "Undefined QSerializer Exception"},
};

QSException::QSException(QSError e)
    : std::exception()
{
    this->e = e;
}

const char *QSException::what() const noexcept
{
    QString desc;
    try {
        desc = descriptions.at(e);
    } catch (...) {

        desc = "Undefined QSerializerException::NotFoundExDescription";
    }

    return desc.toStdString().data();
}

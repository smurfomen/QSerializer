#ifndef QJSONMARSHALER_H
#define QJSONMARSHALER_H
#include <qjsonkeeper.h>
#include <vector>
#include "qjsonmarshalerlib_global.h"
/**
 * @brief QJsonMarshaler представляет собой входную точку библиотеки.
 * Работать с QJsonMarshaler можно по двум сценариям
 * - наследоваться от QJsonMarshaler для получения доступа к функциям задания свойства, маршалинга и демаршалинга
 * - создать объект QJsonMarshaler полем класса, подлежащего маршалингу и демаршалингу, и пользоваться его функционалом через этот объект
 *
 * @details
 *  - для задания свойства следует использовать setJsonProperty([ссылка на поле класса], [анотация в JSON])
 *  - для получения QJsonObject на основе объекта класса следует вызвать Marshal у унаследованного от QJsonMarshaler класса
 *  - для заполнения объекта класса свойствами из QJsonObject следует вызвать Unmarshal
 *
 *  В случае второго сценария использования - аналогичный вызов методов через экземпляр объекта класса QJsonMarshaler.
 */

class QJSONMARSHALERLIBSHARED_EXPORT QJsonMarshaler
{
public:
    QJsonMarshaler ();
    ~QJsonMarshaler();

    void setJsonObjectAnotation(QString annotation);

    static void UnmarshalDocument(QByteArray jsonArr);

    static void UnmarshalDocument(QString jsonStr);

    static void UnmarshalDocument(QJsonDocument doc);


    template<typename T>
    void setJsonProperty(T *pMember, QString annotation)
    {
        propertyes.push_back(getIJ(pMember,annotation));
    }

    QJsonObject Marshal();

    void Unmarshal(QJsonObject obj);

private:
    QString objAnnotation;
    std::vector<PropertyKeeper*> propertyes;

};


#endif // QJSONMARSHALER_H

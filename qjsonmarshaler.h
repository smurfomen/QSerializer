#ifndef QJSONMARSHALER_H
#define QJSONMARSHALER_H
#include <qjsonmarshalerlib_global.h>
#include <keepersfactory.h>
class QJSONMARSHALERLIBSHARED_EXPORT QJsonMarshaler
{
public:
    QJsonMarshaler ();
    ~QJsonMarshaler();
    /// \brief производит анмаршалинг по массиву, строке или JSON документу
    static void UnmarshalDocument(QByteArray jsonArr);
    static void UnmarshalDocument(QString jsonStr);
    static void UnmarshalDocument(QJsonDocument doc);

    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template<typename T>
    static T * Unmarshal(QJsonObject json)
    {
        QObject * qobj = new T();
        T * targetObj = qobject_cast<T*>(qobj);
        if(targetObj == nullptr)
            throw (MarshalExeption(MarshalExeption::FailQObjectCast));
        QJsonMarshaler::Unmarshal(targetObj, json);
        return targetObj;
    }

    /// \brief производит сериализацию JSON в переданный QObject согласно имеющейся у объекта метаинформации
    static void Unmarshal(QObject * obj, QJsonObject json);

    /// \brief производит сериализацию QObject в JSON по имеющейся у объекта метаинформации
    static QJsonObject Marshal(QObject * obj);

    /// \brief производит сериализацию настроенного объекта в JSON по связанным с экземпляром класса PropertyKeeper'ам в векторе propertyes
    QJsonObject Marshal();

    /// \brief производит сериализацию JSON в связанные с экземпляром класса PropertyKeeper'ах в векторе propertyes
    void Unmarshal(QJsonObject json);

    /// \brief задает простой член типа Т в propertyes
    template<typename T>
    void setJsonProperty(T &lField, QString annotation)
    {
        KeepersFactory factory;
        propertyes.push_back(factory.getKeeper(&lField, annotation));
    }

    /// \brief задает вектор типа А в propertyes
    template<typename A>
    void setJsonProperty(std::vector<A> &lArray, QString annotation)
    {
        KeepersFactory factory;
        propertyes.push_back(factory.getKeeper(&lArray, annotation));
    }

    /// \brief задает аннотацию для объекта маршалинга - анмаршалинга.
    void setJsonObjectAnotation(QString annotation);

private:
    QString objAnnotation;
    std::vector<PropertyKeeper*> propertyes;
};





#endif // QJSONMARSHALER_H


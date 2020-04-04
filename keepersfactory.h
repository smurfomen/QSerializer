#ifndef KEEPERSFACTORY_H
#define KEEPERSFACTORY_H
#include <simplekeepers.h>
#include <arraykeepers.h>
#include <propertykeeper.h>

/// \brief фабрика интерфейсов класса PropertyKeeper
class KeepersFactory
{
public:
    /// \brief возвращает простой хранитель для указанного поля
    template<typename T>
    PropertyKeeper * getKeeper(T * lField, QString annotation)
    {
        return new SimpleKeeper<T>(lField, annotation);
    }

    /// \brief возвращает простой хранитель массивов для указанного поля
    template<typename A>
    PropertyKeeper * getKeeper(std::vector<A> *lArray, QString annotation)
    {
        return new SimpleArrayKeeper<A>(lArray, annotation);
    }

    /// \brief возвращает хранитель QMeta хранитель для указанной QMetaProperty объекта
    PropertyKeeper * getKeeper(QObject * obj, QMetaProperty prop);

    /// \brief возвращает полный список QMeta хранителей для объекта по указанным у него QMetaProperty
    std::vector<PropertyKeeper*> getKeepersForObject(QObject *obj);
};

#endif // KEEPERSFACTORY_H

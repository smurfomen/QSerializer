#ifndef KEEPERSFACTORY_H
#define KEEPERSFACTORY_H
#include "qserializerlib_global.h"

#include "metakeepers.h"
#include "propertykeeper.h"

/// \brief фабрика интерфейсов класса PropertyKeeper
class KeepersFactory
{
public:
    ~KeepersFactory();

    /// \brief возвращает Meta хранитель для указанной QMetaProperty объекта
    PropertyKeeper * getKeeper(QObject * obj, QMetaProperty prop);

    /// \brief возвращает полный список Meta хранителей для объекта по указанным у него QMetaProperty
    std::vector<PropertyKeeper*> getKeepersForObject(QObject *obj);

private:

    /// \brief возвращает один из Meta хранителей, подходящих для переданного свойства связанного объекта, обернутый в базовый интерфейс хранителя
    PropertyKeeper * getMetaKeeper(QObject *obj, QMetaProperty prop);

    /// \brief возвраащет коллекцию Meta хранителей для объекта обернутых в базовый интерфейс хранителя
    std::vector<PropertyKeeper*> getMetaKeepers(QObject * obj);


    std::vector<PropertyKeeper*> t_keepers;
};

#endif // KEEPERSFACTORY_H

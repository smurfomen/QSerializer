#include <keepersfactory.h>


PropertyKeeper *KeepersFactory::getKeeper(QObject *obj, QMetaProperty prop)
{
    return getMetaKeeper(obj,prop);
}



std::vector<PropertyKeeper *> KeepersFactory::getKeepersForObject(QObject *obj)
{
    return getMetaKeepers(obj);
}

#include <keepersfactory.h>


KeepersFactory::~KeepersFactory(){

    for(PropertyKeeper * keeper : t_keepers)
    {
        delete keeper;
    }
}

PropertyKeeper *KeepersFactory::getKeeper(QObject *obj, QMetaProperty prop)
{
    PropertyKeeper * keeper = getMetaKeeper(obj, prop);
    t_keepers.push_back(keeper);
    return keeper;
}


std::vector<PropertyKeeper *> KeepersFactory::getKeepersForObject(QObject *obj)
{
    std::vector<PropertyKeeper * > metaKeepers = getMetaKeepers(obj);
    for(PropertyKeeper * keeper : metaKeepers)
    {
        t_keepers.push_back(keeper);
    }
    return metaKeepers;
}

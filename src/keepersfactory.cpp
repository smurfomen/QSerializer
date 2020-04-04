#include <keepersfactory.h>


PropertyKeeper *KeepersFactory::getKeeper(QObject *obj, QMetaProperty prop)
{
    int t_id = QMetaType::type(prop.typeName());
    if( t_id == qMetaTypeId<std::vector<int>>())
        return new QMetaArrayKeeper<int>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<QString>>())
        return new QMetaArrayKeeper<QString>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<double>>())
        return new QMetaArrayKeeper<double>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<bool>>())
        return new QMetaArrayKeeper<bool>(obj, prop);

    return new QMetaSimpleKeeper(obj,prop);
}

std::vector<PropertyKeeper *> KeepersFactory::getKeepersForObject(QObject *obj)
{
    std::vector<PropertyKeeper*> keepers;
    for(int i = 0; i < obj->metaObject()->propertyCount(); i++)
    {
        if(obj->metaObject()->property(i).isUser(obj))
            keepers.push_back(getKeeper(obj, obj->metaObject()->property(i)));
    }
    return keepers;
}

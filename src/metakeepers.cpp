#include <metakeepers.h>
PropertyKeeper *getMetaKeeper(QObject *obj, QMetaProperty prop)
{
    /// JSON поддерживает такие типы данных как int, string, bool, double
    /// соответственно если сериализуемое свойство не представляет собой один из этих типов или массив из этих типов, то это вложенный объект
    /// такой объект нужно разобрать на составляющие, это делается в QMetaObjectKeeper.
    /// В конце концов любой объект состоит либо из элементарных типов либо из таких же объектов
    QObject * castobj = qvariant_cast<QObject *>(prop.read(obj));
    if(castobj)
        return new QMetaObjectKeeper(castobj,prop);

    int t_id = QMetaType::type(prop.typeName());
    if( t_id == qMetaTypeId<std::vector<int>>())
        return new QMetaArrayKeeper<int>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<QString>>())
        return new QMetaArrayKeeper<QString>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<double>>())
        return new QMetaArrayKeeper<double>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<bool>>())
        return new QMetaArrayKeeper<bool>(obj, prop);

    else if (QString(prop.typeName()).contains("std::vector<"))
        return new QMetaObjectArrayKeeper(obj, prop);

    return new QMetaSimpleKeeper(obj,prop);
}



std::vector<PropertyKeeper *> getMetaKeepers(QObject *obj)
{
    std::vector<PropertyKeeper*> keepers;
    for(int i = 0; i < obj->metaObject()->propertyCount(); i++)
    {
        if(obj->metaObject()->property(i).isUser(obj))
            keepers.push_back(getMetaKeeper(obj, obj->metaObject()->property(i)));
    }
    return keepers;
}

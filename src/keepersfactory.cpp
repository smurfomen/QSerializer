#include <keepersfactory.h>

    const std::vector<int> simple_t =
    {
        qMetaTypeId<int>(),
        qMetaTypeId<bool>(),
        qMetaTypeId<double>(),
        qMetaTypeId<QString>(),
    };

    const std::vector<int> array_of_simple_t =
    {
        qMetaTypeId<std::vector<int>>(),
        qMetaTypeId<std::vector<bool>>(),
        qMetaTypeId<std::vector<double>>(),
        qMetaTypeId<std::vector<QString>>(),
    };

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

PropertyKeeper *KeepersFactory::getMetaKeeper(QObject *obj, QMetaProperty prop)
{
    /// JSON поддерживает такие типы данных как int, string, bool, double
    /// соответственно если сериализуемое свойство не представляет собой один из этих типов или массив из этих типов, то это вложенный объект
    /// такой объект нужно разобрать на составляющие, это делается в QMetaObjectKeeper.
    /// В конце концов любой объект состоит либо из элементарных типов либо из таких же объектов
    int t_id = QMetaType::type(prop.typeName());
    if(std::find(simple_t.begin(), simple_t.end(), t_id) != simple_t.end())
        return new QMetaSimpleKeeper(obj,prop);
    else if (std::find(array_of_simple_t.begin(),array_of_simple_t.end(), t_id) != array_of_simple_t.end())
    {
        if( t_id == qMetaTypeId<std::vector<int>>())
            return new QMetaArrayKeeper<int>(obj, prop);

        else if(t_id == qMetaTypeId<std::vector<QString>>())
            return new QMetaArrayKeeper<QString>(obj, prop);

        else if(t_id == qMetaTypeId<std::vector<double>>())
            return new QMetaArrayKeeper<double>(obj, prop);

        else if(t_id == qMetaTypeId<std::vector<bool>>())
            return new QMetaArrayKeeper<bool>(obj, prop);
    }
    /// если не подошел ни один из задекларированных типов - попробовать квалифицировать Property как производную от QObject
    else
    {
        QObject * castobj = qvariant_cast<QObject *>(prop.read(obj));
        if(castobj)
            return new QMetaObjectKeeper(castobj,prop);
        else if (QString(prop.typeName()).contains("std::vector<"))
        {
            QString t = QString(prop.typeName()).remove("std::vector<").remove(">");
            int idOfElement = QMetaType::type(t.toStdString().c_str());
            if(QMetaType::typeFlags(idOfElement).testFlag(QMetaType::PointerToQObject))
                return new QMetaObjectArrayKeeper(obj, prop);
        }
    }
    throw QSException(UnsupportedPropertyType);
}

std::vector<PropertyKeeper *> KeepersFactory::getMetaKeepers(QObject *obj)
{
    std::vector<PropertyKeeper*> keepers;
    for(int i = 0; i < obj->metaObject()->propertyCount(); i++)
    {
        if(obj->metaObject()->property(i).isUser(obj))
            keepers.push_back(getMetaKeeper(obj, obj->metaObject()->property(i)));
    }
    return keepers;
}

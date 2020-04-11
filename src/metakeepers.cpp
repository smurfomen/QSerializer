#include <metakeepers.h>
#include <keepersfactory.h>
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



void MetaObjectKeeper::fillObjectFromJson(QObject *qo, const QJsonValue & json)
{
    if(!json.isObject())
        throw QSException(JsonObjectExpected);

    /// перебор всех ключей у переданного JSON и всех хранителей у объекта, если нашли хранителя с подходящим ключом - отдаем значение по ключу в найденный хранитель
    /// дальше он сам разберется что ему с ним делать, в зависимости от типа хранителя за прослойкой интерфейса
    QJsonObject jsonObject = json.toObject();
    QStringList keys = jsonObject.keys();
    KeepersFactory factory;
    std::vector<PropertyKeeper*> metaKeepers = factory.getKeepersForObject(qo);
    for(QString & key : keys)
    {
        for(int i = 0; i < metaKeepers.size(); i++)
        {
            PropertyKeeper * keeper = metaKeepers.at(i);
            QString keeperKey = keeper->toJson().first;
            if(key == keeperKey)
            {
                keeper->fromJson(jsonObject.value(key));
                metaKeepers.erase(metaKeepers.begin()+i);
            }
        }
    }
}

void MetaObjectKeeper::fillObjectFromXml(QObject *qo, const QDomNode & xml)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);
    QDomDocument doc = xml.toDocument();
    QDomNode node = doc.firstChild().firstChild();
    while(!node.isNull())
    {
        QDomElement element = node.toDocument().documentElement();
        for(int i = 0; i < keepers.size(); i++)
        {
            PropertyKeeper * keeper = keepers.at(i);
            if(element.tagName() == keeper->toXml().first)
            {
                keeper->fromXml(element);
                keepers.erase(keepers.begin()+i);
                break;
            }
        }
        node = node.nextSibling();
    }
}

QJsonObject MetaObjectKeeper::getJsonFromObject(QObject *qo)
{
    /// взять коллекцию простых хранителей, хранящих в себе элементарные данные и взять у каждого хранителя его ключ и JSON значение
    /// составить из этих значений объект.
    /// внутри коллекции хранителей может быть любой хранитель (как элементарный так и хранитель вложенного объекта)
    /// сбор информации будет продолжаться до тех пор, пока не упрется в последние элементарные хранители самых глубоковложенных объектов
    /// и так же JSON значения (QJsonValue) будут возвращаться и сливаться в объекты до тех пор, пока не вернутся в корень для возврата вернут сформированного JSON объекта
    QJsonObject json;
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);
    for(PropertyKeeper * keeper : keepers)
    {
        std::pair<QString, QJsonValue> keeperValue = keeper->toJson();
        json.insert(keeperValue.first, keeperValue.second);
    }
    return json;
}

QDomNode MetaObjectKeeper::getXmlFromObject(QObject *qo)
{
    QDomDocument doc;
    QDomElement element = doc.createElement(prop.name());
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);
    for(PropertyKeeper * keeper : keepers)
    {
        std::pair<QString, QDomNode> keeperValue = keeper->toXml();
        element.appendChild(keeperValue.second);
    }
    doc.appendChild(element);
    return QDomNode(doc);
}

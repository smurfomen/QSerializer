#ifndef QSERIALIZER_H
#define QSERIALIZER_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <type_traits>

#include <QVector>
#include <QVariant>

#include <QMetaProperty>
#include <QMetaObject>
#include <QMetaType>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
Q_DECLARE_METATYPE(QDomNode)
Q_DECLARE_METATYPE(QDomElement)
class QSerializer {
    Q_GADGET
public:
    QSerializer(QMetaObject _mo) : mo(_mo){}
    static QByteArray toByteArray(const QJsonValue & value){
        return QJsonDocument(value.toObject()).toJson();
    }

    static QByteArray toByteArray(const QDomNode & value) {
        QDomDocument doc = value.toDocument();
        return doc.toByteArray();
    }

    ///\brief Serialize all accessed JSON propertyes for this object
    QJsonObject toJson() const {
        QJsonObject json;
        for(int i = 0; i < metaObject()->propertyCount(); i++)
        {
            if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))
                continue;

            json.insert(metaObject()->property(i).name(), metaObject()->property(i).readOnGadget(this).toJsonValue());
        }
        return json;
    }

    ///\brief Deserialize all accessed JSON propertyes for this object
    void fromJson(const QJsonValue & val) {
        if(val.isObject())
        {
            QJsonObject json = val.toObject();
            QStringList keys = json.keys();
            int propCount = metaObject()->propertyCount();
            for(int i = 0; i < propCount; i++)
            {
                if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))
                    continue;

                for(auto key : json.keys())
                {
                    if(key == metaObject()->property(i).name())
                    {
                        metaObject()->property(i).writeOnGadget(this, json.value(key));
                        break;
                    }
                }
            }
        }
    }

    ///\brief Serialize all accessed XML propertyes for this object
    QDomNode toXml() const {
        QDomDocument doc;
        QDomElement el = doc.createElement(metaObject()->className());
        for(int i = 0; i < metaObject()->propertyCount(); i++)
        {
            if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QDomNode>()))
                continue;

            el.appendChild(QDomNode(metaObject()->property(i).readOnGadget(this).value<QDomNode>()));
        }
        doc.appendChild(el);
        return doc;
    }

    ///\brief Deserialize all accessed XML propertyes for this object
    void fromXml(const QDomNode & doc){
        if(metaObject()->className() == doc.firstChildElement().tagName())
        {
            for(int i = 0; i < metaObject()->propertyCount(); i++)
            {
                if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QDomNode>()))
                    continue;

                QDomNode fieldNode = doc.firstChild().firstChild();
                while(!fieldNode.isNull())
                {
                    QDomElement currentElement = fieldNode.toElement();
                    if(metaObject()->property(i).name() == currentElement.tagName())
                    {
                        metaObject()->property(i).writeOnGadget(this, QVariant::fromValue(currentElement));
                        break;
                    }
                    fieldNode = fieldNode.nextSibling();
                }
            }
        }
    }

    void fromXml(const QByteArray & data) {
        fromXml(QDomDocument(data));
    }

    void fromJson(const QByteArray & data) {
        fromJson(QJsonDocument::fromJson(data).object());
    }

    const QMetaObject * metaObject() const {
        return &mo;
    }

private:
    QMetaObject mo;
};

#include <QDebug>
#define GET(prefix, name) get_##prefix##_##name
#define SET(prefix, name) set_##prefix##_##name

#define QS_CLASS                                                                                                                        \
    public:                                                                                                                             \
    QJsonObject toJson() const {                                                                                                        \
        QJsonObject json;                                                                                                               \
        for(int i = 0; i < metaObject()->propertyCount(); i++)                                                                                \
        {                                                                                                                               \
            if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))                               \
                continue;                                                                                                               \
                                                                                                                                        \
            json.insert(metaObject()->property(i).name(), metaObject()->property(i).readOnGadget(this).toJsonValue());                              \
        }                                                                                                                               \
        return json;                                                                                                                    \
    }                                                                                                                                   \
                                                                                                                                        \
    void fromJson(const QJsonValue & val) {                                                                                             \
        if(val.isObject())                                                                                                              \
        {                                                                                                                               \
            QJsonObject json = val.toObject();                                                                                          \
            QStringList keys = json.keys();                                                                                             \
            int propCount = metaObject()->propertyCount();                                                                                    \
            for(int i = 0; i < propCount; i++)                                                                                          \
            {                                                                                                                           \
                if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))                           \
                    continue;                                                                                                           \
                                                                                                                                        \
                for(auto key : json.keys())                                                                                             \
                {                                                                                                                       \
                    if(key == metaObject()->property(i).name())                                                                               \
                    {                                                                                                                   \
                        metaObject()->property(i).writeOnGadget(this, json.value(key));                                                       \
                        break;                                                                                                          \
                    }                                                                                                                   \
                }                                                                                                                       \
            }                                                                                                                           \
        }                                                                                                                               \
    }                                                                                                                                   \
                                                                                                                                        \
    QDomNode toXml() const {                                                                                                            \
        QDomDocument doc;                                                                                                               \
        QDomElement el = doc.createElement(metaObject()->className());                                                                        \
        for(int i = 0; i < metaObject()->propertyCount(); i++)                                                                                \
        {                                                                                                                               \
            if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QDomNode>()))                                 \
                continue;                                                                                                               \
                                                                                                                                        \
            el.appendChild(QDomNode(metaObject()->property(i).readOnGadget(this).value<QDomNode>()));                                         \
        }                                                                                                                               \
        doc.appendChild(el);                                                                                                            \
        QDomDocument d;                                                                                                                 \
        d.setContent(QSerializer::toByteArray(doc));                                                                                    \
        return d;                                                                                                                       \
    }                                                                                                                                   \
                                                                                                                                        \
    void fromXml(const QDomNode & doc){                                                                                                 \
        if(metaObject()->className() == doc.firstChild().nodeName())                                                                          \
        {                                                                                                                               \
            for(int i = 0; i < metaObject()->propertyCount(); i++)                                                                            \
            {                                                                                                                           \
                if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QDomNode>()))                             \
                    continue;                                                                                                           \
                                                                                                                                        \
                QDomNode fieldNode = doc.firstChild().firstChild();                                                                     \
                while(!fieldNode.isNull())                                                                                              \
                {                                                                                                                       \
                    QDomElement currentElement = fieldNode.toElement();                                                                 \
                                                                                                                                        \
                    QDomElement tmp = metaObject()->property(i).readOnGadget(this).value<QDomNode>().firstChildElement();                     \
                                                                                                                                        \
                    if(metaObject()->property(i).name() == currentElement.tagName() || tmp.tagName() == currentElement.tagName())             \
                    {                                                                                                                   \
                        metaObject()->property(i).writeOnGadget(this, QVariant::fromValue<QDomNode>(currentElement));                         \
                        break;                                                                                                          \
                    }                                                                                                                   \
                    fieldNode = fieldNode.nextSibling();                                                                                \
                }                                                                                                                       \
            }                                                                                                                           \
        }                                                                                                                               \
        else if (metaObject()->className() == doc.toElement().tagName())                                                                      \
        {                                                                                                                               \
            for(int i = 0; i < metaObject()->propertyCount(); i++)                                                                            \
            {                                                                                                                           \
                if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QDomNode>()))                             \
                    continue;                                                                                                           \
                                                                                                                                        \
                QDomNode fieldNode = doc.firstChild();                                                                                  \
                while(!fieldNode.isNull())                                                                                              \
                {                                                                                                                       \
                    QDomElement currentElement = fieldNode.toElement();                                                                 \
                                                                                                                                        \
                    QDomElement tmp = metaObject()->property(i).readOnGadget(this).value<QDomNode>().firstChildElement();                     \
                                                                                                                                        \
                    if(metaObject()->property(i).name() == currentElement.tagName() || tmp.tagName() == currentElement.tagName())             \
                    {                                                                                                                   \
                        metaObject()->property(i).writeOnGadget(this, QVariant::fromValue<QDomNode>(currentElement));                         \
                        break;                                                                                                          \
                    }                                                                                                                   \
                    fieldNode = fieldNode.nextSibling();                                                                                \
                }                                                                                                                       \
            }                                                                                                                           \
        }                                                                                                                               \
    }                                                                                                                                   \
                                                                                                                                        \
    void fromXml(const QByteArray & data) {                                                                                             \
        QDomDocument doc;                                                                                                               \
        doc.setContent(data);                                                                                                           \
        fromXml(doc);                                                                                                                   \
    }                                                                                                                                   \
                                                                                                                                        \
    void fromJson(const QByteArray & data) {                                                                                            \
        fromJson(QJsonDocument::fromJson(data).object());                                                                               \
    }                                                                                                                                   \
                                                                                                                                        \
    const QMetaObject * metaObject() const {                                                                                                  \
        return &this->staticMetaObject;                                                                                                 \
    }                                                                                                                                   \












/* Create variable */
#define QS_DECLARE_VARIABLE(type, name)                                                     \
    public :                                                                                \
    type name = type();                                                                     \

/* Create JSON property and methods for primitive type field*/
#define QS_JSON_FIELD(type, name)                                                           \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                  \
    private:                                                                                \
        QJsonValue GET(json, name)() const {                                                \
            QJsonValue val = QJsonValue::fromVariant(QVariant(name));                       \
            return val;                                                                     \
        }                                                                                   \
        void SET(json, name)(const QJsonValue & varname){                                   \
            name = varname.toVariant().value<type>();                                       \
        }                                                                                   \


/* Create XML property and methods for primitive type field*/
#define QS_XML_FIELD(type, name)                                                            \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                      \
    private:                                                                                \
    QDomNode GET(xml, name)() const {                                                       \
        QDomDocument doc;                                                                   \
        QString strname = #name;                                                            \
        QDomElement element = doc.createElement(strname);                                   \
        QDomText valueOfProp = doc.createTextNode(QVariant(name).toString());               \
        element.appendChild(valueOfProp);                                                   \
        doc.appendChild(element);                                                           \
        return  QDomNode(element);                                                          \
    }                                                                                       \
    void SET(xml, name)(const QDomNode &node) {                                             \
        if(!node.isNull() && node.isElement()){                                             \
            QDomElement domElement = node.toElement();                                      \
            if(domElement.tagName() == #name)                                               \
                name = QVariant(domElement.text()).value<type>();                           \
        }                                                                                   \
    }                                                                                       \


/* Generate JSON-property and methods for primitive type objects */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_JSON_ARRAY(itemType, name)                                                       \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                  \
    private:                                                                                \
        QJsonValue GET(json, name)() const {                                                \
            QJsonArray val;                                                                 \
            for(int i = 0; i < name.size(); i++)                                            \
                val.push_back(name.at(i));                                                  \
            return QJsonValue::fromVariant(val);                                            \
        }                                                                                   \
        void SET(json, name)(const QJsonValue & varname) {                                  \
            if(!varname.isArray())                                                          \
                return;                                                                     \
            name.clear();                                                                   \
            QJsonArray val = varname.toArray();                                             \
            for(auto item : val) {                                                          \
                itemType tmp;                                                               \
                tmp = item.toVariant().value<itemType>();                                   \
                name.append(tmp);                                                           \
            }                                                                               \
        }                                                                                   \

/* Generate XML-property and methods for primitive type objects */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_XML_ARRAY(itemType, name)                                                        \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                      \
    private:                                                                                \
        QDomNode GET(xml, name)() const {                                                   \
            QDomDocument doc;                                                               \
            QString strname = #name;                                                        \
            QDomElement arrayXml = doc.createElement(QString(strname));                     \
            arrayXml.setAttribute("type", "array");                                         \
                                                                                            \
            for(int i = 0; i < name.size(); i++) {                                          \
                itemType item = name.at(i);                                                 \
                QDomElement itemXml = doc.createElement("item");                            \
                itemXml.setAttribute("type", #itemType);                                    \
                itemXml.setAttribute("index", i);                                           \
                itemXml.appendChild(doc.createTextNode(QVariant(item).toString()));         \
                arrayXml.appendChild(itemXml);                                              \
            }                                                                               \
                                                                                            \
            doc.appendChild(arrayXml);                                                      \
            return  QDomNode(doc);                                                          \
        }                                                                                   \
        void SET(xml, name)(const QDomNode & node) {                                        \
            QDomNode domNode = node.firstChild();                                           \
            name.clear();                                                                   \
            while(!domNode.isNull()) {                                                      \
                if(domNode.isElement()) {                                                   \
                    QDomElement domElement = domNode.toElement();                           \
                    name.append(QVariant(domElement.text()).value<itemType>());             \
                }                                                                           \
                domNode = domNode.nextSibling();                                            \
            }                                                                               \
        }                                                                                   \





/* Generate JSON-property and methods for some custom class */
/* Custom type must be provide methods fromJson and toJson or inherit from QSerializer */
#define QS_JSON_OBJECT(type, name)                                                          \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                  \
    private:                                                                                \
    QJsonValue GET(json, name)() const {                                                    \
        QJsonObject val = name.toJson();                                                    \
        return QJsonValue(val);                                                             \
    }                                                                                       \
    void SET(json, name)(const QJsonValue & varname) {                                      \
        if(!varname.isObject())                                                             \
        return;                                                                             \
        name.fromJson(varname);                                                             \
    }                                                                                       \

/* Generate XML-property and methods for some custom class */
/* Custom type must be provide methods fromJson and toJson or inherit from QSerializer */
#define QS_XML_OBJECT(type, name)                                                           \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                      \
    private:                                                                                \
        QDomNode GET(xml, name)() const {                                                   \
            return name.toXml();                                                            \
        }                                                                                   \
        void SET(xml, name)(const QDomNode & node){                                         \
            name.fromXml(node);                                                             \
        }                                                                                   \

/* Generate JSON-property and methods for collection of custom type objects */
/* Custom item type must be provide methods fromJson and toJson or inherit from QSerializer */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_JSON_ARRAY_OBJECTS(itemType, name)                                               \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                  \
    private:                                                                                \
        QJsonValue GET(json, name)() const {                                                \
            QJsonArray val;                                                                 \
            for(int i = 0; i < name.size(); i++)                                            \
                val.push_back(name.at(i).toJson());                                         \
            return QJsonValue::fromVariant(val);                                            \
        }                                                                                   \
        void SET(json, name)(const QJsonValue & varname) {                                  \
            if(!varname.isArray())                                                          \
                return;                                                                     \
            name.clear();                                                                   \
            QJsonArray val = varname.toArray();                                             \
            for(int i = 0; i < val.size(); i++) {                                           \
                itemType tmp;                                                               \
                tmp.fromJson(val.at(i));                                                    \
                name.append(tmp);                                                           \
            }                                                                               \
        }                                                                                   \

/* Generate XML-property and methods for collection of custom type objects  */
/* Custom type must be provide methods fromXml and toXml or inherit from QSerializer */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_XML_ARRAY_OBJECTS(itemType, name)                                                \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                      \
    private:                                                                                \
    QDomNode GET(xml, name)() const {                                                       \
        QDomDocument doc;                                                                   \
        QDomElement element = doc.createElement(#name);                                     \
        for(int i = 0; i < name.size(); i++)                                                \
            element.appendChild(name.at(i).toXml());                                        \
        doc.appendChild(element);                                                           \
        return QDomNode(doc);                                                               \
    }                                                                                       \
    void SET(xml, name)(const QDomNode & node) {                                            \
        name.clear();                                                                       \
        QDomNodeList nodesList = node.childNodes();                                         \
        for(int i = 0;  i < nodesList.size(); i++) {                                             \
            itemType tmp;                                                                   \
            tmp.fromXml(nodesList.at(i));                                                   \
            name.append(tmp);                                                               \
        }                                                                                   \
    }                                                                                       \










/* Make primitive field and generate serializable propertyes */
/* For example: QS_FIELD(int, digit), QS_FIELD(bool, flag) */
#define QS_FIELD(type, name)                                                                \
    QS_DECLARE_VARIABLE(type, name)                                                         \
    QS_JSON_FIELD(type, name)                                                               \
    QS_XML_FIELD(type, name)                                                                \

/* Make collection of primitive type objects [collectionType<itemType> name] and generate serializable propertyes for this collection */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_COLLECTION(collectionType, itemType, name)                                       \
    QS_DECLARE_VARIABLE(collectionType<itemType>, name)                                     \
    QS_JSON_ARRAY(itemType, name)                                                           \
    QS_XML_ARRAY(itemType, name)                                                            \

/* Make custom class object and bind serializable propertyes */
/* This class must be inherited from QSerializer */
#define QS_OBJECT(type,name)                                                                \
    QS_DECLARE_VARIABLE(type, name)                                                         \
    QS_JSON_OBJECT(type, name)                                                              \
    QS_XML_OBJECT(type, name)                                                               \


/* Make collection of custom class objects [collectionType<itemType> name] and bind serializable propertyes */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_COLLECTION_OBJECTS(collectionType, itemType, name)                               \
    QS_DECLARE_VARIABLE(collectionType<itemType>, name)                                     \
    QS_JSON_ARRAY_OBJECTS(itemType, name)                                                   \
    QS_XML_ARRAY_OBJECTS(itemType, name)                                                    \



#endif // QSERIALIZER_H


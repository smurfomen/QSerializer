//#include "qjsonkeeper.h"

//IntPropertyKeeper::IntPropertyKeeper(QString name, int *member)
//{
//    this->pName = name;
//    this->pMember = member;
//}

//std::pair<QString, QJsonValue> IntPropertyKeeper::getValue()
//{
//    return std::make_pair(pName, QJsonValue(*pMember));
//}

//void IntPropertyKeeper::setValue(QJsonValue val)
//{
//    *pMember = val.toInt();
//}







//BoolPropertyKeeper::BoolPropertyKeeper(QString name, bool *member)
//{
//    this->pName = name;
//    this->pMember = member;
//}

//std::pair<QString, QJsonValue> BoolPropertyKeeper::getValue()
//{
//    return std::make_pair(pName, QJsonValue(*pMember));
//}

//void BoolPropertyKeeper::setValue(QJsonValue val)
//{

//    *pMember = val.toBool();
//}







//StringPropertyKeeper::StringPropertyKeeper(QString name, QString *member)
//{
//    this->pName = name;
//    this->pMember = member;
//}

//std::pair<QString, QJsonValue> StringPropertyKeeper::getValue()
//{
//    return std::make_pair(pName, QJsonValue(*pMember));

//}

//void StringPropertyKeeper::setValue(QJsonValue val)
//{
//    *pMember = val.toString();
//}


//ArrayPropertyKeeper::ArrayPropertyKeeper(QString pName, std::vector<PropertyKeeper *> elements)
//{
//    this->pName = pName;
//    this->elements = elements;
//}

//std::pair<QString, QJsonValue> ArrayPropertyKeeper::getValue()
//{
//    QJsonValue val(getJsonArray());
//    return std::make_pair(pName,val);
//}

//void ArrayPropertyKeeper::setValue(QJsonValue val)
//{
//    setJsonArray(val.toArray());
//}

//QJsonArray ArrayPropertyKeeper::getJsonArray()
//{
//    QJsonArray arr;
//    for(auto element : elements)
//        arr.append(element->getValue().second);
//    return arr;
//}

//void ArrayPropertyKeeper::setJsonArray(QJsonArray arr)
//{
//    for (int i = 0; i < elements.size();i++) {
//        elements.at(i)->setValue(arr.at(i));
//    }
//}

#include "JsonParser.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QRegExp>
#include <iostream>

JsonParser::JsonParser(QObject *parent) : QObject(parent)
{

}

JsonParser::~JsonParser()
{

}

QList<QPair<int, QString>>* JsonParser::getAppsDataFromJson(const QByteArray& res, int limit)
{
	QRegExp regex;
	regex.setPattern("[a-zA-Z]+[a-zA-Z0-9 \\'\\`\\?\\:\\~\\!]*");
	QJsonArray array = QJsonDocument::fromJson(res).object().
			take("applist").toObject().take("apps").toArray();
	if(limit <= 0 || limit > array.size()){
		limit = array.size();
	}
	QList<QPair<int,QString>>* list = new QList<QPair<int,QString>>();
	list->reserve(limit);
	for(int i=0;i<limit;i++){
		QJsonObject obj = array[i].toObject();
		int id = obj.take("appid").toInt();
		QString name = obj.take("name").toString();
		if(regex.exactMatch(name)){
			list->append(QPair<int, QString>(id, name));
		}
	}
	return list;
}

QPair<int, double> *JsonParser::getAppPrice(const QByteArray& res, const QString &appid)
{
	QJsonObject obj = QJsonDocument::fromJson(res).object()
			.take(appid).toObject();
	bool suc = obj.take("success").toBool();
	QPair<int, double> *pricePair = new QPair<int,double>{appid.toInt(),-1};
	if(suc){
		obj = obj.take("data").toObject();
		if(obj.take("type").toString() == "game"){
			if(!obj.take("is_free").toBool()){
				double price = obj.take("price_overview").toObject().take("final").toDouble() / 100;
				pricePair->second = price;
			}else{
				pricePair->second = 0;
			}
		}

	}
	return pricePair;
}

QPair<int, int>* JsonParser::checkIfIsGame(const QByteArray& res, const QString& appid)
{
	QJsonObject obj = QJsonDocument::fromJson(res).object()
			.take(appid).toObject();
	QPair<int, int> *pricePair = new QPair<int, int>(appid.toInt(),-10); // if -10 returned, means steam not allowed data fetch
	if(!obj.isEmpty()){
		bool suc = obj.take("success").toBool();
		pricePair->second = -1; //if -1 returned, means no success or just not a game
		if(suc){
			obj = obj.take("data").toObject();
			if(obj.take("type").toString() == "game"){
				pricePair->second = 0;
			}
		}
	}
	return pricePair;
}


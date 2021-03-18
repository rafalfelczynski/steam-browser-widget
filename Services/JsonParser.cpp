#include "JsonParser.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QRegExp>
#include <QDebug>

QVector<App> JsonParser::getAppsDataFromJson(const QByteArray &res, int limit)
{
	QJsonArray array = QJsonDocument::fromJson(res).object().take("applist").toObject().take("apps").toArray();
	if(limit <= 0 || limit > array.size()) {
		limit = array.size();
	}
	QVector<App> apps;
	QRegExp regex;
	regex.setPattern("[a-zA-Z]+[a-zA-Z0-9 \\'\\`\\?\\:\\~\\!]*");
	for(int i = 0; i < limit; i++) {
		QJsonObject obj = array[i].toObject();
		int id = obj.take("appid").toInt();
		QString name = obj.take("name").toString();
		if(regex.exactMatch(name)) {
			apps << App {id, name};
		}
	}
	return apps;
}

std::optional<double> JsonParser::getAppPrice(const QByteArray &res, const QString &appid)
{
	QJsonObject obj = QJsonDocument::fromJson(res).object().take(appid).toObject();
	bool suc = obj.take("success").toBool();
	if(suc) {
		obj = obj.take("data").toObject();
		if(obj.take("type").toString() == "game") {
			if(!obj.take("is_free").toBool()) {
				double price = obj.take("price_overview").toObject().take("final").toDouble() / 100;
				return {price};
			} else {
				return {0.0};
			}
		}
	}
	return {};
}

JsonParser::Codes JsonParser::checkIfIsGame(const QByteArray &res, const QString &appid)
{
	QJsonObject obj = QJsonDocument::fromJson(res).object()[appid].toObject();
	JsonParser::Codes code = Codes::SteamFailure; // if -10 returned, means steam not allowed data fetch
	if(!obj.isEmpty()) {
		bool suc = obj.take("success").toBool();
		if(suc) {
			obj = obj.take("data").toObject();
			if(obj.take("type").toString() == "game") {
				code = Codes::Game;
			} else {
				code = Codes::NotGame;
			}
		} else {
			code = Codes::NotGame;
		}
	}
	return code;
}

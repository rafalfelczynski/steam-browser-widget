#include "SteamConnector.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <QFile>
#include <QVariant>
#include <QStandardPaths>

SteamConnector::SteamConnector(QNetworkAccessManager *netMan, QObject *parent) : QObject(parent)
{
	this->netMan = netMan;
}

SteamConnector::~SteamConnector()
{

}

void SteamConnector::sendFetchAllAppsReq()
{
	qDebug() << "send fetch all apps req steam connector";
	QNetworkRequest req((QUrl(ALL_GAMES_ENDPOINT)));
	QNetworkReply *stream = netMan->get(req);
	QObject::connect(stream, &QNetworkReply::finished,
					 [=]()->void{this->recReplyAll(stream);});
}

void SteamConnector::sendFetchGamePriceReq(int gameid)
{
	fetchGamePrice(gameid);
}

void SteamConnector::sendCheckIfIsGameReq(int appid)
{
	checkIfIsGame(appid);
}

void SteamConnector::recError(QNetworkReply::NetworkError code)
{
	std::cout << code << std::endl;
}

void SteamConnector::recReplyAll(QNetworkReply* reply)
{
	std::cout << "error: " << reply->error() << std::endl;
	QByteArray res = reply->readAll();
	std::cout << "byte array size: " << res.size() <<std::endl;
	if(res.size() == 23){
		std::cout << "zawartosc+" << res.toStdString() << "+" << std::endl;
	}
	JsonParser parser;
	QList<QPair<int, QString>>* list = parser.getAppsDataFromJson(res, -1);
	reply->deleteLater();
	std::cout << "list size steam connector " << list->size() << std::endl;
	emit allAppsReady(list);
}

void SteamConnector::recReplyPrice(QNetworkReply* reply, const QString &appid)
{
	//std::cout << "appid: " << appid.toStdString() << std::endl;
	QByteArray res = reply->readAll();
	QPair<int, double> *price = JsonParser().getAppPrice(res, appid);
	reply->deleteLater();
	emit priceReceived(price);
}

void SteamConnector::recIfIsGame(QNetworkReply* reply, const QString &appid)
{
	QByteArray res = reply->readAll();
	QPair<int,int> *check = JsonParser().checkIfIsGame(res, appid);
	reply->deleteLater();
	emit ifGameChecked(check);
}

void SteamConnector::fetchGamePrice(int appid){
	QString id = QString::number(appid);
	QNetworkRequest req((QUrl(SPEC_GAME_ENDPOINT + id)));
	QNetworkReply *stream = netMan->get(req);
	QObject::connect(stream, &QNetworkReply::finished,
					 [=](){this->recReplyPrice(stream, id);});
}

void SteamConnector::checkIfIsGame(int appid)
{
	QString id = QString::number(appid);
	QNetworkRequest req((QUrl(SPEC_GAME_ENDPOINT + id)));
	QNetworkReply *stream = netMan->get(req);
	QObject::connect(stream, &QNetworkReply::finished,
					 [=](){this->recIfIsGame(stream, id);});
}




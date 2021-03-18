#include "SteamConnector.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <QFile>
#include <QVariant>
#include <QStandardPaths>

SteamConnector::SteamConnector(const QSharedPointer<QNetworkAccessManager> &networkMan, QObject *parent)
	: QObject(parent)
{
	this->netMan = networkMan;
}

void SteamConnector::sendFetchAllAppsReq()
{
	QNetworkRequest req({ALL_GAMES_ENDPOINT});
	QNetworkReply *stream = netMan->get(req);
	QObject::connect(stream, &QNetworkReply::finished, [=]() -> void { this->recReplyAll(stream); });
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

void SteamConnector::recReplyAll(QNetworkReply *reply)
{
	std::cout << "error: " << reply->error() << std::endl;
	QByteArray res = reply->readAll();
	std::cout << "byte array size: " << res.size() << std::endl;
	if(res.size() == 23) {
		std::cout << "zawartosc+" << res.toStdString() << "+" << std::endl;
	}
	auto apps = JsonParser::getAppsDataFromJson(res, -1);
	reply->deleteLater();
	std::cout << "list size steam connector " << apps.size() << std::endl;
	emit allAppsReady(apps);
}

void SteamConnector::recReplyPrice(QNetworkReply *reply, const QString &appid)
{
	auto price = JsonParser::getAppPrice(reply->readAll(), appid);
	reply->deleteLater();
	if(price.has_value()) {
		emit priceReceived(appid.toInt(), *price);
	}
}

void SteamConnector::recIfIsGame(QNetworkReply *reply, const QString &appid)
{
	auto code = JsonParser::checkIfIsGame(reply->readAll(), appid);
	reply->deleteLater();
	if(code == JsonParser::Game) {
		emit verifiedAsAGame(appid.toInt());
	} else if(code == JsonParser::NotGame) {
		emit verifiedAsNotAGame(appid.toInt());
	}
}

void SteamConnector::fetchGamePrice(int appid)
{
	QString id = QString::number(appid);
	QNetworkRequest req(QUrl(SPEC_GAME_ENDPOINT + id));
	QNetworkReply *reply = netMan->get(req);
	QObject::connect(reply, &QNetworkReply::finished, [this, reply, id]() { this->recReplyPrice(reply, id); });
}

void SteamConnector::checkIfIsGame(int appid)
{
	QString id = QString::number(appid);
	QNetworkRequest req(QUrl(SPEC_GAME_ENDPOINT + id));
	QNetworkReply *reply = netMan->get(req);
	QObject::connect(reply, &QNetworkReply::finished, [this, reply, id]() { this->recIfIsGame(reply, id); });
}

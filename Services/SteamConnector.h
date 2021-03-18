#ifndef STEAMCONNECTOR_H
#define STEAMCONNECTOR_H
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QHash>
#include <QByteArray>
#include "JsonParser.h"
#include "Models/Game.h"

class SteamConnector : public QObject
{
	Q_OBJECT
	static constexpr char const *SPEC_GAME_ENDPOINT = "https://store.steampowered.com/api/appdetails?format=json&appids=";
	static constexpr char const *ALL_GAMES_ENDPOINT = "https://api.steampowered.com/ISteamApps/GetAppList/v2/?format=json";
	static constexpr char const *FETCH_ALL_TYPE = "all";
	static constexpr char const *FETCH_PRICE_TYPE = "price";

public:
	using Codes = JsonParser::Codes;
	explicit SteamConnector(const QSharedPointer<QNetworkAccessManager> &networkMan, QObject *parent = nullptr);
	void sendFetchAllAppsReq();
	void sendFetchGamePriceReq(int appid);
	void sendCheckIfIsGameReq(int appid);

private:
	QSharedPointer<QNetworkAccessManager> netMan;
	void fetchGamePrice(int appid);
	void checkIfIsGame(int appid);
private slots:
	void recError(QNetworkReply::NetworkError code);
	void recReplyAll(QNetworkReply *reply);
	void recReplyPrice(QNetworkReply *reply, const QString &appid);
	void recIfIsGame(QNetworkReply *reply, const QString &appid);
signals:
	void allAppsReady(const QVector<App> &apps);
	void priceReceived(int appid, double price);
	void verifiedAsAGame(int appId);
	void verifiedAsNotAGame(int appId);
};

#endif // STEAMCONNECTOR_H

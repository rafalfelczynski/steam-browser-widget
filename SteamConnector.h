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
#include <JsonParser.h>


static const QString SPEC_GAME_ENDPOINT = "https://store.steampowered.com/api/appdetails?format=json&appids=";
static const QString ALL_GAMES_ENDPOINT = "https://api.steampowered.com/ISteamApps/GetAppList/v2/?format=json";
static const QString FETCH_ALL_TYPE = "all";
static const QString FETCH_PRICE_TYPE = "price";

class SteamConnector : public QObject
{
	Q_OBJECT
public:
	enum Codes{
		Game=0,
		NotGame=-1,
		SteamFailure=-1
	};
	explicit SteamConnector(QNetworkAccessManager *netMan, QObject *parent = nullptr);
	~SteamConnector();
	void sendFetchAllAppsReq();
	//void sendFetchPricesForSubsReq(QList<Triple<int,double,QString>> *subs);
	//void sendCheckIfGamesReq(QList<QPair<int,QString>> *notTested);
	void sendFetchGamePriceReq(int appid);
	void sendCheckIfIsGameReq(int appid);
private:
	QNetworkAccessManager *netMan;
	void fetchGamePrice(int appid);
	void checkIfIsGame(int appid);
private slots:
	void recError(QNetworkReply::NetworkError code);
	void recReplyAll(QNetworkReply *reply);
	void recReplyPrice(QNetworkReply *reply, const QString &appid);
	void recIfIsGame(QNetworkReply *reply, const QString &appid);
	//void readReply(QNetworkReply *stream, const QString &type);
signals:
	void allAppsReady(QList<QPair<int,QString>>*);
	void priceReceived(QPair<int,double>*);
	void ifGameChecked(QPair<int,int>*);
};

#endif // STEAMCONNECTOR_H

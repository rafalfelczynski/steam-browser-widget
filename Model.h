#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <Triple.h>
#include <Tetrad.h>
#include <DBService.h>
#include <QPair>
#include <QList>
#include <QHash>
#include <QString>
#include <SteamConnector.h>


class Model : public QObject
{
	Q_OBJECT
public:
	Model(QNetworkAccessManager *netMan);
	~Model();
	void startFetchingData();
public slots:
	void insertSubsGame(const QPair<int,double> &gameLimit);
	void deleteSubsGame(const int gameid);
	void gamesListRequested();
	void subsListRequested();
private:
	DBService *db;
	SteamConnector *steamConnector;
	//QList<QPair<int,QString>> *allApps;
	//bool allChanged;
	QList<QPair<int,QString>> *testedGames;
	bool testedChanged;
	//QList<QPair<int,QString>> *notTestedApps;
	//bool notTestedChanged;
	QList<Tetrad<int,double,double,QString>> *subsGames;//id,price,limit,name
	bool subsChanged;
	void fetchAllAppsFromSteam(); //on start
	void fetchPricesForSubs(); //on start // ??and regularly every hour??the same method?
	void checkAppsIfAreGames();
private slots:
	void updateAllApps(QList<QPair<int,QString>> *apps);
	void updateSubsGamePrice(QPair<int, double> *game);
	void updateIfIsGame(QPair<int,int> *game);
signals:
	void allAppsReady(QList<QPair<int,QString>>*);
	void testedGamesReady(QList<QPair<int,QString>> *);
	void notTestedAppsReady(QList<QPair<int,QString>> *);
	void subsGamesReady(QList<Tetrad<int,double,double,QString>> *);
};

#endif // MODEL_H


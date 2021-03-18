#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include "Persistence/GameRepository.h"
#include <QPair>
#include <QList>
#include <QHash>
#include <QString>
#include "Services/SteamConnector.h"
#include "Models/Game.h"

class Model : public QObject
{
	Q_OBJECT
public:
	Model(GameRepository *gameRepository, SteamConnector *steamConnector);
	~Model();
	void startFetchingData();
public slots:
	void insertSubsGame(const QPair<int, double> &gameLimit);
	void deleteSubsGame(const int gameid);
	void gamesListRequested();
	void subsListRequested();

private:
	QScopedPointer<GameRepository> gameRepo;
	QScopedPointer<SteamConnector> steamConnector;
	QVector<App> testedGames;
	QVector<Game> subsGames; //id,price,limit,name
	bool testedChanged = true;
	bool subsChanged = true;
	void fetchAllAppsFromSteam(); //on start
	void fetchPricesForSubs(); //on start // ??and regularly every hour??the same method?
	void checkAppsIfAreGames();
private slots:
	void updateAllApps(const QVector<App> &apps);
	void updateSubsGamePrice(const Game &game);
	void updateIfIsGame(QPair<int, int> *game);
signals:
	void allAppsReady(const QVector<App> &game);
	void testedGamesReady(const QVector<App> &game);
	void notTestedAppsReady(const QVector<App> &game);
	void subsGamesReady(const QVector<Game> &games);
};

#endif // MODEL_H

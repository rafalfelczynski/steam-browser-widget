#ifndef DBSERVICE_H
#define DBSERVICE_H
#include <QList>
#include <QString>
#include <QPair>
#include <Triple.h>
#include <Tetrad.h>
#include <QSqlDatabase>
#include <QVariant>

static const QString DB_NAME = "steam_apps.db";
static const QString ALL_APPS_TAB_NAME = "all_apps";
static const QString TESTED_GAMES_TAB_NAME = "tested_games";
static const QString NOT_TESTED_APPS_TAB_NAME = "not_tested_apps";
static const QString SUBS_GAMES_TAB_NAME = "subs_games";

class DBService
{
public:
	DBService();
	~DBService();
	void start();
	QList<QPair<int, QString>> *getAllApps();
	QList<QPair<int, QString>> *getTestedGames();
	QList<QPair<int, QString>> *getNotTestedApps();
	QList<Tetrad<int,double,double,QString>> *getSubsGames();
	bool insertApp(const QPair<int, QString> &app);
	bool insertTestedGame(const int gameid);
	bool insertNotTestedApp(const int appid);
	bool insertSubsGame(const QPair<int, double> &gameLimit);

	//bool deleteApp(const int appid);
	//bool deleteTestedGame(const int gameid);
	bool deleteNotTestedApp(const int gameid);
	bool deleteSubsGame(const int gameid);

	bool updateSubsGamePrice(const QPair<int,double> &game);

	void many();
	void endMany();

	bool isKnownApp(QPair<int,QString> &app);

private:
	QSqlQuery *q = nullptr;
	QSqlDatabase db;
	void createTables();
	bool insert(const QString &sql, const QVariantList &args);
	bool del(const QString &sql, const QVariantList &whereArgs);
	bool update(const QString &sql, const QVariantList &args);
	QSqlQuery* select(const QString &sql, const QVariantList &whereArgs);
};

#endif // DBSERVICE_H

#pragma once

#include <QList>
#include <QString>
#include <QPair>
#include <QSqlDatabase>
#include <QVariant>
#include "Models/Game.h"

static const QString DB_NAME = "steam_apps.db";
static const QString ALL_APPS_TAB_NAME = "all_apps";
static const QString TESTED_GAMES_TAB_NAME = "tested_games";
static const QString NOT_TESTED_APPS_TAB_NAME = "not_tested_apps";
static const QString SUBS_GAMES_TAB_NAME = "subs_games";

class GameRepository
{
public:
	void start();
	QVector<App> getAllApps();
	QVector<App> getTestedGames();
	QVector<App> getNotTestedApps();
	QVector<Game> getSubsGames();
	bool insertApp(const App &app);
	bool insertTestedGame(const int gameid);
	bool insertNotTestedApp(const int appid);
	bool insertSubsGame(const QPair<int, double> &gameLimit);
	bool deleteNotTestedApp(const int gameid);
	bool deleteSubsGame(const int gameid);
	bool updateSubsGamePrice(const Game &game);
	void many();
	void endMany();
	bool isKnownApp(const App &app);

private:
	QSqlDatabase db;
	void createTables();
	bool insert(const QString &sql, const QVariantList &args);
	bool del(const QString &sql, const QVariantList &whereArgs);
	bool update(const QString &sql, const QVariantList &args);
	QSqlQuery *select(const QString &sql, const QVariantList &whereArgs);
};

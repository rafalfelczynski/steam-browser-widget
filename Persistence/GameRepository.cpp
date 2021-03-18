#include "GameRepository.h"
#include <iostream>
#include <QtSql>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlResult>
#include <QElapsedTimer>

GameRepository::GameRepository()
{
	db = QSqlDatabase::addDatabase("QSQLITE", "conn1"); //tworzy po??czenie z baz?
	db.setDatabaseName(DB_NAME);
	bool suc = db.open();
	if(suc) {
		qDebug() << "sukces polaczenia z baza bazy";
		db.exec("PRAGMA journal_mode = WAL").isValid();
		db.exec("PRAGMA synchronous = NORMAL");
		createTables();
	} else {
		std::cout << "porazka po??czenia z baz?" << std::endl;
	}
}

QVector<App> GameRepository::getAllApps()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, appname FROM all_apps");
	QVector<App> apps;
	while(query.next()) {
		apps << App {query.value(0).toInt(),
					 query.value(1).toString()};
	}
	return apps;
}

QVector<App> GameRepository::getTestedGames()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, appname FROM tested_games"
			   " LEFT JOIN all_apps USING(appid) ORDER BY appname ASC");
	QVector<App> apps;
	while(query.next()) {
		apps << App {query.value(0).toInt(),
					 query.value(1).toString()};
	}
	qDebug() << "tested games:" << apps.size();
	return apps;
}

QVector<App> GameRepository::getNotTestedApps()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, appname FROM not_tested_apps"
			   " LEFT JOIN all_apps USING(appid)");
	QVector<App> apps;
	while(query.next()) {
		apps << App {query.value(0).toInt(),
					 query.value(1).toString()};
	}
	return apps;
}

QVector<Game> GameRepository::getSubsGames()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, price, lim, appname FROM subs_games"
			   " LEFT JOIN all_apps USING(appid) ORDER BY appname ASC");
	QVector<Game> games;
	while(query.next()) {
		Game game;
		game.id = query.value(0).toInt();
		game.price = query.value(1).toDouble();
		game.limit = query.value(2).toDouble();
		game.name = query.value(3).toString();
		games << game;
	}
	return games;
}

bool GameRepository::insertApp(const App &app)
{
	return insert("INSERT OR IGNORE INTO all_apps VALUES(?, ?)", {app.id, app.name});
}

bool GameRepository::insertTestedGame(const int gameid)
{
	return insert("INSERT OR IGNORE INTO tested_games VALUES(?)", {gameid});
}

bool GameRepository::insertNotTestedApp(const int appid)
{
	return insert("INSERT OR IGNORE INTO not_tested_apps VALUES(?)", {appid});
}

bool GameRepository::insertSubsGame(const QPair<int, double> &gameLimit)
{
	return insert("INSERT OR IGNORE INTO subs_games VALUES(?, ?, ?)", {gameLimit.first, 0, gameLimit.second});
}

bool GameRepository::deleteNotTestedApp(const int appid)
{
	return del("DELETE FROM not_tested_apps WHERE appid=?", {appid});
}

bool GameRepository::deleteSubsGame(const int gameid)
{
	return del("DELETE FROM subs_games WHERE appid=?", {gameid});
}

bool GameRepository::updateSubsGamePrice(int appid, double price)
{
	return update("UPDATE subs_games SET price=? WHERE appid=?",
				  {price, appid});
}

void GameRepository::many()
{
	QSqlQuery q(db);
	q.exec("BEGIN TRANSACTION;");
}

void GameRepository::endMany()
{
	QSqlQuery q(db);
	q.exec("COMMIT;");
}

bool GameRepository::isKnownApp(const App &app)
{
	QSqlQuery *q = select("SELECT 1 FROM all_apps where appid=?", {app.id});
	if(q->isActive()) {
		if(q->next()) {
			if(q->value(0).toInt() == 1) {
				delete q;
				return true;
			}
		}
	}
	delete q;
	return false;
}

void GameRepository::createTables()
{
	QSqlQuery query(db);
	query.exec("create table if not exists all_apps("
			   "appid integer primary key,"
			   "appname varchar);");
	query.exec("create table if not exists tested_games("
			   "appid integer primary key,"
			   "FOREIGN KEY(appid) references all_apps(appid));");
	query.exec("create table if not exists not_tested_apps("
			   "appid integer primary key,"
			   "FOREIGN KEY(appid) references all_apps(appid));");
	query.exec("create table if not exists subs_games("
			   "appid integer primary key,"
			   "price real,"
			   "lim real,"
			   "FOREIGN KEY(appid) references all_apps(appid));");
}

bool GameRepository::insert(const QString &sql, const QVariantList &args)
{
	QSqlQuery query(db);
	query.prepare(sql);
	for(int i = 0; i < args.size(); i++) {
		query.bindValue(i, args[i]);
	}
	return query.exec();
}

bool GameRepository::del(const QString &sql, const QVariantList &whereArgs)
{
	QSqlQuery query(db);
	query.prepare(sql);
	for(int i = 0; i < whereArgs.size(); i++) {
		query.bindValue(i, whereArgs[i]);
	}
	return query.exec();
}

bool GameRepository::update(const QString &sql, const QVariantList &args)
{
	QSqlQuery query(db);
	query.prepare(sql);
	for(int i = 0; i < args.size(); i++) {
		query.bindValue(i, args[i]);
	}
	return query.exec();
}

QSqlQuery *GameRepository::select(const QString &sql, const QVariantList &whereArgs)
{
	QSqlQuery *query = new QSqlQuery(db);
	query->prepare(sql);
	for(int i = 0; i < whereArgs.size(); i++) {
		query->bindValue(i, whereArgs[i]);
	}
	query->exec();
	return query;
}

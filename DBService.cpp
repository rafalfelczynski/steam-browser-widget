#include "DBService.h"
#include <iostream>
#include <QtSql>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlResult>
#include <QElapsedTimer>


/*============================================================
 * PUBLIC
 * PUBLIC
 * PUBLIC
 *============================================================
 */

DBService::DBService()
{

}

DBService::~DBService()
{

}

void DBService::start()
{
	db = QSqlDatabase::addDatabase("QSQLITE", "conn1");//tworzy po??czenie z baz?
	db.setDatabaseName(DB_NAME);
	bool suc = db.open();
	if(suc){
		qDebug()<<"sukces po??czenia z baz? bazy";
		//db.exec("CREATE DATABASE IF NOT EXISTS "+DB_NAME);
		db.exec("PRAGMA journal_mode = WAL").isValid();
		db.exec("PRAGMA synchronous = NORMAL");
		createTables();
	}else{
		std::cout<<"porazka po??czenia z baz?"<<std::endl;
	}
}

QList<QPair<int, QString>>* DBService::getAllApps()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, appname FROM all_apps");
	QList<QPair<int, QString>> *list = new QList<QPair<int, QString>>();
	while (query.next()) {
		list->append(QPair<int,QString>(query.value(0).toInt(),
										query.value(1).toString()));
	}
	return list;
}

QList<QPair<int, QString>>* DBService::getTestedGames()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, appname FROM tested_games"
			   " LEFT JOIN all_apps USING(appid) ORDER BY appname ASC");
	QList<QPair<int, QString>> *list = new QList<QPair<int, QString>>();
	while (query.next()) {
		list->append(QPair<int,QString>(query.value(0).toInt(),
										query.value(1).toString()));
	}
	return list;
}

QList<QPair<int, QString>>* DBService::getNotTestedApps()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, appname FROM not_tested_apps"
			   " LEFT JOIN all_apps USING(appid)");
	QList<QPair<int, QString>> *list = new QList<QPair<int, QString>>();
	while (query.next()) {
		list->append(QPair<int,QString>(query.value(0).toInt(),
										query.value(1).toString()));
	}
	return list;
}

QList<Tetrad<int,double,double,QString>>* DBService::getSubsGames()
{
	QSqlQuery query(db);
	query.exec("SELECT appid, price, lim, appname FROM subs_games"
			   " LEFT JOIN all_apps USING(appid) ORDER BY appname ASC");
	QList<Tetrad<int,double,double,QString>> *list = new QList<Tetrad<int,double,double,QString>>();
	while (query.next()) {
		list->append(Tetrad<int,double,double,QString>(query.value(0).toInt(),
												query.value(1).toDouble(),
												query.value(2).toDouble(),
												query.value(3).toString()));
	}
	return list;
}

bool DBService::insertApp(const QPair<int, QString>& app)
{
	return insert("INSERT OR IGNORE INTO all_apps VALUES(?, ?)", {app.first, app.second});
}

bool DBService::insertTestedGame(const int gameid)
{
	return insert("INSERT OR IGNORE INTO tested_games VALUES(?)", {gameid});
}

bool DBService::insertNotTestedApp(const int appid)
{
	return insert("INSERT OR IGNORE INTO not_tested_apps VALUES(?)", {appid});
}

bool DBService::insertSubsGame(const QPair<int, double>& gameLimit)
{
	return insert("INSERT OR IGNORE INTO subs_games VALUES(?, ?, ?)", {gameLimit.first, 0, gameLimit.second});
}

bool DBService::deleteNotTestedApp(const int appid)
{
	return del("DELETE FROM not_tested_apps WHERE appid=?", {appid});
}

bool DBService::deleteSubsGame(const int gameid)
{
	return del("DELETE FROM subs_games WHERE appid=?", {gameid});
}

bool DBService::updateSubsGamePrice(const QPair<int, double>& game)
{
	return update("UPDATE subs_games SET price=? WHERE appid=?",
	{game.second, game.first});
}

void DBService::many()
{
	QSqlQuery q(db);
	q.exec("BEGIN TRANSACTION;");
	q.finish();
}

void DBService::endMany()
{
	QSqlQuery q(db);
	q.exec("COMMIT;");
	q.finish();
}

bool DBService::isKnownApp(QPair<int, QString>& app)
{
	QSqlQuery *q = select("SELECT 1 FROM all_apps where appid=?",{app.first});
	if(q->isActive()){
		if(q->next()){
			if(q->value(0).toInt() == 1){
				q->finish();
				delete q;
				return true;
			}
		}
	}
	q->finish();
	delete q;
	return false;
}

/*=================================================================
 *PRIVATE
 *PRIVATE
 *PRIVATE
 *=================================================================
 */
void DBService::createTables()
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

bool DBService::insert(const QString &sql, const QVariantList &args)
{
	QSqlQuery query(db);
	query.prepare(sql);
	for(int i=0;i<args.size();i++){
		query.bindValue(i, args[i]);
	}
	return query.exec();
	//query.finish();

	//return suc;

}

bool DBService::del(const QString& sql, const QVariantList& whereArgs)
{
	QSqlQuery query(db);
	query.prepare(sql);
	for(int i=0;i<whereArgs.size();i++){
		query.bindValue(i, whereArgs[i]);
	}
	return query.exec();
}

bool DBService::update(const QString& sql, const QVariantList& args)
{
	QSqlQuery query(db);
	query.prepare(sql);
	for(int i=0;i<args.size();i++){
		query.bindValue(i, args[i]);
	}
	return query.exec();
}

QSqlQuery* DBService::select(const QString& sql, const QVariantList& whereArgs)
{
	QSqlQuery *query = new QSqlQuery(db);
	query->prepare(sql);
	for(int i=0;i<whereArgs.size();i++){
		query->bindValue(i, whereArgs[i]);
	}
	query->exec();
	return query;
}

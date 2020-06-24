#include "Model.h"
#include <QTimer>
#include <QElapsedTimer>

/*============================================================
 * PUBLIC
 * PUBLIC
 * PUBLIC
 *============================================================
 */
Model::Model(QNetworkAccessManager *netMan)
{
	db = new DBService();
	steamConnector = new SteamConnector(netMan, this);
	//allChanged = true;
	testedChanged = true;
	//notTestedChanged = true;
	subsChanged = true;
}

Model::~Model()
{
	delete db;
}

void Model::startFetchingData()
{
	db->start();
	//allApps = db->getAllApps();
	//notTestedApps = db->getNotTestedApps();
	testedGames = db->getTestedGames();
	subsGames = db->getSubsGames();
	QObject::connect(steamConnector, &SteamConnector::allAppsReady,
					 this, &Model::updateAllApps);
	QObject::connect(steamConnector, &SteamConnector::priceReceived,
					 this, &Model::updateSubsGamePrice);
	QObject::connect(steamConnector, &SteamConnector::ifGameChecked,
					 this, &Model::updateIfIsGame);
	//fetchAllAppsFromSteam();
	//fetchPricesForSubs();
	//checkAppsIfAreGames();
}

/*============================================================
 * PUBLIC SLOTS
 * PUBLIC SLOTS
 * PUBLIC SLOTS
 *============================================================
 */
void Model::insertSubsGame(const QPair<int,double> &game)
{
	db->insertSubsGame(game);
	steamConnector->sendFetchGamePriceReq(game.first);
	subsChanged = true;
}

void Model::deleteSubsGame(const int gameid)
{
	qDebug() <<"del:" <<db->deleteSubsGame(gameid);
	subsChanged = true;
}

void Model::gamesListRequested()
{
	if(testedChanged){
		delete testedGames;
		testedGames = db->getTestedGames();
		testedChanged = false;
	}
	qDebug() << "games list required" << testedGames->size();
	emit testedGamesReady(testedGames);//jesli bedzie slaby performance to mozna dodac
									//jakas flage ktora bezdzie pokazywac czy od ostatniego czasu dane byly zmieniane
									//i czy okienko w widoku powinno wszystkie usuwac
}

void Model::subsListRequested()
{
	if(subsChanged){
		delete subsGames;
		subsGames = db->getSubsGames();
		subsChanged = false;
		qDebug() << "subs list changed" << subsGames->size();
	}
	qDebug() << "subs list required" << subsGames->size();
	emit subsGamesReady(subsGames);
}


/*=================================================================
 *PRIVATE
 *PRIVATE
 *PRIVATE
 *=================================================================
 */
void Model::fetchAllAppsFromSteam()
{
	qDebug() << "fetching all from steam";
	steamConnector->sendFetchAllAppsReq();
}

void Model::fetchPricesForSubs()
{
	qDebug() <<"fetching prices for subs";
	if(subsChanged){
		delete subsGames;
		subsGames = db->getSubsGames();
		subsChanged = false;
		qDebug() << "subs list changed" << subsGames->size();
	}
	for(Tetrad<int,double,double,QString> &game : *subsGames){
		steamConnector->sendFetchGamePriceReq(game.first);
	}
	QTimer::singleShot(3600000, this, &Model::fetchPricesForSubs); // fetch every hour
}

void Model::checkAppsIfAreGames() // evey 5 minute check remaining apps if the are games or not (videos or whatever)
{
	QList<QPair<int,QString>> *notTestedApps = db->getNotTestedApps();
	int steamLimit = notTestedApps->size() > 150 ? 150 : notTestedApps->size(); // 200 really, but let's take a half
	for(int i=0;i< steamLimit; i++){
		steamConnector->sendCheckIfIsGameReq((*notTestedApps)[i].first);
	}
	delete notTestedApps;
	QTimer::singleShot(310000, this, &Model::checkAppsIfAreGames);
}

/*=================================================================
 *PRIVATE SLOTS
 *PRIVATE SLOTS
 *PRIVATE SLOTS
 *=================================================================
 */
void Model::updateAllApps(QList<QPair<int, QString>> *newApps) // called when all apps were received
{
	qDebug() << "Model: Update all apps " << newApps->size();
	static int tries=0;
	QElapsedTimer tim;
	tim.start();
	db->many();
	for(QPair<int,QString> &app : *newApps){
		if(!db->isKnownApp(app)){
			db->insertApp(app);
			db->insertNotTestedApp(app.first);
		}
	}
	db->endMany();
	qDebug() << "time: " << tim.elapsed();
	if(tries++ < 5 && newApps->size() == 0){ // if nothing fethed from steam server, try 5 times max
		steamConnector->sendFetchAllAppsReq();
	}	//allChanged = true;
	//notTestedChanged = true;
}

void Model::updateSubsGamePrice(QPair<int, double> *game) // called when price was received
{
	qDebug() << "price received" <<game->first <<game->second;
	//if(game->second != kod zwracany jesli steam przekroczy? limit zapyta?){} // zaimplemetowac
	if(game->second != SteamConnector::Codes::SteamFailure){
		qDebug() << "rozne, updatuj";
		db->updateSubsGamePrice(*game);
		subsChanged = true;
	}
}

void Model::updateIfIsGame(QPair<int,int> *game) // called when game-check was received
{
	qDebug() << "update if is game" <<"id gry:"<<game->first<<"kod z konektora:"<<game->second;
	if(game->second == SteamConnector::Codes::Game){
		db->insertTestedGame(game->first);
		db->deleteNotTestedApp(game->first);
		testedChanged = true;
		qDebug() << "new game" << game->first;
		//notTestedChanged = true;
	}else if(game->second == SteamConnector::Codes::NotGame){
		db->deleteNotTestedApp(game->first);
		qDebug() <<"nie gra, usuwanie";
		//notTestedChanged = true;
	}else{
		qDebug() <<"blad steama";
	}
}







#include "Model.h"
#include <QTimer>
#include <QElapsedTimer>

Model::Model(GameRepository *gameRepository, SteamConnector *steamConnector)
	: gameRepo(gameRepository), steamConnector(steamConnector)
{
	QObject::connect(steamConnector, &SteamConnector::allAppsReady, this, &Model::updateAllApps);
	QObject::connect(steamConnector, &SteamConnector::priceReceived, this, &Model::updateSubsGamePrice);
	QObject::connect(steamConnector, &SteamConnector::verifiedAsAGame, this, &Model::saveAppAsAGame);
	QObject::connect(steamConnector, &SteamConnector::verifiedAsNotAGame, this, &Model::deleteAppFromList);
}

void Model::startFetchingData()
{
	steamConnector->sendFetchAllAppsReq();
	testedGames = gameRepo->getTestedGames();
	subsGames = gameRepo->getSubsGames();
	checkAppsIfAreGames();
}

void Model::insertSubsGame(const QPair<int, double> &game)
{
	gameRepo->insertSubsGame(game);
	steamConnector->sendFetchGamePriceReq(game.first);
	subsChanged = true;
}

void Model::deleteSubsGame(const int gameid)
{
	gameRepo->deleteSubsGame(gameid);
	subsChanged = true;
}

void Model::gamesListRequested()
{
	if(testedChanged) {
		testedGames = gameRepo->getTestedGames();
		testedChanged = false;
	}
	emit testedGamesReady(testedGames); //jesli bedzie slaby performance to mozna dodac
		//jakas flage ktora bezdzie pokazywac czy od ostatniego czasu dane byly zmieniane
		//i czy okienko w widoku powinno wszystkie usuwac
}

void Model::subsListRequested()
{
	if(subsChanged) {
		subsGames = gameRepo->getSubsGames();
		subsChanged = false;
	}
	emit subsGamesReady(subsGames);
}

void Model::fetchAllAppsFromSteam()
{
	steamConnector->sendFetchAllAppsReq();
}

void Model::fetchPricesForSubs()
{
	if(subsChanged) {
		subsGames = gameRepo->getSubsGames();
		subsChanged = false;
	}
	for(auto &game: subsGames) {
		steamConnector->sendFetchGamePriceReq(game.id);
	}
	QTimer::singleShot(3600000, this, &Model::fetchPricesForSubs); // fetch every hour
}

void Model::checkAppsIfAreGames() // every 5 minutes check remaining apps if the are games or not (videos or whatever)
{
	qDebug() << "checking apps if are games";
	QVector<App> notTestedApps = gameRepo->getNotTestedApps();
	int steamLimit = notTestedApps.size() > 150 ? 150 : notTestedApps.size(); // 200 really, but let's take less than that
	for(int i = 0; i < steamLimit; i++) {
		steamConnector->sendCheckIfIsGameReq(notTestedApps[i].id);
	}
	QTimer::singleShot(310000, this, &Model::checkAppsIfAreGames);
}

void Model::updateAllApps(const QVector<App> &newApps) // called when all apps were received
{
	qDebug() << "all apps:" << newApps.size();
	static int tries = 0;
	if(tries++ < 5 && newApps.size() == 0) { // if nothing fethed from steam server, try 5 times max
		steamConnector->sendFetchAllAppsReq();
		return;
	} else {
		tries = 0;
	}
	gameRepo->many();
	for(auto &app: newApps) {
		if(!gameRepo->isKnownApp(app)) {
			gameRepo->insertApp(app);
			gameRepo->insertNotTestedApp(app.id);
		}
	}
	gameRepo->endMany();
}

void Model::updateSubsGamePrice(int appid, double price) // called when price was received
{
	//if(game->second != kod zwracany jesli steam przekroczy? limit zapyta?){} // zaimplemetowac
	gameRepo->updateSubsGamePrice(appid, price);
	subsChanged = true;
}

void Model::saveAppAsAGame(int appId)
{
	qDebug() << "new game arrived";
	gameRepo->insertTestedGame(appId);
	gameRepo->deleteNotTestedApp(appId);
	testedChanged = true;
}

void Model::deleteAppFromList(int appId)
{
	gameRepo->deleteNotTestedApp(appId);
}

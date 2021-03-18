#include "Model.h"
#include <QTimer>
#include <QElapsedTimer>

Model::Model(GameRepository *gameRepository, SteamConnector *steamConnector)
	: gameRepo(gameRepository), steamConnector(steamConnector)
{
}

void Model::startFetchingData()
{
	gameRepo->start();
	testedGames = gameRepo->getTestedGames();
	subsGames = gameRepo->getSubsGames();
	QObject::connect(steamConnector.get(), &SteamConnector::allAppsReady, this, &Model::updateAllApps);
	QObject::connect(steamConnector.get(), &SteamConnector::priceReceived, this, &Model::updateSubsGamePrice);
	QObject::connect(steamConnector.get(), &SteamConnector::ifGameChecked, this, &Model::updateIfIsGame);
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

void Model::checkAppsIfAreGames() // evey 5 minute check remaining apps if the are games or not (videos or whatever)
{
	QVector<App> notTestedApps = gameRepo->getNotTestedApps();
	int steamLimit = notTestedApps.size() > 150 ? 150 : notTestedApps.size(); // 200 really, but let's take a half
	for(int i = 0; i < steamLimit; i++) {
		steamConnector->sendCheckIfIsGameReq(notTestedApps[i].id);
	}
	QTimer::singleShot(310000, this, &Model::checkAppsIfAreGames);
}

void Model::updateAllApps(const QVector<App> &newApps) // called when all apps were received
{
	static int tries = 0;
	gameRepo->many();
	for(auto &app: newApps) {
		if(!gameRepo->isKnownApp(app)) {
			gameRepo->insertApp(app);
			gameRepo->insertNotTestedApp(app.id);
		}
	}
	gameRepo->endMany();
	if(tries++ < 5 && newApps.size() == 0) { // if nothing fethed from steam server, try 5 times max
		steamConnector->sendFetchAllAppsReq();
	} //allChanged = true;
	//notTestedChanged = true;
}

void Model::updateSubsGamePrice(const Game &game) // called when price was received
{
	//if(game->second != kod zwracany jesli steam przekroczy? limit zapyta?){} // zaimplemetowac
	if(game.limit != SteamConnector::Codes::SteamFailure) {
		qDebug() << "rozne, updatuj";
		gameRepo->updateSubsGamePrice(game);
		subsChanged = true;
	}
}

void Model::updateIfIsGame(QPair<int, int> *game) // called when game-check was received
{
	if(game->second == SteamConnector::Codes::Game) {
		gameRepo->insertTestedGame(game->first);
		gameRepo->deleteNotTestedApp(game->first);
		testedChanged = true;
	} else if(game->second == SteamConnector::Codes::NotGame) {
		gameRepo->deleteNotTestedApp(game->first);
	} else {
		qDebug() << "blad steama";
	}
}

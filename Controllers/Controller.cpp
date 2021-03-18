#include "Controller.h"
#include <QApplication>
#include <QCollator>
#include <QDesktopWidget>
#include <QSettings>

#include <QTimer>

#include <QMessageBox>
#include <QWidget>

Controller::Controller()
{
	netMan.reset(new QNetworkAccessManager());
	searcherView.reset(new SearcherView());
	model.reset(new Model(new GameRepository(), new SteamConnector(netMan)));
	QCoreApplication::setOrganizationName(ORG_NAME);
	QCoreApplication::setApplicationName(APP_NAME);
}

void Controller::checkConnectionAndStart()
{
	QNetworkReply *reply = netMan->get(QNetworkRequest(QUrl("http://www.google.com")));
	QObject::connect(reply, &QNetworkReply::finished, [=]() { this->connectionChecked(reply); });
}

void Controller::quitClicked()
{
	QApplication::quit();
}

void Controller::start()
{
	QObject::connect(searcherView.get(), &SearcherView::quitClicked, this, &Controller::quitClicked);

	QObject::connect(searcherView.get(), &SearcherView::addClicked, model.get(), &Model::gamesListRequested);
	QObject::connect(model.get(), &Model::testedGamesReady, searcherView.get(), &SearcherView::fillGamesList);

	QObject::connect(searcherView.get(), &SearcherView::showClicked, model.get(), &Model::subsListRequested);
	QObject::connect(model.get(), &Model::subsGamesReady, searcherView.get(), &SearcherView::fillSubsGamesList);

	QObject::connect(searcherView.get(), &SearcherView::subAddClicked, model.get(), &Model::insertSubsGame);
	QObject::connect(searcherView.get(), &SearcherView::subDelClicked, model.get(), &Model::deleteSubsGame);

	searcherView->start();
	model->startFetchingData();
}

void Controller::connectionChecked(QNetworkReply *reply)
{
	if(reply->error() == QNetworkReply::NoError) {
		qDebug() << "polaczenie z netem ok";
		start();
	} else {
		qDebug() << "brak polaczenia z internetem";
		noConnExitShowDialog();
	}
}

void Controller::noConnExitShowDialog()
{
	QMessageBox box;
	box.setText("Internet connection failed!");
	box.setDetailedText("App is using your internet connection"
						" to get data from steam servers"
						" and update the list of games with that data."
						" Internet connection must be set"
						" to get app to work properly."
						"Check your connection and run the app again");
	box.exec();
	quitClicked();
}

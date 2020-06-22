#include "Controller.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QCollator>
#include <QSettings>

#include <QTimer>

#include <QMessageBox>
#include <QWidget>


/*============================================================
 * PUBLIC
 * PUBLIC
 * PUBLIC
 *============================================================
 */
Controller::Controller(){
	netMan = new QNetworkAccessManager(this);
	searcherView = new SearcherView();
	model = new Model(netMan);
	QCoreApplication::setOrganizationName(ORG_NAME);
	QCoreApplication::setApplicationName(APP_NAME);
}

Controller::~Controller(){
	qDebug()<<"destruktor controller";
	delete searcherView;
	delete model;
}

void Controller::checkConnectionAndStart()
{
	QNetworkReply *reply = netMan->get(QNetworkRequest(QUrl("http://www.google.com")));
	QObject::connect(reply, &QNetworkReply::finished,
					 [=](){this->connectionChecked(reply);});
}

/*============================================================
 * PUBLIC SLOTS
 * PUBLIC SLOTS
 * PUBLIC SLOTS
 *============================================================
 */
void Controller::quitClicked()
{
	//model->quit();
	qDebug()<<"quitClicked";
	QApplication::quit();
}

/*=================================================================
 *PRIVATE
 *PRIVATE
 *PRIVATE
 *=================================================================
 */
void Controller::start()
{
	QObject::connect(searcherView, &SearcherView::quitClicked,
					 this, &Controller::quitClicked);

	QObject::connect(searcherView, &SearcherView::addClicked,
					 model, &Model::gamesListRequested);
	QObject::connect(model, &Model::testedGamesReady,
					 searcherView, &SearcherView::fillGamesList);

	QObject::connect(searcherView, &SearcherView::showClicked,
					 model, &Model::subsListRequested);
	QObject::connect(model, &Model::subsGamesReady,
					 searcherView, &SearcherView::fillSubsGamesList);

	QObject::connect(searcherView, &SearcherView::subAddClicked,
					 model, &Model::insertSubsGame);
	QObject::connect(searcherView, &SearcherView::subDelClicked,
					 model, &Model::deleteSubsGame);

	searcherView->start();
	model->startFetchingData();
}

/*=================================================================
 *PRIVATE SLOTS
 *PRIVATE SLOTS
 *PRIVATE SLOTS
 *=================================================================
 */
void Controller::connectionChecked(QNetworkReply* reply)
{
	if(reply->error() == QNetworkReply::NoError){
		qDebug() << "polaczenie ok";
		start();
	}else{
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




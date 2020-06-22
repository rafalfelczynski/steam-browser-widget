#ifndef SEARCHER_H
#define SEARCHER_H
#include <QObject>
#include <QString>
#include <SearcherView.h>
#include <SteamConnector.h>
#include <Triple.h>
#include <Tetrad.h>
#include <QHash>
#include <Model.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>

const static QString APP_NAME = "Steam Browser";
const static QString ORG_NAME = "widget-org";

class Controller: public QObject{
	Q_OBJECT
public:
	Controller();
	~Controller();
	void checkConnectionAndStart();
public slots:
	void quitClicked();

private:
	SearcherView *searcherView;
	Model *model;
	QNetworkAccessManager *netMan;
	void start();
private slots:
	void connectionChecked(QNetworkReply *reply);
	void noConnExitShowDialog();
};

#endif // SEARCHER_H

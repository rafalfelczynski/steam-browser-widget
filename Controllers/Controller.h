#ifndef SEARCHER_H
#define SEARCHER_H
#include <QObject>
#include <QString>
#include "Views/SearcherView.h"
#include "Services/SteamConnector.h"
#include <QHash>
#include "Models/Model.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QScopedPointer>

const static QString APP_NAME = "Steam Browser";
const static QString ORG_NAME = "widget-org";

class Controller : public QObject
{
	Q_OBJECT
public:
	Controller();
	~Controller();
	void checkConnectionAndStart();
public slots:
	void quitClicked();

private:
	QScopedPointer<SearcherView> searcherView;
	QScopedPointer<Model> model;
	QScopedPointer<QNetworkAccessManager> netMan;
	void start();
private slots:
	void connectionChecked(QNetworkReply *reply);
	void noConnExitShowDialog();
};

#endif // SEARCHER_H

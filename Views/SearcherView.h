#ifndef SEARCHERVIEW_H
#define SEARCHERVIEW_H
#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVector>
#include "Views/Popup.h"
#include <QList>
#include "Views/PopupList.h"
#include "Models/Game.h"

class SearcherView : public QObject
{
	Q_OBJECT
public:
	void start();

private:
	QMenu menu;
	QSystemTrayIcon icon;
	Popup popupWindow;
	PopupList popupListWindow;
	void createMenu();
public slots:
	void fillGamesList(const QVector<App> &testedGames);
	void fillSubsGamesList(const QVector<Game> &subsGames);
private slots:
	void iconClicked(QSystemTrayIcon::ActivationReason cl);
signals:
	void quitClicked();
	void addClicked();
	void showClicked();
	void subAddClicked(const QPair<int, double> &gameLimit);
	void subDelClicked(const int gameid);
};

#endif // SEARCHERVIEW_H

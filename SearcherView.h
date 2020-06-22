#ifndef SEARCHERVIEW_H
#define SEARCHERVIEW_H
#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVector>
#include <Popup.h>
#include <QList>
#include <Triple.h>
#include <Tetrad.h>
#include <PopupList.h>

class SearcherView : public QObject{
	Q_OBJECT
public:
	explicit SearcherView();
	~SearcherView();
	void start();
	//void refreshGames(QList<QPair<QString, int>> gamesNames);
	//void refreshSubs(QList<Triple<QString, double, double>> subs);
private:
	QMenu *menu;
	QSystemTrayIcon *icon;
	Popup *popupWindow;
	PopupList *popupListWindow;
	void createMenu();
public slots:
	void fillGamesList(QList<QPair<int,QString>> *testedGames);
	void fillSubsGamesList(QList<Tetrad<int,double,double,QString>> *subsGames);
	//void showList(QList<Triple<QString, double, double>> list);
private slots:
	void iconClicked(QSystemTrayIcon::ActivationReason cl);
signals:
	void quitClicked();
	void addClicked();
	void showClicked();
	void subAddClicked(const QPair<int,double> &gameLimit);
	void subDelClicked(const int gameid);

};

#endif // SEARCHERVIEW_H

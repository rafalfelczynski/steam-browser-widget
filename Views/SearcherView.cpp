#include "SearcherView.h"
#include <QAction>
#include <iostream>
#include <QDebug>

void SearcherView::start()
{
	createMenu();
	icon.setIcon(QIcon(":/img/images/heart.png"));
	icon.show();
}

void SearcherView::createMenu()
{
	QAction *action = new QAction("Show my subs", &menu);
	menu.addAction(action);
	QObject::connect(action, &QAction::triggered, this, &SearcherView::showClicked);

	action = new QAction("Add sub", &menu);
	menu.addAction(action);
	QObject::connect(action, &QAction::triggered, this, &SearcherView::addClicked);

	action = new QAction("Quit", &menu);
	menu.addAction(action);
	QObject::connect(action, &QAction::triggered, this, &SearcherView::quitClicked);

	QObject::connect(&popupWindow, &Popup::subAddClicked, this, &SearcherView::subAddClicked);
	QObject::connect(&popupListWindow, &PopupList::subDelClicked, this, &SearcherView::subDelClicked);

	QObject::connect(&icon, &QSystemTrayIcon::activated, this, &SearcherView::iconClicked);
	menu.show(); // only to calculate width and height on startup
	menu.hide(); //
}

void SearcherView::fillGamesList(const QVector<App> &testedGames)
{
	popupWindow.refreshGames(testedGames);
	popupWindow.show();
}

void SearcherView::fillSubsGamesList(const QVector<Game> &subsGames)
{
	qDebug() << "fill";
	popupListWindow.refreshSubsGames(subsGames);
	popupListWindow.show();
}

void SearcherView::iconClicked(QSystemTrayIcon::ActivationReason r)
{
	if(r == QSystemTrayIcon::Trigger) {
		QRect rect = icon.geometry();
		menu.move(rect.x() + rect.width() / 2,
				  (rect.y() + rect.height() / 2) - menu.height());
		menu.show();
	}
}

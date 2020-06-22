#include "SearcherView.h"
#include <QAction>
#include <iostream>


SearcherView::SearcherView()
{
	icon = new QSystemTrayIcon(this);
	menu = new QMenu();
	popupWindow = new Popup();
	popupListWindow = new PopupList();
}

SearcherView::~SearcherView()
{
	popupWindow->hide();
	popupListWindow->hide();
	icon->hide();
	delete menu;
	delete popupWindow;
	delete popupListWindow;

}

void SearcherView::start()
{
	createMenu();
	icon->setIcon(QIcon(":/res/images/heart.png"));
	icon->show();
}

/*=================================================================
 *PRIVATE
 *PRIVATE
 *PRIVATE
 *=================================================================
 */
void SearcherView::createMenu()
{
	QAction *action = new QAction("Show my subs", menu);
	menu->addAction(action);
	QObject::connect(action, &QAction::triggered, this, &SearcherView::showClicked);

	action = new QAction("Add sub", menu);
	menu->addAction(action);
	QObject::connect(action, &QAction::triggered, this, &SearcherView::addClicked);

	action = new QAction("Quit", menu);
	menu->addAction(action);
	QObject::connect(action, &QAction::triggered, this, &SearcherView::quitClicked);

	QObject::connect(popupWindow, &Popup::subAddClicked,
					 this, &SearcherView::subAddClicked);
	QObject::connect(popupListWindow, &PopupList::subDelClicked,
					 this, &SearcherView::subDelClicked);

	QObject::connect(icon, &QSystemTrayIcon::activated,
					 this, &SearcherView::iconClicked);
	menu->show(); // only to calculate width and height on startup
	menu->hide(); //
}

/*=================================================================
 *PUBLIC SLOTS
 *PUBLIC SLOTS
 *PUBLIC SLOTS
 *=================================================================
 */
void SearcherView::fillGamesList(QList<QPair<int, QString> >* testedGames)
{
	popupWindow->refreshGames(testedGames);
	popupWindow->show();
}

void SearcherView::fillSubsGamesList(QList<Tetrad<int, double, double, QString>> *subsGames)
{
	popupListWindow->refreshSubsGames(subsGames);
	popupListWindow->show();
}

/*=================================================================
 *PRIVATE SLOTS
 *PRIVATE SLOTS
 *PRIVATE SLOTS
 *=================================================================
 */
void SearcherView::iconClicked(QSystemTrayIcon::ActivationReason r)
{
	if (r == QSystemTrayIcon::Trigger){
		QRect rect = icon->geometry();
		menu->move(rect.x()+rect.width()/2,
				   (rect.y() + rect.height()/2) - menu->height());
		menu->show();
	}
}


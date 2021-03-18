#include "PopupList.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QPair>
#include <iostream>
#include <chrono>
#include <QTimer>
#include <QFont>
#include <QEvent>
#include <QDebug>

PopupList::PopupList(QWidget *parent)
	: QWidget(parent)
{
	subsList.setParent(this);
	delBtn.setParent(this);
	priceField.setParent(this);
	limitField.setParent(this);
	priceLabel.setParent(this);
	limitLabel.setParent(this);
	delBtn.setParent(this);
	delBtn.setText("DEL");
	wasCreated = false;
	//createLayout();
}

PopupList::~PopupList()
{
}

void PopupList::show()
{
	if(!wasCreated) {
		createLayout();
	}
	setAppPrice();
	this->QWidget::show();
}

void PopupList::refreshSubsGames(const QVector<Game> &subsGames)
{
	subsList.clear();
	if(subsGames.size() == 0) {
		subsList.addItem("No subscribed games");
		subsList.setEnabled(false);
		priceField.setText("");
		limitField.setText("");
		delBtn.setEnabled(false);
	} else {
		for(auto &sub: subsGames) {
			subsList.addItem(sub.name, QVariant::fromValue(sub));
		}
		delBtn.setEnabled(true);
		subsList.setEnabled(true);
	}
}

void PopupList::changeEvent(QEvent *event)
{
	if(!this->isActiveWindow()) {
		this->hide();
	}
}

void PopupList::createLayout()
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	QPair<int, int> *winPos = determineTaskbarGeom();
	this->setGeometry(winPos->first, winPos->second, WINDOW_WIDTH2, WINDOW_HEIGHT2);
	subsList.setGeometry(MARGIN_WIDTH2 * WINDOW_WIDTH2,
						 MARGIN_HEIGHT2 * WINDOW_HEIGHT2,
						 (1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.55,
						 (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2);
	priceLabel.setGeometry(subsList.x() + subsList.width() + MARGIN_WIDTH2 * WINDOW_WIDTH2,
						   subsList.y(),
						   (1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.15,
						   (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 * 0.4);
	priceField.setGeometry(priceLabel.x(),
						   priceLabel.y() + priceLabel.height(),
						   priceLabel.width(),
						   (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 - priceLabel.height());
	limitLabel.setGeometry(priceLabel.x() + priceLabel.width(),
						   priceLabel.y(),
						   (1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.15,
						   (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 * 0.4);
	limitField.setGeometry(limitLabel.x(),
						   limitLabel.y() + limitLabel.height(),
						   limitLabel.width(),
						   (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 - limitLabel.height());
	delBtn.setGeometry(limitField.x() + limitLabel.width() + MARGIN_WIDTH2 * WINDOW_WIDTH2,
					   limitLabel.y(),
					   (1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.1,
					   (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2);
	QObject::connect(&delBtn, &QPushButton::pressed, this, &PopupList::delClicked);
	QFont font = QFont("Times", 13);
	subsList.setFont(font);
	limitLabel.setFont(font);
	limitField.setFont(font);
	limitField.setEnabled(false);
	priceLabel.setFont(font);
	priceField.setFont(font);
	priceField.setEnabled(false);
	limitLabel.setText("limit");
	priceLabel.setText("price");
	QObject::connect(&subsList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PopupList::setAppPrice);
	wasCreated = true;
}

void PopupList::delClicked()
{
	this->hide();
	int appid = subsList.itemData(subsList.currentIndex()).value<Game>().id;
	emit subDelClicked(appid);
}

void PopupList::setAppPrice()
{
	qDebug() << "index changed, setting price. popup list";
	if(subsList.currentIndex() < 0) {
		subsList.setCurrentIndex(0);
	} else {
		int index = subsList.currentIndex();
		auto game = subsList.itemData(index).value<Game>();
		priceField.setText(game.price > 0 ? QString::number(game.price) : "FREE");
		limitField.setText(QString::number(game.limit));
	}
}

QPair<int, int> *PopupList::determineTaskbarGeom()
{
	QDesktopWidget *desktop = QApplication::desktop();
	QRect all = desktop->screenGeometry();
	QRect avail = desktop->availableGeometry();
	QPair<int, int> *result = new QPair<int, int>();
	if(avail.x() > 0) { // left
		result->first = avail.x();
		result->second = avail.height() - WINDOW_HEIGHT2;
	} else if(avail.y() > 0) { // top
		result->first = avail.width() - WINDOW_WIDTH2;
		result->second = avail.y();
	} else if(avail.width() < all.width()) { // right
		result->first = avail.width() - WINDOW_WIDTH2;
		result->second = avail.height() - WINDOW_HEIGHT2;
	} else { // bottom
		result->first = avail.width() - WINDOW_WIDTH2;
		result->second = avail.height() - WINDOW_HEIGHT2;
	}
	return result;
}

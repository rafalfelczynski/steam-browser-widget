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

PopupList::PopupList(QWidget *parent) : QWidget(parent)
{
	subsList = new QComboBox(this);
	delBtn = new QPushButton("DEL", this);
	priceField = new QTextEdit(this);
	limitField = new QTextEdit(this);
	priceLabel = new QLabel(this);
	limitLabel = new QLabel(this);
	wasCreated = false;
	//createLayout();
}

PopupList::~PopupList()
{

}

void PopupList::show()
{
	if(!wasCreated){
		createLayout();
	}
	setAppPrice();
	this->QWidget::show();
}

void PopupList::refreshSubsGames(QList<Tetrad<int, double, double, QString>>* subsGames)
{
	subsList->clear();
	if(subsGames->size() == 0){
		subsList->addItem("No subscribed games");
		subsList->setEnabled(false);
		priceField->setText("");
		limitField->setText("");
		delBtn->setEnabled(false);
	}else{
		for(auto &sub : *subsGames){
			subsList->addItem(sub.fourth,
							  QList<QVariant>{sub.first, sub.second, sub.third});
			qDebug() <<"refresh subs games popup" <<sub.first<<sub.second<<sub.third<<sub.fourth;
		}
		delBtn->setEnabled(true);
		subsList->setEnabled(true);
	}
}

void PopupList::changeEvent(QEvent *event)
{
	if(!this->isActiveWindow()){
		this->hide();
	}else{
		std::cout << event->type() << std::endl;
	}
}

void PopupList::createLayout()
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	QPair<int, int> *winPos = determineTaskbarGeom();
	this->setGeometry(winPos->first, winPos->second, WINDOW_WIDTH2, WINDOW_HEIGHT2);
	subsList->setGeometry(MARGIN_WIDTH2 * WINDOW_WIDTH2,
						   MARGIN_HEIGHT2 * WINDOW_HEIGHT2,
						   (1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.55,
						   (1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2);
	priceLabel->setGeometry(subsList->x() + subsList->width() + MARGIN_WIDTH2*WINDOW_WIDTH2,
							subsList->y(),
							(1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.15,
							(1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 * 0.4);
	priceField->setGeometry(priceLabel->x(),
							priceLabel->y() + priceLabel->height(),
							priceLabel->width(),
							(1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 - priceLabel->height());
	limitLabel->setGeometry(priceLabel->x() + priceLabel->width(),
							priceLabel->y(),
							(1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.15,
							(1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2* 0.4);
	limitField->setGeometry(limitLabel->x(),
							limitLabel->y() + limitLabel->height(),
							limitLabel->width(),
							(1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2 - limitLabel->height());
	delBtn->setGeometry(limitField->x() + limitLabel->width() + MARGIN_WIDTH2 * WINDOW_WIDTH2,
						limitLabel->y(),
						(1 - 2 * MARGIN_WIDTH2) * WINDOW_WIDTH2 * 0.1,
						(1 - 2 * MARGIN_HEIGHT2) * WINDOW_HEIGHT2);
	QObject::connect(delBtn, &QPushButton::pressed, this, &PopupList::delClicked);
	QFont font = QFont("Times", 13);
	subsList->setFont(font);
	limitLabel->setFont(font);
	limitField->setFont(font);
	limitField->setEnabled(false);
	priceLabel->setFont(font);
	priceField->setFont(font);
	priceField->setEnabled(false);
	limitLabel->setText("limit");
	priceLabel->setText("price");
	QObject::connect(subsList, QOverload<int>::of(&QComboBox::currentIndexChanged),
					 this, &PopupList::setAppPrice);
	wasCreated = true;
}

void PopupList::delClicked()
{
	this->hide();
	int appid = subsList->itemData(subsList->currentIndex()).toList()[0].toInt();
	emit subDelClicked(appid);

}

void PopupList::setAppPrice()
{
	qDebug() << "index changed, setting price. popup list";
	if(subsList->currentIndex() < 0){
		subsList->setCurrentIndex(0);
	}else{
		int index = subsList->currentIndex();
		QVariantList li = subsList->itemData(index).toList();
		double pri = li[1].toDouble();
		priceField->setText(pri > 0 ? QString::number(pri) : "FREE");
		limitField->setText(QString::number(li[2].toDouble()));

		qDebug() << pri << li[2];
	}
}

QPair<int, int>* PopupList::determineTaskbarGeom(){
	QDesktopWidget *desktop = QApplication::desktop();
	QRect all = desktop->screenGeometry();
	QRect avail = desktop->availableGeometry();
	QPair<int, int> *result = new QPair<int, int>();
	if(avail.x() > 0){ // left
		result->first = avail.x();
		result->second = avail.height()- WINDOW_HEIGHT2;
	}else if(avail.y() > 0){ // top
		result->first = avail.width() - WINDOW_WIDTH2;
		result->second = avail.y();
	}else if(avail.width() < all.width()){ // right
		result->first = avail.width() - WINDOW_WIDTH2;
		result->second = avail.height()- WINDOW_HEIGHT2;
	}else{ // bottom
		result->first = avail.width() - WINDOW_WIDTH2;
		result->second = avail.height()- WINDOW_HEIGHT2;
	}
	return result;
}


#include "Popup.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QPair>
#include <QTimer>
#include <QFont>
#include <QEvent>
#include <QRegExp>

Popup::Popup(QWidget *parent) : QWidget(parent)
{
	gamesList = new QComboBox(this);
	nameField = new QComboBox(this);
	priceField = new QTextEdit(this);
	okBtn = new QPushButton("OK", this);
	createLayout();
}

Popup::~Popup()
{

}

void Popup::show()
{
	this->QWidget::show();
}

void Popup::refreshGames(QList<QPair<int, QString>>* gamesNames)
{
	divideApps(gamesNames);
	setAppsSubset();
}

void Popup::changeEvent(QEvent *event)
{
	if(!this->isActiveWindow()){
		this->hide();
	}
}

void Popup::createLayout()
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	QPair<int, int> *winPos = determineTaskbarGeom();
	this->setGeometry(winPos->first, winPos->second, WINDOW_WIDTH, WINDOW_HEIGHT);
	nameField->setGeometry(MARGIN_WIDTH * WINDOW_WIDTH,
						   MARGIN_HEIGHT * WINDOW_HEIGHT,
						   (1 - 3 * MARGIN_WIDTH) * WINDOW_WIDTH * 0.8,
						   WINDOW_HEIGHT * 0.425);
	gamesList->setGeometry(MARGIN_WIDTH * WINDOW_WIDTH,
						   nameField->y() + nameField->height() + MARGIN_HEIGHT * WINDOW_HEIGHT,
						   nameField->width(),
						   nameField->height());
	priceField->setGeometry(gamesList->x() + gamesList->width() + MARGIN_WIDTH * WINDOW_WIDTH,
							MARGIN_HEIGHT * WINDOW_HEIGHT,
							(1 - 3 * MARGIN_WIDTH) * WINDOW_WIDTH * 0.2,
							WINDOW_HEIGHT * 0.425);
	okBtn->setGeometry(priceField->x(),
					   priceField->y() + priceField->height() + MARGIN_HEIGHT * WINDOW_HEIGHT,
					   priceField->width(),
					   priceField->height());
	QObject::connect(okBtn, &QPushButton::pressed, this, &Popup::okClicked);
	QFont font = QFont("Times", 13);
	nameField->setFont(font);
	priceField->setAlignment(Qt::AlignLeft);
	priceField->setFont(font);
	priceField->setPlaceholderText("Price");
	addLetters();
	nameField->setCurrentIndex(0);
	QObject::connect(nameField, QOverload<int>::of(&QComboBox::currentIndexChanged),
					 this, &Popup::setAppsSubset);
}

void Popup::addLetters()
{
	nameField->clear();
	for(char i=65;i<=90;i++){
		QChar ch(i);
		nameField->addItem(ch);
	}
}

void Popup::divideApps(QList<QPair<int,QString>>* gamesNames)
{
	for(int i=0;i<26;i++){
		bins[i].clear();
	}
	for(const auto &pair : *gamesNames){
		QChar ch = pair.second.at(0);
		if(65 <= ch && ch <= 90){
			bins[ch.unicode()-65].append(pair);
		}else if(97 <= ch && ch <= 122){
			bins[ch.unicode()-97].append(pair);
		}
	}

}

void Popup::setAppsSubset()
{
	if(nameField->currentIndex() >= 0){
		gamesList->clear();
		QChar ch = nameField->itemText(nameField->currentIndex()).at(0);
		// games are sorted
		QList<QPair<int,QString>> *list = nullptr;
		if(65 <= ch && ch <= 90){
			list = &bins[ch.unicode()-65];
		}else if(97 <= ch && ch <= 122){
			list = &bins[ch.unicode()-97];
		}
		if (list != nullptr){
			for(auto &pair : *list){
				gamesList->addItem(pair.second, pair.first);
			}
		}
	}
}

void Popup::okClicked(){
	QRegExp regex("[0-9]+");
	QString s = priceField->toPlainText();
	if(s != "" && regex.exactMatch(s)){
		this->hide();
		const QVariant &appid = gamesList->itemData(gamesList->currentIndex());
		emit subAddClicked({appid.toInt(), priceField->toPlainText().toDouble()});
	}else{
		QTimer tim;
		QTimer::singleShot(10, this,
							[&](){priceField
							->setStyleSheet("background-color: red");});
		QTimer::singleShot(1000, this,
							[&](){priceField
							->setStyleSheet("background-color: white");});
	}
}

QPair<int, int>* Popup::determineTaskbarGeom(){
	QDesktopWidget *desktop = QApplication::desktop();
	QRect all = desktop->screenGeometry();
	QRect avail = desktop->availableGeometry();
	QPair<int, int> *result = new QPair<int, int>();
	if(avail.x() > 0){ // left
		result->first = avail.x();
		result->second = avail.height()- WINDOW_HEIGHT;
	}else if(avail.y() > 0){ // top
		result->first = avail.width() - WINDOW_WIDTH;
		result->second = avail.y();
	}else if(avail.width() < all.width()){ // right
		result->first = avail.width() - WINDOW_WIDTH;
		result->second = avail.height()- WINDOW_HEIGHT;
	}else{ // bottom
		result->first = avail.width() - WINDOW_WIDTH;
		result->second = avail.height()- WINDOW_HEIGHT;
	}
	return result;
}



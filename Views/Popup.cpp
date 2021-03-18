#include "Popup.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QPair>
#include <QTimer>
#include <QFont>
#include <QEvent>
#include <QRegExp>
#include <QScreen>
#include <QDebug>
#include <QFile>
#include <QStyledItemDelegate>
#include <QPalette>
#include <QAbstractItemView>
#include <QLabel>

Popup::Popup(QWidget *parent)
	: QWidget(parent)
{
	okBtn.setText("ADD");
	wasCreated = false;
}

void Popup::show()
{
	if(!wasCreated) {
		createLayout();
	}
	setAppsSubset();
	this->QWidget::show();
}

void Popup::refreshGames(const QVector<App> &gamesNames)
{
	divideApps(gamesNames);
}

void Popup::changeEvent(QEvent *event)
{
	if(!this->isActiveWindow()) {
		this->hide();
	}
}

void Popup::createLayout()
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	QPair<int, int> *winPos = determineTaskbarGeom();
	this->setGeometry(winPos->first, winPos->second, WINDOW_WIDTH, WINDOW_HEIGHT);
	infoLbl.setGeometry(MARGIN_WIDTH * WINDOW_WIDTH,
						MARGIN_HEIGHT * WINDOW_HEIGHT,
						(1 - 2 * MARGIN_WIDTH) * WINDOW_WIDTH * 0.50,
						WINDOW_HEIGHT * 0.425);
	letterBox.setGeometry(infoLbl.x() + infoLbl.width() + MARGIN_WIDTH * WINDOW_WIDTH,
						  infoLbl.y(),
						  (1 - 2 * MARGIN_WIDTH) * WINDOW_WIDTH * 0.2,
						  infoLbl.height());
	gamesList.setGeometry(infoLbl.x(),
						  letterBox.y() + letterBox.height() + MARGIN_HEIGHT * WINDOW_HEIGHT,
						  letterBox.width() + infoLbl.width() + MARGIN_WIDTH * WINDOW_WIDTH,
						  letterBox.height());
	priceField.setGeometry(gamesList.x() + gamesList.width() + MARGIN_WIDTH * WINDOW_WIDTH,
						   MARGIN_HEIGHT * WINDOW_HEIGHT,
						   (1 - 2 * MARGIN_WIDTH) * WINDOW_WIDTH * 0.2,
						   WINDOW_HEIGHT * 0.425);
	okBtn.setGeometry(priceField.x(),
					  priceField.y() + priceField.height() + MARGIN_HEIGHT * WINDOW_HEIGHT,
					  priceField.width(),
					  priceField.height());
	this->setObjectName("parent");
	letterBox.setObjectName("letters");
	letterBox.view()->setObjectName("lettersView");
	gamesList.setObjectName("games");
	gamesList.view()->setObjectName("gamesView");
	priceField.setObjectName("priceField");
	okBtn.setObjectName("okBtn");
	infoLbl.setObjectName("infoLbl");
	QFile style(":/styles/gamesListStyle.qss");
	style.open(QFile::ReadOnly);
	this->setStyleSheet(style.readAll());
	style.close();
	QObject::connect(&okBtn, &QPushButton::pressed, this, &Popup::okClicked);
	infoLbl.setWordWrap(true);
	infoLbl.setText("Choose a letter to see related games");
	priceField.setPlaceholderText("Price");
	addLetters();
	letterBox.setCurrentIndex(0);
	QObject::connect(&letterBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Popup::setAppsSubset);
	wasCreated = true;
}

void Popup::addLetters()
{
	letterBox.clear();
	for(char i = 65; i <= 90; i++) {
		QChar ch(i);
		letterBox.addItem(ch);
	}
	//nameField->setCurrentIndex(-1);
}

void Popup::divideApps(const QVector<App> &gamesNames)
{
	for(int i = 0; i < 26; i++) {
		bins[i].clear();
	}
	for(const auto &game: gamesNames) {
		QChar ch = game.name.at(0);
		if(65 <= ch && ch <= 90) {
			bins[ch.unicode() - 65] << game;
		} else if(97 <= ch && ch <= 122) {
			bins[ch.unicode() - 97] << game;
		}
	}
}

void Popup::setAppsSubset()
{
	if(letterBox.currentIndex() < 0) {
		letterBox.setCurrentIndex(0);
	} else {
		gamesList.clear();
		QChar ch = letterBox.itemText(letterBox.currentIndex()).at(0);
		// games are sorted
		QVector<App> apps;
		if(65 <= ch && ch <= 90) {
			apps = bins[ch.unicode() - 65];
		} else if(97 <= ch && ch <= 122) {
			apps = bins[ch.unicode() - 97];
		}
		for(auto &app: apps) {
			gamesList.addItem(app.name, app.id);
		}
	}
}

QPair<int, int> *Popup::determineTaskbarGeom()
{
	QScreen *desktop = QGuiApplication::screens()[0];
	QRect all = desktop->geometry();
	QRect avail = desktop->availableGeometry();
	QPair<int, int> *result = new QPair<int, int>();
	if(avail.x() > 0) { // left
		result->first = avail.x();
		result->second = avail.height() - WINDOW_HEIGHT;
	} else if(avail.y() > 0) { // top
		result->first = avail.width() - WINDOW_WIDTH;
		result->second = avail.y();
	} else if(avail.width() < all.width()) { // right
		result->first = avail.width() - WINDOW_WIDTH;
		result->second = avail.height() - WINDOW_HEIGHT;
	} else { // bottom
		result->first = avail.width() - WINDOW_WIDTH;
		result->second = avail.height() - WINDOW_HEIGHT;
	}
	return result;
}

QString Popup::priceStyleSheetChangeColor(const QString &color)
{
	return "QTextEdit#priceField {"
		   " color: white;"
		   " border-radius: 10px;"
		   " background-color: "
		   + color + ";"
					 " border: 2px solid white;"
					 " font: bold italic \"Times New Roman\";"
					 " font-size: 15px;}";
}

void Popup::okClicked()
{
	QRegExp regex("[0-9]+");
	QString s = priceField.toPlainText();
	if(s != "" && regex.exactMatch(s)) {
		this->hide();
		const QVariant &appid = gamesList.itemData(gamesList.currentIndex());
		emit subAddClicked({appid.toInt(), priceField.toPlainText().toDouble()});
	} else {
		QTimer tim;
		QTimer::singleShot(10, this, [&]() { priceField
												 .setStyleSheet(priceStyleSheetChangeColor("red")); });
		QTimer::singleShot(1000, this, [&]() { priceField
												   .setStyleSheet(priceStyleSheetChangeColor("#111111")); });
	}
}

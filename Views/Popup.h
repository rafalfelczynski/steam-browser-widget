#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QList>
#include <QThread>
#include <QLabel>
#include "Models/Game.h"

class Popup : public QWidget
{
	Q_OBJECT
	static constexpr float WINDOW_WIDTH = 350.0f;
	static constexpr float WINDOW_HEIGHT = 100.0f;
	static constexpr float MARGIN_WIDTH = 0.05f;
	static constexpr float MARGIN_HEIGHT = 0.05f;

public:
	explicit Popup(QWidget *parent = nullptr);
	void show();
	void refreshGames(const QVector<App> &gamesNames);
	void changeEvent(QEvent *event) override;

private:
	bool wasCreated;
	QLabel infoLbl;
	QPushButton okBtn;
	QComboBox letterBox;
	QTextEdit priceField;
	QComboBox gamesList;
	QVector<App> bins[26];
	void createLayout();
	void addLetters();
	void divideApps(const QVector<App> &gamesNames);
	void setAppsSubset();
	QPair<int, int> *determineTaskbarGeom();
	QString priceStyleSheetChangeColor(const QString &color);
private slots:
	void okClicked();
signals:
	void subAddClicked(const QPair<int, double> &limit);
};

#endif // POPUP_H

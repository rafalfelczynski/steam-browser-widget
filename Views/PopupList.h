#ifndef POPUPLIST_H
#define POPUPLIST_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include "Models/Game.h"

class PopupList : public QWidget
{
	Q_OBJECT
	static constexpr float WINDOW_WIDTH2 = 500.0f;
	static constexpr float WINDOW_HEIGHT2 = 60.0f;
	static constexpr float MARGIN_WIDTH2 = 0.01f;
	static constexpr float MARGIN_HEIGHT2 = 0.02f;

public:
	explicit PopupList(QWidget *parent = nullptr);
	~PopupList();
	void show();
	void refreshSubsGames(const QVector<Game> &subsGames);
	void changeEvent(QEvent *event) override;

private:
	bool wasCreated;
	QVector<Game> subsGames;
	QComboBox subsList;
	QPushButton delBtn;
	QTextEdit priceField;
	QTextEdit limitField;
	QLabel priceLabel;
	QLabel limitLabel;
	void createLayout();
	QPair<int, int> *determineTaskbarGeom();
private slots:
	void delClicked();
	void setAppPrice();

signals:
	void subDelClicked(const int game);
};

#endif // POPUPLIST_H

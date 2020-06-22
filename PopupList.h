#ifndef POPUPLIST_H
#define POPUPLIST_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <Triple.h>
#include <QLabel>
#include <Tetrad.h>

const float WINDOW_WIDTH2 = 500;
const float WINDOW_HEIGHT2 = 60;
const float MARGIN_WIDTH2 = 0.01;
const float MARGIN_HEIGHT2 = 0.02;

class PopupList : public QWidget
{
	Q_OBJECT
public:
	explicit PopupList(QWidget *parent = nullptr);
	~PopupList();
	void show();
	void refreshSubsGames(QList<Tetrad<int, double, double, QString>> *subsGames);
	void changeEvent(QEvent *event) override;
private:
	QList<Tetrad<int, double, double, QString>> *subsGames;
	QComboBox *subsList;
	QPushButton *delBtn;
	QTextEdit *priceField;
	QTextEdit *limitField;
	QLabel *priceLabel;
	QLabel *limitLabel;
	void createLayout();
	QPair<int, int>* determineTaskbarGeom();
private slots:
	void delClicked();
	void setAppPrice();

signals:
	void subDelClicked(const int game);
};

#endif // POPUPLIST_H

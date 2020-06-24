#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QList>
#include <QThread>
#include <Triple.h>
#include <QLabel>

const float WINDOW_WIDTH = 350;
const float WINDOW_HEIGHT = 100;
const float MARGIN_WIDTH = 0.05;
const float MARGIN_HEIGHT = 0.05;

class Popup : public QWidget
{
	Q_OBJECT
public:
	explicit Popup(QWidget *parent = nullptr);
	~Popup();
	void show();
	void refreshGames(QList<QPair<int,QString>>* gamesNames);
	void changeEvent(QEvent *event) override;
private:
	bool wasCreated;
	QLabel *infoLbl;
	QPushButton *okBtn;
	QComboBox *letterBox;
	QTextEdit *priceField;
	QComboBox *gamesList;
	QList<QPair<int, QString>> bins[26];
	void createLayout();
	void addLetters();
	void divideApps(QList<QPair<int,QString>>* gamesNames);
	void setAppsSubset();
	QPair<int, int>* determineTaskbarGeom();
	QString priceStyleSheetChangeColor(const QString &color);
private slots:
	void okClicked();
signals:
	void subAddClicked(const QPair<int,double> &limit);

};

#endif // POPUP_H

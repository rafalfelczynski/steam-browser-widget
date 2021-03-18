#pragma once

#include <QString>
#include <QObject>

struct App
{
	int id;
	QString name;
};
Q_DECLARE_METATYPE(App);

struct Game : public App
{
	double price;
	double limit;
};
Q_DECLARE_METATYPE(Game);

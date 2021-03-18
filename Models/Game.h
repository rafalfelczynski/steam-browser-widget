#pragma once

#include <QString>

struct App
{
	int id;
	QString name;
};

struct Game : public App
{
	double price;
	double limit;
};

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QJsonDocument>
#include <QVector>
#include <QPair>
#include "Models/Game.h"
#include <optional>

class JsonParser : public QObject
{
	Q_OBJECT
public:
	enum Codes
	{
		Game = 0,
		NotGame = -1,
		SteamFailure = -10
	};
	explicit JsonParser(QObject *parent = nullptr);
	QVector<App> getAppsDataFromJson(const QByteArray &obj, int limit);
	std::optional<double> getAppPrice(const QByteArray &obj, const QString &appid);
	QPair<int, int> checkIfIsGame(const QByteArray &res, const QString &appid);
};

#endif // JSONPARSER_H

#pragma once

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

class JsonParser
{
public:
	enum Codes
	{
		Game = 0,
		NotGame = -1,
		SteamFailure = -10
	};
	static QVector<App> getAppsDataFromJson(const QByteArray &obj, int limit);
	static std::optional<double> getAppPrice(const QByteArray &obj, const QString &appid);
	static Codes checkIfIsGame(const QByteArray &res, const QString &appid);
};

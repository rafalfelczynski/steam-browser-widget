#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QJsonDocument>
#include <QList>
#include <QPair>
#include <Triple.h>

static const QString APPS_FILE_NEM = "all_apps.json";
static const QString MY_APPS_FILE_NEM = "my_apps.json";
static const QString NOT_CHECKED_APPS_NAME = "not_checked.json";
static const QString CHECKED_APPS_NAME = "checked.json";

class JsonParser : public QObject
{
	Q_OBJECT
public:
	explicit JsonParser(QObject *parent = nullptr);
	~JsonParser();
	QList<QPair<int,QString>> *getAppsDataFromJson(const QByteArray &obj, int limit);
	QPair<int, double> *getAppPrice(const QByteArray &obj, const QString &appid);
	QPair<int,int> *checkIfIsGame(const QByteArray &res, const QString &appid);

};

#endif // JSONPARSER_H

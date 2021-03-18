#include <QApplication>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include "Controllers/Controller.h"
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Controller searcher;
	searcher.checkConnectionAndStart();
	app.setQuitOnLastWindowClosed(false);
	return app.exec();
}

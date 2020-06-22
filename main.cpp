#include <QApplication>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <Controller.h>
#include <QNetworkAccessManager>



int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	//sprawdzic polaczenie z netem!!!!!!!!!!!!!!!
	Controller searcher;
	searcher.checkConnectionAndStart();
	app.setQuitOnLastWindowClosed(false);
	return app.exec();
}

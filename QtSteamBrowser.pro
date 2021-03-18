QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++latest

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	Controllers/Controller.cpp \
	Models/Game.cpp \
	Models/Model.cpp \
	Persistence/GameRepository.cpp \
	Services/JsonParser.cpp \
	Services/SteamConnector.cpp \
	Views/Popup.cpp \
	Views/PopupList.cpp \
	Views/SearcherView.cpp \
	Views/Window.cpp \
	main.cpp

HEADERS += \
	Controllers/Controller.h \
	Models/Game.h \
	Models/Model.h \
	Persistence/GameRepository.h \
	Services/JsonParser.h \
	Services/SteamConnector.h \
	Views/Popup.h \
	Views/PopupList.h \
	Views/SearcherView.h \
	Views/Window.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	gamesListStyle.qss \
	iconengines/qsvgicon.dll \
	images/heart.png

RESOURCES += \
	res.qrc

FORMS +=

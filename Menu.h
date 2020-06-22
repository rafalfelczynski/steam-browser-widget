#ifndef MENU_H
#define MENU_H

#include <QMenu>

class Menu : public QMenu
{
public:
	Menu();
	~Menu(){};
	void show();
};

#endif // MENU_H

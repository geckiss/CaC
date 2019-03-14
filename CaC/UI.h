#pragma once
#include "Company.h"
#include "../CaC/structures/heap_monitor.h"

class UI
{
private:
	Company * cac_;
	Date* date_;

	string pomVstup;
	int iOrderType = 0;
	int i = 0;
	/// Na ukoncenie programu
	bool finish;
public:
	UI();
	UI(Company* cac, Date* date);
	~UI();

	void hi();
	void mainScreen();
	void addUI();
	void printUI();
	void saveUI();
	void bye();

	bool getFinished() { return finish; }
	Company* getCompany() { return cac_; }
	Date* getDate() { return date_; }

	void setI(int newi) { i = newi; }
	void setOrderType(int newType) { iOrderType = newType; }
};


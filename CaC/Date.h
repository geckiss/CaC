#pragma once
#include <time.h>
#include <string>
#include <iostream>
#include "../CaC/structures/heap_monitor.h"
using namespace std;

class Date
{
private:
	time_t t_;
	/// <summary> Smerník na štruktúru obsaujúcu metódy na prácu s dátumom </summary>
	struct tm * t_now_;

public:
	Date();
	~Date();

	/// <summary> Vráti aktuálny deò v mesiaci v tvare èísla </summary>
	int getDay() { return t_now_->tm_mday; }

	/// <summary> Vráti aktuálny mesiacv roku v tvare èísla </summary>
	int getMonth() { return t_now_->tm_mon + 1; }
	
	/// <summary> Vráti aktuálny rok v tvare èísla </summary>
	int getYear() { return t_now_->tm_year + 1900; }

	time_t getTimeNow() { return t_; }
	struct tm* getTm() { return t_now_; }

	time_t stringToDate(string date);
	string dateToString(time_t t);

	char* getDate();

	/// <summary> Systém prejde na ïalší deò </summary>
	void nextDay();

	/// <summary> Najdenie casu o days dni dalej alebo naspat </summary>
	/// <param name = "days"> Poèet dní, o ko¾ko sa má posunú (+dopredu/-dozadu) </param>
	/// <returns> Cas o days dni dopedu/dozadu </returns>
	time_t daysBackOrForward(int days);
};


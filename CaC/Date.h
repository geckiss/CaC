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
	/// <summary> Smern�k na �trukt�ru obsauj�cu met�dy na pr�cu s d�tumom </summary>
	struct tm * t_now_;

public:
	Date();
	~Date();

	/// <summary> Vr�ti aktu�lny de� v mesiaci v tvare ��sla </summary>
	int getDay() { return t_now_->tm_mday; }

	/// <summary> Vr�ti aktu�lny mesiacv roku v tvare ��sla </summary>
	int getMonth() { return t_now_->tm_mon + 1; }
	
	/// <summary> Vr�ti aktu�lny rok v tvare ��sla </summary>
	int getYear() { return t_now_->tm_year + 1900; }

	time_t getTimeNow() { return t_; }
	struct tm* getTm() { return t_now_; }

	time_t stringToDate(string date);
	string dateToString(time_t t);

	char* getDate();

	/// <summary> Syst�m prejde na �al�� de� </summary>
	void nextDay();

	/// <summary> Najdenie casu o days dni dalej alebo naspat </summary>
	/// <param name = "days"> Po�et dn�, o ko�ko sa m� posun�� (+dopredu/-dozadu) </param>
	/// <returns> Cas o days dni dopedu/dozadu </returns>
	time_t daysBackOrForward(int days);
};


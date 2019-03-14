#pragma once
#include "Date.h"
#include "../CaC/structures/heap_monitor.h"
#include <stdlib.h> /* srand, rand, */
#include <iostream>

/// Hodnoty, pomoocu ktorych sa generuje
static int Q_MIN = 0;
static int Q_POT_MAX = 5000;
static int Q_OIL_MAX = 1000;
static int Q_FLV_MAX = 1000;

static int P_MIN = 1;
static int P_POT_MAX = 3;
static int P_OIL_MAX = 4;
static int P_FLV_MAX = 2;

/// <summary> Pomocné enum pre h¾adanie polotovaru pod¾a názvu </summary>
enum InterEnum { ZEMIAKY = 1, OLEJ = 2, OCHUCOVADLA = 4};

using namespace std;

class Intermediate
{
private:
	/// <summary> Zemiaky, olej, ochucovadla </summary>
	InterEnum iType_;

	bool inUse_;

	/// <summary> Aktualna cena, za akú sa polotovar predava </summary>
	double currentSellPrice_;

	/// <summary> Priemerná cena, za akú C&C nakúpilo polotovar </summary>
	double avgSellPrice_;

	/// <summary> Dostupne mnozstvo tovaru na predaj </summary>
	double availableQuantity_;


public:
	Intermediate();
	Intermediate(InterEnum type);
	~Intermediate();

	InterEnum getType() { return iType_; }
	double getCurrentSellPrice() { return currentSellPrice_; }
	double getAvgSellPrice() { return avgSellPrice_; }
	double getAvailableQuantity() { return availableQuantity_; }
	bool getInUse() { return inUse_; }

	void initCurrentSellPrice();
	void initAvailableQuantity();

	void setCurrentSellPrice(float newSellPrice) { currentSellPrice_ = newSellPrice; }
	void setAvgSellPrice(double newAvg) { avgSellPrice_ = newAvg; }
	void setAvailableQuantity(double newQ) { availableQuantity_ = newQ; }
	void setInUse(bool inUse) { inUse_ = inUse; }

	void printAvgSellPrice() { cout << avgSellPrice_ << endl; }
};


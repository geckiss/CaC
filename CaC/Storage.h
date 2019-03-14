#pragma once
#include "Date.h"
#include "../CaC/structures/heap_monitor.h"

const double POTATOES_CHIPS = 1.5;
const double OIL_CHIPS = 0.2;
const int POTATOES_CRISPS = 2;
const double OIL_CRISPS = 0.4;
const double FLAVORS_CRISPS = 0.02;

static double potatoes_;
static double oil_;
static double flavors_;

static int totalChips_;
static int totalCrisps_;

class Storage
{
private:
	Date * date_;
public:
	Storage();
	Storage(Date* date);
	~Storage();

	int getCrisps() { return totalCrisps_; }
	int getChips() { return totalChips_; }
	double getPotatoes() { return potatoes_; }
	double getOil() { return oil_; }
	double getFlavors() { return flavors_; }

	void setTotalCrisps(int crisps) { totalCrisps_ = crisps; }
	void setTotalChips(int chips) { totalChips_ = chips; }
};


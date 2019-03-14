#include "Intermediate.h"
#include "../CaC/structures/heap_monitor.h"


Intermediate::Intermediate()
{
}

Intermediate::Intermediate(InterEnum type)
{
	iType_ = type;
	initAvailableQuantity();
	initCurrentSellPrice();
	avgSellPrice_ = 0;
}

Intermediate::~Intermediate()
{
	currentSellPrice_ = 0;
	avgSellPrice_ = 0;
	availableQuantity_ = 0;
}

void Intermediate::initCurrentSellPrice()
{
	//srand(time(0));
	switch (iType_) {
	case ZEMIAKY:
		currentSellPrice_ = rand() % P_POT_MAX + P_MIN;
		break;
	case OLEJ:
		currentSellPrice_ = rand() % P_OIL_MAX + P_MIN;
		break;
	case OCHUCOVADLA:
		currentSellPrice_ = rand() % P_FLV_MAX + P_MIN;
		break;
	}
}

void Intermediate::initAvailableQuantity()
{
	//srand(time(0));
	switch (iType_) {
	case ZEMIAKY:
		availableQuantity_ = rand() % Q_POT_MAX + Q_MIN;
		break;
	case OLEJ:
		availableQuantity_ = rand() % Q_OIL_MAX + Q_MIN;
		break;
	case OCHUCOVADLA:
		availableQuantity_ = rand() % Q_FLV_MAX + Q_MIN;
		break;
	}
}



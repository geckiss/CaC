#include "InterOrder.h"
#include "../CaC/structures/heap_monitor.h"

InterOrder::InterOrder()
{
}

InterOrder::InterOrder(Intermediate* inter, double quantity) :
	quantityToOrder_(quantity) 
{
	if (inter != nullptr && quantity > 0) {
		buyingPrice_ = inter->getCurrentSellPrice();
		dateOfOrder_ = time(0);
		interTypeToOrder_ = inter->getType();
	}
}


InterOrder::~InterOrder()
{
	quantityToOrder_ = 0;
	buyingPrice_ = 0;
	dateOfOrder_ = 0;
}


#include "AVehicle.h"
#include "../CaC/structures/heap_monitor.h"


AVehicle::AVehicle()
{
}

AVehicle::AVehicle(string SPZ, int type)
{
	// Type overujem v UI pri vstupe
	if (SPZ.length() != 0) {
		SPZ_ = SPZ;
		switch (type) {
		case 1:
			type_ = LUPIENKY;
			carryWeight_ = 2000;
			maintenance_ = 70;
			break;
		case 2:
			type_ = HRANOLKY;
			carryWeight_ = 5000;
			maintenance_ = 100;
			break;
		}
		usedWeight_ = 0;
		totalMaintenance_ = 0;
		inPark_ = false;
		shipment_ = new ExplicitStack<Order*>();
		firstRegion_ = 0;
	}
}

AVehicle::AVehicle(const string SPZ, OrderType type) : type_(type)
{
	if (SPZ.length() != 0) {
		SPZ_ = SPZ;
		switch (type) {
		case LUPIENKY:
			type_ = LUPIENKY;
			carryWeight_ = 2000;
			maintenance_ = 70;
			break;
		case HRANOLKY:
			type_ = HRANOLKY;
			carryWeight_ = 5000;
			maintenance_ = 100;
			break;
		}
		usedWeight_ = 0;
		totalMaintenance_ = 0;
		inPark_ = false;	
		shipment_ = new ExplicitStack<Order*>();
		firstRegion_ = 0;
	}
}


AVehicle::~AVehicle()
{
	dateOfUsing_ = "";
	inPark_ = false;
	SPZ_ = "";
	carryWeight_ = 0;
	usedWeight_ = 0;
	maintenance_ = 0;
	totalMaintenance_ = 0;
	shipment_->clear();
	delete shipment_;
	firstRegion_ = 0;
}

string AVehicle::getType()
{
	if (type_ == LUPIENKY) {
		return "LUPIENKY";
	}
	else {
		return "HRANOLKY";
	}
}

void AVehicle::addShipment(Order * order)
{
	shipment_->push(order);
	firstRegion_ = order->getCustomer()->getRegion();
	setUsedWeight(order->getQuantity());
}

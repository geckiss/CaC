#include "Order.h"
#include "../CaC/structures/heap_monitor.h"


Order::Order()
{
}

Order::Order(const Order& other)
{
	dateOfOrder_ = other.dateOfOrder_;
	dateOfDelivery_ = other.dateOfDelivery_;
	customer_ = other.customer_;
	type_ = other.type_;
	quantity_ = other.quantity_;
	sellPrice_ = other.sellPrice_;
	acceptDecline_ = other.acceptDecline_;
	doneCanceled_ = other.doneCanceled_;
	inVehicle_ = false;
}


Order::Order(Customer * custom, OrderType typeOfProduct, int quan, double sellPrice, string deliveryDate, bool state, Date* d) :
	type_(typeOfProduct), acceptDecline_(state), date_(d)
{	
	dateOfOrder_ = date_->dateToString(time(0));
	dateOfDelivery_ = deliveryDate;
	customer_ = custom;
	type_ = typeOfProduct;
	quantity_ = quan;
	sellPrice_ = sellPrice;
	inVehicle_ = false;
}

Order::~Order()
{
	dateOfOrder_ = "";
	dateOfDelivery_ = "";
	customer_ = nullptr;
	quantity_ = 0;
	sellPrice_ = 0;
	inVehicle_ = false;
}

string Order::getType()
{
	if (type_ == LUPIENKY) return "LUPIENKY";
	return "HRANOLKY";
}

void Order::toString()
{
	cout << "Objednavka:" << endl << "Datum objednania: " << dateOfOrder_ << ", datum dodania: " << dateOfDelivery_ << endl;
	cout << "Zakaznik: " << customer_->getName() << ", typ tovaru: " << type_ << endl;
	cout << "Mnozstvo: " << quantity_ << ", nakupna cena: " << sellPrice_ << endl << endl;
}

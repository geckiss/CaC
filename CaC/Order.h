#pragma once
#include "Date.h"
#include "../CaC/structures/heap_monitor.h"
#include "CustomerManager.h"

enum OrderType { LUPIENKY, HRANOLKY };

class Order
{
private:
	Date * date_;

	/// <summary> Datum odbjednania </summary>
	string dateOfOrder_;

	/// <summary> Pozadovany datum dorucenia </summary>
	string dateOfDelivery_;

	/// <summary> Zakaznik, ktoreho to je objednavka  </summary>
	Customer* customer_;

	/// <summary> Tovar, ktory si zakaznik objednava  </summary>
	OrderType type_;

	/// <summary> Mnozstvo tovaru, ktory si zakaznik objednava  </summary>
	int quantity_;

	/// <summary> Predajna cena tovaru  </summary>
	double sellPrice_;

	/// <summary> Pociatocny stav objednavky - prijata = true, zamietnuta = false </summary>
	bool acceptDecline_;

	/// <summary> Stav objednavky - zrealizovana = true, zrusena = false </summary>
	bool doneCanceled_;

	/// <summary> Objednavka je vo vozidle = true, inak = false </summary>
	bool inVehicle_;

public:
	Order();
	Order(const Order& other);
	Order(Customer* custom, OrderType typeOfProduct, int quan, double sellPrice, string deliveryDate, bool state, Date* d);
	~Order();

	string getDateOfOrder() { return dateOfOrder_; }
	string getDateOfDelivery() { return dateOfDelivery_; }
	Customer* getCustomer() { return customer_; }
	string getType();
	int getQuantity() { return quantity_; }
	double getSellPrice() { return sellPrice_; }
	bool getAcceptedOrDeclined() { return acceptDecline_; }
	bool getDoneOrCanceled() { return doneCanceled_; }
	bool getInVehicle() { return inVehicle_; }

	void setDoneOrCanceled(bool s) { doneCanceled_ = s; }
	void setInVehicle(bool state) { inVehicle_ = state; }

	void toString();
};


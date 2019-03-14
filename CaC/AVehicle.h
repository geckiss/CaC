#pragma once
#include <string.h>
#include <iostream>
#include "Order.h"
#include "../CaC/structures/heap_monitor.h"
#include "../CaC/structures/stack/explicit_stack.h"

using namespace std;

class AVehicle
{
private:
	/// <summary> Datum zaradenia vozidla do vozoveho parku </summary>
	time_t addedToPark_;

	/// <summary> Den, kedy sa vozidlo pouziva </summary>
	string dateOfUsing_;

	/// <summary> Urcuje, ci je vozidlo zaradene do vozoveho parku </summary>
	bool inPark_;

	/// <summary> Typ vozidla, lupienkar(Crisper) alebo hranolkar(Chipser) </summary>
	OrderType type_;

	/// <summary> Unikatna SPZ </summary>
	string SPZ_;

	/// <summary> Celkova vaha, aku vozidlo dokaze odviest </summary>
	int carryWeight_;

	/// <summary> Pouzita vaha vozidla </summary>
	int usedWeight_;

	/// <summary> Prevadzkove naklady podla typu vozidla per region </summary>
	int maintenance_;

	/// <summary> Prevadzkove naklady od datumu zaradenia do vozoveho parku </summary>
	int totalMaintenance_;

	/// <summary> Objednavky nalozene vo vozidle </summary>
	ExplicitStack<Order*>* shipment_;

	/// <summary> Cislo najnizsieho regionu, do ktoreho vozidlo pojde </summary>
	int firstRegion_;

public:
	AVehicle();
	AVehicle(string SPZ, int type);
	AVehicle(string SPZ, OrderType type);
	~AVehicle();

	time_t getAddedToPark() { return addedToPark_; }
	string getDateOfUsing() { return dateOfUsing_; }
	const string getSPZ() { return SPZ_; }
	bool getInPark() { return inPark_; }
	ExplicitStack<Order*>* getShipment() { return shipment_; }
	/// <summary> Metoda vrati typ vozidla podla atributu type_ </summary>
	/// <returns> Typ vozidla v tvare stringu </returns>
	string getType();

	int getMaintenance() { return maintenance_;	}
	int getTotalMaintenance() { return totalMaintenance_; }
	int getCarryWeight() { return carryWeight_; }
	int getUsedWeight() { return usedWeight_; }
	int getFreeWeight() { return (carryWeight_ - usedWeight_); }
	int getFirstRegion() { return firstRegion_; }

	void setInPark(bool inPark) { inPark_ = inPark; }
	void setAddedToPark(time_t now) { addedToPark_ = now; }
	void setFirstRegion(int region) { firstRegion_ = region; }
	void setTotalMaintenace(int maintenance) { totalMaintenance_ += maintenance; }
	void setUsedWeight(int used) { usedWeight_ += used; }

	// Vahu a kapacitu vozidiel nemusim kontrolovat, ta sa spravi pri evidovani objednavky
	/// <summary> Nalozenie objednavky do vozidla </summary>
	/// <param name = "order"> Objednavka na vybavenie </param>
	void addShipment(Order* order);
};


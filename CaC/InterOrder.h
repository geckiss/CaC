#pragma once
#include "Intermediate.h"
#include "../CaC/structures/heap_monitor.h"

class InterOrder
{
private:
	/// <summary> Mnozstvo polotovaru na dodanie spolocnosti </summary>
	double quantityToOrder_;

	/// <summary> Cena, za ktoru sa nakupi kus polotovaru. Dana typom polotovaru </summary>
	double buyingPrice_;

	/// <summary> Datum vytvorenia objednavky </summary>
	time_t dateOfOrder_;

	/// <summary> Typ polotovaru, ktory chce spolocnost objednat </summary>
	InterEnum interTypeToOrder_;

public:
	InterOrder();
	InterOrder(Intermediate* inter, double quantity);
	~InterOrder();

	double getQuantityToOrder() { return quantityToOrder_; }
	double getBuyingPrice() { return buyingPrice_; }
	time_t getDateOfOrder() { return dateOfOrder_; }
	InterEnum getInterTypeToOrder() { return interTypeToOrder_; }
};


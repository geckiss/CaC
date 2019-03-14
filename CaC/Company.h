#pragma once
#include "Date.h"
#include "VehicleManager.h"
#include "BiofarmerManager.h"
#include "CustomerManager.h"
#include "../CaC/structures/heap_monitor.h"

class Company
{
private:
	Date* date_;
	VehicleManager* vm_;
	BiofarmerManager* bm_;
	CustomerManager* cm_;
	Storage* st_;
	double revenue_;

	bool incomingInters_;
	bool checkedForWeek_;
public:
	Company();
	Company(Date* date);
	~Company();

	Date* getDate() { return date_; }
	VehicleManager* getVehicleMan() { return vm_; }
	BiofarmerManager* getBiofarmerMan() { return bm_; }
	CustomerManager* getCustomerMan() { return cm_; }
	Storage* getStorage() { return st_; }
	double getRevenue() { return revenue_; }

	/// <summary> Skontroluju sa zasoby polotovarov </summary>
	/// <param name = "desiredChips"> Mnozstvo hranolcekov, ktore sa maju vyrobit a zajtra dodat </param>
	/// <param name = "desiredCrisps"> Mnozstvo lupienkov, ktore sa maju vyrobit a zajtra dodat </param>
	/// <returns> True, ak je dostatok polotovarov; false, ak je nedostatok polotovarov na vyrobu
	bool checkStorage(int desiredChips, int desiredCrisps);

	/// <summary> Skontroluju sa objednavky na zajtra </summary>
	void checkForNextDay();

	/// <summary> Skontroluju sa zasoby polotovarov na najblizsi tyzden, pripadne sa objednaju polotovary </summary>
	void checkForNextWeek();

	void fillVehicles();

	/// <summary> Zistim, ci mozem vyrobit vacsie mnozstvo produktov, ako su poziadavky na zajtra </summary>
	void canIDoMore();

	/// <summary> Metoda vyroby pozadovane mnozstvo produktov a odoberie o skladu potrebne polotovary </summary>
	/// <param name = "desiredChips"> Mnozstvo hranolcekov, ktore sa maju vyrobit na zajtra </param>
	/// <param name = "desiredCrisps"> Mnozstvo lupienkov, ktore sa maju vyrobit na zajtra </param>
	void createProducts(int desiredChips, int desiredCrisps);

	/// <summary> Posunie sa cas o 1 den, vykonaju sa kontroly </summary>
	void nextDay();

	void printCustomersForPrintUI(int region, string date);

	/// <summary> Metoda vypise vsetky objednavky vzostupne podla datumu, ktore sa podarilo zrealizovat do minDate </summary>
	/// <param name = "minDate"> Spodna hranica datumu, do ktoreho sa budu vypisovat zrealizovane objednavky </param>
	void printDoneOrdersForPrintUI(string minDate);

	void printDCOrdersForPrintUI(string minDate);

	/// <summary> Nacita objednavky polotovarov zo suboru a (ne)prida ich do zoznamu objednavok </summary>
	/// <param name = "filename"> Nazov suboru v tvare "subor.txt" </param>
	void loadOrders(const char * filename);

	/// <summary> Nacita objednavky polotovarov zo suboru a (ne)prida ich do zoznamu objednavok </summary>
	/// <param name = "filename"> Nazov suboru v tvare "subor.txt" </param>
	void saveOrders(const char* filename);

	void printRevenue(string date);
};


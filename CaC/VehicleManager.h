#pragma once
#include "Date.h"
#include "AVehicle.h"
#include "Storage.h"
#include <fstream>
#include "../CaC/structures/list/linked_list.h"
#include "../CaC/structures/heap_monitor.h"

using namespace structures;

class VehicleManager
{
private:

	void swap(Order* p1, Order* p2)
	{
		Order* temp = p1;
		p1 = p2;
		p2 = temp;
	}

	Date* date_;
	List<AVehicle*>* vehiclePark_;
	
	/// <summary> Implicitne idu nove objednavky sem, zoradene podla trzby </summary>
	List<Order*>* acceptedOrders_;
	
	/// <summary> Objednavky dodane zakaznikom, zoradene podla datumu realizacie </summary>
	List<Order*>* doneOrders_;

	/// <summary> Zamietnute a zrusene objednavky, zoradene podla datumu zaevidovania </summary>
	List<Order*>* declinedCanceledOrders_;
	
	int totalCapacity_;
	int usedCapacity_;

	bool alreadyFilledToday;

public:
	VehicleManager(Date* d);
	~VehicleManager();

	int getTotalCapacity() { return totalCapacity_; }
	int getUsedCapacity() { return usedCapacity_; }
	
	List<AVehicle*>* getVehicles() { return vehiclePark_; }
	List<Order*>* getAcceptedOrders() { return acceptedOrders_; }
	List<Order*>* getDoneOrders() { return doneOrders_; }
	List<Order*>* getDeclinedCanceledOrders() { return declinedCanceledOrders_; }
	
	void setTotalCapacity(int capacity) { totalCapacity_ += capacity; }
	int getOrderedQuantityForDate(string deliveryDate);
	bool getFilledToday() { return alreadyFilledToday; }

	void setFilledToday(bool filled) { alreadyFilledToday = filled; }

	/// <summary> Nacita vozidla zo suboru a (ne)prida ich do zoznamu vozidiel </summary>
	/// <param name = "filename"> Nazov suboru v tvare "subor.txt" </param>
	void loadVehicles(const char* filename);

	/// <summary> Ulozi vozidla do suboru </summary>
	/// <param name = "filename"> Nazov suboru v tvare "subor.txt" </param>
	void saveVehicles(const char* filename);

	void addNewVehicle(AVehicle* vehicle);
	void printVehiclePark();

	/// <summary> Naplni vozidla objednanymi tovarmi </summary>
	void fillVehicles();

	/// <summary> Odovzdanie tovaru zakaznikom </summary>
	void deliverGoods();

	/// <summary> SelectionSort podla regionu </summary>
	LinkedList<Order*>* sortOrdersByRegion();

	void addNewOrder(Customer* custom, OrderType type, int quan, double sellPrice, string deliveryDate);
	void addDoneOrder(Customer* custom, OrderType type, int quan, double sellPrice, string deliveryDate);
	void addDCOrder(Customer* custom, OrderType type, int quan, double sellPrice, string deliveryDate);

	/// <summary> Vypise objednavky, ktore maju byt dorucene zajtra </summary>
	void printTomorrowOrders();

	double getIncomeFromOrders(string date);

	class DuplicateVehicleException : public exception {
	private:
		string name_;
	public:
		DuplicateVehicleException(string name) : name_(name), exception() {};

		void printError() {
			cout << "Duplicitne vozidlo " << name_ << ". Nemozno pridat do vozoveho parku." << endl;
		}
	};
};


#include "VehicleManager.h"
#include "../CaC/structures/heap_monitor.h"


VehicleManager::VehicleManager(Date* d) : date_(d)
{
	vehiclePark_ = new ArrayList<AVehicle*>();
	
	acceptedOrders_ = new LinkedList<Order*>();
	doneOrders_ = new LinkedList<Order*>();
	declinedCanceledOrders_ = new LinkedList<Order*>();
	
	totalCapacity_ = 0;
	usedCapacity_ = 0;
}

VehicleManager::~VehicleManager()
{
	for (int i = 0; i < vehiclePark_->size(); i++) {
		delete (*vehiclePark_)[i];
	}
	vehiclePark_->clear();
	
	for (int i = 0; i < acceptedOrders_->size(); i++) {
		delete (*acceptedOrders_)[i];
	}
	acceptedOrders_->clear();
	
	for (int i = 0; i < doneOrders_->size(); i++) {
		delete (*doneOrders_)[i];
	}
	doneOrders_->clear();
	
	for (int i = 0; i < declinedCanceledOrders_->size(); i++) {
		delete (*declinedCanceledOrders_)[i];
	}
	declinedCanceledOrders_->clear();
	/*
	for (Order* order : *orders_) {
		delete order;
	}
	orders_->clear();
	delete orders_;
	orders_ = nullptr;
	*/
	date_ = nullptr;
	delete vehiclePark_;
	vehiclePark_ = nullptr;
	
	delete acceptedOrders_;
	acceptedOrders_ = nullptr;
	delete doneOrders_;
	doneOrders_ = nullptr;
	delete declinedCanceledOrders_;
	declinedCanceledOrders_ = nullptr;
	
	totalCapacity_ = 0;
	usedCapacity_ = 0;
}

int VehicleManager::getOrderedQuantityForDate(string deliveryDate)
{
	// Format datumu kontrolujem pri vytvarani novej objednavky
	int q = 0;
	time_t deliveryTime = date_->stringToDate(deliveryDate);
	for (Order* order : *acceptedOrders_) {
		if (order->getDateOfDelivery() == deliveryDate) {
			q += order->getQuantity();
		}
	}
	return q;
}

void VehicleManager::saveVehicles(const char * filename)
{
	if (filename != nullptr) {
		ofstream file;
		file.open(filename);
		for (AVehicle* veh : (*vehiclePark_)) {
			file << veh->getSPZ() << ", " << " zaradene do vozoveho parku " << date_->dateToString(veh->getAddedToPark()) << endl;
			file << "Rozvaza " << veh->getType() << ", celkove naklady od zaradenia do parku: " << veh->getTotalMaintenance();
			file << endl << endl;
		}

		file.close();
	}
}

void VehicleManager::addNewVehicle(AVehicle * vehicle)
{
	if (vehicle != nullptr) {
		if (vehicle->getInPark() == false) {
			Iterator<AVehicle*>* ite = vehiclePark_->getBeginIterator();
			for (AVehicle* veh : *vehiclePark_) {
				if (veh != vehicle) {
					if (veh->getSPZ() == vehicle->getSPZ()) {
						throw DuplicateVehicleException(vehicle->getSPZ());
					}
				}
			}
			vehicle->setAddedToPark(time(0));
			vehiclePark_->add(vehicle);
			vehicle->setInPark(true);
			setTotalCapacity(vehicle->getCarryWeight());
			cout <<  "Vozidlo bolo vytvorene a pridane do vozoveho parku." << endl;
			delete ite;
		}
		else {
			cout << endl << "	Nemozno pridat nove vozidlo; vozidlo uz je vo vozovom parku." << endl;
		}
	}
	else {
		cout << endl << "	Nemozno pridat nove vozidlo; nullptr" << endl;
	}
}

void VehicleManager::printVehiclePark()
{
	cout << endl;
	for (AVehicle* veh : *vehiclePark_) {
		cout << veh->getSPZ() << ", rozvaza " << veh->getType();
		cout << ", celkove naklady od zaradenia do evidencie = " << veh->getTotalMaintenance();
		cout << endl;
	}
	cout << endl;
}

// Dnesne objednavky
// TODO canIDoMore a nakladanie vozidiel by sa malo volat z Company pre pristup ku canIDoMore
void VehicleManager::fillVehicles()
{
	if (vehiclePark_->size() != 0) {
		int currVehicle = 0;
		LinkedList<Order*>* sortedOrders = sortOrdersByRegion();

		Iterator<AVehicle*>* ite = vehiclePark_->getBeginIterator();
		AVehicle* veh = ite->operator*();

		for (Order* order : *sortedOrders) {
			while (veh->getType() != order->getType() && veh->getFreeWeight() >= order->getQuantity()) {
				*ite = ite->operator++();
				veh = ite->operator*();
			}
			// Ak chcem dat vozidlu objednavku do rovnakeho regionu
			int firstReg = veh->getFirstRegion();
			if (firstReg == 0 || firstReg == order->getCustomer()->getRegion()) {
				/*
				veh->addShipment(order);
				order->setInVehicle(true);
				cout << "Objednavka nalozena do vozidla " << veh->getSPZ() << endl;
				*/
			}
			// Ak chcem dat vozidlu objednavku do ineho regionu
			else {
				// Ak mam viacej vozidiel
				if (vehiclePark_->size() != 1) {
					// Ak som na konci vozoveho parku, idem od zaciatku
					if (currVehicle == vehiclePark_->size() - 1) {
						delete ite;
						ite = vehiclePark_->getBeginIterator();
						veh = ite->operator*();
					}
				}
				/*
				veh->addShipment(order);
				cout << "Objednavka nalozena do vozidla " << veh->getSPZ() << endl;
				*/
			}
			veh->addShipment(order);
			order->setInVehicle(true);
			cout << "Objednavka nalozena do vozidla " << veh->getSPZ() << endl;
		}

		delete ite;
		delete sortedOrders;

		
		cout << "Operacia nakladania ukoncena" << endl;
	}
	else {
		cout << endl << "	Nemozno naplnit vozidla; vozovy park je prazdny!" << endl;
	}
	
}

void VehicleManager::deliverGoods()
{
	unsigned char regions;
	int regionCount;
	unsigned char shifted;
	for (AVehicle* veh : *vehiclePark_) {
		regions = 0;
		regionCount = 0;
		shifted = 0;
		// Vybavena objednavka
		Order* order;
		for (int i = 0; i < veh->getShipment()->size(); i++) {
			// POP
			order = veh->getShipment()->pop();
			order->setInVehicle(false);
			regions |= 2 ^ (order->getCustomer()->getRegion() - 1);
			// Objednavka je vybavena
			// Musim ju dat prec z acceptedOrders
			Order* doneOrder = acceptedOrders_->removeAt(acceptedOrders_->getIndexOf(order));
			if (doneOrder->getType() == "HRANOLKY") {
				addDoneOrder(doneOrder->getCustomer(), HRANOLKY, doneOrder->getQuantity(), doneOrder->getSellPrice(), doneOrder->getDateOfDelivery());
			}
			else {
				addDoneOrder(doneOrder->getCustomer(), LUPIENKY, doneOrder->getQuantity(), doneOrder->getSellPrice(), doneOrder->getDateOfDelivery());
			}
		}
		while (regions != 0) {
			shifted = regions;
			shifted >>= 1;
			// Bola tam jednotka
			if (regions - shifted != shifted) {
				regionCount++;
			}
			regions = shifted;
		}
		cout << "Update nakladov vozidla " << veh->getSPZ() << ": +" << veh->getMaintenance() * regionCount << endl;
		veh->setTotalMaintenace(veh->getMaintenance() * regionCount);
	}
}

LinkedList<Order*>* VehicleManager::sortOrdersByRegion()
{
	LinkedList<Order*>* regOrders = new LinkedList<Order*>();
	time_t today = date_->getTimeNow();
	for (Order* order : *acceptedOrders_) {
		if (date_->stringToDate(order->getDateOfDelivery()) == today) {
			regOrders->add(order);
		}
	}
	int size = regOrders->size();	
	int min_index;

	for (int i = 0; i < size - 1; i++)
	{
		min_index = i;
		for (int j = i + 1; j < size; j++)
			if ((*regOrders)[j]->getCustomer()->getRegion() > (*regOrders)[min_index]->getCustomer()->getRegion()) {
				min_index = j;
			}
		swap((*regOrders)[min_index], (*regOrders)[i]);
	}
	return regOrders;
}

void VehicleManager::addNewOrder(Customer* custom, OrderType type, int quan, double sellPrice, string deliveryDate)
{
	if (custom != nullptr && quan > 0 && sellPrice > 0 && deliveryDate != "") {
		// Ziskam cas dorucenia objednavky
		time_t deliveryTime = date_->stringToDate(deliveryDate);
		// Ziskam cas o 7 dni
		time_t minTime = date_->daysBackOrForward(7);
		int orderedQuan = getOrderedQuantityForDate(deliveryDate);
		// Ak by pozadovane mnozstvo prekrocilo kapacitu vozidiel rozvazajucich v ten den, zamietnem objednavku
		if (orderedQuan + quan > getTotalCapacity() || deliveryTime < minTime) {
			addDCOrder(custom, type, quan, sellPrice, deliveryDate);
		} else {
			Order* newOrder = new Order(custom, type, quan, sellPrice, deliveryDate, true, date_);
			if (acceptedOrders_->size() != 0) {
				// Sort podla trzby za objednavku
				int i = 0;
				for (Order* ord : *acceptedOrders_) {
					// Ak su trzby vkladanej objednavky vacsie
					if (ord->getQuantity() * ord->getSellPrice() < quan * sellPrice) {
						// A som na konci, vlozim ju na koniec
						if (i == acceptedOrders_->size() - 1) {
							acceptedOrders_->insert(newOrder, i);
							break;
						}
						// Hladam dalej
					}
					else {
						// Mam kapacitu vo vozidlach
						// Cas dorucenia neskor ako za 7 dni
						acceptedOrders_->insert(newOrder, i);
						break;
					}
					i++;
				}
			}
			else {
				acceptedOrders_->add(newOrder);
			}
			cout << "Objednavka sa prijima." << endl;
		}
	}
	else {
		cout << "	Nemozno pridat novu objednavku; zly format parametrov." << endl;
	}
}

// Sort podla datumu realizacie objednavky
void VehicleManager::addDoneOrder(Customer* custom, OrderType type, int quan, double sellPrice, string deliveryDate)
{
	int index = 0;
	Order* newOrder = new Order(custom, type, quan, sellPrice, deliveryDate, true, date_);
	newOrder->setDoneOrCanceled(true);
	if (doneOrders_->size() != 0) {
		for (Order* insideOrder : *doneOrders_) {
			if (insideOrder->getDateOfDelivery().compare(deliveryDate) < 0) {
				doneOrders_->insert(newOrder, index);
				cout << "Objednavka pre " << custom->getName() << " bola vybavena " << date_->dateToString(time(0)) << endl << endl;
				break;
			}
			else {
				// Hladam dalej
			}
		}
	}
	else {
		doneOrders_->add(newOrder);
	}

	// Znizim mnozstvo v skladoch
	if (type == HRANOLKY) {
		totalChips_ -= quan;
	}
	else {
		totalCrisps_ -= quan;
	}
}

// Sort podla datumu zaevidovania objednavky
// TODO pridaj parameter, ci bola declined, lebo state(ten bool co tam je teraz) urcuje len ci bola prijata alebo zamietnuta
void VehicleManager::addDCOrder(Customer* custom, OrderType type, int quan, double sellPrice, string deliveryDate)
{
	int index = 0;
	Order* decOrder = new Order(custom, type, quan, sellPrice, deliveryDate, false, date_);
	if (declinedCanceledOrders_->size() != 0) {
		for (Order* inOrder : *declinedCanceledOrders_) {
			if (date_->stringToDate(inOrder->getDateOfOrder()) < time(0)) {
				declinedCanceledOrders_->insert(decOrder, index);
				break;
			}
			else {
				if (index == declinedCanceledOrders_->size() - 1) {
					declinedCanceledOrders_->add(decOrder);
					break;
				}
				else {
					// Hladam dalej
				}
			}
			index++;
		}
	}
	else {
		declinedCanceledOrders_->add(decOrder);
	}
	if (getOrderedQuantityForDate(deliveryDate) + quan > (double)getTotalCapacity()) {
		cout << "	Objednavka sa zamieta; nedostatocna kapacita vozidiel." << endl << endl;
	}
	else {
		cout << "	Objednavka sa zamieta" << endl;
	}
}

void VehicleManager::loadVehicles(const char* filename)
{
	ifstream f;
	f.open(filename);
	string spz;
	string type;
	if (filename && *filename) {
		if (f.is_open()) {
			while (getline(f, spz, '/')) {
				getline(f, type, '/');
				f.get();
				if (type == "LUPIENKY") {
					addNewVehicle(new AVehicle(spz, LUPIENKY));
				}
				else {
					if (type == "HRANOLKY") {
						addNewVehicle(new AVehicle(spz, HRANOLKY));
					}
					else {
						cout << "	Nemozno nacitat vozidlo, zle parametre: " << spz << ", " << type << endl;
						cout << "	Pokracujem v nacitavani vozidiel" << endl;
					}
				}
			}
			cout << "	Nacitavanie vozidiel dokoncene." << endl << endl;
		}
	}
	f.close();
}

void VehicleManager::printTomorrowOrders()
{
	int q;
	double s;
	time_t tom = date_->daysBackOrForward(1);		// tomorrow
	string sTom = date_->dateToString(tom);			// tomorrow v stringu
	if (acceptedOrders_->size() != 0) {
		cout << "Objednavky na zajtra, " << sTom << " , pre:" << endl << endl;
		for (Order* ord : *acceptedOrders_) {
			if (date_->stringToDate(ord->getDateOfDelivery()) == tom) {
				q = ord->getQuantity();
				s = ord->getSellPrice();
				cout << ord->getCustomer()->getName() << ", region " << ord->getCustomer()->getRegion();
				cout << ", datum zaevidovania: " << ord->getDateOfOrder() << endl << "Typ tovaru: " << ord->getType();
				cout << ", mnozstvo: " << q << ", jednotkova cena: " << s << endl;
				cout << "Celkove trzby: " << (q * s) << endl << endl;
			}
		}
		cout << endl;
	}
	else {
		cout << endl << "	Na zajtra, " << sTom << ", nie su ziadne objednavky." << endl;
	}
}

double VehicleManager::getIncomeFromOrders(string date)
{
	double income = 0;
	for (Order* order : *doneOrders_) {
		if (date_->stringToDate(order->getDateOfDelivery()) >= (date_->stringToDate(date))) {
			income += (order->getSellPrice() + order->getQuantity());
		}
	}
	return income;
}

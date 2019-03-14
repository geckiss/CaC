#include "Company.h"
#include "../CaC/structures/heap_monitor.h"

Company::Company() : incomingInters_(false), checkedForWeek_(false)
{
	date_ = new Date();
	vm_ = new VehicleManager(date_);
	bm_ = new BiofarmerManager(date_);
	cm_ = new CustomerManager(date_);
	st_ = new Storage(date_);
}

Company::Company(Date * date) : incomingInters_(false), checkedForWeek_(false)
{
	if (date != nullptr) {
		date_ = date;
		vm_ = new VehicleManager(date_);
		bm_ = new BiofarmerManager(date_);
		cm_ = new CustomerManager(date_);
		st_ = new Storage(date_);
	}
}

Company::~Company()
{
	delete st_;
	delete cm_;
	delete bm_;
	delete vm_;
	date_ = nullptr;
}

// Chips = 1,5kg zemiakov + 0,2l oleja
// Crisps = 2kg zemiakov + 0,4l oleja + 0,02kg ochucovadiel
bool Company::checkStorage(int desiredChips, int desiredCrisps)
{
	// Ak mam dostatok polotovarov na vyrobu pozadovaneho mnozstva tovarov, vraciam true
	if ((desiredChips * POTATOES_CHIPS) + (desiredCrisps * POTATOES_CRISPS) <= potatoes_ &&
		((desiredChips * OIL_CHIPS) + (desiredCrisps * OIL_CRISPS) <= oil_ ||
		desiredCrisps * FLAVORS_CRISPS <= flavors_)) return true;
	return false;
}

void Company::checkForNextDay()
{
	string tomorrow = date_->dateToString(date_->daysBackOrForward(1));
	int orderedCrisps = 0;
	int orderedChips = 0;
	// Nech nepristupujem 2x ku kvantite objednavky
	int pomQuan;
	List<Order*>* acceptedOrders = vm_->getAcceptedOrders();
	for (Order* order : *acceptedOrders) {
		if (order->getDateOfDelivery() == tomorrow && order->getDoneOrCanceled() != false) {
			pomQuan = order->getQuantity();
			if (order->getType() == "LUPIENKY") {
				orderedCrisps += pomQuan;
			}
			else {
				orderedCrisps += pomQuan;
			}
			
		}
	}
	// Ak nemam dostatok polotovarov, rusim objednavky, lebo od farmarov som uz nakupil
	if (checkStorage(orderedChips, orderedCrisps) == false) {
		for (Order* order : *acceptedOrders) {
			if (order->getDateOfDelivery() == tomorrow) {
				order->setDoneOrCanceled(false);
				if (order->getType() == "HRANOLKY") {
					vm_->addDCOrder(order->getCustomer(), HRANOLKY, order->getQuantity(), order->getSellPrice(), order->getDateOfDelivery());
				}
				else {
					vm_->addDCOrder(order->getCustomer(), LUPIENKY, order->getQuantity(), order->getSellPrice(), order->getDateOfDelivery());
				}
				cout << "	Nemozno zrealizovat objednavku pre " << order->getCustomer()->getName() << "; nedostatok polotovarov" << endl << endl;
				order->toString();
				pomQuan = order->getQuantity();
				if (order->getType() == "HRANOLKY") {
					orderedChips -= pomQuan;
				}
				else {
					orderedCrisps -= pomQuan;
				}
			}
			// Ak uz mozem vyrobit tovary z aktualnych zasob, koncim s rusenim objednavok
			if (checkStorage(orderedChips, orderedCrisps) == true) break;
		}
	}
	// Vytvorim tovary, ktore neskor nahadzem do vozidiel
	if (orderedChips != 0 || orderedCrisps != 0) {
		createProducts(orderedChips, orderedCrisps);
	}
	//canIDoMore();
}

void Company::checkForNextWeek() {

	if (!checkedForWeek_) {
		size_t prioSize = bm_->getPriorityHeap()->size();
		for (int i = 0; i < prioSize; i++) {
			bm_->getPriorityHeap()->pop();
		}

		// Nastavim priemery
		bm_->setBiofarmersAvgPrices();

		// Podla priemerov ich dam do haldy(priorita)
		for (Person* bio : *(bm_->getPersonsList())) {
			bm_->getPriorityHeap()->push(((Biofarmer*)bio)->getAvgSellPrice(), ((Biofarmer*)bio));
		}

		// Updatujem ponuku podla tabulky
		bm_->initBiofarmersInters();
	}
	time_t seventhDay = date_->daysBackOrForward(7);
	int orderedCrisps = 0;
	int orderedChips = 0;
	int pomQuan;
	List<Order*>* orders = vm_->getAcceptedOrders();
	for (Order* ord : *orders) {
		time_t deliveryDate = date_->stringToDate(ord->getDateOfDelivery());
		if (deliveryDate <= seventhDay && deliveryDate > time(0) && ord->getDoneOrCanceled() != false) {
			pomQuan = ord->getQuantity();
			if (ord->getType() == "LUPIENKY") {
				orderedCrisps += pomQuan;
			}
			else {
				orderedChips += pomQuan;
			}
		}
	}
	cout << "Objednane mnozstvo lupienkov: " << orderedCrisps << endl;
	cout << "Objednane mnozstvo hranolkov: " << orderedChips << endl;

	if (!checkedForWeek_) {
		if (checkStorage(orderedChips, orderedCrisps) == false) {
			double neededPotatoes = (orderedChips - totalChips_) * 1.5 + (orderedCrisps - totalCrisps_) * 2 - potatoes_;
			double neededOil = (orderedChips - totalChips_) * 0.2 + (orderedCrisps - totalCrisps_) * 0.4 - oil_;
			double neededFlavors = (orderedCrisps - totalCrisps_) * 0.02 - flavors_;
			double* inters = bm_->buyInters(neededPotatoes, neededOil, neededFlavors);
			potatoes_ += inters[0];
			oil_ += inters[1];
			flavors_ += inters[2];
		}
		checkedForWeek_ = true;
	}
}

void Company::fillVehicles()
{
	vm_->fillVehicles();
	canIDoMore();
}

void Company::canIDoMore()
{
	// Nema zmysel kontrolovat, ak nemam polotovary
	if (potatoes_ != 0 && (oil_ != 0 || flavors_ != 0)) {
		string tomm = date_->dateToString(time(0));
		for (AVehicle* veh : *(vm_->getVehicles())) {
			for (Order* order : *(vm_->getAcceptedOrders())) {
				if (order->getDateOfDelivery() != tomm && veh->getFirstRegion() == order->getCustomer()->getRegion()) {
					int q = order->getQuantity();
					if (order->getType() == "HRANOLKY") {
						if (checkStorage(q, 0) == true) {
							createProducts(q, 0);
							order->setInVehicle(true);
							veh->addShipment(order);
						}
					}
					else {
						if (checkStorage(0, q) == true) {
							createProducts(0, q);
							order->setInVehicle(true);
							veh->addShipment(order);
						}
					}
				}
			}
		}
	}
}

void Company::createProducts(int desiredChips, int desiredCrisps)
{
	totalChips_ += desiredChips;
	totalCrisps_ += desiredCrisps;
	potatoes_ -= (desiredChips * POTATOES_CHIPS) + (desiredCrisps * POTATOES_CRISPS);
	oil_ -= (desiredChips * OIL_CHIPS) + (desiredCrisps * OIL_CRISPS);
	flavors_ -= desiredCrisps * FLAVORS_CRISPS;
}

void Company::nextDay()
{
	checkedForWeek_ = false;
	date_->nextDay();
	// TODO tieto dve vymen
	checkForNextWeek();
	checkForNextDay();
}

void Company::printCustomersForPrintUI(int region, string date)
{
	float totalRev;
	int canceledOrderCount;
	int canceledWeight;
	float canceledRev;

	int declinedOrderCount;
	int declinedWeight;
	float declinedRev;

	List<Customer*>* customers = dynamic_cast<List<Customer*>*>(getCustomerMan()->getPersonsList());
	List<Order*>* orders = getVehicleMan()->getDoneOrders();
	for (Customer* custom : *customers) {
		if (custom->getRegion() == region) {
			totalRev = 0;
			canceledOrderCount = 0;
			canceledWeight = 0;
			canceledRev = 0;

			declinedOrderCount = 0;
			declinedWeight = 0;
			declinedRev = 0;

			cout << custom->getName() << endl;
			cout << "Celkovy prijem C&C z predaja produktov zakaznikovi: ";
			for (Order* order : *orders) {
				if (order->getCustomer()->getName() == custom->getName()) {
					if (date_->stringToDate(order->getDateOfDelivery()) >= date_->stringToDate(date)) {
						totalRev += order->getQuantity() * order->getSellPrice();
					}
				}
			}

			orders = vm_->getDeclinedCanceledOrders();
			for (Order* order : *orders) {
				if (order->getCustomer()->getName() == custom->getName()) {
					if (date_->stringToDate(order->getDateOfDelivery()) >= date_->stringToDate(date)) {
						// Zrusene objednavky
						if (order->getDoneOrCanceled() == false) {
							canceledOrderCount++;
							canceledWeight = order->getQuantity();
							canceledRev = canceledWeight * order->getSellPrice();
						}
						else {
							// Zamietnute objednavky(na zaciatku pri objednavani)
							if (order->getAcceptedOrDeclined() == false) {
								declinedOrderCount++;
								declinedWeight = order->getQuantity();
								declinedRev = declinedWeight * order->getSellPrice();
							}
						}
					}
				}
			}
			cout << totalRev << endl << "Zrusene objednavky: " << endl;
			cout << "Pocet: " << canceledOrderCount << "	Hmotnost: " << canceledWeight << "	Straty: " << canceledRev << endl;
			cout << "Zamietnute objednavky:" << endl << "Pocet: " << declinedOrderCount;
			cout << "	Hmotnost: " << declinedWeight << "	Straty: " << declinedRev << endl << endl;
		}
	}
}

void Company::printDoneOrdersForPrintUI(string minDate)
{
	List<Order*>* doneOrders = vm_->getDoneOrders();
	time_t minTime = date_->stringToDate(minDate);
	for (Order* order : *doneOrders) {
		// Vypisem objednavky vybavene od teraz po minDate
		if (date_->stringToDate(order->getDateOfDelivery()) >= minTime) {
			cout << "Datum realizacie: " << order->getDateOfDelivery() << endl;
			cout << "Zakaznik: " << order->getCustomer()->getName() << endl;
			cout << "Typ produktu: " << order->getType() << endl;
			cout << "Mnozstvo: " << order->getQuantity() << "		";
			cout << "Trzby: " << order->getQuantity() * order->getSellPrice() << endl << endl;
		}
	}
}

void Company::printDCOrdersForPrintUI(string minDate)
{
	List<Order*>* decCanOrders = vm_->getDeclinedCanceledOrders();
	time_t minTime = date_->stringToDate(minDate);
	for (Order* order : *decCanOrders) {
		// Vypisem objednavky vybavene od teraz po minDate
		if (date_->stringToDate(order->getDateOfDelivery()) >= minTime) {
			cout << "Datum zevidovania: " << order->getDateOfOrder() << endl;
			cout << "Zakaznik: " << order->getCustomer()->getName() << endl;
			cout << "Typ produktu: " << order->getType() << endl;
			cout << "Mnozstvo: " << order->getQuantity() << "		";
			cout << "Trzby: " << order->getQuantity() * order->getSellPrice() << endl << endl;
		}
	}
}

void Company::loadOrders(const char * filename)
{

	if (filename != nullptr && *filename) {
		ifstream f;
		f.open(filename);
		string customerName;
		string type;
		string quant;
		string price;
		string deliveryDate;
		int q;
		double p;
		if (f.is_open()) {
			List<Person*>* customers = cm_->getPersonsList();
			while (getline(f, customerName, '/')) {
				getline(f, type, '/');
				getline(f, quant, '/');
				getline(f, price, '/');
				getline(f, deliveryDate, '/');
				f.get(); // Skip \n v subore
				q = stoi(quant);
				p = stod(price);
				if (q > 0 && p > 0 && (type == "LUPIENKY" || type == "HRANOLKY")) {
					for (Person* customer : *customers) {
						if (customer->getName() == customerName) {
							if (type == "LUPIENKY") {
								vm_->addNewOrder((Customer*)customer, LUPIENKY, q, p, deliveryDate);
							}
							else {
								if (type == "HRANOLKY") {
									vm_->addNewOrder((Customer*)customer, HRANOLKY, q, p, deliveryDate);
								}
							}
							break;
						}
					}
				}
				else {
					cout << "	Nemozno nacitat objednavku; zle parametre" << endl;
					cout << "	Pokracujem v nacitavani objednavok" << endl;
				}
			}
			cout << "	Nacitavanie objednavok dokoncene." << endl;
		}
		f.close();
	}
	else {
		cout << "	Zly nazov vstupneho suboru" << endl;
	}

}

void Company::saveOrders(const char * filename)
{
	if (filename != nullptr && *filename) {
		ofstream file;
		file.open(filename);
		for (Order* ord : *(vm_->getAcceptedOrders())) {
			file << ord->getCustomer()->getName() << endl << "Datum zaevidovania: " << ord->getDateOfOrder();
			file << ". Datum vybavenia: " << ord->getDateOfDelivery() << endl;
			file << "Prijata: ANO" << endl;
			file << "Mnozstvo: " << ord->getQuantity() << "    Predajna cena: " << ord->getSellPrice();
			file << endl << endl;
		}

		for (Order* ord : *(vm_->getDoneOrders())) {
			file << ord->getCustomer()->getName() << endl << "Datum zaevidovania: " << ord->getDateOfOrder();
			file << ". Datum vybavenia: " << ord->getDateOfDelivery() << endl;
			file << "	Vybavena: ANO" << endl;
			file << "Mnozstvo: " << ord->getQuantity() << "    Predajna cena: " << ord->getSellPrice();
			file << endl << endl;
		}

		for (Order* ord : *(vm_->getDeclinedCanceledOrders())) {
			file << ord->getCustomer()->getName() << endl << "Datum zaevidovania: " << ord->getDateOfOrder();
			file << ". Datum vybavenia: " << ord->getDateOfDelivery() << endl;
			if (ord->getAcceptedOrDeclined()) {
				file << "Prijata: ANO";
			}
			else {
				file << "Prijata: NIE";
			}
			file << "	Vybavena: NIE" << endl;
			file << "Mnozstvo: " << ord->getQuantity() << "    Predajna cena: " << ord->getSellPrice();
			file << endl << endl;
		}

		file.close();
	}
}

void Company::printRevenue(string date)
{
	double income = vm_->getIncomeFromOrders(date);
	//int maintenance;
	double outcome = bm_->getIncomeFromInterOrders(date);
	cout << "Zisk spolocnosti do " << date << " je " << (income - outcome) << " Eur" << endl;
}

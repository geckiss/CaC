#include "BiofarmerManager.h"
#include "../CaC/structures/heap_monitor.h"


BiofarmerManager::BiofarmerManager()
{
	persons_ = new ArrayList<Person*>();
	priorityFarmers_ = new Heap<Biofarmer*>();
}

BiofarmerManager::BiofarmerManager(Date * date)
{
	//biofarmers_ = new ArrayList<Biofarmer*>();
	persons_ = new ArrayList<Person*>();
	priorityFarmers_ = new Heap<Biofarmer*>();
	date_ = date;
}


BiofarmerManager::~BiofarmerManager()
{
	
	for (int i = 0; i < persons_->size(); i++) {
		delete ((Biofarmer*)((*persons_)[i]));
	}
	
	/*
	for (Person* bio : *persons_) {
		delete ((Biofarmer*)bio);
	}
	*/
	persons_->clear();
	delete persons_;
	persons_ = nullptr;
	date_ = nullptr;

	priorityFarmers_->clear();
	delete priorityFarmers_;
	priorityFarmers_ = nullptr;
}
/*
void BiofarmerManager::addNewBiofarmer(Biofarmer* bio) throw ()
{
	//size_t numberOfFarmers_ = biofarmers_->size();
	size_t numberOfFarmers_ = persons_->size();
		if (numberOfFarmers_ != 0) {
			string newName = bio->getName();
			for (int i = 0; i < numberOfFarmers_; i++) {
				//string inName = ((*biofarmers_)[i])->getName();
				string inName = ((*persons_)[i])->getName();
				if (inName != newName) {
					// Ak pridane meno je skorej v abecede ako pridavane
					if (newName.compare(inName) > 0) {
						if (i == numberOfFarmers_ - 1) {
							persons_->insert(bio, i + 1);
							//biofarmers_->insert(bio, i + 1);
							addNewBiofarmerPriority(bio);
							break;
						}
						// Hladam dalej

					} else {
						// Ak pridane meno NIE je skorej v abecede ako pridavane
						biofarmers_->insert(bio, i);
						addNewBiofarmerPriority(bio);
						break;
					}
				} else {
					// Biofarmar uz je v zozname
					cout << endl << "	Nemozno pridat biofarmara; uz je v zozname." << endl;
				}
			}
		}
		else {
			biofarmers_->add(bio);
			addNewBiofarmerPriority(bio);
		}
		cout << endl << "Biofarmar bol vytvoreny a pridany do zoznamu." << endl;
}
*/

void BiofarmerManager::addNewPerson(Person * p)
{
	if (p != nullptr) {
		Biofarmer* bio = dynamic_cast<Biofarmer*>(p);
		size_t numberOfFarmers_ = persons_->size();
		if (numberOfFarmers_ != 0) {
			string newName = bio->getName();
			for (int i = 0; i < numberOfFarmers_; i++) {
				string inName = ((*persons_)[i])->getName();
				if (inName != newName) {
					// Ak pridane meno je skorej v abecede ako pridavane
					if (newName.compare(inName) > 0) {
						if (i == numberOfFarmers_ - 1) {
							//persons_->insert(bio, i + 1);
							persons_->add(bio);
							addNewBiofarmerPriority(bio);
							break;
						}
						// Hladam dalej
					}
					else {
						// Ak pridane meno NIE je skorej v abecede ako pridavane
						persons_->insert(bio, i);
						addNewBiofarmerPriority(bio);
						break;
					}
				}
				else {
					// Biofarmar uz je v zozname
					cout << endl << "	Nemozno pridat biofarmara " << bio->getName() << "; uz je v zozname." << endl;
				}
			}
		}
		else {
			persons_->add(bio);
			addNewBiofarmerPriority(bio);
		}
		cout <<  "Biofarmar bol vytvoreny a pridany do zoznamu." << endl;
	}
}

void BiofarmerManager::addNewBiofarmerPriority(Biofarmer * bio)
{
	priorityFarmers_->push(bio->getAvgSellPrice(), bio);
}

double * BiofarmerManager::buyInters(double pots, double oil, double flavs)
{
	double inters [3] = { 0, 0, 0 };
	inters[0] = 0;
	inters[1] = 0;
	inters[2] = 0;
	Biofarmer* bio;
	do {
		bio = priorityFarmers_->pop();
		if (bio->getCharIntermediate() != 0) {
			Intermediate* inter = bio->getIntermediate(ZEMIAKY);
			// Ak som este nekupil pozadovane mnozstvo polotovaru od biofarmara
			if (inter != nullptr && inters[0] < pots) {
				// Ma viac ako potrebujem
				if (inter->getAvailableQuantity() >= (pots - inters[0])) {
					bio->addInterOrder(inter, pots - inters[0]);
					inter->setAvailableQuantity(inter->getAvailableQuantity() - (pots - inters[0]));
					cout << "Nakup polotovarov od " << bio->getName() << ": " << (pots - inters[0]) << "kg zemiakov" << endl;
					inters[0] = pots;
				}
				else {
					// Ma menej nez potrebujem
					bio->addInterOrder(inter, inter->getAvailableQuantity());
					inters[0] += inter->getAvailableQuantity();
					cout << "Nakup polotovarov od " << bio->getName() << ": " << inter->getAvailableQuantity() << "kg zemiakov" << endl;
					inter->setAvailableQuantity(0);
				}
			}
			inter = bio->getIntermediate(OLEJ);
			if (inter != nullptr && inters[1] < oil) {
				if (inter->getAvailableQuantity() >= (oil - inters[1])) {
					bio->addInterOrder(inter, oil - inters[1]);
					inter->setAvailableQuantity(inter->getAvailableQuantity() - (oil - inters[1]));
					cout << "Nakup polotovarov od " << bio->getName() << ": " << (oil - inters[1]) << "l oleja" << endl;
					inters[1] = oil;
				}
				else {
					bio->addInterOrder(inter, inter->getAvailableQuantity());
					inters[1] += inter->getAvailableQuantity();
					cout << "Nakup polotovarov od " << bio->getName() << ": " << inter->getAvailableQuantity() << "l oleja" << endl;
					inter->setAvailableQuantity(0);
				}
			}
			inter = bio->getIntermediate(OCHUCOVADLA);
			if (inter != nullptr && inters[2] < flavs) {
				if (inter->getAvailableQuantity() >= (flavs - inters[2])) {
					bio->addInterOrder(inter, flavs - inters[2]);
					inter->setAvailableQuantity((inter->getAvailableQuantity() - (flavs - inters[2])));
					cout << "Nakup polotovarov od " << bio->getName() << ": " << (flavs - inters[2]) << "g ochucovadiel" << endl;
					inters[2] = flavs;
				}
				else {
					bio->addInterOrder(inter, inter->getAvailableQuantity());
					inters[2] += inter->getAvailableQuantity();
					cout << "Nakup polotovarov od " << bio->getName() << ": " << inter->getAvailableQuantity() << "g ochucovadiel" << endl;
					inter->setAvailableQuantity(0);
				}
			}
		}
	} while ((inters[0] < pots || inters[1] < oil || inters[2] < flavs) && priorityFarmers_->size() != 0);
	return inters;
}

void BiofarmerManager::printBiofarmers(InterEnum inter)
{
	cout << endl;
	for (Person* bio : *persons_) {
		if (((Biofarmer*)bio)->checkForCharIntermediate(inter)) {
			((Biofarmer*)bio)->getPersonDesc();
		}
	}
}

void BiofarmerManager::printPersons()
{
	for (Person* bio : *persons_) {
		((Biofarmer*)bio)->getPersonDesc();
	}
}

void BiofarmerManager::printBiofarmersForUI()
{
	cout << endl;
	int i = 1;
	for (Person* bio : *persons_) {
		cout << "	" << i << ". " << bio->getName() << endl;
		i++;
	}
	cout << endl;
}

/*
void BiofarmerManager::loadBiofarmers(const char* filename)
{
	ifstream f;
	f.open(filename);
	if (filename && *filename) {
		if (f.is_open()) {
			string newName;
			size_t numberOfFarmers_;
			while (getline(f, newName, '/')) {
				f.get();
				numberOfFarmers_ = biofarmers_->size();
				addNewBiofarmer(new Biofarmer(newName, date_));
			}
			cout << "	Nacitavanie biofarmarov dokoncene." << endl << endl;
		}
	}
	f.close();
}
*/

void BiofarmerManager::loadPersons(const char * filename)
{
	ifstream f;
	f.open(filename);
	if (filename && *filename) {
		if (f.is_open()) {
			string newName;
			size_t numberOfFarmers_;
			while (getline(f, newName, '/')) {
				f.get();
				numberOfFarmers_ = persons_->size();
				addNewPerson(new Biofarmer(newName, date_));
			}
			cout << "	Nacitavanie biofarmarov dokoncene." << endl << endl;
		}
	}
	f.close();
}

/*
void BiofarmerManager::saveBiofarmers(const char * filename)
{
	if (filename != nullptr && *filename) {
		ofstream file;
		file.open(filename);
		for (Biofarmer* bio : (*biofarmers_)) {
			file << bio->getName() << endl << "Priemerna cena za posledny mesiac, za ktoru nam biofarmar predal produkty: ";
			file << bio->getAvgSellPrice() << endl << endl;
		}
		file.close();
	}
}
*/

void BiofarmerManager::savePersons(const char * filename)
{
	if (filename != nullptr && *filename) {
		ofstream file;
		file.open(filename);
		for (Person* bio : *persons_) {
			file << bio->getName() << endl << "Priemerna cena za posledny mesiac, za ktoru nam biofarmar predal produkty: ";
			file << ((Biofarmer*)bio)->getAvgSellPrice() << endl << endl;
		}
		file.close();
	}
}

void BiofarmerManager::printBestBiofarmer(InterEnum inter)
{
	int finalQuant = 0;
	double finalAvg = 0;
	double finalTotalPrice = 0;
	time_t minTime = date_->daysBackOrForward(-30);
	int quant;			// zatial naratane
	float price;
	int totalQuant;			// celkovo predane
	double totalPrice;
	double avgPrice;	// priemerna jednotkova cena
	int index = 0;			// index biofarmara
	Biofarmer* bio;
	if (persons_->size() != 0) {
		for (int i = 0; i < persons_->size(); i++) {
			bio = ((Biofarmer*)(*persons_)[i]);
			quant = 0;
			price = 0;
			totalQuant = 0;
			totalPrice = 0;
			avgPrice = 0;
			for (InterOrder* iOrder : *(bio->getInterOrders())) {
				if (iOrder->getDateOfOrder() >= minTime && iOrder->getInterTypeToOrder() == inter) {
					quant = iOrder->getQuantityToOrder();
					price = (quant * iOrder->getBuyingPrice());
					
					totalPrice += price;
					avgPrice = ((totalQuant * avgPrice) + price) / (totalQuant + quant);
					totalQuant += quant;
				}
			}
			if (finalQuant < totalQuant) {
				finalQuant = totalQuant;
				finalAvg = avgPrice;
				finalTotalPrice = totalPrice;
				index = i;
			}
		}
		if (finalQuant != 0) {
			bio = ((Biofarmer*)persons_->operator[](index));
			cout << bio->getName() << endl << "Celkove zakupene mnozstvo: " << finalQuant << endl;
			cout << "Celkova cena zakupeneho polotovaru: " << finalTotalPrice << endl;
			cout << "Priemerna jednotkova cena: " << finalAvg << endl << endl;
		}
	}
	else {
		cout << "	Zoznam biofarmarov je prazdny!" << endl << endl;
	}
}

void BiofarmerManager::setBiofarmersAvgPrices()
{
	for (Person* bio : *persons_) {
		((Biofarmer*)bio)->setAvgFarmerPrice();
	}
}

void BiofarmerManager::initBiofarmersInters()
{
	srand(time(0));
	for (Person* bio : *persons_) {
		((Biofarmer*)bio)->initInterValues();
	}
}

double BiofarmerManager::getIncomeFromInterOrders(string date)
{
	double income = 0;		
	for (Person* bio : *persons_) {
		income += ((Biofarmer*)bio)->getRevenue(date);
	}
	return income;
}

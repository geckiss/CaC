#include "CustomerManager.h"
#include "../CaC/structures/heap_monitor.h"


CustomerManager::CustomerManager()
{
}

CustomerManager::CustomerManager(Date * date)
{
	if (date != nullptr) {
		date_ = date;
		persons_ = new ArrayList<Person*>();
	}
}


CustomerManager::~CustomerManager()
{
	for (int i = 0; i < persons_->size(); i++) {
		delete (*persons_)[i];
	}
	delete persons_;
	date_ = nullptr;
	persons_ = nullptr;
}
/*
void CustomerManager::addNewCustomer(Customer * custom)
{
	if (custom != nullptr) {
		if (customers_->size() != 0) {
			size_t numberOfCustomers_ = customers_->size();
			string newName = custom->getName();
			for (int i = 0; i < numberOfCustomers_; i++) {
				string inName = ((*customers_)[i])->getName();
				// Ak pridavame rovnaky smernik, len na iny objekt, zle nedobre je
				if (inName != newName && (*customers_)[i] != custom) {
					// Ak pridane meno je skorej v abecede ako pridavane
					if (newName.compare(inName) > 0) {
						if (i == numberOfCustomers_ - 1) {
							customers_->insert(custom, i + 1);
							cout << endl << "Zakaznik bol vytvoreny a pridany do zoznamu." << endl;
							break;
						}
						// Hladam dalej

					}
					else {
						// Ak pridane meno NIE je skorej v abecede ako pridavane
						customers_->insert(custom, i);
						cout << endl << "Zakaznik bol vytvoreny a pridany do zoznamu." << endl;
						break;
					}
				}
				else {
					cout << endl << "	Nemozno pridat zakaznika; uz je v zozname." << endl;
					break;
				}
			}
		}
		else {
			customers_->add(custom);
		}
	}
	else {
		cout << endl << "Nemozno pridat zakaznika, nullptr" << endl;
	}
}
*/

void CustomerManager::addNewPerson(Person * custom)
{
	if (custom != nullptr) {
		if (persons_->size() != 0) {
			size_t numberOfCustomers_ = persons_->size();
			string newName = custom->getName();
			for (int i = 0; i < numberOfCustomers_; i++) {
				string inName = ((*persons_)[i])->getName();
				//  && (*persons_)[i] != custom
				// rovnaky smernik, len na iny objekt
				if (inName != newName) {
					// Ak pridane meno je skorej v abecede ako pridavane
					if (newName.compare(inName) > 0) {
						if (i == numberOfCustomers_ - 1) {
							//persons_->insert(custom, i + 1);
							persons_->add(custom);
							cout << "Zakaznik bol vytvoreny a pridany do zoznamu." << endl;
							break;
						}
						// Hladam dalej

					}
					else {
						// Ak pridane meno NIE je skorej v abecede ako pridavane
						persons_->insert(custom, i);
						cout << "Zakaznik bol vytvoreny a pridany do zoznamu." << endl;
						break;
					}
				}
				else {
					cout << endl << "	Nemozno pridat zakaznika; uz je v zozname." << endl;
					break;
				}
			}
		}
		else {
			persons_->add(custom);
		}
	}
	else {
		cout << endl << "	Nemozno pridat zakaznika, nullptr" << endl;
	}
}

void CustomerManager::savePersons(const char * filename)
{
	if (filename != nullptr && *filename) {
		ofstream file;
		file.open(filename);
		for (Person* custom : *persons_) {
			file << custom->getName() << ", z regionu " << ((Customer*)custom)->getRegion() << endl;
		}
		file.close();
	}
}

void CustomerManager::printCustomersForAddUI()
{
	int region = 1;
	for (Person* custom : *persons_) {
		cout << "	" << region << ". " << custom->getName() << ", region " << ((Customer*)custom)->getRegion() << endl;
		region++;
	}
}
void CustomerManager::printPersons()
{
	for (Person* custom : *persons_) {
		((Customer*)custom)->getPersonDesc();
	}
}

void CustomerManager::loadPersons(const char * filename)
{
	ifstream f;
	f.open(filename);
	string name;
	string region;
	if (filename && *filename) {
		if (f.is_open()) {
			while (getline(f, name, '/')) {
				getline(f, region, '/');
				f.get();
				int reg = stoi(region);
				if (reg > 0 && reg < 9) {
					addNewPerson(new Customer(name, reg));
				}
				else {
					cout << "	Nemozno nacitat zakaznika; zle parametre" << endl;
					cout << "	Pokracujem v nacitavani zakaznikov" << endl;
				}
			}
			cout << "	Nacitavanie zakaznikov dokoncene." << endl << endl;
		}
	}
	f.close();
}


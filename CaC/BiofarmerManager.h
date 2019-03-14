#pragma once
#include "Biofarmer.h"
#include "PersonManager.h"
#include <string>	/* string, stod, stoi, getline */
#include <fstream>	/* ifstream, ofstream */
#include "../CaC/structures/list/array_list.h"
#include "../CaC/structures/heap_monitor.h"
#include "../CaC/structures/priority_queue/heap.h"

using namespace std;

class BiofarmerManager : public PersonManager
{
private:
	//Date * date_;
	//ArrayList<Biofarmer*>* biofarmers_;

	Heap<Biofarmer*>* priorityFarmers_;

public:
	BiofarmerManager();
	BiofarmerManager(Date* date);
	~BiofarmerManager();

	/// <summary> Vr·ti zoznam dod·vateæov - biofarm·rov spoloËnosti </summary>
	/// <returns> SmernÌk na pole smernÌkov na biofarm·rov </returns>
	// getPersonsList
	//ArrayList<Biofarmer*>* getBiofarmersList() { return biofarmers_; }
	virtual List<Person*>* getPersonsList() { return persons_; }
	Heap<Biofarmer*>* getPriorityHeap() { return priorityFarmers_; }

	/// <summary> Prid· farm·ra do zoznamu dod·vateæov polotovarov pre spoloËnosù </summary>
	//void addNewBiofarmer(Biofarmer* bio);
	void addNewPerson(Person* p);

	/// <summary> Prid· farm·ra do haldy, kde priorita je cena nakupu od biofarmarov za posledny mesiac </summary>
	void addNewBiofarmerPriority(Biofarmer* bio);
	
	double* buyInters(double pots, double oil, double flavs);

	/// Vypise biofarmarov, spolu s ich ponukanymi polotovarmi a priemernymi cenami
	/// <param name = "inter"> Vypise biofarmarov ponukajucich len tento polotovar </param>
	void printBiofarmers(InterEnum inter);

	/// <summary> zdedene od PersonManager </summary>
	void printPersons();

	/// Vypise nazvy biofarmarov pre potreby UI
	void printBiofarmersForUI();

	/// <summary> Nacita biofarmarov zo suboru a prida ich do zoznamu biofarmarov, zdedene od PersonManager </summary>
	/// <param name = "filename"> Nazov suboru v tvare "subor.txt" </param>
	//void loadBiofarmers(const char* filename);
	void loadPersons(const char* filename);

	/// <summary> Ulozi biofarmarov do suboru, zdedene od PersonManager </summary>
	/// <param name = "filename"> Nazov suboru v tvare "subor.txt" </param>
	//void saveBiofarmers(const char* filename);
	void savePersons(const char* filename);

	/// <summary> Vypise informacie o biofarmarovi, od ktoreho sme nakupili najviac polotovarov za posledny mesiac </summary>
	/// <param name = "inter"> Typ polotovaru, ktory sme nakupovali </param>
	void printBestBiofarmer(InterEnum inter);

	void setBiofarmersAvgPrices();
	void initBiofarmersInters();
	double getIncomeFromInterOrders(string date);

};


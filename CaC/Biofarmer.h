#pragma once
#include <exception>
#include "InterOrder.h"
#include "Person.h"
#include "../CaC/structures/heap_monitor.h"
#include "../CaC/structures/list/linked_list.h"

/// Maximum typov polotovarov, ktore moze biofarmar ponukat
const short int MAX_PONUKA = 3;

using namespace structures;
class Biofarmer : public Person
{
private:
	/// <summary> Znakova reprezentacia ponuky polotovarov </summary>
	unsigned char intermediates_;

	/// <summary> Datum </summary>
	Date* date_;

	/// <summary> Portfólio polotovarov, ktoré biofarmár predáva </summary>
	Intermediate** stock_ = new Intermediate*[MAX_PONUKA];

	/// <summary> Pocet polotovarov, ktoré biofarmár predáva </summary>
	int stockCount_;

	/// <summary> Objednavky polotovaru </summary>
	LinkedList<InterOrder*>* interOrders_;

	/// <summary> Priemerna cena za posledny mesiac, za ktoru nam biofarmar predal produkty </summary>
	double avgSellPrice_;

public:
	Biofarmer();
	Biofarmer(string companyName, Date* date);
	~Biofarmer();

	int getStockCount() { return stockCount_; }
	unsigned char getCharIntermediate() { return intermediates_; }
	LinkedList<InterOrder*>* getInterOrders() { return interOrders_; }
	Intermediate* getIntermediate(InterEnum inter);
	double getAvgSellPrice() { return avgSellPrice_; }

	void initInterValues();

	void addIntermediate(Intermediate* inter);
	void setCharIntermediate(InterEnum inter);

	Intermediate* checkForIntermediate(InterEnum inter);
	bool checkForCharIntermediate(InterEnum inter);

	void addInterOrder(Intermediate* inter, double quantity);

	/// <summary> Biofarmarov zisk z objednavok polotovarov do urciteho datumu </summary>
	double getRevenue(string date);

	void printStock();

	void setAvgFarmerPrice();

	/// <summary> Metóda spocita priemernu predajnu cenu polotovarov za poslednych 30 dni </summary>
	Intermediate* setInterAvgPrices(InterEnum inter);

	void getPersonDesc();
	class DuplicateBiofarmerException : public exception {
	private:
		string name_;
	public:
		DuplicateBiofarmerException(string name) : name_(name), exception() {};

		void printError() {
			cout << "Duplicitny biofarmar " << name_ << ". Nemozno pridat do zoznamu dodavatelov." << endl;
		}
	};

	class LowQuantityException : public exception {
	private:
		float q_;
	public:
		LowQuantityException(float desiredQuan) : q_(desiredQuan), exception() {};

		void printError() {
			cout << "Nemozno dodat pozadovane mnozstvo(" << q_ << ") tovaru." << endl;
		}
	};

	class DuplicateInterException : public exception {
	private:
		InterEnum inter_;
		Biofarmer* bio_;
	public:
		DuplicateInterException(InterEnum inter, Biofarmer* bio) : inter_(inter), exception() {
			if (bio != nullptr) bio_ = bio;
		};

		void printError() {
			cout << "Nemozno pridat" << inter_ << " do ponuky " << bio_->getName();
			cout << ". Biofarmar uz polotovar ponuka." << endl;
		}
	};

	class NoInterException : public exception {
	private:
		InterEnum it_; 
		Biofarmer* bio_;
	public:
		NoInterException(InterEnum inter, Biofarmer* bio) : it_(inter), exception() {
			if (bio != nullptr) bio_ = bio;
		};

		void printError()
		{
			cout << "Biofarmar " << bio_->getName() << " nema v ponuke polotovar " << it_ << endl;
		}
	};
};


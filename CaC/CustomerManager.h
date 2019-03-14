#pragma once
#include "Customer.h"
#include "PersonManager.h"
#include <fstream>
#include "../CaC/structures/list/array_list.h"
#include "../CaC/structures/heap_monitor.h"

using namespace structures;

class CustomerManager : public PersonManager
{
private:
	//Date * date_;
	//ArrayList<Customer*>* customers_;

public:
	CustomerManager();
	CustomerManager(Date* date);
	~CustomerManager();

	List<Person*>* getPersonsList() { return persons_; }
	//ArrayList<Customer*>* getCustomersList() { return customers_; }
	//void addNewCustomer(Customer* custom);

	void addNewPerson(Person* custom);

	/// <summary> Zakaznikov nacita zo/ulozi do suboru </summary>
	//void loadCustomers(const char* filename);
	void loadPersons(const char* filename);

	//void saveCustomers(const char* filename);
	void savePersons(const char* filename);

	void printCustomersForAddUI();

	void printPersons();
	class DuplicateCustomerException : public exception{
	private:
		string dup_;
	public:
		DuplicateCustomerException(string dup) : dup_(dup), exception() {};

		void printError() {
			cout << "Duplicitny zakaznik " << dup_ << " ! Nemozon pridat do zoznamu zakaznikov." << endl;
		}
	};
};


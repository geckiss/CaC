#pragma once
#include <exception>
#include "Person.h"
#include "Date.h"
#include "../CaC/structures/list/array_list.h"
#include "../CaC/structures/heap_monitor.h"

using namespace structures;

class PersonManager
{
protected:
	List<Person*>* persons_;
	Date* date_;
public:
	PersonManager();
	PersonManager(Date* d);
	~PersonManager();

	virtual List<Person*>* getPersonsList() = 0;
	virtual void addNewPerson(Person* per) = 0;
	virtual void loadPersons(const char* filename) = 0;
	virtual void savePersons(const char* filename) = 0;
	virtual void printPersons() = 0;
};


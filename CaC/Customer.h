#pragma once
#include "Date.h"
#include "Person.h"
#include "../CaC/structures/heap_monitor.h"
#include <string>
#include <iostream>

using namespace std;

class Customer : public Person
{
private:
	int region_;
public:
	Customer(string name, int region);
	~Customer();

	string getName() { return name_; }
	int getRegion() { return region_; }

	void getPersonDesc();

};


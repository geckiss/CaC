#pragma once
#include <exception>
#include <string>
#include "../CaC/structures/heap_monitor.h"

using namespace std;

class Person
{
protected:
	string name_;
public:
	Person();
	Person(string name);
	~Person();

	string getName() { return name_; }
	// TOOD farmer - meno, atd.... , customer - meno, region
	virtual void getPersonDesc() = 0;
	
};


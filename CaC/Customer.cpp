#include "Customer.h"
#include "../CaC/structures/heap_monitor.h"


Customer::Customer(string name, int region)
{
	if (region > 0 && region < 9 && name != "") {
		name_ = name;
		region_ = region;
	}
}


Customer::~Customer()
{
	name_ = "";
	region_ = 0;
}

void Customer::getPersonDesc()
{
	cout << "Zakaznik " << name_ << ", z regionu " << region_ << endl << endl;
}



#include "Person.h"



Person::Person()
{
	name_ = "Volas zly konstruktor";
}

Person::Person(string name) : name_(name)
{
}

Person::~Person()
{
	name_ = "";
}

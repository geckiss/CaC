#include "Storage.h"
#include "../CaC/structures/heap_monitor.h"


Storage::Storage()
{
	potatoes_ = 0;
	oil_ = 0;
	flavors_ = 0;
	totalChips_ = 0;
	totalCrisps_ = 0;
}

Storage::Storage(Date * date)
{
	date_ = date;
}


Storage::~Storage()
{
	date_ = nullptr;
	potatoes_ = 0;
	oil_ = 0;
	flavors_ = 0;
	totalChips_ = 0;
	totalCrisps_ = 0;
}

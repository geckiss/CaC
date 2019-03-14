#pragma once
#include "UI.h"
#include "../CaC/structures/heap_monitor.h"
int main() {
	try {
		initHeapMonitor();

		Date* d1 = new Date();
		Company* cac = new Company(d1);
		UI* ui = new UI(cac, d1);
		ui->getCompany()->getBiofarmerMan()->loadPersons("InputBiofarmers.txt");

		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](0))->addIntermediate(
			new Intermediate(ZEMIAKY)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](0))->addIntermediate(
			new Intermediate(OLEJ)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](0))->addIntermediate(
			new Intermediate(OCHUCOVADLA)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](1))->addIntermediate(
			new Intermediate(ZEMIAKY)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](1))->addIntermediate(
			new Intermediate(OLEJ)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](1))->addIntermediate(
			new Intermediate(OCHUCOVADLA)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](2))->addIntermediate(
			new Intermediate(ZEMIAKY)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](2))->addIntermediate(
			new Intermediate(OLEJ)
		);
		((Biofarmer*)cac->getBiofarmerMan()->getPersonsList()->operator[](2))->addIntermediate(
			new Intermediate(OCHUCOVADLA)
		);

		ui->getCompany()->getVehicleMan()->loadVehicles("InputVehicles.txt");
		ui->getCompany()->getCustomerMan()->loadPersons("InputCustomers.txt");
		ui->getCompany()->loadOrders("InputOrders.txt");
		ui->mainScreen();
		
		delete ui;
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
}
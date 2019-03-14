#include "UI.h"
#include "../CaC/structures/heap_monitor.h"

UI::UI()
{
}

UI::UI(Company * cac, Date * date)
{
	if (cac != nullptr && date != nullptr) {
		cac_ = cac;
		date_ = date;
		finish = false;
	}
}


UI::~UI()
{
	delete cac_;
	delete date_;
	date_ = nullptr;
	cac_ = nullptr;
	iOrderType = 0;
	i = 0;
}

void UI::hi()
{
	cout << endl;
	cout << "		*****************************************************" << endl;
	cout << "				Vita Vas spolocnost C&C," << endl;
	cout << "		jednotka vo vyrobe zemiakovych lupienkov a hranolkov!" << endl;
	cout << "		*****************************************************" << endl << endl;
}

void UI::mainScreen()
{
	hi();
	do {
		setI(0);
		pomVstup = "";
		cout << endl;
		cout << "	1. NOVY/PRIDAJ" << endl;
		cout << "	2. VYPIS" << endl;
		cout << "	3. ULOZ" << endl;
		cout << "	4. NOVY DEN" << endl;
		cout << "	5. KONTROLA POZIADAVEK NA NAJBLIZSICH 7 DNI" << endl;
		cout << "	6. NAPLNENIE VOZIDIEL" << endl;
		cout << "	7. ODOVZDANIE OBJEDNAVOK" << endl;
		cout << "	8. KONIEC" << endl;
		cout << endl;
		do {
			cout << "Zadajte cislo akcie, ktoru si prajete vykonat(od 1 do 8 vratane): ";
			try {
				getline(cin, pomVstup);
				i = stoi(pomVstup);
			}
			catch (exception e) {
				e.what();
			}
		} while (i < 1 || i > 8);
		switch (i) {
		case 1:
			addUI();
			break;
		case 2:
			printUI();
			break;
		case 3:
			saveUI();
			break;
		case 4:
			// S tymto su spojene operacie vykonavajuce sa kazdy den
			cac_->nextDay();
			break;
		case 5:
			cac_->checkForNextWeek();
			break;
		case 6:
			cac_->fillVehicles();
			break;
		case 7:
			cac_->getVehicleMan()->deliverGoods();
			break;
		case 8:
			finish = true;
			bye();
			break;
		}
	} while (!finish);
}

void UI::addUI()
{
	setI(0);
	pomVstup = "";
	cout << endl << endl;
	cout << "*********************************************" << endl;
	cout << "	1. NOVY BIOFARMAR" << endl;
	cout << "	2. NOVE VOZIDLO" << endl;
	cout << "	3. NOVY ZAKAZNIK" << endl;
	cout << "	4. NOVA OBJEDNAVKA" << endl;
	cout << "	5. PRIDAJ POLOTOVAR BIOFARMAROVI" << endl;
	cout << "	6. NASPAT" << endl;
	cout << "*********************************************" << endl << endl;
	do {
		cout << "Napiste cislo akcie, ktoru si prajete vykonat(v tvare 1): ";
		try {
			getline(cin, pomVstup);
			i = stoi(pomVstup);
		}
		catch (exception e) {
			e.what();
		}
	} while (i < 1 || i > 6);

	string s = "";
	int countOf = 0;

	int orderQuan = 0;
	double orderPrice = 0;
	string dateOfDelivery = "";

	switch (i) {
	case 1:
		try {
			while (s.length() == 0) {
				cout << "Zadajte obchodny nazov biofarmara: ";
				getline(cin, s);
			};
			cac_->getBiofarmerMan()->addNewPerson(new Biofarmer(s, date_));
		}
		catch (exception e) {
			e.what();
		}
		break;

	case 2:
		try {
			setI(0);
			pomVstup = "";
			while (s.length() == 0) {
				cout << "Zadajte SPZ vozidla , prosim(v tvare AB123XY): ";
				getline(cin, s);
			}
			while (i < 1 || i > 2) {
				cout << "	1. LUPIENKY" << endl;
				cout << "	2. HRANOLKY" << endl;
				cout << "Zadajte typ tovaru, ktory vozidlo rozvaza, prosim(v tvare 1): ";
				getline(cin, pomVstup);
				i = stoi(pomVstup);
			}
			cac_->getVehicleMan()->addNewVehicle(new AVehicle(s, i));
		}
		catch (exception e) {
			e.what();
		}
		break;

	case 3:
		try {
			setI(0);
			pomVstup = "";
			while (s.length() == 0) {
				cout << "Zadajte obchodny nazov zakaznika: ";
				getline(cin, s);
			}
			while (i < 1 || i > 8) {
				cout << "Zadajte cislo regionu, v ktorom sa zakaznik nachadza(v tvare 1, od 1 do 8 vratane): ";
				getline(cin, pomVstup);
				i = stoi(pomVstup);
			}
			cac_->getCustomerMan()->addNewPerson(new Customer(s, i));
		}
		catch (exception e) {
			e.what();
		}
		break;

	case 4:
		setI(0);
		pomVstup = "";
		iOrderType = 0;
		orderQuan = 0;
		orderPrice = 0;
		try {
			countOf = cac_->getCustomerMan()->getPersonsList()->size();
			cac_->getCustomerMan()->printCustomersForAddUI();
			while (i < 1 || i > countOf) {
				cout << endl << "Zvolte zakaznika, ktory vystavuje objednavku(v tvare 1): ";
				getline(cin, pomVstup);
				i = stoi(pomVstup);
			}
			pomVstup = "";
			while (iOrderType < 1 || iOrderType > 2) {
				cout << "	1. LUPIENKY" << endl;
				cout << "	2. HRANOLKY" << endl;
				cout << "Zvolte typ produktu, ktory chcete objednat(v tvare 1): ";
				getline(cin, pomVstup);
				iOrderType = stoi(pomVstup);
			}
			pomVstup = "";
			while (orderQuan <= 0) {
				cout << "Zadajte mnozstvo(v tvare 1.00, vacsie ako 0.00): ";
				getline(cin, pomVstup);
				orderQuan = stoi(pomVstup);
			}
			pomVstup = "";
			while (orderPrice <= 0) {
				cout << "Zadajte predajnu cenu za 1kg tovaru(v tvare 1.00, vacsie ako 0.00): ";
				getline(cin, pomVstup);
				orderPrice = stod(pomVstup);
			}
			while (dateOfDelivery.length() == 0) {
				cout << "Zadajte datum dorucenia objednavky(v tvare DD.MM.YYYY)." << endl;
				cout << "Objednavku nie je mozne dorucit skor ako za 7 dni od vystavenia objednavky!" << endl;
				cout << "Datum: ";
				getline(cin, dateOfDelivery);
			}
			if (iOrderType == 1) {
				cac_->getVehicleMan()->addNewOrder(
					((Customer*)cac_->getCustomerMan()->getPersonsList()->operator[](i - 1)),
					LUPIENKY, orderQuan, orderPrice, dateOfDelivery
				);
			}
			else {
				cac_->getVehicleMan()->addNewOrder(
					((Customer*)cac_->getCustomerMan()->getPersonsList()->operator[](i - 1)),
					HRANOLKY, orderQuan, orderPrice, dateOfDelivery
				);
			}
		}
		catch (exception e) {
			e.what();
		}
		pomVstup = "";
		break;
	case 5:
		try {
			setI(0);
			pomVstup = "";
			iOrderType = 0;
			countOf = cac_->getBiofarmerMan()->getPersonsList()->size();
			cac_->getBiofarmerMan()->printBiofarmersForUI();
			while (i < 1 || i > countOf) {
				cout << "Zvolte biofarmara, ktoremu chcete pridat polotovar do ponuky(v tvare 1): ";
				getline(cin, pomVstup);
				i = stoi(pomVstup);
			}
			pomVstup = "";
			cout << endl << endl;
			cout << "	1. ZEMIAKY" << endl;
			cout << "	2. OLEJ" << endl;
			cout << "	3. OCHUCOVADLA" << endl;
			while (iOrderType < 1 || iOrderType > 3) {
				cout << "Zvolte typ polotovaru, ktory chcete pridat biofarmarovi(v tvare 1): ";
				getline(cin, pomVstup);
				iOrderType = stoi(pomVstup);
			}
			InterEnum orderType;
			switch (iOrderType) {
			case 1:
				orderType = ZEMIAKY;
				break;
			case 2:
				orderType = OLEJ;
				break;
			case 3:
				orderType = OCHUCOVADLA;
				break;
			}
			// Ak biofarmar este nema dany polotovar v ponuke
			if (((Biofarmer*)cac_->getBiofarmerMan()->getPersonsList()->operator[](i - 1))->checkForCharIntermediate(orderType) == false) {
				((Biofarmer*)cac_->getBiofarmerMan()->getPersonsList()->operator[](i - 1))->addIntermediate(
					new Intermediate(orderType)
				);
				cout << endl << "Polotovar bol vytvoreny a pridany biofarmarovi." << endl;
			}
			else {
				cout << endl << "	Biofarmar uz ponuka zvoleny polotovar." << endl;
			}
		}
		catch (exception e) {
			e.what();
		}
		pomVstup = "";
		break;
	}
}

void UI::printUI()
{
	setI(0);
	pomVstup = "";
	cout << endl;
	cout << "*************************************************************************" << endl;
	cout << "	1. BIOFARMAROV" << endl;
	cout << "	2. VOZIDLA" << endl;
	cout << "	3. ZAKAZNIKOV" << endl;
	cout << "	4. OBJEDNAVKY NA ZAJTRA" << endl;
	cout << "	5. USPESNE OBJEDNAVKY ZA DANE OBDOBIE" << endl;
	cout << "	6. NEUSPESNE OBJEDNAVKY ZA DANE OBDOBIE" << endl;
	cout << "	7. BIOFARMAR, OD KTOREHO SME NAKUPILI NAJVIAC POLOTOVAROV ZA POSLEDNY MESIAC" << endl;
	cout << "	8. CELKOVY ZISK SPOLOCNOSTI ZA DANE OBDOBIE" << endl;
	cout << "	9. AKTUALNY DEN" << endl;
	cout << "	0. NASPAT" << endl;
	cout << "*************************************************************************" << endl;
	cout << endl;

	do {
		cout << "Napiste cislo kategorie, ktoru si prajete vypisat(v tvare 1): ";
		try {
			getline(cin, pomVstup);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
	} while (pomVstup == "");
	i = stoi(pomVstup);

	string date = "";
	pomVstup = "";
	switch (i) {
	case 1:
		iOrderType = 0;
		cout << endl;
		cout << "	1. ZEMIAKY" << endl;
		cout << "	2. OLEJ" << endl;
		cout << "	3. OCHUCOVADLA" << endl;
		while (iOrderType < 1 || iOrderType > 3) {
			cout << "Zvolte polotovar, podla ktoreho sa vypisu biofarmari ponukajuci dany polotovar(v tvare 1): ";
			getline(cin, pomVstup);
			iOrderType = stoi(pomVstup);
		}
		if (iOrderType == 1) {
			cac_->getBiofarmerMan()->printBiofarmers(ZEMIAKY);
		}
		else {
			if (iOrderType == 2) {
				cac_->getBiofarmerMan()->printBiofarmers(OLEJ);
			}
			else {
				cac_->getBiofarmerMan()->printBiofarmers(OCHUCOVADLA);
			}
		}
		cout << endl;
		pomVstup = "";
		break;

	case 2:
		cac_->getVehicleMan()->printVehiclePark();
		break;
	case 3:
		setI(0);
		try {
			while (i < 1 || i > 8) {
				cout << "Zadajte cislo regionu, z ktoreho sa vypisu zakaznici: ";
				getline(cin, pomVstup);
				i = stoi(pomVstup);
			}
			while (date == "") {
				cout << "Zadajte datum, do ktoreho sa o zakaznikoch vypisu informacie o ich objednavkach." << endl;
				cout << "	Format DD.MM.RRRR" << endl;
				getline(cin, date);
			}
			cac_->printCustomersForPrintUI(i, date);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		setI(0);
		date = "";
		break;
	case 4:
		cac_->getVehicleMan()->printTomorrowOrders();
		break;
	case 5:
		date = "";
		setI(0);
		try {
			while (date == "" || date.length() != 10) {
				cout << "Zadajte datum, do ktoreho sa maju vypisat objednavky(tvar DD.MM.RRRR)" << endl;
				cout << "Datum: ";
				getline(cin, date);
			}
			cac_->printDoneOrdersForPrintUI(date);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		break;
	case 6:
		date = "";
		setI(0);
		try {
			while (date == "" || date.length() != 10) {
				cout << "Zadajte datum, do ktoreho sa maju vypisat objednavky(tvar DD.MM.RRRR)" << endl;
				cout << "Datum: ";
				getline(cin, date);
			}
			cac_->printDCOrdersForPrintUI(date);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		break;
	case 7:
		iOrderType = 0;
		try {
			cout << endl;
			cout << "	1. ZEMIAKY" << endl;
			cout << "	2. OLEJ" << endl;
			cout << "	3. OCHUCOVADLA" << endl;
			while (iOrderType < 1 || iOrderType > 3) {
				cout << "Zvolte polotovar, podla ktoreho sa vypise biofarmar, od ktoreho sme nakupili najviac polotovarov daneho typu(v tvare 1): ";
				getline(cin, pomVstup);
				iOrderType = stoi(pomVstup);
			}
			if (iOrderType == 1) {
				cac_->getBiofarmerMan()->printBestBiofarmer(ZEMIAKY);
			}
			else {
				if (iOrderType == 2) {
					cac_->getBiofarmerMan()->printBestBiofarmer(OLEJ);
				}
				else {
					cac_->getBiofarmerMan()->printBestBiofarmer(OCHUCOVADLA);
				}
			}
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		break;
	case 8:
		date = "";
		try {
			while (date == "" || date.length() != 10) {
				cout << "Zadajte datum, do ktoreho sa ma vypisat zisk spolocnosti(tvar DD.MM.RRRR)" << endl;
				cout << "Datum: ";
				getline(cin, date);
			}
			cac_->printRevenue(date);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		break;
	case 9:
		cout << "	Dneska je " << date_->getDate() << endl;
		break;
	}
	date = "";
	pomVstup = "";
	iOrderType = 0;
}

void UI::saveUI()
{
	try {
		cout << endl << "Ukladam databazu..." << endl;
		cac_->getVehicleMan()->saveVehicles("OutputVehicles.txt");
		cac_->getBiofarmerMan()->savePersons("OutputBiofarmers.txt");
		cac_->saveOrders("OutputOrders.txt");
		cac_->getCustomerMan()->savePersons("OutputCustomers.txt");
		cout << endl << "Databaza ulozena do Output suborov..." << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}


void UI::bye()
{
	saveUI();
	cout << endl;
	cout << "		******************************************************" << endl;
	cout << "		*	Dakujeme za vyuzivanie nasich sluzieb.	     *" << endl;
	cout << "		*		  Spolocnost C&C		     *" << endl;
	cout << "		******************************************************" << endl << endl;
}


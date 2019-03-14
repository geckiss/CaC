#include "Biofarmer.h"
#include "../CaC/structures/heap_monitor.h"

using namespace std;
Biofarmer::Biofarmer()
{
}

Biofarmer::Biofarmer(string companyName, Date* date)
{
	if (companyName.length() != 0) {
		name_ = companyName;
		intermediates_ = 0;
		date_ = date;
		stockCount_ = 0;
		interOrders_ = new LinkedList<InterOrder*>();
		avgSellPrice_ = 0;
	}
}


Biofarmer::~Biofarmer()
{
	name_ = "";
	date_ = nullptr;

	for (int i = 0; i < stockCount_; i++) {
		delete stock_[i];
	}

	delete[] stock_;
	stock_ = nullptr;

	for (int i = 0; i < interOrders_->size(); i++) {
		delete (*interOrders_)[i];
	}
	delete interOrders_;
	interOrders_ = nullptr;
	intermediates_ = 0;
	stockCount_ = 0;
	avgSellPrice_ = 0;
}

Intermediate* Biofarmer::getIntermediate(InterEnum inter) throw()
{
	for (int i = 0; i < stockCount_; i++) {
		if (stock_[i]->getType() == inter) {
			return stock_[i];
		}
	}
	return nullptr;
}

void Biofarmer::initInterValues()
{
	for (int i = 0; i < stockCount_; i++) {
		stock_[i]->initAvailableQuantity();
		stock_[i]->initCurrentSellPrice();
	}
}

void Biofarmer::addIntermediate(Intermediate* inter) throw() {
	// Ak uz ma biofarmar typ polotovaru v zozname kontrolujem v UI
	stock_[stockCount_] = inter;
	setCharIntermediate(inter->getType());
	inter->setInUse(true);
	stockCount_++;
}

void Biofarmer::setCharIntermediate(InterEnum inter) throw() {
	switch (inter) {
	case ZEMIAKY:
		intermediates_ |= 1;
		break;
	case OLEJ:
		intermediates_ |= 2;
		break;
	case OCHUCOVADLA:
		intermediates_ |= 4;
		break;
	default:
		break;
	}
}

Intermediate* Biofarmer::checkForIntermediate(InterEnum inter)
{
	for (int i = 0; i < stockCount_; i++) {
		if (stock_[i]->getType() == inter) {
			return stock_[i];
		}
	}
	return nullptr;
}

bool Biofarmer::checkForCharIntermediate(InterEnum inter) {
	unsigned char i = getCharIntermediate();
	if (i == 7) return true;
	switch (inter) {
	case ZEMIAKY:
		if (i == 1 || i == 3 || i == 5) {
			return true;
			break;
		}
	case OLEJ:
		if (i == 2 || i == 3 || i == 6) {
			return true;
			break;
		}
	case OCHUCOVADLA:
		if (i == 4 || i == 5 || i == 6) {
			return true;
			break;
		}
	default:
		break;
	}
	return false;
}
void Biofarmer::addInterOrder(Intermediate * inter, double quantity)
{
	if (inter != nullptr && quantity >= 0) {
		interOrders_->add(new InterOrder(inter, quantity));
	}
}

double Biofarmer::getRevenue(string date)
{
	time_t time = date_->stringToDate(date);
	double rev = 0;
	for (InterOrder* iOrder : *(interOrders_)) {
		if (iOrder->getDateOfOrder() >= time) {
			rev += (iOrder->getQuantityToOrder() * iOrder->getBuyingPrice());
		}
	}
	return rev;
}

void Biofarmer::printStock() {
	switch (getCharIntermediate()) {
	case 1:
		cout << "Zemiaky." << endl;
		break;
	case 2:
		cout << "Olej." << endl;
		break;
	case 3:
		cout << "Zemiaky a olej." << endl;
		break;
	case 4:
		cout << "Ochucovadla." << endl;
		break;
	case 5:
		cout << "Zemiaky a ochucovadla." << endl;
		break;
	case 6:
		cout << "Olej a ochucovadla." << endl;
		break;
	case 7:
		cout << "Zemiaky a olej a ochucovadla." << endl;
		break;
	default:
		break;
	}
}

void Biofarmer::setAvgFarmerPrice()
{
	if (getCharIntermediate() != 0) {
		int counter = 0;
		int avg1 = 0;
		int avg2 = 0;
		int avg3 = 0;
		Intermediate* i1 = setInterAvgPrices(ZEMIAKY);
		if (i1 != nullptr) {
			counter++;
			avg1 = i1->getAvgSellPrice();
		}
		Intermediate* i2 = setInterAvgPrices(OLEJ);
		if (i2 != nullptr) {
			counter++;
			avg2 = i2->getAvgSellPrice();
		}
		Intermediate* i3 = setInterAvgPrices(OCHUCOVADLA);
		if (i3 != nullptr) {
			counter++;
			avg3 = i3->getAvgSellPrice();
		}
		if (counter != 0) {
			avgSellPrice_ = (avg1 + avg2 + avg3) / counter;
		}
	}
}

Intermediate* Biofarmer::setInterAvgPrices(InterEnum inter)
{
	Intermediate* i = checkForIntermediate(inter);
	if (i != nullptr) {
		// Za poslednych 30 dni
		time_t lastDay = date_->daysBackOrForward(-30);
		double price = 0;
		double avg = 0;
		double count = 0;
		// Pre kazdu objednavku
		for (InterOrder* iOrder : *interOrders_) {
			// Zistim, ci je pre dany polotovar
			if (iOrder->getInterTypeToOrder() == inter) {
				// Zistim cas objednania a ci bol do 30-tich dni
				time_t dateOfOrder = iOrder->getDateOfOrder();
				if (dateOfOrder >= lastDay && dateOfOrder <= time(0)) {
					// Cena, za ktoru sa tovar kupil. Je urcena biofarmarom a cenou, za ktoru predava dany polotovar
					price += (iOrder->getBuyingPrice()) * (iOrder->getQuantityToOrder());
					count += iOrder->getQuantityToOrder();
				}
			}
		}
		// Delenie nulou nechcem
		if (count != 0) {
			avg = price / count;
			i->setAvgSellPrice(avg);
		}
	}
	return i;
}

void Biofarmer::getPersonDesc()
{
	cout << name_ << endl;
	printStock();
	cout << "Priemerna predajna cena polotovarov: " << getAvgSellPrice() << endl << endl;
}





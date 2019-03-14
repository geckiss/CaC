#include "Date.h"
#include "../CaC/structures/heap_monitor.h"

using namespace std;

Date::Date()
{
	// Získame aktuálny systémový èas
	t_ = time(0); 
	// localtime vráti smerník na staticky alokovanú struct tm z Date.h
	t_now_ = localtime(&t_);
}

Date::~Date()
{
	t_ = 0;
	t_now_ = nullptr;
}

char * Date::getDate()
{
	return ctime(&t_);
}

void Date::nextDay()
{
	const time_t DEN = 24 * 60 * 60;

	time_t date_seconds = mktime(t_now_) + (1 * DEN);
	t_ = date_seconds;
	localtime_s(t_now_, &t_);
}

time_t Date::daysBackOrForward(int days)
{
	const time_t ONE_DAY = 24 * 60 * 60;

	time_t newDate = mktime(t_now_) + (days * ONE_DAY);
	return newDate;
}

time_t Date::stringToDate(string date)
{
	struct tm tim;
	time_t t = mktime(t_now_);
	tim = *localtime(&t);
	int d, m, y;
	sscanf(date.c_str(), "%2d.%2d.%4d", &d, &m, &y);
	tim.tm_mday = d;
	tim.tm_mon = m - 1;
	tim.tm_year = y - 1900;
	t = mktime(&tim);
	return t;
}

string Date::dateToString(time_t t)
{
	time_t halp = t_;
	struct tm tim = *localtime(&t);
	char s[11];
	strftime(s, sizeof s, "%d.%m.%Y", &tim);
	time_t now = time(0);
	tim = *localtime(&halp);
	string str = s;
	return str;
}

#include "date.h"
#include <iostream>
using namespace std;


static bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


static int monthDays(int year, int month) {
    static int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && isLeap(year))
        return 29;
    return days[month - 1];
}
//date.cpp实现
Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
    if(y<1||m<1||m>12||d<1||d>getMonthDays(y, m)){
        throw std::runtime_error("Invalid date: "
            +std::to_string(y)+"-"
            +std::to_string(m)+"-"
            +std::to_string(d));
    }
}

Date Date::read(){
    int y, m, d;
    std::cin>>y>>m>>d;
    return Date(y, m, d);
}

int Date::toDays() const {
    int days = 0;
    for (int y = 1; y < year; ++y)         
        days += isLeap(y) ? 366 : 365;
    for (int m = 1; m < month; ++m)        
        days += monthDays(year, m);
    days += day - 1;                       
    return days;
}

int Date::operator-(const Date& other) const {
    return toDays() - other.toDays();
}

void Date::show() const {
    cout << year << "-" << month << "-" << day;
}

int Date::getMonthDays(int year, int month){
    return monthDays(year, month);
}

bool Date::isLeapYear(int year){
    return isLeap(year);
}

int Date::getMaxDay()const{
    return getMonthDays(year, month);
}

bool Date::operator<(const Date& other)const{
    return toDays() < other.toDays();
}

#ifndef DATE_H
#define DATE_H

#include <stdexcept>
#include <string>

class Date {
private:
    int year, month, day;

public:
    //构造函数，若年月日非法，抛出runtime_error
    Date(int y = 0, int m = 0, int d = 0);
    //静态输入函数，输入非法时抛出异常
    static Date read();

    int getYear()const {return year;}
    int getMonth()const {return month;}
    int getDay()const {return day;}

    int toDays() const; 


    int operator-(const Date& other) const;

    void show() const; 

    int getMaxDay()const;

    static int getMonthDays(int year, int month);
    static bool isLeapYear(int year); 
    
    bool operator<(const Date& other)const;

};

#endif
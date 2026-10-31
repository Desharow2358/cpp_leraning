#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include "date.h"

class Accumulator{
    Date lastDate;
    double value;
    double sum;

    public:
    Accumulator(const Date& date, double val)
    : lastDate(date), value(val), sum(0){}

    void change(const Date& date, double newValue){
        int days=date-lastDate;
        sum+=value*days;
        value=newValue;
        lastDate=date;
    }

    void accumulate(const Date& date){
        sum+=value*(date-lastDate);
        lastDate=date;
    }

    double getSum()const{return sum;}
    void resetSum(){sum=0;}
};

#endif
// account.cpp
#include "account.h"
#include "account_exception.h"
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

// 静态成员初始化
double Account::total = 0;
multimap<Date, AccountRecord> Account::recordMap;
// ---------- Account 基类实现 ----------
Account::Account(const Date& date, const std::string& id) : id(id), balance(0) {
    date.show();
    cout << "\t#" << id << " created" << endl;
}

// ---------- SavingsAccount 实现 ----------
SavingsAccount::SavingsAccount(const Date& date, const string& id, double rate)
    : Account(date, id), rate(rate), acc(date, 0) {}

void SavingsAccount::deposit(const Date& date, double amount, const string& desc) {
    acc.change(date, balance);
    balance += amount;
    total += amount;
    acc.change(date, balance);
    record(date, amount, desc);

    date.show();
    cout << "\t#" << id << "\t" << amount << "\t" << balance;
    if (!desc.empty()) cout << "\t" << desc;
    cout << endl;
}

void SavingsAccount::withdraw(const Date& date, double amount, const string& desc) {
    acc.change(date, balance);
    balance -= amount;
    total -= amount;
    acc.change(date, balance);
    record(date, -amount, desc);

    date.show();
    cout << "\t#" << id << "\t-" << amount << "\t" << balance;
    if (!desc.empty()) cout << "\t" << desc;
    cout << endl;
}

void SavingsAccount::settle(const Date& date) {
    acc.accumulate(date);
    if(date.getMonth()==1&&date.getDay()==1){
        double interest=acc.getSum()*rate/366.0;
        interest=floor(interest*100+0.5)/100;
        if(interest!=0){
            balance+=interest;
            total+=interest;
            record(date, interest, "interest");
        }

        acc.resetSum();
        acc.change(date, balance);
        date.show();


        cout<<"\t#"<<id<<"\t"<<interest<<"\t"<<balance<<"\tinterest"<<endl;
    }else{
        acc.change(date, balance);
    }
}

void SavingsAccount::show()const{
    cout<<"Balance: "<<balance;
}

// ---------- CreditAccount 实现 ----------
CreditAccount::CreditAccount(const Date& date, const string& id, double credit, double rate, double fee)
    : Account(date, id), credit(credit), rate(rate), fee(fee), debtAcc(date, 0), lastSettleDate(date) {}

void CreditAccount::deposit(const Date& date, double amount, const string& desc) {
    double oldDebt = getDebt();
    debtAcc.change(date, oldDebt);

    balance += amount;
    total += amount;

    double newDebt = getDebt();
    debtAcc.change(date, newDebt);

    record(date, amount, desc);
    date.show();
    // 修正：去掉 '+' 符号，与期望输出一致
    cout << "\t#" << id << "\t" << amount << "\t" << balance;
    if (!desc.empty()) cout << "\t" << desc;
    cout << endl;
}

void CreditAccount::withdraw(const Date& date, 
    double amount, const string& desc) {
    double newBalance = balance - amount;
    double newDebt = newBalance < 0 ? -newBalance : 0;
    if (newDebt > credit) {
        //抛出带账户信息的异常
        throw AccountException("Credit limit exceeded", this);
    }

    double oldDebt = getDebt();
    debtAcc.change(date, oldDebt);

    balance = newBalance;
    total -= amount;

    double newDebtAfter = getDebt();
    debtAcc.change(date, newDebtAfter);

    record(date, -amount, desc);
    date.show();
    cout << "\t#" << id << "\t-" << amount << "\t" << balance;
    if (!desc.empty()) cout << "\t" << desc;
    cout << endl;
}

void CreditAccount::settle(const Date& date) {
    debtAcc.accumulate(date);
    if(date.getDay()==1){
    double debtSum = debtAcc.getSum();

    double interest = debtSum * rate;
    interest = floor(interest * 100 + 0.5) / 100;
    if (interest != 0) {
        balance -= interest;
        total -= interest;
        record(date, -interest, "interest");

        date.show();
        cout << "\t#" << id << "\t" << -interest << "\t" << balance << "\tinterest" << endl;
    }
    debtAcc.resetSum();
}

    if (date.getMonth() == 1 && date.getDay() == 1) {
        balance -= fee;
        total -= fee;
        record(date, -fee, "annual fee");

        date.show();
        cout << "\t#" << id << "\t" << -fee << "\t" << balance << "\tannual fee" << endl;
    }
 
    debtAcc.change(date, getDebt());
}

// 补充缺失的 show() 实现
void CreditAccount::show() const {
    cout<< "Balance: " << balance
         << "\tAvailable credit: " << getAvailableCredit();
}

//AccountRecord

AccountRecord::AccountRecord(const Date& d, const Account* acc, double amt, double bal, const string& dsc)
: date(d), account(acc), amount(amt), balance(bal), desc(dsc){}

void AccountRecord::show()const{
    date.show();
    cout<<"\t#"<<account->getId()<<"\t"<<amount<<"\t"<<balance;
    if(!desc.empty())cout<<"\t"<<desc;
    cout<<endl;
}

void Account::query(const Date& begin, const Date& end){
    auto it_low=recordMap.lower_bound(begin);
    auto it_up=recordMap.upper_bound(end);
    for(auto it=it_low; it!=it_up; ++it){
        it->second.show();
    }
}


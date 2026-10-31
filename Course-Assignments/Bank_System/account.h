// account.h
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "date.h"
#include "accumulator.h"
#include <map>
#include <vector>
#include <string>
#include <utility>

class Account;

class AccountRecord{
    private:
    Date date;
    const Account *account;
    double amount;
    double balance;
    std::string desc;

    public:
    AccountRecord(const Date& d, const Account* acc, double amt, double bal, const std::string& dsc);
    void show()const;

    const Date& getDate()const {return date;}
    double getAmount()const {return amount;}
    const Account* getAccount()const {return account;}
};

class Account{
    protected:
    std::string id;
    double balance;
    static double total;
    static std::multimap<Date, AccountRecord> recordMap;

    struct Record{
        Date date;
        double amount;
        std::string desc;
    };
    std::vector<Record>records;

    void record(const Date& date, double amount, 
        const std::string& desc){records.push_back({date, amount, desc});

    recordMap.insert({date, AccountRecord(date, this, amount, balance, desc)});}
    

    public:
    Account(const Date& date, const std::string& id);

    virtual ~Account(){}

    const std::string& getId()const{return id;}
    double getBalance()const {return balance;}
    static double getTotal(){return total;}

    virtual void deposit(const Date& date, 
        double amount, const std::string& desc="")=0;

    virtual void withdraw(const Date& date, 
        double amount, const std::string& desc="")=0;

    virtual void settle(const Date& date)=0;

    virtual void show() const =0;

    static void query(const Date& begin, const Date& end);

    static void resetTotal(){total=0;}
    static void clearRecords(){recordMap.clear();}

    static const std::multimap<Date, AccountRecord>& getRecordMap(){
        return recordMap;
    }
};

class SavingsAccount:public Account {
private:
    double rate;
    Accumulator acc;

public:
    SavingsAccount(const Date& date, const std::string& id, double rate);

    void deposit(const Date& date, double amount,
         const std::string& desc = "")override;
    void withdraw(const Date& date, double amount, 
        const std::string& desc = "")override;
    void settle(const Date& date)override;          
    void show() const override;                      
};

class CreditAccount:public Account{
    double credit;
    double rate;
    double fee;
    Accumulator debtAcc;
    Date lastSettleDate;

    double getDebt()const{
        return balance<0?-balance:0;
    }

    public:
    CreditAccount(const Date& date, 
        const std::string& id, double credit, 
        double rate, double fee);
    double getCredit() const { return credit; }
    double getRate() const { return rate; }
    double getFee() const { return fee; }
    double getAvailableCredit() const { return credit - getDebt(); }

    void deposit(const Date& date, double amount, 
        const std::string& desc = "") override;
    void withdraw(const Date& date, double amount, 
        const std::string& desc = "") override;
    void settle(const Date& date) override;
    void show() const override;
};

#endif
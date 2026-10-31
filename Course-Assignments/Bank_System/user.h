// user.h
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "account.h"

class User {
private:
    std::string username;
    std::string password;
    std::vector<Account*> accounts;  // 该用户拥有的账户
    bool belongsToUser(const Account* acc)const;
    
public:
    User(const std::string& name, const std::string& pwd);
    const std::string& getPassword()const {return password;}
    const std::string& getName() const { return username; }
    bool checkPassword(const std::string& pwd) const;
    void addAccount(Account* acc);
    Account* getAccountByIdx(int idx) const;
    size_t getAccountCount() const { return accounts.size(); }
    void showAccounts() const;
    void depositTo(int idx, const Date& date, double amt, const std::string& desc);
    void withdrawFrom(int idx, const Date& date, double amt, const std::string& desc);
    // 提示还款：遍历信用账户，若balance<0则提醒
    void checkRepaymentReminder() const;
    // 月度统计：计算某月收入和支出总额
    std::pair<double, double> monthlyStat(int year, int month) const;
    // 按时间排序查询某月记录
    void queryByDate(int year, int month) const;
    // 按金额排序查询某月记录
    void queryByAmount(int year, int month) const;
    ~User();
};

#endif
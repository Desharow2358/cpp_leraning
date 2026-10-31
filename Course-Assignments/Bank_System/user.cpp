#include "user.h"// user.cpp

#include "account.h"
#include "date.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// ---------- User 类实现 ----------
User::User(const string& name, const string& pwd)
    : username(name), password(pwd) {}

User::~User() {
    for (auto acc : accounts) {
        delete acc;
    }
    accounts.clear();
}

bool User::checkPassword(const string& pwd) const {
    return password == pwd;
}

void User::addAccount(Account* acc) {
    accounts.push_back(acc);
}

Account* User::getAccountByIdx(int idx) const {
    if (idx >= 0 && idx < (int)accounts.size()) {
        return accounts[idx];
    }
    return nullptr;
}

void User::showAccounts() const {
    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << "[" << i << "] " << accounts[i]->getId() << "\t";
        accounts[i]->show();
        cout << endl;
    }
}

void User::depositTo(int idx, const Date& date, double amt, const string& desc) {
    Account* acc = getAccountByIdx(idx);
    if (acc) {
        acc->deposit(date, amt, desc);
    } else {
        cerr << "Invalid account index: " << idx << endl;
    }
}

void User::withdrawFrom(int idx, const Date& date, double amt, const string& desc) {
    Account* acc = getAccountByIdx(idx);
    if (acc) {
        acc->withdraw(date, amt, desc);
    } else {
        cerr << "Invalid account index: " << idx << endl;
    }
}

void User::checkRepaymentReminder() const {
    for (auto acc : accounts) {
        CreditAccount* ca = dynamic_cast<CreditAccount*>(acc);
        if (ca && ca->getBalance() < 0) {
            cout << "Reminder: Credit account " << ca->getId()
                 << " has debt " << -ca->getBalance() << endl;
        }
    }
}

pair<double, double> User::monthlyStat(int year, int month) const {
    double income = 0.0, expense = 0.0;
    Date begin(year, month, 1);
    Date end(year, month, Date::getMonthDays(year, month));

    // 遍历全局 recordMap，筛选属于当前用户账户的记录
    const auto& recordMap = Account::getRecordMap(); // 需要 Account 提供静态访问函数
    auto it_low = recordMap.lower_bound(begin);
    auto it_up = recordMap.upper_bound(end);
    for (auto it = it_low; it != it_up; ++it) {
        const AccountRecord& rec = it->second;
        // 检查该记录所属账户是否属于当前用户
        const Account* acc = rec.getAccount(); // 需要在 AccountRecord 中添加 getAccount()
        if (acc && belongsToUser(acc)) {       // 需要实现 belongsToUser
            double amt = rec.getAmount();      // 需要在 AccountRecord 中添加 getAmount()
            if (amt > 0) income += amt;
            else expense -= amt;               // expense 记录正数支出
        }
    }
    return {income, expense};
}

void User::queryByDate(int year, int month) const {
    Date begin(year, month, 1);
    Date end(year, month, Date::getMonthDays(year, month));

    const auto& recordMap = Account::getRecordMap();
    auto it_low = recordMap.lower_bound(begin);
    auto it_up = recordMap.upper_bound(end);
    vector<AccountRecord> records;
    for (auto it = it_low; it != it_up; ++it) {
        if (belongsToUser(it->second.getAccount())) {
            records.push_back(it->second);
        }
    }
    // 按时间排序（recordMap 已按日期排序，但为了明确，可以再排一次）
    sort(records.begin(), records.end(),
         [](const AccountRecord& a, const AccountRecord& b) {
             return a.getDate() < b.getDate(); // 需要在 AccountRecord 中添加 getDate()
         });
    for (const auto& rec : records) {
        rec.show();
    }
}

void User::queryByAmount(int year, int month) const {
    Date begin(year, month, 1);
    Date end(year, month, Date::getMonthDays(year, month));

    const auto& recordMap = Account::getRecordMap();
    auto it_low = recordMap.lower_bound(begin);
    auto it_up = recordMap.upper_bound(end);
    vector<AccountRecord> records;
    for (auto it = it_low; it != it_up; ++it) {
        if (belongsToUser(it->second.getAccount())) {
            records.push_back(it->second);
        }
    }
    // 按金额绝对值降序排序
    sort(records.begin(), records.end(),
         [](const AccountRecord& a, const AccountRecord& b) {
             return fabs(a.getAmount()) > fabs(b.getAmount());
         });
    for (const auto& rec : records) {
        rec.show();
    }
}

// 辅助函数：判断账户是否属于当前用户（通过比较账户指针是否在 accounts 中）
bool User::belongsToUser(const Account* acc) const {
    if (!acc) return false;
    for (const auto& myAcc : accounts) {
        if (myAcc == acc) return true;
    }
    return false;
}
#ifndef ACCOUNT_EXCEPTION_H
#define ACCOUNT_EXCEPTION_H

#include <stdexcept>
#include <string>
class Account;  // 前置声明

class AccountException : public std::runtime_error {
private:
    const Account* account;
public:
    AccountException(const std::string& msg, const Account* acc)
        : std::runtime_error(msg), account(acc) {}
    const Account* getAccount() const { return account; }
};

#endif
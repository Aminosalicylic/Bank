#ifndef USER_H
#define USER_H

#include "linkedlist.h"
#include "account.h"
#include <QString>

class User {
protected:
    QString username;
    QString password;
    QString name;
    QString familyName;
    QString nationalCode;
    int age;

public:
    User(QString uname, QString pass, QString fname, QString lname, QString natCode, int age);
    virtual ~User() {}

    // Getters
    QString getUsername() const;
    QString getName() const;
    QString getFamilyName() const;

    bool checkPassword(const QString& pass) const;
    void setPassword(const QString& newPass);
    void setName(const QString& newName);
    void setFamilyName(const QString& newFamilyName);

    virtual bool isAdmin() const = 0;
};


class Customer : public User {
private:
    LinkedList<Account*> accounts;
    QString dynamicSecondPin;
    QDateTime lastTransactionTime;
    double dailyTransferSum;

public:
    Customer(QString uname, QString pass, QString fname, QString lname, QString natCode, int age);
    ~Customer();

    bool isAdmin() const override { return false; }
    void addAccount(Account* account);
    LinkedList<Account*>& getAccounts();


    void generateAndSetDynamicPin();
    QString getDynamicPin() const;
    void consumeDynamicPin();


    void resetDailyTransferIfNeeded();
    void add_to_daily_transfer(double amount);
    double get_daily_transfer_sum() const;
};


class Admin : public User {
public:
    Admin(QString uname, QString pass, QString fname, QString lname, QString natCode, int age);
    bool isAdmin() const override { return true; }
};

#endif // USER_H

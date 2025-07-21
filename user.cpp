#include "user.h"
#include <QRandomGenerator>

User::User(QString uname, QString pass, QString fname, QString lname, QString natCode, int userAge)
    : username(uname),
    password(pass),
    name(fname),
    familyName(lname),
    nationalCode(natCode),
    age(userAge)
{}

QString User::getUsername() const { return username; }
QString User::getName() const { return name; }
QString User::getFamilyName() const { return familyName; }

bool User::checkPassword(const QString& pass) const {
    return password == pass;
}

void User::setPassword(const QString& newPass) {
    if (!newPass.isEmpty()) {
        password = newPass;
    }
}

void User::setName(const QString& newName) {
    if (!newName.trimmed().isEmpty()) {
        this->name = newName;
    }
}

void User::setFamilyName(const QString& newFamilyName) {
    if (!newFamilyName.trimmed().isEmpty()) {
        this->familyName = newFamilyName;
    }
}

Admin::Admin(QString uname, QString pass, QString fname, QString lname, QString natCode, int userAge)
    : User(uname, pass, fname, lname, natCode, userAge)
{}

Customer::Customer(QString uname, QString pass, QString fname, QString lname, QString natCode, int userAge)
    : User(uname, pass, fname, lname, natCode, userAge),
    dynamicSecondPin(""),
    dailyTransferSum(0.0)
{
    lastTransactionTime = QDateTime::currentDateTime();
}

Customer::~Customer() {

    for (int i = 0; i < accounts.size(); ++i) {
        delete accounts.at(i);
    }
}

void Customer::addAccount(Account* account) {
    if (accounts.size() < 5) {
        accounts.append(account);
    }
}

LinkedList<Account*>& Customer::getAccounts() {
    return accounts;
}

void Customer::generateAndSetDynamicPin() {

    int randomPin = QRandomGenerator::global()->bounded(100000, 999999);
    dynamicSecondPin = QString::number(randomPin);
}

QString Customer::getDynamicPin() const {
    return dynamicSecondPin;
}

void Customer::consumeDynamicPin() {

    dynamicSecondPin = "";
}

void Customer::resetDailyTransferIfNeeded() {
    QDateTime now = QDateTime::currentDateTime();

    if (lastTransactionTime.daysTo(now) >= 1) {
        dailyTransferSum = 0.0;
        lastTransactionTime = now;
    }
}

void Customer::add_to_daily_transfer(double amount) {
    dailyTransferSum += amount;
    lastTransactionTime = QDateTime::currentDateTime();
}

double Customer::get_daily_transfer_sum() const {
    return dailyTransferSum;
}

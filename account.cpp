#include "account.h"
#include <QRandomGenerator>

Account::Account(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance)
    : accountNumber(accNum),
    shebaNumber(sheba),
    cardNumber(cardNum),
    cvv2(cvv),
    expiryDate(expiry),
    balance(initialBalance)
{

    pin = "1234";
    staticSecondPin = "1111";
}

QString Account::getAccountNumber() const { return accountNumber; }
QString Account::getCardNumber() const { return cardNumber; }
double Account::getBalance() const { return balance; }
QDate Account::getExpiryDate() const { return expiryDate; }
QString Account::getPin() const { return pin; }
QString Account::getStaticSecondPin() const { return staticSecondPin; }

void Account::setPin(const QString& newPin) {
    if (newPin.length() == 4 && newPin.toInt()) {
        pin = newPin;
    }
}

void Account::setStaticSecondPin(const QString& newPin) {
    if (newPin.length() >= 4) {
        staticSecondPin = newPin;
    }
}

void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

bool Account::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

DepositAccount::DepositAccount(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance)
    : Account(accNum, sheba, cardNum, cvv, expiry, initialBalance) {}

QString DepositAccount::getAccountType() const {
    return "حساب سپرده";
}

void DepositAccount::applyMonthlyInterest() {

    double interest = getBalance() * 0.015;
    deposit(interest);
}

CurrentAccount::CurrentAccount(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance)
    : Account(accNum, sheba, cardNum, cvv, expiry, initialBalance) {}

QString CurrentAccount::getAccountType() const {
    return "حساب جاری";
}

void CurrentAccount::issueChequeBook() {

    qDebug() << "A cheque book has been requested for account " << getAccountNumber();
}

QarzolHasanehAccount::QarzolHasanehAccount(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance)
    : Account(accNum, sheba, cardNum, cvv, expiry, initialBalance) {}

QString QarzolHasanehAccount::getAccountType() const {
    return "حساب قرض‌الحسنه";
}

bool QarzolHasanehAccount::canRequestLoan() {

    return getBalance() > 1000000;
}

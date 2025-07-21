#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QDateTime>

class Account {
protected:
    QString accountNumber;
    QString shebaNumber;
    QString cardNumber;
    QString cvv2;
    QDate expiryDate;
    QString pin;
    QString staticSecondPin;
    double balance;

public:
    Account(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance);
    virtual ~Account() {}


    QString getAccountNumber() const;
    QString getCardNumber() const;
    double getBalance() const;
    QDate getExpiryDate() const;
    QString getPin() const;
    QString getStaticSecondPin() const;


    void setPin(const QString& newPin);
    void setStaticSecondPin(const QString& newPin);

    void deposit(double amount);
    bool withdraw(double amount);


    virtual QString getAccountType() const = 0;
};



class DepositAccount : public Account {
public:
    DepositAccount(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance);
    QString getAccountType() const override;
    void applyMonthlyInterest();
};

class CurrentAccount : public Account {
public:
    CurrentAccount(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance);
    QString getAccountType() const override;
    void issueChequeBook();
};

class QarzolHasanehAccount : public Account {
public:
    QarzolHasanehAccount(QString accNum, QString sheba, QString cardNum, QString cvv, QDate expiry, double initialBalance);
    QString getAccountType() const override;
    bool canRequestLoan();
};

#endif // ACCOUNT_H

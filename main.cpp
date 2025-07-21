#include "logindialog.h"
#include "linkedlist.h"
#include "user.h"
#include "account.h"
#include <QApplication>

LinkedList<User*> database;

void setupInitialData() {

    Admin* admin = new Admin("admin", "1234", "مدیریت", "سیستم", "0000000000", 30);
    database.append(admin);

    Customer* customer = new Customer("amin", "1234", "محمد امین", "تاجیک", "2111236888", 25);

    Account* acc1 = new DepositAccount("0001", "IR0001", "1111-2222-3333-4444", "123", QDate(2025, 12, 1), 5000000);
    customer->addAccount(acc1);

    database.append(customer);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setupInitialData();

    LoginDialog w(&database);
    w.show();

    return a.exec();
}

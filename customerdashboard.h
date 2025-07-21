#ifndef CUSTOMERDASHBOARD_H
#define CUSTOMERDASHBOARD_H

#include <QMainWindow>
#include <QMessageBox>
#include<QListWidgetItem>
#include "account.h"
#include "user.h"


namespace Ui {
class CustomerDashboard;
}

class CustomerDashboard : public QMainWindow
{
    Q_OBJECT

public:

    CustomerDashboard(Customer* customer, QWidget *parent = nullptr);
    ~CustomerDashboard();

private slots:

    void on_pushButton_transfer_clicked();

    void on_pushButton_getDynamicPin_clicked();

    void on_comboBox_sourceCard_currentIndexChanged(int index);

    void on_listWidget_accounts_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btn_changePassword_clicked();
private:
    Ui::CustomerDashboard *ui;
    Customer* m_customer;
    void populateAccountComboBox();
    void updateAccountInfo();
    Account* getSelectedAccount();
    void populateAccountList();
    void loadProfileInfo();
};

#endif // CUSTOMERDASHBOARD_H

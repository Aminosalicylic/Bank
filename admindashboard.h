#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include <QRandomGenerator>
#include "user.h"
#include "linkedlist.h"

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QMainWindow
{
    Q_OBJECT

public:

    explicit AdminDashboard(Admin* admin, LinkedList<User*>* database, QWidget *parent = nullptr);
    ~AdminDashboard();

private slots:

    void on_btn_refreshUsers_clicked();
    void on_btn_deleteUser_clicked();
    void on_btn_addUser_clicked();
    void on_btn_editUser_clicked();
    void on_btn_refreshAccounts_clicked();
    void on_btn_addAccount_clicked();
    void on_btn_changePassword_clicked();

private:
    Ui::AdminDashboard *ui;
    Admin* m_admin;
    LinkedList<User*>* m_database;


    void populateUserTable();
    void populateAccountTable();
    void loadProfileInfo();
};

#endif // ADMINDASHBOARD_H

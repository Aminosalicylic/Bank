#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include "user.h"
#include "linkedlist.h"

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(LinkedList<User*>* database, QWidget *parent = nullptr);
    ~AddUserDialog();

    Customer* getNewCustomer() const;

private slots:
    void accept() override;

private:
    Ui::AddUserDialog *ui;
    LinkedList<User*>* m_database;
    Customer* m_newCustomer;
};

#endif // ADDUSERDIALOG_H

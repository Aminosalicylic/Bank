#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QMainWindow>
#include <QDialog>
#include"user.h"

namespace Ui {
class EditUserDialog;
}

class EditUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserDialog(User* user, QWidget *parent = nullptr);
    ~EditUserDialog();
    void accept();
private:
    User* m_targetUser;
    Ui::EditUserDialog *ui;
};

#endif // EDITUSERDIALOG_H

#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QMainWindow>
#include <QDialog>

namespace Ui {
class AddAccountDialog;
}

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
    ~AddAccountDialog();
    QString getSelectedAccountType() const;
    double getInitialBalance() const;

private:
    Ui::AddAccountDialog *ui;
};

#endif // ADDACCOUNTDIALOG_H

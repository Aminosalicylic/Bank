#include "addaccountdialog.h"
#include "ui_addaccountdialog.h"

AddAccountDialog::AddAccountDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddAccountDialog) {
    ui->setupUi(this);
    ui->comboBox_accountType->addItems({"حساب سپرده", "حساب جاری", "حساب قرض‌الحسنه"});
}
QString AddAccountDialog::getSelectedAccountType() const { return ui->comboBox_accountType->currentText(); }
double AddAccountDialog::getInitialBalance() const { return ui->lineEdit_initialBalance->text().toDouble(); }

AddAccountDialog::~AddAccountDialog()
{
    delete ui;
}

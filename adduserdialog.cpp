#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include <QMessageBox>

AddUserDialog::AddUserDialog(LinkedList<User*>* database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog),
    m_database(database),
    m_newCustomer(nullptr)
{
    ui->setupUi(this);
}

AddUserDialog::~AddUserDialog()
{

    if (!this->result()) {
        delete m_newCustomer;
    }
    delete ui;
}

Customer* AddUserDialog::getNewCustomer() const
{
    return m_newCustomer;
}

void AddUserDialog::accept()
{

    QString name = ui->lineEdit_name->text().trimmed();
    QString family = ui->lineEdit_family->text().trimmed();
    QString natCode = ui->lineEdit_nationalCode->text().trimmed();
    QString ageStr = ui->lineEdit_age->text().trimmed();
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();
    QString confirmPass = ui->lineEdit_confirmPassword->text();


    if (name.isEmpty() || family.isEmpty() || natCode.isEmpty() || ageStr.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "تمام فیلدها باید پر شوند.");
        return;
    }

    if (password != confirmPass) {
        QMessageBox::warning(this, "خطا", "رمز عبور با تکرار آن مطابقت ندارد.");
        return;
    }

    if (password.length() < 4) {
        QMessageBox::warning(this, "خطا", "رمز عبور باید حداقل ۴ کاراکتر باشد.");
        return;
    }


    if (m_database->find([&](User* u){ return u->getUsername() == username; })) {
        QMessageBox::warning(this, "خطا", "این نام کاربری قبلاً استفاده شده است.");
        return;
    }

    bool isAgeOk;
    int age = ageStr.toInt(&isAgeOk);
    if (!isAgeOk || age <= 0) {
        QMessageBox::warning(this, "خطا", "سن وارد شده معتبر نیست.");
        return;
    }

    m_newCustomer = new Customer(username, password, name, family, natCode, age);

    QDialog::accept();
}

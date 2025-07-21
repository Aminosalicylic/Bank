#include "admindashboard.h"
#include "ui_admindashboard.h"
#include <QMessageBox>
#include <QInputDialog>
#include "addaccountdialog.h"
#include "edituserdialog.h"
#include "adduserdialog.h"

void AdminDashboard::on_btn_addAccount_clicked() {
    auto selectedItems = ui->tableWidget_users->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً یک مشتری را از جدول کاربران انتخاب کنید.");
        return;
    }
    QString username = ui->tableWidget_users->item(selectedItems.first()->row(), 0)->text();
    User* user = m_database->find([&](User* u){ return u->getUsername() == username; });

    if (!user || user->isAdmin()) {
        QMessageBox::warning(this, "خطا", "فقط برای مشتریان می‌توان حساب باز کرد.");
        return;
    }
    Customer* customer = static_cast<Customer*>(user);

    AddAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {

        QString accNum = QString::number(QRandomGenerator::global()->bounded(1000, 9999));
        QString cardNum = "6037-" + QString::number(QRandomGenerator::global()->bounded(1000, 9999)) + "-....";

        Account* newAccount = nullptr;
        if (dialog.getSelectedAccountType() == "حساب سپرده") {
            newAccount = new DepositAccount(accNum, "IR...", cardNum, "123", QDate::currentDate().addYears(4), dialog.getInitialBalance());
        }

        if (newAccount) {
            customer->addAccount(newAccount);
            QMessageBox::information(this, "موفق", "حساب جدید با موفقیت ایجاد شد.");
            populateAccountTable();
        }
    }
}

void AdminDashboard::on_btn_editUser_clicked() {
    auto selectedItems = ui->tableWidget_users->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً یک کاربر را انتخاب کنید.");
        return;
    }
    QString username = ui->tableWidget_users->item(selectedItems.first()->row(), 0)->text();
    User* userToEdit = m_database->find([&](User* u){ return u->getUsername() == username; });

    if (userToEdit) {
        EditUserDialog dialog(userToEdit, this);
        if (dialog.exec() == QDialog::Accepted) {
            QMessageBox::information(this, "موفق", "اطلاعات کاربر با موفقیت ویرایش شد.");
            populateUserTable();
        }
    }
}

AdminDashboard::AdminDashboard(Admin* admin, LinkedList<User*>* database, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard),
    m_admin(admin),
    m_database(database)
{
    ui->setupUi(this);

    ui->tableWidget_users->setColumnCount(5);
    ui->tableWidget_users->setHorizontalHeaderLabels({"نام کاربری", "نام", "نام خانوادگی", "کد ملی", "نوع کاربر"});
    ui->tableWidget_users->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_users->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget_accounts->setColumnCount(5);
    ui->tableWidget_accounts->setHorizontalHeaderLabels({"شماره حساب", "شماره کارت", "نوع حساب", "موجودی", "صاحب حساب"});
    ui->tableWidget_accounts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_accounts->setSelectionBehavior(QAbstractItemView::SelectRows);

    loadProfileInfo();
    populateUserTable();
    populateAccountTable();
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::populateUserTable()
{
    ui->tableWidget_users->setRowCount(0);

    for (int i = 0; i < m_database->size(); ++i) {
        User* user = m_database->at(i);
        if (!user) continue;

        int row = ui->tableWidget_users->rowCount();
        ui->tableWidget_users->insertRow(row);

        ui->tableWidget_users->setItem(row, 0, new QTableWidgetItem(user->getUsername()));
        ui->tableWidget_users->setItem(row, 1, new QTableWidgetItem(user->getName()));
        ui->tableWidget_users->setItem(row, 2, new QTableWidgetItem(user->getFamilyName()));

        QString userType = user->isAdmin() ? "ادمین" : "مشتری";
        ui->tableWidget_users->setItem(row, 4, new QTableWidgetItem(userType));
    }
}

void AdminDashboard::populateAccountTable()
{
    ui->tableWidget_accounts->setRowCount(0);

    for (int i = 0; i < m_database->size(); ++i) {
        User* user = m_database->at(i);

        if (!user->isAdmin()) {
            Customer* customer = static_cast<Customer*>(user);
            LinkedList<Account*>& accounts = customer->getAccounts();

            for (int j = 0; j < accounts.size(); ++j) {
                Account* acc = accounts.at(j);
                if (!acc) continue;

                int row = ui->tableWidget_accounts->rowCount();
                ui->tableWidget_accounts->insertRow(row);

                ui->tableWidget_accounts->setItem(row, 0, new QTableWidgetItem(acc->getAccountNumber()));
                ui->tableWidget_accounts->setItem(row, 1, new QTableWidgetItem(acc->getCardNumber()));
                ui->tableWidget_accounts->setItem(row, 2, new QTableWidgetItem(acc->getAccountType()));
                ui->tableWidget_accounts->setItem(row, 3, new QTableWidgetItem(QString::number(acc->getBalance())));
                ui->tableWidget_accounts->setItem(row, 4, new QTableWidgetItem(customer->getUsername()));
            }
        }
    }
}

void AdminDashboard::on_btn_refreshUsers_clicked()
{
    populateUserTable();
    QMessageBox::information(this, "تازه‌سازی", "لیست کاربران با موفقیت به‌روز شد.");
}

void AdminDashboard::on_btn_deleteUser_clicked()
{
    auto selectedItems = ui->tableWidget_users->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً ابتدا یک کاربر را برای حذف انتخاب کنید.");
        return;
    }

    int row = selectedItems.first()->row();
    QString usernameToDelete = ui->tableWidget_users->item(row, 0)->text();
    QString userType = ui->tableWidget_users->item(row, 4)->text();

    if (userType == "ادمین") {
        QMessageBox::critical(this, "خطا", "شما نمی‌توانید یک ادمین را حذف کنید.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "تایید حذف", "آیا از حذف کاربر " + usernameToDelete + " مطمئن هستید؟",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "انجام شد", "کاربر حذف شد (شبیه‌سازی شده). لیست را تازه‌سازی کنید.");

    }
}

void AdminDashboard::on_btn_refreshAccounts_clicked()
{
    populateAccountTable();
    QMessageBox::information(this, "تازه‌سازی", "لیست حساب‌ها با موفقیت به‌روز شد.");
}

void AdminDashboard::on_btn_addUser_clicked()
{
    AddUserDialog dialog(m_database, this);

    if (dialog.exec() == QDialog::Accepted) {
        Customer* newCustomer = dialog.getNewCustomer();

        if (newCustomer) {
            m_database->append(newCustomer);
            QMessageBox::information(this, "موفقیت", "مشتری جدید با نام کاربری '" + newCustomer->getUsername() + "' با موفقیت اضافه شد.");
            populateUserTable();
        }
    }
}

void AdminDashboard::loadProfileInfo()
{
    if (m_admin) {
        ui->lbl_profile_name->setText(m_admin->getName());
        ui->lbl_profile_family->setText(m_admin->getFamilyName());
        ui->lbl_profile_username->setText(m_admin->getUsername());
    }
}

void AdminDashboard::on_btn_changePassword_clicked()
{
    QString currentPass = ui->lineEdit_currentPass->text();
    QString newPass = ui->lineEdit_newPass->text();
    QString confirmPass = ui->lineEdit_confirmPass->text();

    if (!m_admin->checkPassword(currentPass)) {
        ui->lbl_profile_status->setText("رمز عبور فعلی اشتباه است.");
        ui->lbl_profile_status->setStyleSheet("color: red;");
        return;
    }
    if (newPass.isEmpty() || newPass.length() < 4) {
        ui->lbl_profile_status->setText("رمز عبور جدید باید حداقل ۴ کاراکتر باشد.");
        ui->lbl_profile_status->setStyleSheet("color: red;");
        return;
    }
    if (newPass != confirmPass) {
        ui->lbl_profile_status->setText("رمز عبور جدید و تکرار آن مطابقت ندارند.");
        ui->lbl_profile_status->setStyleSheet("color: red;");
        return;
    }

    m_admin->setPassword(newPass);

    ui->lbl_profile_status->setText("رمز عبور با موفقیت تغییر کرد.");
    ui->lbl_profile_status->setStyleSheet("color: green;");

    ui->lineEdit_currentPass->clear();
    ui->lineEdit_newPass->clear();
    ui->lineEdit_confirmPass->clear();
}

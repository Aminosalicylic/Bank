#include"customerdashboard.h"
#include "ui_customerdashboard.h"

void CustomerDashboard::populateAccountComboBox()
{
    ui->comboBox_sourceCard->clear();
    LinkedList<Account*>& accounts = m_customer->getAccounts();
    for (int i = 0; i < accounts.size(); ++i) {
        Account* acc = accounts.at(i);

        QString displayText = acc->getAccountType() + " - " + acc->getCardNumber();
        ui->comboBox_sourceCard->addItem(displayText, QVariant::fromValue(acc->getAccountNumber()));
    }
}

void CustomerDashboard::updateAccountInfo()
{
    Account* selectedAccount = getSelectedAccount();
    if (!selectedAccount) return;
}

Account* CustomerDashboard::getSelectedAccount()
{
    int currentIndex = ui->comboBox_sourceCard->currentIndex();
    if (currentIndex < 0) {
        return nullptr;
    }

    if (currentIndex < m_customer->getAccounts().size()) {
        return m_customer->getAccounts().at(currentIndex);
    }

    return nullptr;
}

void CustomerDashboard::on_comboBox_sourceCard_currentIndexChanged(int index)
{

    updateAccountInfo();

    ui->label_transferStatus->clear();
    ui->lineEdit_secondPin->clear();
}

CustomerDashboard::CustomerDashboard(Customer* customer, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerDashboard),
    m_customer(customer)
{
    ui->setupUi(this);
    loadProfileInfo();
    populateAccountList();
    connect(ui->listWidget_accounts, &QListWidget::currentItemChanged, this, &CustomerDashboard::on_listWidget_accounts_currentItemChanged);

    if (ui->listWidget_accounts->count() > 0) {
        ui->listWidget_accounts->setCurrentRow(0);
    }
}


void CustomerDashboard::on_pushButton_transfer_clicked()
{
    QString destCardNumber = ui->lineEdit_destCard->text();
    double amount = ui->lineEdit_amount->text().toDouble();
    QString secondPin = ui->lineEdit_secondPin->text();
    Account* sourceAccount = getSelectedAccount();

    if (!sourceAccount) {
        ui->label_transferStatus->setText("خطا: حساب مبدا انتخاب نشده است.");
        return;
    }

    if (sourceAccount->getExpiryDate() < QDate::currentDate()) {
        ui->label_transferStatus->setText("خطا: کارت مبدا منقضی شده است.");
        return;
    }

    if (amount > 3000000) {
        ui->label_transferStatus->setText("خطا: سقف هر تراکنش ۳ میلیون تومان است.");
        return;
    }

    m_customer->resetDailyTransferIfNeeded();
    if (m_customer->get_daily_transfer_sum() + amount > 6000000) {
        ui->label_transferStatus->setText("خطا: از سقف انتقال روزانه عبور کرده‌اید.");
        return;
    }

    if (amount > 100000) {

        if (secondPin != m_customer->getDynamicPin() || secondPin.isEmpty()) {
            ui->label_transferStatus->setText("خطا: رمز دوم پویا نامعتبر است.");
            return;
        }
    } else {

        if (secondPin != sourceAccount->getStaticSecondPin()) {
            ui->label_transferStatus->setText("خطا: رمز دوم ثابت نامعتبر است.");
            return;
        }
    }

    double fee = amount * 0.0001;
    double totalDeduction = amount + fee;

    if (sourceAccount->getBalance() < totalDeduction) {
        ui->label_transferStatus->setText("خطا: موجودی کافی نیست.");
        return;
    }

    sourceAccount->withdraw(totalDeduction);

    m_customer->add_to_daily_transfer(amount);

    if (amount > 100000) {
        m_customer->consumeDynamicPin();
    }

    ui->label_transferStatus->setText("انتقال با موفقیت انجام شد.");

}


void CustomerDashboard::on_pushButton_getDynamicPin_clicked()
{
    m_customer->generateAndSetDynamicPin();
    QString pin = m_customer->getDynamicPin();
    QMessageBox::information(this, "رمز پویا", "رمز پویای شما: " + pin);
}

CustomerDashboard::~CustomerDashboard()
{
    delete ui;
}

void CustomerDashboard::populateAccountList()
{
    ui->listWidget_accounts->clear();
    LinkedList<Account*>& accounts = m_customer->getAccounts();
    for (int i = 0; i < accounts.size(); ++i) {
        Account* acc = accounts.at(i);
        QListWidgetItem* item = new QListWidgetItem(acc->getCardNumber(), ui->listWidget_accounts);
        item->setData(Qt::UserRole, QVariant::fromValue(acc));
    }
}

void CustomerDashboard::on_listWidget_accounts_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (!current) {

        ui->lbl_acc_type->setText("-");
        ui->lbl_acc_number->setText("-");
        ui->lbl_card_number->setText("-");
        ui->lbl_balance->setText("-");
        return;
    }


    Account* selectedAccount = current->data(Qt::UserRole).value<Account*>();

    if (selectedAccount) {
        ui->lbl_acc_type->setText(selectedAccount->getAccountType());
        ui->lbl_acc_number->setText(selectedAccount->getAccountNumber());
        ui->lbl_card_number->setText(selectedAccount->getCardNumber());
        ui->lbl_balance->setText(QString::number(selectedAccount->getBalance()));
    }
}

void CustomerDashboard::loadProfileInfo()
{
    if (m_customer) {
        ui->lbl_profile_name->setText(m_customer->getName());
        ui->lbl_profile_family->setText(m_customer->getFamilyName());
        ui->lbl_profile_username->setText(m_customer->getUsername());
    }
}

void CustomerDashboard::on_btn_changePassword_clicked()
{
    QString currentPass = ui->lineEdit_currentPass->text();
    QString newPass = ui->lineEdit_newPass->text();
    QString confirmPass = ui->lineEdit_confirmPass->text();

    // اعتبارسنجی‌ها
    if (!m_customer->checkPassword(currentPass)) {
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

    m_customer->setPassword(newPass);

    ui->lbl_profile_status->setText("رمز عبور با موفقیت تغییر کرد.");
    ui->lbl_profile_status->setStyleSheet("color: green;");

    ui->lineEdit_currentPass->clear();
    ui->lineEdit_newPass->clear();
    ui->lineEdit_confirmPass->clear();
}

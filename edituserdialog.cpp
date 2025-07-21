#include "edituserdialog.h"
#include "ui_edituserdialog.h"

EditUserDialog::EditUserDialog(User* user, QWidget *parent) :
    QDialog(parent), m_targetUser(user), ui(new Ui::EditUserDialog) {
    ui->setupUi(this);
    ui->lineEdit_name->setText(m_targetUser->getName());
    ui->lineEdit_family->setText(m_targetUser->getFamilyName());
    ui->label_username->setText(m_targetUser->getUsername());
}

void EditUserDialog::accept() {

    m_targetUser->setName(ui->lineEdit_name->text());
    m_targetUser->setFamilyName(ui->lineEdit_family->text());

    QString newPass = ui->lineEdit_newPassword->text();
    if (!newPass.isEmpty()) {
        m_targetUser->setPassword(newPass);
    }

    QDialog::accept();
}

EditUserDialog::~EditUserDialog()
{
    delete ui;
}

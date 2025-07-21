#include "logindialog.h"
#include "ui_logindialog.h"
#include "admindashboard.h"
#include "customerdashboard.h"

LoginDialog::LoginDialog(LinkedList<User*>* database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_database(database)
{
    ui->setupUi(this);

    this->setStyleSheet(
        "QDialog#LoginDialog {"
        "   background-image: url(:/background.jpg);"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "}"
        "QLabel {"
        "   color: white;"
        "   background-color: rgba(0, 0, 0, 0.5);"
        "   border-radius: 5px;"
        "   padding: 2px;"
        "}"
    );

    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);

    m_player->setAudioOutput(m_audioOutput);

    m_player->setSource(QUrl("qrc:/background.mp3"));

    m_audioOutput->setVolume(0.5); // 50% volume

    m_player->setLoops(QMediaPlayer::Infinite);

    m_player->play();

    this->setWindowTitle("ورود به سامانه بانکی");
    ui->lineEdit_username->setPlaceholderText("نام کاربری خود را وارد کنید");
    ui->lineEdit_password->setPlaceholderText("رمز عبور خود را وارد کنید");
}

LoginDialog::~LoginDialog()
{
    delete m_player;
    delete m_audioOutput;
    delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->label_status->setText("نام کاربری و رمز عبور نمی‌توانند خالی باشند.");
        ui->label_status->setStyleSheet("color: red;");
        return;
    }

    User* foundUser = m_database->find([&](User* u){
        return u->getUsername() == username;
    });

    if (foundUser && foundUser->checkPassword(password)) {
        ui->label_status->setText("ورود موفقیت‌آمیز!");
        ui->label_status->setStyleSheet("color: green;");

        if (foundUser->isAdmin()) {

            AdminDashboard* adminPanel = new AdminDashboard(static_cast<Admin*>(foundUser), m_database);
            adminPanel->show();
        } else {

            CustomerDashboard* customerPanel = new CustomerDashboard(static_cast<Customer*>(foundUser));
            customerPanel->show();
        }


    } else {
        ui->label_status->setText("نام کاربری یا رمز عبور اشتباه است.");
        ui->label_status->setStyleSheet("color: red;");
    }
}

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "user.h"
#include "linkedlist.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(LinkedList<User*>* database, QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::LoginDialog *ui;
    LinkedList<User*>* m_database;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
};

#endif // LOGINDIALOG_H

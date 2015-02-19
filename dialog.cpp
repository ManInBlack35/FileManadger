#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->label->setAlignment ( Qt::AlignHCenter | Qt::AlignTop); // вирівнювання
    ui->label_2 ->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::myexec(const QString & str)
{
    QString str_temp(str);
    if (str_temp.size()>60) //якщо дуже довгий шлях, частину замінюємо на ..
    {
        str_temp.replace(24,str_temp.size()-50,"..");
    }
    ui->label->setText(str_temp);
    return exec();
}

void Dialog::on_buttonBox_accepted()
{
    if (ui->checkBox->isChecked()) //якщо галочка натиснута
        emit check(true);
    else emit check(false); //якщо галочка не натиснута
    emit this->accept();
}

void Dialog::on_buttonBox_rejected() //відміна діалогового вікна (cansel)
{
    emit check(false);
    emit this->reject();
}

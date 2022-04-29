#include "dialog.h"
#include "ui_dialog.h"
#include"QPushButton"
#include<QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->dateTimeEdit_start->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime().addSecs(600));
}

Dialog::Dialog(QString title,QDateTime start,QDateTime end,QString detail,QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->lineEdit_title->setText(title);
    ui->dateTimeEdit_start->setDateTime(start);
    ui->dateTimeEdit_end->setDateTime(end);
    ui->textEdit->setText(detail);
}

Dialog::~Dialog()
{
    delete ui;

}

void Dialog::on_buttonBox_accepted()
{
    //从对应控件的内容获得对应的数据
    title=ui->lineEdit_title->text();

    start=ui->dateTimeEdit_start->dateTime();

    end=ui->dateTimeEdit_end->dateTime();

    detail=ui->textEdit->toPlainText();
}

void Dialog::on_buttonBox_rejected()
{

}

QString Dialog::getTitle()
{
    return title;
}

QDateTime Dialog::getstart()
{
    return start;
}

QDateTime Dialog::getend()
{
    return end;
}

QString Dialog::getdetail()
{
    return detail;
}

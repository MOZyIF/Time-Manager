#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QString>
#include<QDateTime>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    explicit Dialog(QString title,QDateTime start,QDateTime end,QString detail,QWidget *parent = 0);
    ~Dialog();
    QString getTitle();
    QDateTime getstart();
    QDateTime getend();
    QString getdetail();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
    QString title;//事件标题
    QDateTime start;//开始时间
    QDateTime end;//结束时间
    QString detail;//事件详情

};

#endif // DIALOG_H

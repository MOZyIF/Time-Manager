#ifndef SCHEDULE_H
#define SCHEDULE_H

#include<QString>
#include<QDateTime>
class schedule
{
private:
    QString name;//事件名
    QString detail;//事件详情
    QDateTime start;//开始事件
    QDateTime end;//结束事件
    bool is_delete;//是否被删除
public:
    //构造函数
    schedule();
    schedule(QString name,QString datail,QDateTime start,QDateTime end);

    //返回对应的私有成员变量
    QString getname();
    QString getdetail();
    QDateTime getstart();
    QDateTime getend();
    bool get_is_delete();
    bool is_time;//是否到结束时间

    //更改事件信息，用于编辑事件时
    void change_data(QString n,QString d,QDateTime start,QDateTime end);

    //删除事件，用户删除事件时
    void Delete();

    ~schedule();

};

bool compare(schedule&a,schedule&b);

#endif // SCHEDULE_H

#ifndef SCHEDULEOPERATE_H
#define SCHEDULEOPERATE_H

#include"schedule.h"
#include<Qmap>
#include<QVector>


class scheduleoperate
{
public:
    scheduleoperate();

    ~scheduleoperate();

    void add_schedule(QDate Date,schedule sche);//添加事件

    QVector<schedule>* getschedule(QDate Date);//获取Date这一天的所有事件



 public:
    QMap<QDate, QVector<schedule>>* allschedule;//键值对，QVector中存放某一日期下的所有事件

};

#endif // SCHEDULEOPERATE_H

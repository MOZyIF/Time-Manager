#include"scheduleoperate.h"
#include<QMap>
#include<QVector>

scheduleoperate::scheduleoperate()
{
    allschedule=new QMap<QDate,QVector<schedule>>;//初始化键值对
}

scheduleoperate::~scheduleoperate()
{
    delete []allschedule;
}

void scheduleoperate::add_schedule(QDate Date,schedule sche)//添加事件
{
    //如果键值对不含该日期，则创建一个并加入要添加的事件
    if(allschedule->contains(Date))
    {
        (*allschedule)[Date].append(sche);
    }
    else//如果键值对含该日期，则将要添加的事件加入到该日期下面
    {
        QVector<schedule> V;
        V.append(sche);
        (*allschedule)[Date]=V;
    }

}


QVector<schedule>* scheduleoperate::getschedule(QDate Date)//获取这一天所有事件
{
    if(allschedule->contains(Date))
    {
        return &(*allschedule)[Date];
    }
    else
    {
        return nullptr;
    }
}



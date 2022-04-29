#include"schedule.h"

schedule::schedule()
{

}

schedule::schedule(QString name,QString detail,QDateTime start,QDateTime end)
{
    this->name=name;
    this->detail=detail;
    this->start=start;
    this->end=end;
    this->is_delete=false;//构造事件时，将事件初始化为未删除
    this->is_time=false;
}

schedule::~schedule()
{

}

QString schedule::getname()
{
    return name;
}

QString schedule::getdetail()
{
    return detail;
}

QDateTime schedule::getstart()
{
    return start;
}

QDateTime schedule::getend()
{
    return end;
}

bool schedule::get_is_delete()
{
    return is_delete;
}


void schedule::Delete()
{
    is_delete=true;//设置删除标志为已删除
}

void schedule::change_data(QString n,QString d,QDateTime s,QDateTime e)
{
    name=n;
    detail=d;
    start=s;
    end=e;

}

bool compare(schedule&a,schedule&b){
    return a.getend()<b.getend();
}

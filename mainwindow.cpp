#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"dialog.h"
#include"schedule.h"
#include<QDebug>
#include"QMessageBox"
#include"QListWidgetItem"
#include"QContextMenuEvent"
#include<qtextcodec.h>
#include<qfiledialog.h>
#include"time.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*QDate data;
    data = ui->calendarWidget->selectedDate();

    QString a;
    a = data.toString();
    ui->pushButton_insert->setStyleSheet("QPushButton{color:white;background:green}"
                                                             "QPushButton::hover{background:rgb(110,115,100);}"
                                                             "QPushButton::pressed{background:#eb7350;}"
                                         );*/
    //ui->centralwidget->setStyleSheet(styleshell1);
    clicked=false;
    this->setFixedSize(this->size());
    ScheduleOperate = new scheduleoperate;
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    popMenu = new QMenu(this);
    action_Delete = new QAction(tr("删除"), this);
    action_Edit= new QAction(tr("编辑"), this);
    popMenu->addAction(action_Delete);
    popMenu->addAction(action_Edit);
    connect(this->action_Delete, SIGNAL(triggered()), this, SLOT(onActionDelete()));
    connect(this->action_Edit,SIGNAL(triggered()),this,SLOT(onActionEdit()));

    time.setHMS(0,0,0,0);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update_time()));

    ui->label->setText(time.toString("hh:mm:ss"));
    //ui->listWidget->addItem(a);
    mytimer = new QTimer(this);
    connect(mytimer,SIGNAL(timeout()),this,SLOT(time_done()));
    mytimer->start(1000);
    QPixmap pixmap = QPixmap(":/new/prefix1/pic/pic_1.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));

    this->setPalette(palette);
    sound=new QSound(":/new/prefix1/music/canon.wav");
    sound->play();//
    sound->setLoops(-1);//循环次数，-1代表一致循环
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setWeekdayTextFormat(Qt::Saturday,ui->calendarWidget->weekdayTextFormat(Qt::Monday));
    ui->calendarWidget->setWeekdayTextFormat(Qt::Sunday,ui->calendarWidget->weekdayTextFormat(Qt::Monday));
    QBrush b(QColor(22, 88, 203));
    QTextCharFormat format ;
    format.setBackground(b);
    format.setForeground(QColor("white"));
    ui->calendarWidget->setHeaderTextFormat(format);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::update_time(){
    static quint32 time_out = 0;
    time_out++;
    time = time.addSecs(1);
    ui->label->setText(time.toString("hh:mm:ss"));
};



void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    //获取当前点击的日期下的所有事件并将事件标题显示在listWidght上
    QVector<schedule>* temp=ScheduleOperate->getschedule(date);
    if(temp==nullptr)
    {
        ui->listWidget->clear();
    }
    else
    {
        ui->listWidget->clear();
        if(temp != nullptr)
        {
            for(int i=0; i<temp->size(); ++i)
            {
                if(!(*temp)[i].get_is_delete())
                {
                    ui->listWidget->addItem((*temp)[i].getname());
                }
            }
        }
    }
}

void MainWindow::on_pushButton_insert_clicked()
{
    //弹出对话框，用户可输入相应内容
    dialog =new Dialog(this);
    dialog->setModal(true);
    dialog->show();
    dialog->exec();

    //点击了对话框的“确定”按钮，即可添加事件
    if(dialog->result()==dialog->Accepted)
    {
        //获取用户输入的事件的相关信息
        QString t=dialog->getTitle();//事件名
        QDateTime s=dialog->getstart();//事件开始时间
        QDateTime e=dialog->getend();//事件结束时间
        QString d=dialog->getdetail();//事件详细信息

        //确保结束时间不能小于开始时间
        if(e<s)
        {
            QMessageBox::information(this, "警告","结束时间不能早于开始时间");
            return;
        }

        //确保事件标题不能为空
        if(t.isEmpty())
        {
            QMessageBox::information(this, "警告","事件名不能为空");
            return;
        }

        //用用户输入的数据实例化一个schedule对象
        schedule sche(t,d,s,e);

        //获取开始时间和结束时间的日期
        QDate D1=s.date();
        QDate D2=e.date();

        //开始日期和结束日期在同一天，只用在该日期的事件集合中添加该事件
        all_schedule.push_back(sche);
        if(D2==D1)
        {
            ScheduleOperate->add_schedule(D1,sche);
        }
        else if(D2>D1)
        {
            //开始日期和结束日期不在同一天，则这两个日期之间的所有日期都要添加该事件
            while(D1<=D2)
            {
                ScheduleOperate->add_schedule(D1,sche);
                D1=D1.addDays(1);
            }
        }
    }
    //在listWidght中显示日历控件当前所选择的日期下的所有事件
    QDate date=ui->calendarWidget->selectedDate();//获取日历控件当前所选的日期
    QVector<schedule>* temp=ScheduleOperate->getschedule(date);//获取该日期下的所有事件的集合
    if(temp==nullptr)
    {
        //当前日期下没有事件，清空listWidght内容即可
        ui->listWidget->clear();
    }
    else
    {
        //当前日期下有事件
        //先清空listWidght内容
        ui->listWidget->clear();
        //遍历日期下所有事件，并显示事件标题到listWidght上
        if(temp != nullptr)
        {
            for(int i=0; i<temp->size(); ++i)
            {
                if(!(*temp)[i].get_is_delete())
                {
                    ui->listWidget->addItem((*temp)[i].getname());
                }
            }
        }
    }
    delete dialog;
}



//删除事件
void MainWindow::onActionDelete()
{
    QDate date=ui->calendarWidget->selectedDate();//获取当前点击的日期
    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    if(items.empty()){
        return;
    }
    QString delete_s=items.first()->text();//获取要删除的事件名


    //获取要删除的事件的起始日期和结束日期
    QVector<schedule>* temp=ScheduleOperate->getschedule(date);
    QDate D1,D2;
    for(int i=0;i<temp->size();i++)
    {
        if((*temp)[i].getname()==delete_s&&!(*temp)[i].get_is_delete())
        {
            D1=(*temp)[i].getstart().date();
            D2=(*temp)[i].getend().date();
            break;
        }
    }
    //起始日期与结束日期相同，只用删除一遍
    if(D1==D2)
    {
        QVector<schedule>* temp1=ScheduleOperate->getschedule(D1);
        for(int i=0;i<temp1->size();i++)
        {
            if((*temp1)[i].getname()==delete_s&&!(*temp1)[i].get_is_delete())
            {
                (*temp1)[i].Delete();//删除该事件
                ui->listWidget->clear();//清空listWidght
                if(temp1 != nullptr)//重新显示listWidght的内容
                {
                    for(int i=0;i<temp1->size();i++)
                    {
                        if(!(*temp1)[i].get_is_delete())
                        {
                            ui->listWidget->addItem((*temp1)[i].getname());
                        }
                    }
                }
                return;
            }
        }
    }
    //起始日期与结束日期不同，要删除多遍
    else if(D2>D1)
    {
        while(D1<=D2)
        {
            QVector<schedule>* temp2=ScheduleOperate->getschedule(D1);
            for(int i=0;i<temp2->size();i++)
            {
                if((*temp2)[i].getname()==delete_s&&!(*temp2)[i].get_is_delete())
                {
                    (*temp2)[i].Delete();//删除事件
                }
            }
            D1=D1.addDays(1);//D1增加一天
        }
        //获取当前点击的日期下的所有事件并将事件标题显示在listWidght上
        QVector<schedule>* temp3=ScheduleOperate->getschedule(date);
        if(temp3==nullptr)
        {
            ui->listWidget->clear();
        }
        else
        {
            ui->listWidget->clear();
            if(temp3 != nullptr)
            {
                for(int i=0; i<temp3->size(); ++i)
                {
                    if(!(*temp3)[i].get_is_delete())
                    {
                        ui->listWidget->addItem((*temp3)[i].getname());
                    }
                }
            }
        }
    }
}


//编辑事件
void MainWindow::onActionEdit()
{
    QDate date=ui->calendarWidget->selectedDate();
    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    if(items.empty()){
        return;
    }
    QString edit_s=items.first()->text();

    QVector<schedule>* temp=ScheduleOperate->getschedule(date);
    for(int i=0;i<temp->size();++i)
    {
        if((*temp)[i].getname()==edit_s&&!(*temp)[i].get_is_delete())
        {
            //记录下更改前的事件的内容
            QString detail=(*temp)[i].getdetail();
            QDateTime start=(*temp)[i].getstart();
            QDateTime end=(*temp)[i].getend();
            //弹出对话框，用户在对话框中对原本的事件信息编辑
            dialog_edit=new Dialog(edit_s,start,end,detail,this);
            dialog_edit->setModal(true);
            dialog_edit->show();
            dialog_edit->exec();

            //更改时间信息后点击了对话框的“确定”按钮，确定更改
            if(dialog_edit->result()==dialog_edit->Accepted)
            {
                //记录下更改后的事件信息
                QString t=dialog_edit->getTitle();
                QString d=dialog_edit->getdetail();
                QDateTime s=dialog_edit->getstart();
                QDateTime e=dialog_edit->getend();
                //必须保证结束时间大于开始时间
                if(e<s)
                {
                    QMessageBox::information(this, "警告","结束时间不能早于开始时间");
                    return;
                }

                //确保事件标题不能为空
                if(t.isEmpty())
                {
                    QMessageBox::information(this, "警告","事件名不能为空");
                    return;
                }

                //找到该日期下的原事件，将原事件删除
                QVector<schedule>* temp=ScheduleOperate->getschedule(date);
                for(int i=0;i<temp->size();++i)
                {
                    if((*temp)[i].getname()==edit_s&&!(*temp)[i].get_is_delete())
                    {
                        QDate D1=(*temp)[i].getstart().date();
                        QDate D2=(*temp)[i].getend().date();
                        while(D1<=D2)
                        {
                            QVector<schedule>* temp1=ScheduleOperate->getschedule(D1);
                            for(int i=0;i<temp1->size();i++)
                            {
                                if((*temp1)[i].getname()==edit_s&&!(*temp1)[i].get_is_delete())
                                {
                                    (*temp1)[i].Delete();
                                }
                            }
                            D1=D1.addDays(1);
                        }
                        break;
                    }
                }
                //重新加入编辑后的事件
                schedule sche(t,d,s,e);
                QDate D3=s.date();
                QDate D4=e.date();
                while(D3<=D4)
                {
                    ScheduleOperate->add_schedule(D3,sche);
                    D3=D3.addDays(1);
                }
            }

            //在listWidght上显示当天的事件
            ui->listWidget->clear();//首先清空原有的内容
            QDate date=ui->calendarWidget->selectedDate();//获取日历空间所选中的日期
            QVector<schedule>* temp=ScheduleOperate->getschedule(date);//获取该日期下的所有事件的集合
            if(temp==nullptr)
            {
                ui->listWidget->clear();
            }
            else
            {
                ui->listWidget->clear();
                if(temp != nullptr)
                {
                    for(int i=0; i<temp->size(); ++i)
                    {
                        if(!(*temp)[i].get_is_delete())
                        {
                            ui->listWidget->addItem((*temp)[i].getname());
                        }
                    }
                }
            }
            delete dialog_edit;
            return;
        }
    }
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    popMenu->exec(QCursor::pos());//显示菜单栏
}

void MainWindow::on_pushButton_save_clicked()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString path =QFileDialog::getSaveFileName(this,"save","../","TXT(*.txt)");
    if(path.isEmpty()==false)
    {
        //文件对象
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        /*QMap<QDate, QVector<schedule>>::iterator iter = ScheduleOperate->allschedule->begin();
        while(iter!=ScheduleOperate->allschedule->end()){
            QVector<schedule> tmp = iter.value();
            QDate tmp_date = iter.key();
            for(int i=0;i<tmp.size();i++){
                if(!tmp[i].get_is_delete()){
                    QString out = tmp_date.toString()+"|" + tmp[i].getname() + "|" + tmp[i].getstart().toString()+"|" + tmp[i].getend().toString()
                            +"|" + tmp[i].getdetail();
                    file.write(out.toUtf8());

                }
            }
            iter++;
        }*/
        for(int i=0;i<all_schedule.size();i++){
            if(!all_schedule[i].get_is_delete()){
                QString out =  all_schedule[i].getname() + "|" + all_schedule[i].getstart().toString()+"|" + all_schedule[i].getend().toString()
                        +"|" + all_schedule[i].getdetail();
                file.write(out.toUtf8());

            }
        }
        QMessageBox::about(this,"temp","文件保存成功！");
        file.close();
    }

}

void MainWindow::on_pushButton_begin_clicked()
{
    timer->start(1000);
}

void MainWindow::on_pushButton_stop_clicked()
{
    if(clicked == false)//表示倒计时器处于运行状态，单击之后按钮状态变成“继续”
    {
        ui->pushButton_stop->setText(QString::fromUtf8("继续计时"));
        timer->stop();
        clicked = true;

    }
    else if(clicked == true)//表示倒计时器处于停止状态，单击之后按钮状态变成“暂停”
    {
        ui->pushButton_stop->setText(QString::fromUtf8("暂停计时"));
        timer->start(1000);
        clicked = false;
    }
}

void MainWindow::on_pushButton_set0_clicked()
{
    timer->stop();
    time.setHMS(0,0,0,0);
    ui->label->setText(time.toString("hh:mm:ss"));
}

void MainWindow::time_done()
{
    if(mytimer->isActive())
    {
        QDate date=QDate::currentDate();
        QVector<schedule>* temp=ScheduleOperate->getschedule(date);
        if(temp==NULL)
        {
            return;
        }
        for(int i=0;i<temp->size();++i)
        {
            if((*temp)[i].getend().time().hour()==QDateTime::currentDateTime().time().hour()&&
                    (*temp)[i].getend().time().minute()==QDateTime::currentDateTime().time().minute()&&
                    !(*temp)[i].get_is_delete()&&!(*temp)[i].is_time)
            {
                QMessageBox::information(NULL, "您有待做的事件", (*temp)[i].getname(),
                                         QMessageBox::Yes);
                (*temp)[i].is_time=true;
            }
        }
    }
}

void MainWindow::on_pushButton_set0_2_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("选择背景音乐");
    dialog.resize(QSize(250,100));
    QFormLayout form1(&dialog);
    QPushButton* button1 = new QPushButton(tr("无"));
    QPushButton* button2 = new QPushButton(tr("潺潺溪流"));
    QPushButton* button3 = new QPushButton(tr("春日鸟鸣"));
    QPushButton* button4 = new QPushButton(tr("卡农(canon)"));
    QPushButton* button5 = new QPushButton(tr("梦中的婚礼钢琴曲"));
    QPushButton* button6 = new QPushButton(tr("舒缓雨声"));
    QPushButton* button7 = new QPushButton(tr("千与千寻钢琴曲"));
    form1.addRow(button1);
    form1.addRow(button2,button3);
    form1.addRow(button4,button6);
    form1.addRow(button5);
    form1.addRow(button7);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form1.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    connect(button1, &QPushButton::clicked, this, [&]() {
        sound->stop();
        dialog.close();
    }
    );
    connect(button2, &QPushButton::clicked, this, [&]() {
        sound->stop();
        sound=new QSound(":/new/prefix1/music/xiliu.wav",this);
        sound->play();
        sound->setLoops(-1);//循环次数，-1代表一致循环
        dialog.close();
    }
    );
    connect(button3, &QPushButton::clicked, this, [&]() {
        sound->stop();
        sound=new QSound(":/new/prefix1/music/bird.wav",this);
        sound->play();
        sound->setLoops(-1);//循环次数，-1代表一致循环
        dialog.close();
    }
    );
    connect(button4, &QPushButton::clicked, this, [&]() {
        sound->stop();
        sound=new QSound(":/new/prefix1/music/canon.wav",this);
        sound->play();
        sound->setLoops(-1);//循环次数，-1代表一致循环
        dialog.close();
    }
    );
    connect(button5, &QPushButton::clicked, this, [&]() {
        sound->stop();
        sound=new QSound(":/new/prefix1/music/mengzhongde.wav",this);
        sound->play();
        sound->setLoops(-1);//循环次数，-1代表一致循环
        dialog.close();
    }
    );
    connect(button6, &QPushButton::clicked, this, [&]() {
        sound->stop();
        sound=new QSound(":/new/prefix1/music/rain.wav",this);
        sound->play();
        sound->setLoops(-1);//循环次数，-1代表一致循环
        dialog.close();
    }
    );
    connect(button7, &QPushButton::clicked, this, [&]() {
        sound->stop();
        sound=new QSound(":/new/prefix1/music/qianyuqianxun.wav",this);
        sound->play();
        sound->setLoops(-1);//循环次数，-1代表一致循环
        dialog.close();
    }
    );
    //QObject::connect(button_add, SIGNAL(clicked(bool)), this, SLOT(button_add_clicked()));
    if (dialog.exec() == QDialog::Accepted) {

    }
}



void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox->currentText()=="雪山"){
        QPixmap pixmap1 = QPixmap(":/new/prefix1/pic/pic_1.jpg").scaled(this->size());
        QPalette palette(this->palette());
        palette.setBrush(QPalette::Background, QBrush(pixmap1));
        this->setPalette(palette);
        ui->label_2->setStyleSheet("QLabel{color:rgb(0,0,0);font: 11pt;}");
        //ui->label_3->setStyleSheet("QLabel{color:rgb(0,0,0);font: 11pt;}");
    }else if(ui->comboBox->currentText()=="火车"){
        QPixmap pixmap2 = QPixmap(":/new/prefix1/pic/pic_2.jpg").scaled(this->size());
        QPalette palette(this->palette());
        palette.setBrush(QPalette::Background, QBrush(pixmap2));
        this->setPalette(palette);
        ui->label_2->setStyleSheet("QLabel{color:rgb(0,0,0);font: 11pt;}");
        //ui->label_3->setStyleSheet("QLabel{color:rgb(0,0,0);font: 11pt;}");
    }else if(ui->comboBox->currentText()=="窗外"){
        QPixmap pixmap3 = QPixmap(":/new/prefix1/pic/pic_3.jpg").scaled(this->size());
        QPalette palette(this->palette());
        palette.setBrush(QPalette::Background, QBrush(pixmap3));
        this->setPalette(palette);
        ui->label_2->setStyleSheet("QLabel{color:rgb(0,0,0);font: 11pt;}");
        //ui->label_3->setStyleSheet("QLabel{color:rgb(0,0,0);font: 11pt;}");
    }else if(ui->comboBox->currentText()=="草原"){
        QPixmap pixmap4 = QPixmap(":/new/prefix1/pic/pic_4.jpg").scaled(this->size());
        QPalette palette(this->palette());
        palette.setBrush(QPalette::Background, QBrush(pixmap4));
        this->setPalette(palette);
        //设置指定背景色
        ui->label_2->setStyleSheet("QLabel{color:rgb(255,255,255);font: 11pt;}");
        //ui->label_3->setStyleSheet("QLabel{color:rgb(255,255,255);font: 11pt;}");
    }else{
        QString path = QFileDialog::getOpenFileName(this, tr("选择图片"), ".", tr("Image Files(*.jpg *.png)"));
        QPixmap pixmap5 = QPixmap(path).scaled(this->size());
        QPalette palette(this->palette());
        palette.setBrush(QPalette::Background, QBrush(pixmap5));
        this->setPalette(palette);
    }
}

void MainWindow::on_pushButton_showall_clicked()
{
    ui->listWidget->clear();
    qSort(all_schedule.begin(),all_schedule.end(),compare);
    for(int i=0;i<all_schedule.size();i++){
        QString tmp;
        tmp = all_schedule[i].getname()+"   "+ all_schedule[i].getend().toString();
        ui->listWidget->addItem(tmp);
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QDialog dialog(this);
      dialog.setWindowTitle("详细信息");
      dialog.resize(QSize(250,100));
      QFormLayout form1(&dialog);
      QDate date=ui->calendarWidget->selectedDate();//获取当前点击的日期
      QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
      if(items.empty()){
          return;
      }
      QString name=items.first()->text();//获取事件名
      //获取要事件的起始日期和结束日期
      QVector<schedule>* temp=ScheduleOperate->getschedule(date);
      QDate D1,D2;
      for(int i=0;i<temp->size();i++)
      {
          if((*temp)[i].getname()==name&&!(*temp)[i].get_is_delete())
          {
              D1=(*temp)[i].getstart().date();
              D2=(*temp)[i].getend().date();
              break;
          }
      }
      form1.addRow(new QLabel("事件创建日期："+date.toString("yyyy/M/d")));
      form1.addRow(new QLabel("事件起始日期："+D1.toString("yyyy/M/d")));
      form1.addRow(new QLabel("事件结束日期："+D2.toString("yyyy/M/d")));
      form1.addRow(new QLabel("事件名："+name));
      QString detail;
      for(int i=0;i<temp->size();++i)
      {
          if((*temp)[i].getname()==name&&!(*temp)[i].get_is_delete())
          {
               detail=(*temp)[i].getdetail();
          }
      }
      form1.addRow(new QLabel("事件详细信息："+detail));
      QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                 Qt::Horizontal, &dialog);
      form1.addRow(&buttonBox);
      QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
      QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
      //QObject::connect(button_add, SIGNAL(clicked(bool)), this, SLOT(button_add_clicked()));
      if (dialog.exec() == QDialog::Accepted) {

      }
}





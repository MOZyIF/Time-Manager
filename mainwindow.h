#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"dialog.h"
#include"scheduleoperate.h"
#include"QListWidget"
#include<QTime>
#include<qtimer.h>
#include <QSound>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QTextCharFormat>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_calendarWidget_clicked(const QDate &date);  //点击日历事件

    void on_pushButton_insert_clicked();

    void onActionDelete();//删除事件

    void onActionEdit();//编辑事件

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_save_clicked();

    void update_time();
    //计时相关函数
    void on_pushButton_begin_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_set0_clicked();

    //事件到期提示
    void time_done();

    void on_pushButton_set0_2_clicked();


    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_showall_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);


private:
    Ui::MainWindow *ui;
    Dialog* dialog;//创建事件时的对话框
    Dialog* dialog_edit;//编辑事件时对话框
    scheduleoperate* ScheduleOperate=nullptr;//事件操作类

    QMenu *popMenu;//弹出菜单
    QAction *action_Delete;//删除
    QAction *action_Edit;//编辑

    //WarnDialog* warndialog;//事件提醒窗口
    QTimer* mytimer;//计时器
    QTimer *timer;
    QTime time;
    bool clicked;

    QSound *sound;//背景音乐

    QVector<schedule> all_schedule;

};
#endif // MAINWINDOW_H

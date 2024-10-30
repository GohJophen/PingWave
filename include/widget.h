#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QSettings>
#include <QCloseEvent>
#include <QProcess>
#include <QStringList>
#include <QRegularExpression>
#include <QDir>
#include <QDateTime>
#include <QTimer>
#include <QShortcut>
#include "mainwindow.h"

class widget : public QWidget{
    Q_OBJECT
public:
    widget(QWidget *parent = nullptr);
    ~widget();

private:
    //组件
    QLineEdit *txtIpOrDomain;//输入ip或域名的单行文本框
    QPushButton* btnPing;//ping按钮
    QLabel* lblIntervalPrefix;//文本标签“每隔”
    QSpinBox *spnInterval;//间隔时间数值选择器
    QLabel* lblIntervalSuffix;//文本标签"秒,发送一次；"
    QLabel* lblNumOfTimesPrefix;//文本标签"发送"
    QSpinBox *spnNumOfTimes;//发送次数数值选择器
    QLabel* lblNumOfTimesSuffix;//文本标签"次；"
    QLabel* lblTakeTimePrefix;//文本标签"用时:"
    QLabel* lblTakeTime;//文本标签;“用时”;动态数值
    QLabel* lblTakeTimeSuffix;//文本标签"秒"
    QLabel* lblBeExecuting;
    QPushButton *btnOpenFile;//打开文件
    QLabel* lblTime;
    QLineEdit* txtPathFile;

    //数据
    QString targetHost; // 目标主机
    int interval; // 获取用户选择的间隔时间
    int numOfTimes; // 获取用户选择的发送次数
    int maxTime;
    int realityNumOfTimes;//实际ping数
    QFile file;
    QString formattedDateTime;
    QString fileName;

    //布局
    QHBoxLayout *hlayout1;//第一行布局
    QHBoxLayout *hlayout2;//第二行布局
    QHBoxLayout *hlayout3;//第三行布局
    QHBoxLayout *hlayout4;//第四行布局
    QVBoxLayout *vlayoutMain;//主布局(第一行、第二行、第三行和第四行垂直)

    //正则表达式
    QRegularExpression regexData;  // 匹配ping返回的响应时间
    QRegularExpression regexXExtent;//匹配文件名中的x轴长度信息
    QRegularExpression regexYExtent;//匹配文件名中的y轴长度信息
    QRegularExpression regexInterval;

    //独立窗口
    mainWindow *mainwindow = nullptr;  // 声明为成员变量并初始化为nullptr，则该窗口为无父窗口的顶层窗口

    //定时器
    QTimer *timer;

    //用户配置数据
    QSettings settings;
    int defaultInterval;
    int defaultNumOfTimes;

    //重写事件函数
    void closeEvent(QCloseEvent *event) override;

    //普通函数
    void writeIntToFile(const QString &fileName, int value);//文件写入函数
    void ping();//启动定时器函数
    void enablePing();
    void notPing();
    void onPingTimeout();//定时期间，循环操作与定时结束处理函数

private slots:
    void onSpinBoxValueChanged();
    void enableOrNot();
    void onOpenFile();

};



#endif // WIDGET_H

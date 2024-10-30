#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent) : QWidget(parent) {
    // 创建一个滚动区域;
    //this： mainWindow对象的指针传递给scrollArea的构造函数，
    //使得scrollArea原本默认的空指针(QWidget *parent = nullptr)变成(QWidget *parent = mainWindow)
    scrollarea = new scrollArea(this);

    // 创建布局，并将滚动区域加入布局中
    layout = new QVBoxLayout;
    layout->addWidget(scrollarea);
    this->setLayout(layout);  // 将布局应用到主窗口

    /*
    // 连接 CustomPlot 的信号到主窗口的 resize() 函数
    QObject::connect(scrollarea->plot, &CustomPlot::scaleChanged, this, [this](int newWidth, int newHeight) {
        this->resize(newWidth, newHeight);
    });
    */

    // 连接 CustomPlot 的信号到 scrollArea 的槽;由于
    QObject::connect(scrollarea->plot, &customPlot::scaleChanged, this, &mainWindow::handleScaleChanged);


    // 禁用最大化按钮
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
}

void mainWindow::handleScaleChanged(int newWidth, int newHeight) {
    // 处理信号，比如调整 scrollArea 的大小
    this->resize(newWidth, newHeight);
}


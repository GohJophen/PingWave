#include "scrollArea.h"

scrollArea::scrollArea(QWidget *parent) : QScrollArea(parent) {
    plot = new customPlot(this);//指定customPlot为scrollArea的子窗口
    this->setWidget(plot);// 将绘图窗口放入滚动区域
}





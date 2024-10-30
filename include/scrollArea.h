#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include "customPlot.h"
#include <QScrollArea>


class scrollArea : public QScrollArea {
    Q_OBJECT

public:
    explicit scrollArea(QWidget *parent = nullptr);

    customPlot *plot; // 自定义绘图窗口的指针

};

#endif // SCROLLAREA_H

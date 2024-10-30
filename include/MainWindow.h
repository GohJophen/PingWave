#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scrollArea.h"
#include <QVBoxLayout>

class mainWindow : public QWidget {
    Q_OBJECT  // 这用于启用 Qt 的信号槽机制

public:
    explicit mainWindow(QWidget *parent = nullptr);
    scrollArea *scrollarea; // 滚动区域

private:
    QVBoxLayout *layout;     // 主窗口布局

private slots:
    void handleScaleChanged(int newWidth, int newHeight); // 处理信号的槽
};

#endif // MAINWINDOW_H

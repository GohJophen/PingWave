#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#include <QDataStream>
#include <QScreen>//用以获取屏幕尺寸
#include <QToolTip>



class customPlot : public QWidget {
    Q_OBJECT

public:
    explicit customPlot(QWidget *parent = nullptr);
    void readBinFile(const QString &filePath,int fileNameXMaxExtent,int fileNameYMaxExtent,int fileNameInterval);

private:
    //数据
    int mainWindowWidth;
    int mainWindowHeight;
    int screenWidthTwoOverThree;
    int screenHeightTwoOverThree;
    int gap;
    int xExtent;
    int yExtent;
    // int xNumOfScale;
    // int yNumOfScale;
    float xScaleFactor;
    float yScaleFactor;
    QVector<QPoint> points;
    QVector<QPoint> pointsReset;
    QPoint closestPoint;


    //普通函数
    QSize getScreenSize();

    //重写事件函数
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void scaleChanged(int newWidth, int newHeight);

};

#endif // CUSTOMPLOT_H


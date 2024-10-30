#include "customPlot.h"

customPlot::customPlot(QWidget *parent): QWidget(parent),closestPoint(QPoint()) {

    mainWindowWidth=0;
    mainWindowHeight=0;

    //获取屏幕宽度
    QSize screenSize = getScreenSize();
    screenWidthTwoOverThree = screenSize.width()/1.5;
    screenHeightTwoOverThree= screenSize.height()/1.5;

    //x轴与y轴的缩放因子
    xScaleFactor = 1;
    yScaleFactor = 1;

    //图形与窗口宽和高边框的间距
    gap = 50;

    // 启用鼠标追踪
    setMouseTracking(true);

}

//获取屏幕尺寸
QSize customPlot::getScreenSize() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        return QSize(screenGeometry.width(), screenGeometry.height());
    } else {
        qDebug() << "No screen available!";
        return QSize(0, 0);
    }
}

//重写鼠标滚轴事件，使其达到缩放效果
void customPlot::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        xScaleFactor *= 1.1;  // 放大
        yScaleFactor *= 1.1;
    } else {
        xScaleFactor /= 1.1;  // 缩小
        yScaleFactor /= 1.1;
    }

    // 限制最小缩放比例为0.5，最大缩放比例为4.0
    if (xScaleFactor < 1.0) xScaleFactor = 1.0;
    if (yScaleFactor < 1.0) yScaleFactor = 1.0;
    if (xScaleFactor > 100.0) xScaleFactor = 100.0;
    if (yScaleFactor > 100.0) yScaleFactor = 100.0;

    //使图像也跟着缩放
    setFixedSize(xScaleFactor * xExtent + 2 * gap, yScaleFactor*yExtent+2*gap);

    // 发送信号，通知主窗口大小改变
    if(xScaleFactor*xExtent+2*gap<=screenWidthTwoOverThree){
        mainWindowWidth = xScaleFactor*xExtent+2*gap;
    }else{
        mainWindowWidth = screenWidthTwoOverThree;
    }

    if(yScaleFactor*yExtent+3*gap<=screenHeightTwoOverThree){
        mainWindowHeight = yScaleFactor*yExtent+3*gap;
    }else{
        mainWindowHeight = screenHeightTwoOverThree;
    }

    emit scaleChanged(mainWindowWidth,mainWindowHeight);


    // 重新绘制
    update();
}

//读取文件
void customPlot::readBinFile(const QString &filePath,int fileNameXMaxExtent,int fileNameYMaxExtent,int fileNameInterval){
    //打开传入的文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QDataStream in(&file);

    //大端读取
    in.setByteOrder(QDataStream::BigEndian);

    //初始化数据点
    points.clear();

    //逐行读取文件里的响应时间序列，并分别计算“时间点”，组成坐标图里的数据点，逐一导入到pointsInit数据点列表
    int i= 0;
    while (!in.atEnd()) {
        int responseTime;
        in >> responseTime;
        if (in.status() != QDataStream::Ok) {
            qDebug() << "读取数据时出错。";
            break;
        }

        i++;
        points.append(QPoint(i*fileNameInterval, responseTime));
    }

    file.close();


    //x轴和y轴长度
    xExtent = fileNameXMaxExtent;
    yExtent = fileNameYMaxExtent;

    //customPlot部件的初始大小
    resize(xScaleFactor*xExtent+2*gap, yScaleFactor*yExtent+2*gap );

    //发送信号
    if(xScaleFactor*xExtent+2*gap<=screenWidthTwoOverThree){
        mainWindowWidth = xScaleFactor*xExtent+2*gap;
    }else{
        mainWindowWidth = screenWidthTwoOverThree;
    }

    if(yScaleFactor*yExtent+3*gap<=screenHeightTwoOverThree){
        mainWindowHeight = yScaleFactor*yExtent+3*gap;
    }else{
        mainWindowHeight = screenHeightTwoOverThree;
    }

    emit scaleChanged(mainWindowWidth,mainWindowHeight);
}

//描绘坐标图
void customPlot::paintEvent(QPaintEvent *) {

    QPainter painter(this);//this 是一个指针，指向调用该成员函数的 customPlot 对象的实例。

    painter.setPen(QPen(Qt::blue, 1));

    // 计算矩形边框的区域（确保边框大小与绘图区域一致）
    QRect rect(0, 0,  width(),height());

    // 绘制矩形边框
    painter.drawRect(rect);

    // 调整原点位置，始终从左下角开始绘制
    painter.translate(gap, height()-gap); // 将原点移动到窗口的左下角
    painter.scale(1, -1); // X轴方向不变，Y 轴方向反转

    //随着缩放而更新数据点
    painter.setPen(QPen(Qt::red, 1));
    pointsReset.clear();
    for (const QPoint &point : points) {
        pointsReset.append(QPoint(point.x()*xScaleFactor,point.y()*yScaleFactor));
    }

    //连接数据点，使之成为折线图
    painter.drawPolyline(pointsReset.data(), pointsReset.size());

    painter.setBrush(Qt::red);  // 圆圈内部填充为红色
    // 逐个绘制每个数据点的小圆圈
    for (const QPoint &point : pointsReset) {
        // 绘制一个小圆圈，假设半径为 1
        painter.drawEllipse(point, 1, 1);
    }

    painter.setPen(QPen(Qt::blue, 1));

    //画x轴和y轴线段
    painter.drawLine(0, 0, xExtent*xScaleFactor, 0);
    painter.drawLine(0, 0, 0, yExtent*yScaleFactor);

    //画x轴刻度线和刻度值
    for(int x = 1 ; x <= xExtent ; ++x){
        if(x % 60 ==0){
            painter.drawLine(x*xScaleFactor,0,x*xScaleFactor,xScaleFactor*2);
            // 保存当前的坐标系状态
            painter.save();

            // 先将坐标系恢复到正常（取消Y轴的镜像效果）
            painter.scale(1, -1);

            // 绘制文本（刻度值）
            painter.drawText(x  * xScaleFactor-10, -xScaleFactor+20,  QString::number(x) + "s");

            // 恢复坐标系状态
            painter.restore();
        }
    }

    //画y轴刻度线和刻度值
    for(int y = 1 ; y<= yExtent; ++y){
        if(y % (yExtent/5)==0){
            painter.drawLine(0,y*yScaleFactor,yScaleFactor*2,y*yScaleFactor);
            // 保存当前的坐标系状态
            painter.save();

            // 先将坐标系恢复到正常（取消Y轴的镜像效果）
            painter.scale(1, -1);

            // 因为坐标系已经反转，y 的值也需要反转来恢复文本的正确显示位置
            //int adjustedY = -(y * yOneScale * yScaleFactor);

            // 绘制文本（刻度值）
            painter.drawText(-45, -y * yScaleFactor+3, QString::number(y) + " ms");
            //qDebug()<<QString::number(y * yOneScale);
            // 恢复坐标系状态
            painter.restore();
        }
    }

    // 高亮显示鼠标悬停的数据点
    if (closestPoint != QPoint()) {
        painter.setBrush(Qt::yellow);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(closestPoint, static_cast<int>(2), static_cast<int>(2));
    }
}

//重写鼠标移动事件；实时跟踪光标坐标
void customPlot::mouseMoveEvent(QMouseEvent *event) {
    // 获取鼠标位置
    QPoint mousePos = event->pos();

    //鼠标悬停的坐标点
    QPoint adjustedMousePos = QPoint(mousePos.x() - gap, height() - mousePos.y() - gap);

    // 搜索与鼠标位置最近的数据点
    const int threshold = 10; // 定义一个阈值，用于判断鼠标接近数据点的范围

    bool found = false;
    QPoint foundPoint;

    // 查找距离鼠标最近的数据点
    for (const QPoint &point : pointsReset) {
        if ((qAbs(adjustedMousePos.x() - point.x()) < threshold) &&
            (qAbs(adjustedMousePos.y() - point.y()) < threshold)) {
            foundPoint = point;
            found = true;
            break;
        }
    }

    // 如果找到最近点并且与当前点不同，更新提示和图形
    if (found) {
        if (closestPoint != foundPoint) { // 仅在点变化时更新
            closestPoint = foundPoint;
            // 显示工具提示
            QString tooltipText = QString("时间: %1 s,  响应: %2 ms")
                                      .arg(static_cast<int>(closestPoint.x() / xScaleFactor))
                                      .arg(static_cast<int>(closestPoint.y() / yScaleFactor));
            QToolTip::showText(event->globalPosition().toPoint(), tooltipText, this);
            update();// 触发重新绘制，确保高亮显示
        }
    } else {
        if (closestPoint != QPoint()) { // 仅在之前有点时重置
            QToolTip::hideText();
            closestPoint = QPoint();// 重置最近点
            update();
        }
    }
}

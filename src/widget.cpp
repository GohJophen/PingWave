#include "widget.h"


widget::widget(QWidget *parent):QWidget(parent),
    regexData(R"(时间=(\d+)ms)"),
    regexXExtent(R"(_xE(\d+))"),
    regexYExtent(R"(_yE(\d+))"),
    regexInterval(R"(_i(\d+))"),
    settings("GohJophen", "PingWave"){

    // 读取默认值
    defaultInterval= settings.value("interval", 5).toInt();
    defaultNumOfTimes= settings.value("numOfTimes", 60).toInt();

    //创建 文本标签 对象实例
    lblIntervalPrefix=new QLabel("每隔",this);
    lblIntervalSuffix=new QLabel("秒,发送一次；",this);
    lblNumOfTimesPrefix=new QLabel("发送",this);
    lblNumOfTimesSuffix=new QLabel("次；",this);
    lblTakeTimePrefix=new QLabel("用时:",this);
    lblTakeTime=new QLabel(this);
    lblTakeTimeSuffix=new QLabel("秒",this);
    lblBeExecuting=new QLabel(this);
    lblTime = new QLabel(this);

    //创建 数字输入框 对象实例
    spnNumOfTimes = new QSpinBox(this);
    spnInterval = new QSpinBox(this);

    //创建 按钮 对象实例
    btnPing  =  new QPushButton("Ping",this);//ping按钮
    btnOpenFile = new QPushButton("打开文件",this);

    //创建 单行文本输入框 对象实例
    txtIpOrDomain = new QLineEdit(this);// 创建一个 QLineEdit 对象
    txtPathFile= new QLineEdit(this);

    //创建 布局 对象实例
    hlayout1 = new  QHBoxLayout;
    hlayout2 = new  QHBoxLayout;
    hlayout3 = new  QHBoxLayout;
    hlayout4 = new  QHBoxLayout;
    vlayoutMain = new  QVBoxLayout(this);

    spnInterval->setRange(1, 86400);  // 设置范围：0 到 100
    spnNumOfTimes->setRange(1, 31622400);  // 设置范围：0 到 100
    //spnInterval->setValue(5);      // 设置初始值为 10
    //spnNumOfTimes->setValue(60);      // 设置初始值为 10
    spnInterval->setValue(defaultInterval);//设置初始值
    spnNumOfTimes->setValue(defaultNumOfTimes);

    lblTakeTime->setText(QString::number(spnInterval->value() * spnNumOfTimes->value()));
    txtPathFile->setText(QDir::currentPath());
    txtIpOrDomain->setFixedWidth(450);  // 设置文本框的固定宽度;将文本框的宽度设置为 200 像素
    txtIpOrDomain->setPlaceholderText("请输入ip或域名");// 设置占位符文本


    //设置第一行布局
    hlayout1->addWidget(txtIpOrDomain);
    hlayout1->addWidget(btnPing);

    //设置第二行布局
    hlayout2->addWidget(lblIntervalPrefix);
    hlayout2->addWidget(spnInterval);
    hlayout2->addWidget(lblIntervalSuffix);
    hlayout2->addWidget(lblNumOfTimesPrefix);
    hlayout2->addWidget(spnNumOfTimes);
    hlayout2->addWidget(lblNumOfTimesSuffix);
    hlayout2->addWidget(lblTakeTimePrefix);
    hlayout2->addWidget(lblTakeTime);
    hlayout2->addWidget(lblTakeTimeSuffix);
    hlayout2->addStretch();  // 在右边添加一个水平弹簧
    hlayout2->addWidget(lblBeExecuting);
    hlayout2->addSpacing(10);

    //设置第三行布局
    hlayout3->addWidget(btnOpenFile);
    hlayout3->addStretch();
    hlayout3->addWidget(lblTime);
    hlayout3->addSpacing(10);

    //设置第四行布局
    hlayout4->addWidget(txtPathFile);

    //主布局(第一行、第二行、第三行和第四行垂直)
    vlayoutMain->addLayout(hlayout1);
    vlayoutMain->addLayout(hlayout2);
    vlayoutMain->addLayout(hlayout3);
    vlayoutMain->addLayout(hlayout4);

    // 设置窗口的布局
    setLayout(vlayoutMain);

    //绑定信号与槽
    connect(btnPing,&QPushButton::clicked,this,&widget::enableOrNot);
    connect(spnInterval, QOverload<int>::of(&QSpinBox::valueChanged), this, &widget::onSpinBoxValueChanged);
    connect(spnNumOfTimes, QOverload<int>::of(&QSpinBox::valueChanged), this, &widget::onSpinBoxValueChanged);
    connect(btnOpenFile,&QPushButton::clicked,this,&widget::onOpenFile);

    //打开文件按钮快捷键
    QShortcut* shortcutOpen = new QShortcut(QKeySequence(tr("Ctrl+O","File|Open")),this);
    connect(shortcutOpen,&QShortcut::activated,this,[=](){onOpenFile();});

    //Ping按钮快捷键
    QShortcut* shortcutPing = new QShortcut(QKeySequence(Qt::Key_Return), this);//Enter键
    connect(shortcutPing, &QShortcut::activated, this, [=](){ enableOrNot(); });
}

widget::~widget() {
    // 删除独立窗口
    delete mainwindow;

}

//打开文件
void widget::onOpenFile(){

    if (!mainwindow) {
        mainwindow = new mainWindow(nullptr);  //设置父窗口为nullptr，创建顶层窗口
    }


    int xMaxExtentInt = 0;
    QRegularExpressionMatch matchXExtent = regexXExtent.match(txtPathFile->text());
    if (matchXExtent.hasMatch()) {
        QString xExtentStr = matchXExtent.captured(1);
        xMaxExtentInt = xExtentStr.toInt();
    } else {
        lblTime->setText("文件打开失败！获取X轴长度错误");
        lblTime->setStyleSheet("color:red;");
        return;
    }


    int yMaxExtentInt = 0;
    QRegularExpressionMatch matchYExtent = regexYExtent.match(txtPathFile->text());
    if (matchYExtent.hasMatch()) {
        QString yExtentStr = matchYExtent.captured(1);
        yMaxExtentInt = yExtentStr.toInt();
    } else {
        lblTime->setText("文件打开失败！获取Y轴长度错误");
        lblTime->setStyleSheet("color:red;");
        return;
    }

    int intervalInt = 0;
    QRegularExpressionMatch matchInterval = regexInterval.match(txtPathFile->text());
    if (matchInterval.hasMatch()) {
        QString intervalStr = matchInterval.captured(1);
        intervalInt = intervalStr.toInt();
    } else {
        lblTime->setText("文件打开失败！获取间隔时间错误");
        lblTime->setStyleSheet("color:red;");
        return;
    }

    mainwindow->scrollarea->plot->readBinFile(txtPathFile->text(),xMaxExtentInt,yMaxExtentInt,intervalInt);

    mainwindow->setWindowTitle(txtPathFile->text());
    // 显示新窗口
    mainwindow->show();
}

//计算、获取监控总时长
void widget::onSpinBoxValueChanged()
{
    unsigned int TakeTimeValue = spnInterval->value() * spnNumOfTimes->value();
    lblTakeTime->setText(QString::number(TakeTimeValue));
}

//文件写入
void widget::writeIntToFile(const QString &fileName, int value) {
    file.setFileName(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QDataStream out(&file);
        out << value;
        file.close();
    }
}

void widget::enablePing(){
    // 开始Ping操作
    btnPing->setText("Stop");
    lblBeExecuting->setText("正在执行...");
    lblBeExecuting->setStyleSheet("color: green;");
    lblTime->setText("");
    realityNumOfTimes=0;
    formattedDateTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");//以yyyyMMddHHmmss格式显示时间
    fileName = QDir::currentPath() + "/" + formattedDateTime + ".bin";
    txtPathFile->setText(fileName);
    maxTime = 0;

    //使组件变灰色，不可使用
    txtIpOrDomain->setEnabled(false);
    spnInterval->setEnabled(false);
    spnNumOfTimes->setEnabled(false);
    btnOpenFile->setEnabled(false);
    txtPathFile->setEnabled(false);
}

void widget::notPing(){
    btnPing->setText("Ping");
    lblBeExecuting->setText("执行结束!");
    lblTime->setText("");
    int yMaxExtent = 0;
    if(maxTime % 50>0){
        yMaxExtent = maxTime+50-maxTime % 50;//凑整增加到50，获取最大y轴长度
    }else{
        yMaxExtent = maxTime;//获取最大y轴长度
    }
    int xMaxExtent = interval*realityNumOfTimes;
    QString newFileName = QDir::currentPath() + "/" + formattedDateTime
                          +"_xE"+QString::number(xMaxExtent)
                          +"_yE"+QString::number(yMaxExtent)
                          +"_i"+QString::number(interval)
                          +"_r"+QString::number(realityNumOfTimes)
                          + ".bin";
    //ping间隔、ping次数、最大y轴、最大x轴
    file.rename(newFileName);
    txtPathFile->setText(newFileName);  // 更新显示的文件路径

    txtIpOrDomain->setEnabled(true);
    spnInterval->setEnabled(true);
    spnNumOfTimes->setEnabled(true);
    btnOpenFile->setEnabled(true);
    txtPathFile->setEnabled(true);
}
//判断是否发送命令
void widget::enableOrNot(){
    //从组件中获取用户设定的请求信息
    targetHost = txtIpOrDomain->text();
    interval = spnInterval->value();
    numOfTimes = spnNumOfTimes->value();

    if(targetHost.isEmpty()){
        //qDebug() << "请求地址不能为空";
        lblBeExecuting->setText("请求地址不能为空!");
        lblBeExecuting->setStyleSheet("color: red;");
        return;
    }

    if (btnPing->text() == "Ping") {
        enablePing();
        ping();  // 调用ping函数
    } else if((btnPing->text() == "Stop")){
        // 立即停止定时器
        if (timer) {
            timer->stop();
            timer->deleteLater();
            timer = nullptr; // 清空定时器指针
            notPing();
            return;
        }

        // 停止Ping操作
        numOfTimes = 0;
    }
}

//启动定时器
void widget::ping() {
    timer = new QTimer;//创建一个定时器对象
    timer->setInterval(interval * 1000);//设置成每隔多少毫秒 发一次信号
    connect(timer, &QTimer::timeout, this, &widget::onPingTimeout);//定时器信号绑定槽函数，每隔毫秒，就会触发一次，然后执行槽函数
    timer->start();  // 启动定时器
}

//定时期间与定时结束时的操作
void widget::onPingTimeout() {
    //设定  定时器停止条件
    if (numOfTimes <= 0) {
        timer->stop();//停止计时
        timer->deleteLater();//删除操作推迟到事件循环的空闲时刻，从而避免在定时器事件处理过程中直接删除对象可能引起的问题。
        timer = nullptr; // 清空定时器指针
        notPing();
        return;
    }

    realityNumOfTimes++;

    int count = 1; // 发送的包数
    int size = 1; // 数据包大小
    int timeout = interval*1000; // 超时时间（毫秒）


    // 组装 ping 命令和参数
    QStringList arguments;
    arguments << targetHost; // 目标主机
    arguments << "-n" << QString::number(count); // 发送包数
    arguments << "-l" << QString::number(size); // 数据包大小
    arguments << "-w" << QString::number(timeout); // 超时时间

    QProcess pingProcess; // 创建 QProcess 对象
    pingProcess.start("ping", arguments); // 启动 ping 命令

    // 等待进程结束并读取输出
    if (pingProcess.waitForFinished()) {
        QByteArray outputArr = pingProcess.readAllStandardOutput();
        //QList<QByteArray> outputLst = outputArr.split('\n'); // 或者使用 output.split("\r\n")
        QString dataStr = QString::fromLocal8Bit(outputArr);
        QRegularExpressionMatch matchData = regexData.match(dataStr);
        if (matchData.hasMatch()) {
            QString timeStr = matchData.captured(1);
            int time = timeStr.toInt();
            if(time>maxTime){
                maxTime = time;
            }
            writeIntToFile(fileName,time);
            lblTime->setText("("+QString::number(realityNumOfTimes)+")    "+timeStr+"ms");
            lblTime->setStyleSheet("color:green;");
        } else {
            writeIntToFile(fileName,-1);
            lblTime->setText("请求错误");
            lblTime->setStyleSheet("color:red;");
        }
    }else {
        lblTime->setText("");
        lblBeExecuting->setText("ping进程发生错误");
        lblBeExecuting->setStyleSheet("color: red;");
        numOfTimes = -1;
    }
    --numOfTimes;
}

void widget::closeEvent(QCloseEvent *event)
{
    // 保存当前的间隔值defaultNumOfTimes
    settings.setValue("interval", spnInterval->value());
    settings.setValue("numOfTimes", spnNumOfTimes->value());

    // 调用基类的 closeEvent 以确保正常关闭
    QWidget::closeEvent(event);
}

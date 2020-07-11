#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QList>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(640,720);
    setWindowTitle("Serial");

    text = new QTextEdit(this);
    text->setGeometry(320,40,310,565);

    text->setReadOnly(true);

    addmenu();
    serial_init();

}

/*串口初始化*/
void MainWindow::serial_init()
{
    if(serial.isOpen())
        return;

    serialinfo = QSerialPortInfo::availablePorts();
    qDebug() << "可用串口数:" << serialinfo.count();
    if(serialinfo.count())
    {
        ui->serial_box->clear();
        ui->serial_box->setStyleSheet("QComboBox{color:red}");
        for(const QSerialPortInfo &serialport : serialinfo)
        {
            QStringList list;
            list << serialport.portName();
            ui->serial_box->addItem(list.first(),list);
        }
    }
    else
        breakenslot();

    connect(&serial,&QSerialPort::readyRead,this,&MainWindow::serial_read_slot);
}



MainWindow::~MainWindow()
{
    delete ui;
}

/*打开文件对话框槽函数*/
void MainWindow::openslot()
{
    QString fileformat = "All file(*);;"
                         "Text file(*.text);;"
                         "C/C++ source file(*.c);;"
                         "C/C++ head file(*.h)";
    QString filename = QFileDialog::getOpenFileName(this,"open file","/",fileformat);

    if( !filename.isEmpty() )
    {
        QFile file(filename);
        if(file.open(QFileDevice::ReadOnly))
        {
            QTextStream input(&file);
            QString txt;

            input.setCodec("UTF-8");
            while(1)
            {
                input>>txt;
                if(txt.isEmpty())
                    break;
                text->insertPlainText(txt);
            }
            file.close();
        }
    }
}

void MainWindow::buildslot()
{
    text->setText(NULL);
}

/*保存窗口数据为文件槽函数*/
void MainWindow::saveslot()
{
    QString fileformat = "Text file(*.text);;"
                         "C/C++ source file(*.c);;"
                         "C/C++ head file(*.h);;"
                         "All file(*)";
    QString filename = QFileDialog::getSaveFileName(this,"save file","/Qt",fileformat);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"save file","the text save failed!");
    }
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream output(&file);
        output.setCodec("UTF-8");

        QString txt = text->toPlainText();
        output << txt;
        QMessageBox::information(this,"save","the text save to"+filename);
        file.close();
    }
}

/*另存为槽函数*/
void MainWindow::save_atslot()
{
    QString fileformat = "Text file(*.text);;"
                         "C/C++ source file(*.c);;"
                         "C/C++ head file(*.h);;"
                         "All file(*)";
    QString filename = QFileDialog::getSaveFileName(this,"save file","/",fileformat);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"save as file","the text save failed!");
    }
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream output(&file);
        output.setCodec("UTF-8");

        QString txt = text->toPlainText();
        output << txt;
        QMessageBox::information(this,"save as","the text save to"+filename);
        file.close();
    }
}


/*获取时间*/
QDateTime MainWindow::gettime()
{
    return QDateTime::currentDateTime();
}

/*添加菜单栏*/
void MainWindow::addmenu(void)
{
    /*添加菜单*/
    file = menuBar()->addMenu("&文件");
    edit = menuBar()->addMenu("&编辑");
    tool = menuBar()->addMenu("&工具");
    help = menuBar()->addMenu("&帮助");

    /*添加file动作*/
    build = new QAction("&新建",this);
    build->setShortcut(QKeySequence::New);
    build->setStatusTip("build a file");
    file->addAction(build);

    open = new QAction("&打开",this);
    open->setShortcut(QKeySequence::Open);
    open->setStatusTip("open a file");
    file->addAction(open);

    save = new QAction("&保存",this);
    save->setShortcut(QKeySequence::Save);
    save->setStatusTip("save a file");
    file->addAction(save);

    save_at = new QAction("&另存为",this);
    save_at->setShortcut(QKeySequence::SaveAs);
    save_at->setStatusTip("save a file at...");
    file->addAction(save_at);

    closed = new QAction("&关闭",this);
    closed->setShortcut(QKeySequence::Close);
    closed->setStatusTip("close a file");
    file->addAction(closed);

    /*添加edit动作*/
    begin = new QAction("开始",this);
    edit->addAction(begin);

    stop = new QAction("暂停",this);
    edit->addAction(stop);

    breaken = new QAction("断开",this);
    edit->addAction(breaken);

    /*添加工具动作*/
    set = new QAction("设置",this);
    tool->addAction(set);

    /*添加帮助动作*/
    about = new QAction("关于",this);
    help->addAction(about);

    /*radio默认选项*/
    ui->rx_asc_radio->setChecked(true);
    ui->send_asc_radio->setChecked(true);

    /*发送记录框*/
    ui->send_text_history->setMaxCount(10);

    /*状态栏*/
    sta = new QLabel("CLOSE  ",this);
    sta->setStyleSheet("QLabel{color:red}");
    statusBar()->addWidget(sta);

    time_label = new QLabel(gettime().toString("  yyyy年MM月dd日 hh时mm分ss秒 ddd"));

    time_label->setMinimumSize(time_label->sizeHint());
    time_label->setAlignment(Qt::AlignRight);

    statusBar()->addWidget(time_label);

    /*工具栏*/
    QToolBar *toolbar = new QToolBar("&file");
    toolbar->addAction(build);

    /*绑定文件动作*/
    connect(open,SIGNAL(triggered(bool)),this,SLOT(openslot()));
    connect(build,SIGNAL(triggered(bool)),this,SLOT(buildslot()));
    connect(save,SIGNAL(triggered(bool)),this,SLOT(saveslot()));
    connect(save_at,SIGNAL(triggered(bool)),this,SLOT(save_atslot()));
    connect(closed,SIGNAL(triggered(bool)),this,SLOT(close()));

    /*绑定编辑动作*/
    connect(begin,SIGNAL(triggered(bool)),this,SLOT(beginslot()));
    connect(stop,SIGNAL(triggered(bool)),this,SLOT(stopslot()));
    connect(breaken,SIGNAL(triggered(bool)),this,SLOT(breakenslot()));
    connect(ui->bt_box,SIGNAL(currentIndexChanged(int)),this,SLOT(btchangedslot(int)));

}

/*自定义波特率槽函数*/
void MainWindow::btchangedslot(int dex)
{
    if(dex == 5)
        ui->bt_box->setEditable(true);
}

/*开启连接槽函数*/
void MainWindow::beginslot()
{
    QString com = ui->serial_box->currentText();
    QString bt = ui->bt_box->currentText();
    QString data = ui->data_box->currentText();
    QString check = ui->check_box->currentText();
    QString stop = ui->stop_box->currentText();
    QString stream = ui->stram_box->currentText();


    serial.setPortName(com);
    serial.setBaudRate(bt.toInt());

    switch (data.toInt()) {
    case 5:
        serial.setDataBits(QSerialPort::Data5);
        break;
    case 6:
        serial.setDataBits(QSerialPort::Data6);
        break;
    case 7:
        serial.setDataBits(QSerialPort::Data7);
        break;
    case 8:
        serial.setDataBits(QSerialPort::Data8);
        break;
    default:
        break;
    }

    if(check == "None")
        serial.setParity(QSerialPort::NoParity);
    else if(check == "Even")
        serial.setParity(QSerialPort::EvenParity);
    else if(check == "Odd")
        serial.setParity(QSerialPort::OddParity);
    else if(check == "Space")
        serial.setParity(QSerialPort::SpaceParity);
    else if(check == "Mark")
        serial.setParity(QSerialPort::MarkParity);

    if(stop == "1.5")
        serial.setStopBits(QSerialPort::OneAndHalfStop);
    else
        switch (stop.toInt()) {
        case 1:
            serial.setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serial.setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }

    if(stream == "None")
        serial.setFlowControl(QSerialPort::NoFlowControl);
    else if(stream == "RTS/CTS")
        serial.setFlowControl(QSerialPort::HardwareControl);
    else if(stream == "XON/XOFF")
        serial.setFlowControl(QSerialPort::SoftwareControl);

    if(serial.open(QIODevice::ReadWrite))
    {
        sta->setText("OPEN  ");

        ui->serial_box->setEnabled(false);
        ui->bt_box->setEnabled(false);
        ui->check_box->setEnabled(false);
        ui->data_box->setEnabled(false);
        ui->stop_box->setEnabled(false);
        ui->stram_box->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this,"Notice","open "+ui->serial_box->currentText()+" false");
    }

}


/*读取串口数据槽函数*/
void MainWindow::serial_read_slot()
{
    QByteArray temp = serial.readAll();
    qDebug()<<temp;

    /*二进制 or 十六进制*/
//    QByteArray Asc = QByteArray::number(temp,2);

//    if( ui->rx_hex_radio->isChecked() )
//        Asc = QByteArray::number(temp,16);
    /*显示时间*/
    if(ui->time_check->isChecked())
        text->append(gettime().toString("hh.mm.ss") + ":");

    text->append(temp);
}

/*暂停显示数据槽函数*/
void MainWindow::stopslot()
{
    disconnect(&serial,&QSerialPort::readyRead,this,&MainWindow::serial_read_slot);
}

/*断开串口连接槽函数*/
void MainWindow::breakenslot()
{
    serial.clear();
    serial.close();
    ui->serial_box->setStyleSheet("QComboBox{color:black}");
    ui->serial_box->setEnabled(true);
    ui->bt_box->setEnabled(true);
    ui->check_box->setEnabled(true);
    ui->data_box->setEnabled(true);
    ui->stop_box->setEnabled(true);
    ui->stram_box->setEnabled(true);
}

/*发送串口数据槽函数*/
void MainWindow::on_open_file_btn_clicked()
{
    QString buf = ui->send_text->toPlainText();

    /*显示时间*/
    if(ui->time_check->isChecked())
        text->append(gettime().toString("hh.mm.ss") + ":");

    /*16进制*/
    if(ui->send_hex_radio->isChecked())
    {
        QByteArray Hex = buf.toLatin1();
        Hex = QByteArray::fromHex(Hex);
        Hex = Hex.toHex();
        serial.write(Hex);

        /*显示发送*/
        if(ui->send_check->isChecked())
            text->append(Hex);
    }
    else
    {
        /*二进制*/
        int temp = buf.toInt();
        QByteArray Asc = QByteArray::number(temp,2);
        serial.write(Asc);

        /*显示发送*/
        if(ui->send_check->isChecked())
            text->append(Asc);
    }

    ui->send_text_history->addItem(buf);
    if(ui->send_text_history->count() == 10)
        ui->send_text_history->clear();
}

void MainWindow::on_send_text_history_currentTextChanged(const QString &arg1)
{
    if(ui->send_text_history->currentText() == NULL)
        return;
    ui->send_text->setText(ui->send_text_history->currentText());
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == time1)
    {
        time_label->setText(gettime().toString("  yyyy年MM月dd日 hh时mm分ss秒 ddd"));
        //serial_init();

    }
    if(event->timerId() == time2)
        on_open_file_btn_clicked();
}

void MainWindow::on_send_check_2_stateChanged(int arg1)
{
    if(ui->send_check_2->isChecked())
        time2 = startTimer(ui->send_check_time->text().toInt());
    else
    {
        if(time2 == 0)
            return;
        killTimer(time2);
        time2 = 0;
    }
}

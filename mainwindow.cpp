#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(400,480);
    setWindowTitle("Serial");

    text = new QTextEdit(this);
    text->setGeometry(175,50,220,300);
    text->setStyleSheet("QTextEdit{background-color:black}"
                        "QTextEdit{color:white}");
    text->setReadOnly(true);

    addmenu();
    serial_init();

}

void MainWindow::serial_init()
{
//    QString com;

//    com = serial.portName();
//   qDebug() << "name:" <<com;

//    com = serial.systemLocation();
//    qDebug() << "\npc:" << com;

//    com = serial.serialNumber();
//    qDebug() << "\nnum:" << com;

//    if( com.isEmpty() )
//        QMessageBox::information(this,"serial link","have not serialCOM");

    connect(&serial,&QSerialPort::readyRead,this,&MainWindow::serial_read_slot);


}



MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::btchangedslot(int dex)
{
    if(dex == 5)
        ui->bt_box->setEditable(true);
}

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
        ui->serial_box->setEnabled(false);
        ui->bt_box->setEnabled(false);
        ui->check_box->setEnabled(false);
        ui->data_box->setEnabled(false);
        ui->stop_box->setEnabled(false);
        ui->stram_box->setEnabled(false);
    }

}


void MainWindow::serial_read_slot()
{
    QByteArray buf;
    buf = serial.readAll();

    text->append(buf);

}

void MainWindow::stopslot()
{
    //disconnect(&serial,&QSerialPort::readyRead,this,&QIODevice::readData);
}

void MainWindow::breakenslot()
{
    serial.clear();
    serial.close();
    ui->serial_box->setEnabled(true);
    ui->bt_box->setEnabled(true);
    ui->check_box->setEnabled(true);
    ui->data_box->setEnabled(true);
    ui->stop_box->setEnabled(true);
    ui->stram_box->setEnabled(true);
}

void MainWindow::on_send_btn_clicked()
{

}

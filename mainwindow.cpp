#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "text.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(320,400);

    addmenu();



}

void MainWindow::openslot()
{
    qDebug()<<"open file";
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

    close = new QAction("&关闭",this);
    close->setShortcut(QKeySequence::Close);
    close->setStatusTip("close a file");
    file->addAction(close);

    /*添加edit动作*/
    begin = new QAction("开始",this);
    edit->addAction(begin);

    stop = new QAction("暂停",this);
    edit->addAction(stop);

    close = new QAction("关闭",this);
    edit->addAction(close);

    /*添加工具动作*/
    set = new QAction("设置",this);
    tool->addAction(set);

    /*添加帮助动作*/
    about = new QAction("关于",this);
    help->addAction(about);

    /*绑定菜单栏*/
    connect(open,SIGNAL(triggered(bool)),this,SLOT(openslot()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

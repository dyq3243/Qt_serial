#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QToolBox>
#include <QKeySequence>
#include "text.h"
#include <QFile>
#include <QFileDialog>
#include <QFileDevice>
#include <QLabel>
#include <QState>
#include <QStatusBar>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDateTime>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openslot(void);
    void buildslot(void);
    void saveslot(void);
    void save_atslot(void);

    void beginslot(void);
    void stopslot(void);
    void breakenslot(void);
    void btchangedslot(int);

    void serial_read_slot(void);

    void on_open_file_btn_clicked();

    void timerEvent(QTimerEvent *event);

    void on_send_text_history_currentTextChanged(const QString &arg1);

    void on_send_check_2_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    void addmenu(void);
    void serial_init(void);
    QDateTime gettime(void);

    QTextEdit *text;
    QSerialPort serial;
    QLabel *sta;
    QLabel *time_label;
    QList<QSerialPortInfo> serialinfo;

    int time1 = startTimer(1000);
    int time2 = 0;

    /*菜单栏*/
    QMenu *file;
    QMenu *edit;
    QMenu *tool;
    QMenu *help;

    /*文件动作*/
    QAction *open;
    QAction *build;
    QAction *closed;
    QAction *save;
    QAction *save_at;
    /*编辑动作*/
    QAction *begin;
    QAction *stop;
    QAction *breaken;
    /*工具动作*/
    QAction *set;
    /*帮助动作*/
    QAction *about;
};

#endif // MAINWINDOW_H

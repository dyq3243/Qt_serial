#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QKeySequence>

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

private:
    Ui::MainWindow *ui;

    void addmenu(void);

    /*菜单栏*/
    QMenu *file;
    QMenu *edit;
    QMenu *tool;
    QMenu *help;

    /*文件动作*/
    QAction *open;
    QAction *build;
    QAction *close;
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

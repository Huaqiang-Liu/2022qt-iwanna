#ifndef MAINWINDOW_H
#define MAINWINDOW_H

extern bool paint_what;
extern int choice;

#include <QMainWindow>
#include "exitwindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QFont>
#include <QPen>
#include "kid.h"
#include "Map.h"
#include "stab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//
extern int map[5][22][32];//????????????????????????????????????????


//
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    //…………………………………………………………………

    int timer1;
    bool is_kill_timer1;//管人物左右移动的计时器
    int timer2;
    int timer3;

    //…………………………………………………………………

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    int direction;//-1左0中1右
    void Kid_Move(const int &dir);
    void Kid_Die();
    void Jump_Fall();
    void Kid_Win();
private slots:


    void on_chapter1_toggled(bool checked);

    void on_chapter2_toggled(bool checked);

    void on_chapter3_toggled(bool checked);

    void on_chapter4_toggled(bool checked);

    void on_chapter5_toggled(bool checked);

    void on_exit_button_clicked();




    void on_help_toggled(bool checked);

    void on_exitHelp_toggled(bool checked);

public:
    Ui::MainWindow *ui;
    Exitwindow* exit;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%#@%$^%^&*(&^%$#@#$#%$^%^&*()
};
#endif // MAINWINDOW_H
//

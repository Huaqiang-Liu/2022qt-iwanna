#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <exitwindow.h>
#include <QPushButton>

bool holyflag = 0;

bool paint_what;                //0则画游戏界面背景，1则画菜单背景
int choice;                     //选择关卡，在下面的绘图事件中
bool can_left, can_right;
bool alive;                     //1活0死
bool win;                       //1赢，kill timer1
int up_down;                    //1上0停-1下
int count;                      //Kid_Move被调用就+1，%12余0时改变动作（kid1->kid2）
int count_;                     //Jump_Fall被调用就+1

int init_dy=0;                  //一个像素很小(确信)，所以只能int
int ddy=1;                      //init_dy每次+或者-ddy
              //80
int born_x[5]={1140,40,600,40,1000}, born_y[5]={100,160,80,40,40}, ending_x[5]={1200,1200,1240,1240,1240}, ending_y[5]={600,200,80,520,640};
//                       都是左上角
bool menuOrHelp;//0画主菜单，1画help

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIconText(QString("iwanna"));
    ui->exit_button->hide();
    ui->exit_button->setFocus();
    ui->exit_button->setShortcut(Qt::Key_Escape);
    ui->exitHelp->hide();

    ui->label->hide();
    ui->label->raise();
    paint_what = 1;
    choice = 0;
    direction = 0;
    win = 0;
    alive = 1;
    can_left = 1, can_right = 1;
    count = 0;
    menuOrHelp = 0;

    QTimer::singleShot(1500, this, [=](){
        timer1 = startTimer(10);                //10ms触发一次!!!!!!!!!!!!!!!
    });

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {//画主菜单
    QPainter painter(this);
    if(paint_what == 1)
    {//画菜单
        if(menuOrHelp == 0)//主页面
            painter.drawPixmap(0, 0, 1280, 880, QPixmap(":/images/menu.png"));//横纵宽高
        else if(menuOrHelp == 1)//帮助界面
        {
            painter.drawPixmap(0, 0, 1280, 880, QPixmap(":/images/menu.png"));
            painter.drawPixmap(800, 0, 480, 480, QPixmap(":/images/tutorial.png"));
        }
    }
    if(paint_what == 0)
    {//画游戏背景
        if(choice != 0)
        {
            for(int i=0;i<22;i++)
                for(int j=0;j<32;j++)
                    switch(map[choice-1][i][j])
                    {
                    case BA:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/background.png"));break;}
                    case PB:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/pure_block.png"));break;}
                    case BG:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/block_with_grass.png"));break;}
                    case CP:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/checkpoint.png"));break;}
                    case SU:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/stab_up.png"));break;}
                    case SD:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/stab_down.png"));break;}
                    case SL:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/stab_left.png"));break;}
                    case SR:{painter.drawPixmap(40*j, 40*i, 40, 40, QPixmap(":/images/stab_right.png"));break;}
                    default:break;
                    }
        }
    }


    //qDebug()<<win;
    if(win == 1)
    {
        alive = 1;
        painter.drawPixmap(0,178,1280,525,QPixmap(":/images/win.png"));
    }
    if(alive == 0)
    {
        painter.drawPixmap(0,35,1280,720,QPixmap(":/images/death_menu.png"));
    }

}
//键盘事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(win == 1)
    {//赢了按enter返回主菜单
        if (event->key() == Qt::Key_Return)
        {
            paint_what = 1;
            win = 0;
            alive = 1;
            update();


            ui->help->setChecked(0);
            ui->help->setVisible(1);

            ui->chapter1->setChecked(0);
            ui->chapter1->setVisible(1);
            ui->chapter2->setChecked(0);
            ui->chapter2->setVisible(1);
            ui->chapter3->setChecked(0);
            ui->chapter3->setVisible(1);
            ui->chapter4->setChecked(0);
            ui->chapter4->setVisible(1);
            ui->chapter5->setChecked(0);
            ui->chapter5->setVisible(1);//让被隐藏的按钮回来
            ui->exit_button->hide();
            ui->label->hide();
        }
    }


    if (event->key() == Qt::Key_A)
        direction = -1;
    if (event->key() == Qt::Key_D)
        direction = 1;
    if (event->key() == Qt::Key_Space && up_down == 0)
    {
        init_dy = 11;//上跳高度，等会要调！！！！
        up_down = 1;
    }

    if (alive == 0)
    {//死了按R重开
        if (event->key() == Qt::Key_R)
        {
            win = 0;
            alive = 1;
            ui->exit_button->setVisible(1);
            ui->label->move(born_x[choice-1], born_y[choice-1]);
            QPixmap kid1_image(":/images/kid1.png");
            ui->label->setPixmap(kid1_image);
        }
    }


}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
        direction = 0;
    if (event->key() == Qt::Key_D)
        direction = 0;
}
//左右移动
void MainWindow::Kid_Move(const int &dir)
{
    if(win)return;

    QPixmap kid1_image(":/images/kid1.png");
    QPixmap kid2_image(":/images/kid2.png");
    QPixmap kid3_image(":/images/kid3.png");
    QPixmap kid4_image(":/images/kid4.png");
    QPixmap deadkid_image(":/images/dead_kid.png");
    int x = ui->label->x(), y=ui->label->y();
    int i = x / 40, j = y / 40;
    //label的左上角在map[choice-1][j][i]中,检查能否移动

    if(x>5 && y<=800)
    {
        if(y%40 == 0)
            can_left = (map[choice-1][j][i] != PB && map[choice-1][j][i] != BG);
        else
            can_left = (map[choice-1][j][i] != PB && map[choice-1][j][i] != BG
                    && map[choice-1][j+1][i] != PB && map[choice-1][j+1][i] != BG);
    }
    else can_left = 0;
    if(x<1240 && y<=800)
    {
        if(y%40 == 0)
            can_right = (map[choice-1][j][i+1] != PB && map[choice-1][j][i+1] != BG);
        else
            can_right = (map[choice-1][j][i+1] != PB && map[choice-1][j][i+1] != BG
                    && map[choice-1][j+1][i+1] != PB && map[choice-1][j+1][i+1] != BG);
    }
    else can_right = 0;
    if(!alive)
    {
        ui->label->setPixmap(deadkid_image);
        return;
    }
    //活着且能移动
    if(dir == -1 && can_left)
    {
        if(x<4){ui->label->move(0,y);}
        else
        {
            if(map[choice-1][j][(x-4)/40]==PB || map[choice-1][j][(x-4)/40]==BG)
                ui->label->move(x-x%40,y);
            else
                ui->label->move(x-4,y);
        }
        count++;if(count >= 10)count %= 10;
        if(count % 10 >= 5)
            ui->label->setPixmap(kid3_image);
        else
            ui->label->setPixmap(kid4_image);
    }
    if(dir == 1 && can_right)
    {
        if(x>1236){ui->label->move(1240,y);}
        else
        {
            if(map[choice-1][j][(x+4)/40+1]==PB || map[choice-1][j][(x+4)/40+1]==BG)
                ui->label->move(x+4-(x+4)%40,y);
            else
                ui->label->move(x+4,y);
        }
        count++;if(count > 10)count %= 10;
        if(count % 10 >= 5)
            ui->label->setPixmap(kid1_image);
        else
            ui->label->setPixmap(kid2_image);
    }
}
//跳与下落
void MainWindow::Jump_Fall()
{
    if(win)return;
    QPixmap deadkid_image(":/images/dead_kid.png");
    if(!alive)
    {
        ui->label->setPixmap(deadkid_image);
        return;
    }
    count_++;
    if(count_ >= 114)count_ = 0;
    int x = ui->label->x(), y=ui->label->y();
    int i = x / 40, j = y / 40;
    //label的左上角在map[choice-1][j][i]中
    if(j >= 21){alive = 0; return;}//判断越界而死
    if(up_down == -1)// && (map[choice-1][j+1][i] == PB || map[choice-1][j+1][i] == BG))
    {//落地咯
        if(x%40==0)
        {
            if(map[choice-1][j+1][i] == PB || map[choice-1][j+1][i] == BG)
            {
                up_down = 0;
                ui->label->move(x, y-y%40);//防止“陷到地里”
            }
        }
        else
        {

            if(map[choice-1][j+1][i] == PB || map[choice-1][j+1][i] == BG
                    || map[choice-1][j+1][i+1] == PB || map[choice-1][j+1][i+1] == BG)
            {
                up_down = 0;
                ui->label->move(x, y-y%40);//防止“陷到地里”
            }
        }
    }



    if(direction == 1 && up_down == 0 && map[choice-1][j+1][i] == BA)
    {//防止人物走出平台之后浮空
        up_down = -1;
        init_dy = 0;
    }
    //if(direction == 1 && up_down == 0 && map[choice-1][j+1][i] == BG)
    if(direction == -1 && up_down == 0 && map[choice-1][j+1][i+1] == BA)
    {
        up_down = -1;
        init_dy = 0;
    }



    if(x%40 == 0 && (map[choice-1][j-1][i] == PB || map[choice-1][j-1][i] == BG) && y%40<3)
    {//碰头咯
        up_down = -1;
        ui->label->move(x, y-y%40);
    }
    else if(x%40 != 0 && ((map[choice-1][j-1][i] == PB || map[choice-1][j-1][i] == BG)
            || (map[choice-1][j-1][i] == PB || map[choice-1][j-1][i] == BG)) && y%40<3)
    {//同时碰头2块砖（悲
        up_down = -1;
        ui->label->move(x, y-y%40);
    }


    if(x%40 == 0 && (map[choice-1][j][i] == PB || map[choice-1][j][i] == BG))
    {//头已经卡墙里咯！（要不是刷新频率不够。。。）
        up_down = -1;
        ui->label->move(x, y+40-y%40);
    }
    else if(x%40 != 0 && ((map[choice-1][j][i] == PB || map[choice-1][j][i] == BG)
            || (map[choice-1][j][i+1] == PB || map[choice-1][j][i+1] == BG)))
    {
        up_down = -1;
        ui->label->move(x, y+40-y%40);
    }



    if(up_down == 1)
    {
        ui->label->move(x, y-init_dy);
        if(count_ % 3 == 0)init_dy -= ddy;
        if(init_dy <= 0){init_dy = 0; up_down = -1;}
    }
    else if(up_down == 0)
    {
        //init_dy = 0;
    }
    else if(up_down == -1)
    {
        ui->label->move(x, y+init_dy);
        if(count_ % 3 == 0)init_dy += ddy;
    }


}
//为了左右移动和跳跃，需要加入qTimer

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timer1)
    {
        if(!win && alive)
        {
            Kid_Move(direction);
            Jump_Fall();
            Kid_Win();

        }
        Kid_Die();
        ui->exit_button->setShortcut(Qt::Key_Escape);
    }
}


//死亡判定（遇刺/掉下边界）与重新开始
void MainWindow::Kid_Die()
{
    QPixmap deadkid_image(":/images/dead_kid.png");
    int x = ui->label->x(), y=ui->label->y();
    int i = x / 40, j = y / 40;
    //label的左上角在map[choice-1][j][i]中
    //判断死没死，一共12个，小心越界！！！

    //头顶撞刺
    if(x<1240 && x%40>20 && y%40>=35 && map[choice-1][j][i+1]==SD)
    {alive = 0;qDebug()<<0; goto door;}
    else if(x%40<20 && y%40>=35 && map[choice-1][j][i]==SD)//                    OK
    {alive = 0;qDebug()<<1; goto door;}
    //直击腰子（左右2种）
        //左边挨刀子
    if(y<840 && y%40>20 && x%40>=35 && map[choice-1][j+1][i]==SR)
    {alive = 0;qDebug()<<2; goto door;}
    else if(x%40<20 && y%40>=35 && map[choice-1][j][i]==SR)//                    OK
    {alive = 0;qDebug()<<3; goto door;}
        //右边挨刀子
    if(x<1240 && y<840 && y%40>20 && x%40<=5 && map[choice-1][j+1][i+1]==SL)
    {alive = 0;qDebug()<<4; goto door;}
    else if(x<1240 && y%40<20 && x%40<=5 && map[choice-1][j][i+1]==SL)//         OK
    {alive = 0;qDebug()<<5; goto door;}
    //爆菊
    if(x<1240 && y<840 && x%40>20 && y%40<=5 && map[choice-1][j+1][i+1]==SU)
    {alive = 0;qDebug()<<6; goto door;}
    else if(y<840 && x%40<20 && x%40!=0 && y%40<=5 && map[choice-1][j+1][i]==SU)//OK
    {alive = 0;qDebug()<<7; goto door;}
    //下面擦边（4种）           y-2*x>=40*j-80*i          2*x+y>=80*i+80+40*j
        //左纵
    if(y<840 && y-2*x>=40*(j+1)-80*(i+1) && map[choice-1][j+1][i]==SU)
    {alive = 0;qDebug()<<8; goto door;}
        //左横
    if(y<840 && 2*y-x>=80*j-40*i && map[choice-1][j+1][i]==SR)
    {alive = 0;qDebug()<<9; goto door;}
        //右纵
    if(x<1240 && y<840 && 2*x+y>=80*i+40*(j+1) && map[choice-1][j+1][i+1]==SU)
    {alive = 0;qDebug()<<10; goto door;}
        //右横
    if(x<1240 && y<840 && x+2*y>=40*(i+1)+80*j && map[choice-1][j+1][i+1]==SL)
    {alive = 0;qDebug()<<11; goto door;}
    //头上擦边（4种）
        //左纵
    if(2*x+y<=80*i+80+40*j && map[choice-1][j][i]==SD)//                         OK
    {alive = 0;qDebug()<<12; goto door;}
        //左横
    if(2*y+x<=80*j+80+40*i && map[choice-1][j][i]==SR)//                         OK
    {alive = 0;qDebug()<<13; goto door;}
        //右纵
    if(x<1240 && y-2*(x+40)<=40*j-80*(i+1) && map[choice-1][j][i+1]==SD)//       OK
    {alive = 0;qDebug()<<14; goto door;}
        //右横
    if(x<1240 && y-2*(x+40)<=80*j-40*i && map[choice-1][j][i+1]==SL)//           OK
    {alive = 0;qDebug()<<15; goto door;}




door:if(!alive)
    {
        ui->exit_button->hide();
        ui->label->setPixmap(deadkid_image);
        update();
    }


}
//胜利判定
void MainWindow::Kid_Win()
{
    if(holyflag)return;
    if(win)return;
    int x = ui->label->x(), y=ui->label->y();
    //判断赢没赢
    if(abs(x-ending_x[choice-1])+abs(y-ending_y[choice-1])<=40)
        win = 1;
    if(win == 1)
    {
        holyflag = 1;
        ui->exit_button->hide();
        ui->label->hide();
        update();
        return;
    }
}





void MainWindow::on_chapter1_toggled(bool checked)
{
    if(checked){
        holyflag = 0;
        paint_what = 0;
        choice = 1;
        ui->label->setVisible(1);
        ui->label->move(born_x[0],born_y[0]);

        win = 0;
        alive = 1;

        update();
        ui->chapter1->hide();
        ui->chapter2->hide();
        ui->chapter3->hide();
        ui->chapter4->hide();
        ui->chapter5->hide();
        ui->exit_button->show();

        ui->help->hide();
        ui->exitHelp->hide();

    }
}
void MainWindow::on_chapter2_toggled(bool checked)
{
    if(checked){
        holyflag = 0;
        paint_what = 0;
        choice = 2;
        ui->label->setVisible(1);
        ui->label->move(born_x[1],born_y[1]);

        win = 0;
        alive = 1;

        update();
        ui->chapter1->hide();
        ui->chapter2->hide();
        ui->chapter3->hide();
        ui->chapter4->hide();
        ui->chapter5->hide();
        ui->exit_button->show();

        ui->help->hide();
        ui->exitHelp->hide();

    }
}
void MainWindow::on_chapter3_toggled(bool checked)
{
    if(checked){
        holyflag = 0;
        paint_what = 0;
        choice = 3;
        ui->label->setVisible(1);
        ui->label->move(born_x[2],born_y[2]);

        win = 0;
        alive = 1;

        update();
        ui->chapter1->hide();
        ui->chapter2->hide();
        ui->chapter3->hide();
        ui->chapter4->hide();
        ui->chapter5->hide();
        ui->exit_button->show();

        ui->help->hide();
        ui->exitHelp->hide();

    }
}
void MainWindow::on_chapter4_toggled(bool checked)
{
    if(checked){
        holyflag = 0;
        paint_what = 0;
        choice = 4;
        ui->label->setVisible(1);
        ui->label->move(born_x[3],born_y[3]);

        win = 0;
        alive = 1;

        update();
        ui->chapter1->hide();
        ui->chapter2->hide();
        ui->chapter3->hide();
        ui->chapter4->hide();
        ui->chapter5->hide();
        ui->exit_button->show();

        ui->help->hide();
        ui->exitHelp->hide();

    }
}
void MainWindow::on_chapter5_toggled(bool checked)
{
    if(checked){
        holyflag = 0;
        paint_what = 0;
        choice = 5;
        ui->label->setVisible(1);
        ui->label->move(born_x[4],born_y[4]);

        win = 0;
        alive = 1;

        update();
        ui->chapter1->hide();
        ui->chapter2->hide();
        ui->chapter3->hide();
        ui->chapter4->hide();
        ui->chapter5->hide();
        ui->exit_button->show();

        ui->help->hide();
        ui->exitHelp->hide();



    }
}

void MainWindow::on_exit_button_clicked()
{
    exit = new Exitwindow(this);
    exit->exec();
}
void MainWindow::on_help_toggled(bool checked)
{
    if(checked){
            paint_what = 1;
            menuOrHelp = 1;
            update();


            ui->help->hide();
            ui->exitHelp->show();
            ui->exitHelp->setChecked(0);
        }
}
void MainWindow::on_exitHelp_toggled(bool checked)
{
    if(checked){
            paint_what = 1;
            menuOrHelp = 0;
            update();

            ui->chapter1->show();
            ui->chapter2->show();
            ui->chapter3->show();
            ui->chapter4->show();
            ui->chapter5->show();
            ui->help->show();
            ui->help->setChecked(0);
            ui->exitHelp->hide();
        }
}


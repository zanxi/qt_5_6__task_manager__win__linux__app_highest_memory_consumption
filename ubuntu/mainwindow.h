#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QtCharts>
#include <QChartView>
#include <QSplineSeries>
#include <QLayout>
#include <proctool.h>
#include <sys/types.h>
#include <signal.h>

//#include "filtermemory.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double calculate(int num);

    void SortMemory();

public slots:
    void update();
    void update2();
    void customMenuRequested(QPoint);
    void killpid();
    void showDetail();
    void mRun();
    void mKill();
    void mDetail();
    void chartDraw();

private:
    Ui::MainWindow *ui;

    QMultiMap<long long, QVector<QString>> map;

    QLabel* lstatu;
    QPushButton* bstatu;
    QSplineSeries* series,*series2,*series3;
    QChart* chart,*chart2;
    int max_Y = 2000;

    //FilterMemory *fm;
};

#endif // MAINWINDOW_H

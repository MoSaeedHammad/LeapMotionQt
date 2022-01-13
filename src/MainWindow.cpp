#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "LeapConnection.h"
#include "LeapC.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "stdio.h"
#include <QtCore>
#include <QDebug>

int64_t lastFrameID = 0; //The last frame received


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    
{
    ui->setupUi(this);

    OpenConnection();
    while(!IsConnected)
      millisleep(100); //wait a bit to let the connection complete
    printf("Connected.");
    LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties();
    if(deviceProps)
      printf("Using device %s.\n", deviceProps->serial);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timer_timeout_handler()));
    timer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timer_timeout_handler()
{
    LEAP_TRACKING_EVENT *frame = GetFrame();
    if(frame && (frame->tracking_frame_id > lastFrameID)){
      lastFrameID = frame->tracking_frame_id;
      printf("Frame %lli with %i hands.\n", (long long int)frame->tracking_frame_id, frame->nHands);
      for(uint32_t h = 0; h < frame->nHands; h++){
        LEAP_HAND* hand = &frame->pHands[h];
        printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
                    hand->id,
                    (hand->type == eLeapHandType_Left ? "left" : "right"),
                    hand->palm.position.x,
                    hand->palm.position.y,
                    hand->palm.position.z);
        if (hand->type == eLeapHandType_Left)
        {
            ui->lxposition_value->setText(QString::number(hand->palm.position.x));
            ui->lyposition_value->setText(QString::number(hand->palm.position.y));
            ui->lzposition_value->setText(QString::number(hand->palm.position.z));
        }
        else if (hand->type == eLeapHandType_Right)
        {
            ui->rxposition_value->setText(QString::number(hand->palm.position.x));
            ui->ryposition_value->setText(QString::number(hand->palm.position.y));
            ui->rzposition_value->setText(QString::number(hand->palm.position.z));
        }
      }
    }

}

#include "LeapConnection.c"

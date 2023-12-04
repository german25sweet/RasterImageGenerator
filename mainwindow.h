#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>

#include "GrayscaleImageGenerator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartButtonClicked();
    void onStopButtonClicked();
    void onRotationChanged(int);

private:
    GrayscaleImageGenerator *_imageGenerator;

    QPushButton *startButton;
    QPushButton *stopButton;

    QSlider *rotationSlider;
    QLabel *rotationLabel;
};

#endif // MAINWINDOW_H

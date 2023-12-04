#include <QGraphicsView>
#include <QHBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _imageGenerator(nullptr)
{
    // Создание объекта GrayscaleImageGenerator
    _imageGenerator = new GrayscaleImageGenerator(5, 10, this);

    // Создание кнопок и подключение слотов
    startButton = new QPushButton("ВКЛ", this);
    stopButton = new QPushButton("ВЫКЛ", this);
    rotationSlider = new QSlider(Qt::Horizontal, this);
    rotationLabel = new QLabel("Угол: 0", this);
    rotationSlider->setFixedWidth(200);
    rotationSlider->setMinimum(0);
    rotationSlider->setMaximum(90);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);
    connect(rotationSlider, &QSlider::valueChanged, this, &MainWindow::onRotationChanged);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(rotationSlider);
    buttonLayout->addWidget(rotationLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(_imageGenerator->getGraphicsView());

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setFixedSize(_imageGenerator->getGraphicsView()->width(), 800);
    show();
}

MainWindow::~MainWindow()
{
    if (_imageGenerator) {
        delete _imageGenerator;
    }
}

void MainWindow::onStartButtonClicked()
{
    if (_imageGenerator) {
        _imageGenerator->startGeneration();
        startButton->setEnabled(false);
        stopButton->setEnabled(true);
    }
}

void MainWindow::onStopButtonClicked()
{
    if (_imageGenerator) {
        _imageGenerator->stopGeneration();
        stopButton->setEnabled(false);
        startButton->setEnabled(true);
    }
}

void MainWindow::onRotationChanged(int value)
{
    if (_imageGenerator) {
        qreal angle = static_cast<qreal>(value);
        _imageGenerator->setRotationAngle(angle);
    }

    rotationLabel->setText(QString("Угол: %1").arg(value));
}

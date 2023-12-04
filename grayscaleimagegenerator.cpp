#include <QPixmap>
#include <QRandomGenerator>
#include <QTransform>

#include "GrayscaleImageGenerator.h"
#include "ImageGenerationThread.h"

const int CELL_SIZE = 50;
const int MAX_RANDOM_VALUE = 1001;
const int _timerINTERVAL = 1000;

GrayscaleImageGenerator::GrayscaleImageGenerator(int rows, int columns, QWidget *parent)
    : QObject(parent)
    , _rows(rows)
    , _columns(columns)
    , _view(new QGraphicsView())
    , _timer(nullptr)
    , _timerActive(false)
    , _rotationAngle(0)
    , generationThread(nullptr)
    , _valuesArray(rows, QVector<unsigned short>(columns, 0))
{
    int maxSize = std::max(_columns, _rows) + 2;
    _view->setScene(&_scene);
    _view->setFixedSize(maxSize * CELL_SIZE, maxSize * CELL_SIZE);
    _view->setFrameShape(QFrame::NoFrame);

    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &GrayscaleImageGenerator::updateImage);

    generationThread = new ImageGenerationThread(this);
    connect(generationThread,
            &ImageGenerationThread::generationComplete,
            this,
            &GrayscaleImageGenerator::handleImageGenerationComplete);
}

GrayscaleImageGenerator::~GrayscaleImageGenerator()
{
    if (generationThread) {
        if (generationThread->isRunning()) {
            generationThread->quit();
            generationThread->wait();
        }
        delete generationThread;
    }

    clearScene();
    delete _view;
}

void GrayscaleImageGenerator::handleImageGenerationComplete()
{
    clearScene();
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _columns; ++j) {
            unsigned short value = _valuesArray[i][j];
            QImage image = createGrayImage(value);

            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
            item->setPos(j * CELL_SIZE, i * CELL_SIZE);

            _scene.addItem(item);
            _items.append(item);
        }
    }
    if (_view)
        _view->viewport()->repaint();

    if (_timerActive)
        _timer->start(_timerINTERVAL);
}

void GrayscaleImageGenerator::generateRandomValues()
{
    _valuesArray.clear();
    for (int i = 0; i < _rows; ++i) {
        QVector<unsigned short> rowValues;
        for (int j = 0; j < _columns; ++j) {
            unsigned short value = static_cast<unsigned short>(
                QRandomGenerator::global()->generate() % MAX_RANDOM_VALUE);
            rowValues.append(value);
        }
        _valuesArray.append(rowValues);
    }
}

void GrayscaleImageGenerator::updateImage()
{
    if (!generationThread->isRunning())
        generationThread->start();
}

void GrayscaleImageGenerator::startGeneration()
{
    if (!_timerActive) {
        updateImage();
        _timer->start(_timerINTERVAL);
        _timerActive = true;
    }
}

void GrayscaleImageGenerator::stopGeneration()
{
    if (_timerActive) {
        _timer->stop();
        if (generationThread->isRunning()) {
            generationThread->quit();
            generationThread->wait();
        }
        clearScene();
        if (_view)
            _view->viewport()->repaint();
        _timerActive = false;
    }
}

void GrayscaleImageGenerator::clearScene()
{
    qDeleteAll(_items);
    _items.clear();
}

void GrayscaleImageGenerator::rotateWidget()
{
    if (_view) {
        QTransform transform;
        transform.rotate(_rotationAngle);
        _view->setTransform(transform);
    }
}

void GrayscaleImageGenerator::setGraphicsView(QGraphicsView *newView)
{
    if (_view != newView) {
        if (_view) {
            _view->setScene(nullptr);
            delete _view;
        }
        _view = newView;
        if (_view)
            _view->setScene(&_scene);
    }
}

QGraphicsView *GrayscaleImageGenerator::getGraphicsView() const
{
    return _view;
}

void GrayscaleImageGenerator::setRotationAngle(qreal angle)
{
    _rotationAngle = angle;
    rotateWidget();
}

QImage GrayscaleImageGenerator::createGrayImage(unsigned short value)
{
    unsigned short scaledValue = static_cast<unsigned short>((value / 1000.0) * 0xFFFF);

    QImage image(CELL_SIZE, CELL_SIZE, QImage::Format_Grayscale16);
    image.fill(scaledValue);
    return image;
}

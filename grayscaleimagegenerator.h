#ifndef GRAYSCALEIMAGEGENERATOR_H
#define GRAYSCALEIMAGEGENERATOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QVector>

class ImageGenerationThread;

class GrayscaleImageGenerator : public QObject
{
    Q_OBJECT

public:
    GrayscaleImageGenerator(int rows, int columns, QWidget *parent = nullptr);
    ~GrayscaleImageGenerator();

    void setGraphicsView(QGraphicsView *);
    void setRotationAngle(qreal);
    void generateRandomValues();
    QGraphicsView *getGraphicsView() const;

public slots:
    void startGeneration();
    void stopGeneration();
    void updateImage();
    void handleImageGenerationComplete();

private:
    void clearScene();
    void rotateWidget();
    QImage createGrayImage(unsigned short);

    int _rows;
    int _columns;
    QGraphicsView *_view;
    QGraphicsScene _scene;
    QTimer *_timer;
    bool _timerActive;
    qreal _rotationAngle;
    QList<QGraphicsPixmapItem *> _items;
    ImageGenerationThread *generationThread;
    QVector<QVector<unsigned short>> _valuesArray;
};

#endif // GRAYSCALEIMAGEGENERATOR_H

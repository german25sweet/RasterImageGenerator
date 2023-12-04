#ifndef IMAGEGENERATIONTHREAD_H
#define IMAGEGENERATIONTHREAD_H

#include <QThread>
#include <QVector>

class GrayscaleImageGenerator;

class ImageGenerationThread : public QThread
{
    Q_OBJECT

public:
    ImageGenerationThread(GrayscaleImageGenerator *);

protected:
    void run() override;

signals:
    void generationComplete();

private:
    GrayscaleImageGenerator *_generator;
};

#endif // IMAGEGENERATIONTHREAD_H

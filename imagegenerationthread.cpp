#include "ImageGenerationThread.h"
#include "GrayscaleImageGenerator.h"

ImageGenerationThread::ImageGenerationThread(GrayscaleImageGenerator *generator)
    : QThread()
    , _generator(generator)
{}

void ImageGenerationThread::run()
{
    _generator->generateRandomValues();
    emit generationComplete();
}

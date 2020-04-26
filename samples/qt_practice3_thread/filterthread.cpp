#include "filterthread.h"

#include <QPixmap>
#include <QDebug>


QImage cvMatToQImage(const cv::Mat &inMat);
cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData = true);
inline cv::Mat QPixmapToCvMat(const QPixmap &inPixmap, bool inCloneImageData = true);
inline QPixmap cvMatToQPixmap(const cv::Mat &inMat);


FilterThread::FilterThread(const cv::Mat &image)
{
    setData(image);
}
FilterThread::FilterThread(const QPixmap &image)
{
    setData(image);
}


void FilterThread::initMedianBlur(int kernel)
{
    FilterMedianBlur *filter = new FilterMedianBlur(kernel);
    m_filter.reset(filter);
    emit filter_initialized();
}
void FilterThread::initGray()
{
    GrayFilter *filter = new GrayFilter;
    m_filter.reset(filter);
    emit filter_initialized();
}
void FilterThread::initResize(int width, int height)
{
    ResizeFilter *filter = new ResizeFilter(width, height);
    m_filter.reset(filter);
    emit filter_initialized();
}
void FilterThread::initGaussian(int kernel)
{
    GaussianFilter *filter = new GaussianFilter(kernel);
    m_filter.reset(filter);
    emit filter_initialized();
}
void FilterThread::initBarleyBreak(int scale)
{
    FilterBarleyBreak *filter = new FilterBarleyBreak(scale);
    m_filter.reset(filter);
    emit filter_initialized();
}
void FilterThread::initScharr(int ddepth, int dx, int dy)
{
    FilterScharr *filter = new FilterScharr(ddepth, dx, dy);
    m_filter.reset(filter);
    emit filter_initialized();
}
void FilterThread::initSobel(int ddepth, int dx, int dy)
{
    FilterSobel *filter = new FilterSobel(ddepth, dx, dy);
    m_filter.reset(filter);
    emit filter_initialized();
}


bool FilterThread::isActive()
{
    return (bool)m_filter;
}


void FilterThread::clear()
{
    m_filter.reset();
    m_image.release();
    emit filter_not_initialized();
}


void FilterThread::setData(const cv::Mat &image)
{
    image.copyTo(m_image);
}
void FilterThread::setData(const QPixmap &image)
{
    m_image = QPixmapToCvMat(image);
}


void FilterThread::run()
{
    if(!m_image.empty() && m_filter)
    {
        cv::Mat dst;
        m_filter->processImage(m_image, dst);
        if (!dst.empty())
        {
            emit send_mat(dst);
            emit send_pixmap(cvMatToQPixmap(dst));
            emit processing_complete();
			return;
        }
    }
    emit processing_failed();
}

QImage cvMatToQImage(const cv::Mat &inMat)
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );

        return image;
    }
    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

inline QPixmap cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData)
{
    switch ( inImage.format() )
    {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return (inCloneImageData ? mat.clone() : mat);
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB32:
    {
        if ( !inCloneImageData )
        {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        cv::Mat  matNoAlpha;

        cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        if ( !inCloneImageData )
        {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        QImage   swapped = inImage.rgbSwapped();

        return cv::Mat( swapped.height(), swapped.width(),
                        CV_8UC3,
                        const_cast<uchar*>(swapped.bits()),
                        static_cast<size_t>(swapped.bytesPerLine())
                        ).clone();
    }

        // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC1,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}

// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
// with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
inline cv::Mat QPixmapToCvMat(const QPixmap &inPixmap, bool inCloneImageData)
{
    return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
}

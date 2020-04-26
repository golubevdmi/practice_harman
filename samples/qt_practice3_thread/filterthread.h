#ifndef FILTERTHREAD_H
#define FILTERTHREAD_H

#include <QThread>

#include <filter.h>

class FilterThread : public QThread
{
    Q_OBJECT

public:
    FilterThread() {}
    FilterThread(const cv::Mat &image);
    FilterThread(const QPixmap &image);

    void initMedianBlur(int kernel);
    void initGray();
    void initResize(int width, int height);
    void initGaussian(int kernel);
    void initBarleyBreak(int scale);
    void initScharr(int ddepth, int dx, int dy);
    void initSobel(int ddepth, int dx, int dy);

    bool isActive();
    void clear();

    void setData(const cv::Mat &image);
    void setData(const QPixmap &image);
signals:
    void send_pixmap(const QPixmap&);
    void send_mat(const cv::Mat&);
    void processing_complete();
    void processing_failed();
    void filter_initialized();
    void filter_not_initialized();
protected:
    void run() override;
private:
    std::unique_ptr<Filter> m_filter;
    cv::Mat m_image;
};

#endif // FILTERTHREAD_H

#include "filter.h"
#include <opencv2/imgproc.hpp>


FilterMedianBlur::FilterMedianBlur(int kernel) : m_kernel(kernel) {}
void FilterMedianBlur::processImage(const cv::Mat &src, cv::Mat &dst)
{
    assert(m_kernel > 0 && m_kernel % 2 > 0);
    cv::medianBlur(src, dst, m_kernel);
}


void GrayFilter::processImage(const cv::Mat &src, cv::Mat &dst)
{
    assert(src.type() == CV_8UC3);
    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
}


ResizeFilter::ResizeFilter(int width, int height)
    : m_width(width), m_height(height) {}
void ResizeFilter::processImage(const cv::Mat &src, cv::Mat &dst)
{
    assert(m_width > 0 || m_height > 0);
    cv::resize(src, dst, cv::Size(m_width, m_height));
}


GaussianFilter::GaussianFilter(int kernel) : m_kernel(kernel) {}
void GaussianFilter::processImage(const cv::Mat &src, cv::Mat &dst)
{
    if (m_kernel > 0 && m_kernel % 2 > 0)
        GaussianBlur(src, dst, cv::Size(m_kernel, m_kernel), 0);
}


FilterBarleyBreak::FilterBarleyBreak(int scale) : m_scale(scale) {}
void FilterBarleyBreak::processImage(const cv::Mat &src, cv::Mat &dst)
{
    src.copyTo(dst);

    int x = dst.size().width / m_scale;
    int y = dst.size().height / m_scale;

    for (int i = 0; i < 100; i++)
    {
        cv::Rect rect1(x * (rand() % m_scale), y * (rand() % m_scale), dst.size().width / m_scale, dst.size().height / m_scale);
        cv::Rect rect2(x * (rand() % m_scale), y * (rand() % m_scale), dst.size().width / m_scale, dst.size().height / m_scale);

        cv::Mat tmp;
        dst(rect2).copyTo(tmp);
        dst(rect1).copyTo(dst(rect2));
        tmp.copyTo(dst(rect1));
    }
}


FilterScharr::FilterScharr(int ddepth, int dx, int dy) : m_ddepth(ddepth), m_dx(dx), m_dy(dy) {}
void FilterScharr::processImage(const cv::Mat &src, cv::Mat &dst)
{
    cv::Scharr(src, dst, m_ddepth, m_dx, m_dy);
    cv::convertScaleAbs(dst, dst);
}


FilterSobel::FilterSobel(int ddepth, int dx, int dy) : m_ddepth(ddepth), m_dx(dx), m_dy(dy) {}
void FilterSobel::processImage(const cv::Mat &src, cv::Mat &dst)
{
    cv::Sobel(src, dst, m_ddepth, m_dx, m_dy);
    cv::convertScaleAbs(dst, dst);
}

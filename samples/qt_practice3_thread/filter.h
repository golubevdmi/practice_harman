#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <opencv2/imgproc.hpp>


class Filter
{
public:
	virtual ~Filter() {};
    virtual void processImage(const cv::Mat &src, cv::Mat &dst) = 0;
};


class FilterMedianBlur : public Filter
{
public:
    FilterMedianBlur(int kernel);
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
private:
    int m_kernel;
};


class GrayFilter : public Filter
{
public:
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
};


class ResizeFilter : public Filter
{
public:
    ResizeFilter(int width, int height);
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
private:
    int m_width, m_height;
};


class GaussianFilter : public Filter
{
public:
    GaussianFilter(int kernel);
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
private:
    int m_kernel;
};


class FilterBarleyBreak : public Filter
{
public:
    FilterBarleyBreak(int scale);
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
private:
    int m_scale;
};


class FilterScharr : public Filter
{
public:
    FilterScharr(int ddepth, int dx, int dy);
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
private:
    int m_ddepth;
    int m_dx, m_dy;
};


class FilterSobel : public Filter
{
public:
    FilterSobel(int ddepth, int dx, int dy);
    void processImage(const cv::Mat &src, cv::Mat &dst) override;
private:
    int m_ddepth;
    int m_dx, m_dy;
};

#endif // FILTER_H

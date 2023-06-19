#pragma once
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include "basic.hpp"
using namespace cv;

//这个hpp文件用来存放滤波器函数
//如均值滤波，高斯滤波等

cv::Mat MeanFilt(cv::Mat img);

//均值滤波
cv::Mat MeanFilt(cv::Mat img)
{
	assert(img.data != NULL);
	cv::Mat image(img.rows, img.cols, img.type());
	if (img.channels() == 1)
	{
		for (int i = 1; i < img.rows - 1; i++)
		{
			for (int j = 1; j < img.cols - 1; j++)
			{
				image.ptr<uchar>(i)[j] = (img.ptr<uchar>(i - 1)[j - 1] + img.ptr<uchar>(i - 1)[j] +
					img.ptr<uchar>(i - 1)[j + 1] + img.ptr<uchar>(i)[j - 1] + img.ptr<uchar>(i)[j] +
					img.ptr<uchar>(i)[j + 1] + img.ptr<uchar>(i + 1)[j - 1] + img.ptr<uchar>(i + 1)[j] +
					img.ptr<uchar>(i + 1)[j + 1]) / 9;
			}
		}
		for (int i = 0; i < img.rows; i++)
		{
			image.ptr<uchar>(i)[0] = img.ptr<uchar>(i)[0];
			image.ptr<uchar>(i)[img.cols - 1] = img.ptr<uchar>(i)[img.cols - 1];
		}
		for (int i = 0; i < img.cols; i++)
		{
			image.ptr<uchar>(0)[i] = img.ptr<uchar>(0)[i];
			image.ptr<uchar>(img.rows - 1)[i] = img.ptr<uchar>(img.rows - 1)[i];
		}
	}
	else
	{
		std::vector<cv::Mat> chann;
		std::vector<cv::Mat> chann_2;
		Mysplit(img, chann);
		for (int i = 0; i < img.channels(); i++)
			chann_2.push_back(MeanFilt(chann.at(i)));
		merge(chann_2, image);
	}
	return image;
}

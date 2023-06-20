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
void Mysort(int* array, int Len);
cv::Mat MidFilt(cv::Mat img);

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
		image = Mymerge(chann_2);
	}
	return image;
}

//冒泡排序
//输入为数组与数组长度
void Mysort(int* array, int Len)
{
	assert(Len > 0);

	for (int i = 0; i < Len - 1; i++)
	{
		for (int j = 0; j < Len - 1 - i; j++)
		{
			if (array[j] < array[j + 1])
			{
				int temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
}

//中值滤波
cv::Mat MidFilt(cv::Mat img)
{
	assert(img.data != NULL);
	cv::Mat image(img.rows, img.cols, img.type());
	int array[9];
	if (img.channels() == 1)
	{
		for (int i = 1; i < img.rows - 1; i++)
		{
			for (int j = 1; j < img.cols - 1; j++)
			{
				array[0] = img.ptr<uchar>(i - 1)[j - 1];
				array[1] = img.ptr<uchar>(i - 1)[j];
				array[2] = img.ptr<uchar>(i - 1)[j + 1];
				array[3] = img.ptr<uchar>(i)[j - 1];
				array[4] = img.ptr<uchar>(i)[j];
				array[5] = img.ptr<uchar>(i)[j + 1];
				array[6] = img.ptr<uchar>(i + 1)[j - 1];
				array[7] = img.ptr<uchar>(i + 1)[j];
				array[8] = img.ptr<uchar>(i + 1)[j + 1];
				Mysort(array, 9);
				image.ptr<uchar>(i)[j] = array[4];
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
			chann_2.push_back(MidFilt(chann.at(i)));
		image = Mymerge(chann_2);
	}
	return image;
}

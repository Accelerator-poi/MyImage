#pragma once
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
//这个hpp用来存放一些基础操作
//如图像缩放、图像翻转等

//rows:行数 cols:列数
//RGB图像转灰度图
//输入RGB图像，输出灰度图
cv::Mat RGB2Gray(const cv::Mat img);

//通道分离
//输入图片和vector，图片通道会依次存放在vector中
void Mysplit(const cv::Mat img, std::vector<cv::Mat>& channels);


cv::Mat RGB2Gray(const cv::Mat img)
{
	assert(img.data != NULL && img.channels() == 3);
	cv::Mat image(img.rows, img.cols, CV_8UC1);
	for (int i = 0; i < img.rows; i++)
	{
		const Vec3b* row = img.ptr<Vec3b>(i);
		uchar* row_2 = image.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			const Vec3b col = row[j];
			uchar& col_2 = row_2[j];
			col_2 = 0.299f * col[2] + 0.587f * col[1] + 0.114f * col[1];
		}
	}
	assert(image.data != NULL);
	return image;
}

void Mysplit(const cv::Mat img, std::vector<cv::Mat>& channels)
{
	cv::Mat image(img.rows,img.cols,0);
	switch (img.channels())
	{
	case 1:
		channels.push_back(img);
		break;
	case 2:
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < img.rows; j++)
			{
				const Vec2b* row = img.ptr<Vec2b>(j);
				uchar* row_2 = image.ptr<uchar>(j);
				for (int k = 0; k < img.cols; k++)
				{
					row_2[k] = row[k][i];
				}
			}
			channels.push_back(image);
		}
	
		break;
	case 3:
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < img.rows; j++)
			{
				const Vec3b* row = img.ptr<Vec3b>(j);
				uchar* row_2 = image.ptr<uchar>(j);
				for (int k = 0; k < img.cols; k++)
				{
					row_2[k] = row[k][i];
				}
			}
			channels.push_back(image);
		}
	
		break;
	}
};

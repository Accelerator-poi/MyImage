#pragma once
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
//这个头文件用来存放一些基础操作
//如图像缩放、图像翻转等


//RGB图像转灰度图
//姑且考虑了2通道和1通道的情况
//好像有点不对，既然是RGB图像，为什么会有2通道和1通道？
//rows:行数 cols:列数
cv::Mat RGB2Gray(cv::Mat img)
{
	if (img.data == NULL)
		std::cout << "img is NULL!" << std::endl;
	cv::Mat image(img.rows, img.cols, CV_8UC1);
	switch (img.channels())
	{
	case 1:
		image = img;
		break;
	case 2:
		for (int i = 0; i < img.rows; i++)
		{
			Vec2b* row = img.ptr<Vec2b>(i);
			uchar* row_2 = image.ptr<uchar>(i);
			for (int j = 0; j < image.cols; j++)
			{
				Vec2b col = row[j];
				uchar col_2 = row_2[j];
				col_2 = int((col[1] + col[0]) / 2);
			}
		}
		break;
	case 3:
		for (int i = 0; i < img.rows; i++)
		{
			Vec3b* row = img.ptr<Vec3b>(i);
			uchar* row_2 = image.ptr<uchar>(i);
			for (int j = 0; j < img.cols; j++)
			{
				Vec3b col = row[j];
				uchar col_2 = row_2[j];
				col_2 = cv::saturate_cast<uchar>(0.299*col[0] + 0.587*col[1] + 0.114*col[2]);
			}
		}
		break;
	}
	if (image.data == NULL)
		std::cout << "暂不支持" << std::endl;
	return image;
}


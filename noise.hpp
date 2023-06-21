#pragma once
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "basic.hpp"

//用来存放噪声函数
//如椒盐噪声，高斯噪声等

cv::Mat SaltNoise(cv::Mat img, float rate);

//添加椒盐噪声
//参数1是图片，参数2是噪点占全图的比例
cv::Mat SaltNoise(cv::Mat img, float rate)
{
	assert(img.data != NULL);
	assert(rate <= 1 && rate>=0);

	std::vector<cv::Mat> channels;
	Mysplit(img, channels);
	for (int i = 0; i < channels.size(); i++)
	{
		for (int j =0;j < img.rows * img.cols * rate; j++)
		{
			int h_s = rand() % img.rows;
			int w_s = rand() % img.cols;
			int h_p = rand() % img.rows;
			int w_p = rand() % img.cols;
			channels.at(i).at<uchar>(h_s,w_s) = 0;
			channels.at(i).at<uchar>(h_p, w_p) = 255;
		}
	}
	cv::Mat image = Mymerge(channels);
	return image;

}

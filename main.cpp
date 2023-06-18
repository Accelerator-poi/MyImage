
//打开摄像头样例
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

//RGB图像转灰度图
//姑且考虑了2通道和1通道的情况
//好像有点不对，既然是RGB图像，为什么会有2通道和1通道？
cv::Mat RGB2Gray(cv::Mat img)
{
	if (img.data == NULL)
		std::cout << "img is NULL!" << std::endl;
	if (img.channels() == 1)
		return img;
	cv::Mat image(img.cols, img.rows, CV_8UC1);
	for (int i = 0; i < image.cols; i++)
	{
		for (int j = 0; j < image.rows; j++)
		{
			if (img.channels() == 3)
			{
				image.at<uchar>(i, j) = int((img.at<Vec3b>(i, j)[0] + img.at<Vec3b>(i, j)[1] + img.at<Vec3b>(i, j)[2]) / 3);
			}
			else if (img.channels() == 2)
			{
				image.at<uchar>(i, j) = int((img.at<Vec3b>(i, j)[0] + img.at<Vec3b>(i, j)[1]) / 2);
			}
		}
	}
	if (image.data == NULL)
		std::cout << "暂不支持" << std::endl;
	return image;
}

int main()
{
	cv::Mat image;
	cv::Mat image_2;
	cv::Mat image_3(1080, 1080, CV_8UC1);
	image = cv::imread("./lena.jpg");
	if (image.data == NULL)
		std::cout << "image data is NULL";
	namedWindow("beautiful",WINDOW_NORMAL);
	namedWindow("beautiful_2", WINDOW_NORMAL);
	image_2 = RGB2Gray(image);
	for (int i = 0; i < image.rows; i++)
	{
		image_3.at<uchar>(i, 1) = image_2.at<uchar>(i, 1);
		image_3.at<uchar>(1, i) = image_2.at<uchar>(1, i);
		image_3.at<uchar>(i, 1079) = image_2.at<uchar>(i, 1079);
		image_3.at<uchar>(1079, i) = image_2.at<uchar>(1079, i);
	}
	for (int i = 1; i < image_2.rows-1; i++)
	{
		for (int j = 1; j < image_2.cols - 1; j++)
		{
			image_3.at<uchar>(i, j) = int((image_2.at<uchar>(i - 1, j - 1) + image_2.at<uchar>(i, j - 1) + image_2.at<uchar>(i + 1, j - 1)
				+ image_2.at<uchar>(i - 1, j) + image_2.at<uchar>(i, j) + image_2.at<uchar>(i + 1, j)
				+ image_2.at<uchar>(i - 1, j + 1) + image_2.at<uchar>(i, j + 1) + image_2.at<uchar>(i + 1, j + 1)) / 9);
		}
	}
	
	imshow("beautiful", image_3);
	imshow("beautiful_2", image_2);
	waitKey(60000);
	return 0;
	    
}

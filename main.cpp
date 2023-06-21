#include "basic.hpp"
#include "filter.hpp"
#include "noise.hpp"
#include <ctime>

clock_t start_1, end_1;
clock_t start_2, end_2;
clock_t start_3, end_3;
clock_t start_4, end_4;

int main()
{
	cv::Mat image;
	image = cv::imread("./lena.jpg");
	if (image.data == NULL)
		std::cout << "image data is NULL";
	std::cout << image.type() << std::endl;
	std::vector<cv::Mat>channels;
	std::vector<cv::Mat>channels_2;
	cv::Mat image_2 = GaussNoise(image, 0, 30);
	cv::Mat image_3 = MeanFilt(image_2);
	imshow("img", image);
	imshow("Salt", image_2);
	imshow("Mean", image_3);
	//	imshow("0", channels.at(0));
	//	imshow("1", channels.at(1));
	//	imshow("2", channels.at(2));
	waitKey(60000);
	return 0;
}
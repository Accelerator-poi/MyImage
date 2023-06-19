#include "basic.hpp"
#include "filter.hpp"
#include <ctime>


int main()
{
	cv::Mat image;
	image = cv::imread("./lena.jpg");
	if (image.data == NULL)
		std::cout << "image data is NULL";
	std::cout << image.type() << std::endl;
	std::vector<cv::Mat>channels;
	split(image, channels);
	std::cout << channels.at(0).type() << std::endl;
	cv::Mat image_3 = MeanFilt(image);
	imshow("img", image);
	imshow("Mean", image_3);
	waitKey(60000);
	return 0;
	    
}

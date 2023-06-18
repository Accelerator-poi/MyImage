#include "basic.h"
#include <ctime>

clock_t start, end;
clock_t start_2, end_2;


int main()
{
	cv::Mat image;
	image = cv::imread("./lena.jpg");
	if (image.data == NULL)
		std::cout << "image data is NULL";
	std::cout << image.channels();
	namedWindow("beautiful",WINDOW_NORMAL);
	cv::Mat image_2;
	image_2 = RGB2Gray(image);
	imshow("beautiful", image_2);
	waitKey(60000);
	return 0;
	    
}

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

/// Image Global variables
Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

// Video Glabal variables
VideoCapture cap("C:\\Users\\ResearchBeast\\Documents\\E16-SMH-041-E16-SMH-039_012215_1372_1.MPG"); // open the default camera
Mat frame;
Mat video_edges, video_dst;

int videoEdgeThresh = 1;
int videoLowThreshold;
int const video_max_lowThreshold = 100;
int videoRatio = 3;
int video_kernel_size = 3;
char* video_window_name = "Edge Map";

void CannyThreshold(int, void*);
void VideoCannyThreshold(int, void*);

int canyImageTest()
{
	/// Load an image
	src = imread("C:\\Users\\ResearchBeast\\Documents\\Visual Studio 2015\\Projects\\EdgeDetecionTest\\EdgeDetecionTest\\insidepipe.png");

	if (!src.data)
		return -1;

	/// Create a matrix of the same type and size as src (for dst)
	dst.create(src.size(), src.type());

	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Create a window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// Create a Trackbar for user to enter threshold
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	/// Show the image
	CannyThreshold(0, 0);

	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}

/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));

	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

void VideoCannyThreshold(int, void*)
{
	cvtColor(frame, video_edges, CV_BGR2GRAY);
	GaussianBlur(video_edges, video_edges, Size(7, 7), 1.5, 1.5);

	/// Canny detector
	Canny(video_edges, video_edges, videoLowThreshold, videoLowThreshold*videoRatio, video_kernel_size);

	/// Using Canny's output as a mask, we display our result
	video_dst = Scalar::all(0);

	frame.copyTo(video_dst, video_edges);
	imshow(video_window_name, video_edges);
}

int TestVideoCapture()
{
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	namedWindow(video_window_name, CV_WINDOW_AUTOSIZE);
	/// Create a Trackbar for user to enter threshold
	createTrackbar("Min Threshold:", video_window_name, &videoLowThreshold, video_max_lowThreshold, VideoCannyThreshold);
	for (;;)
	{
		cap >> frame;
		VideoCannyThreshold(0, 0);
		// get a new frame from camera
		/*cvtColor(frame, video_edges, CV_BGR2GRAY);
		GaussianBlur(video_edges, video_edges, Size(7, 7), 1.5, 1.5);
		Canny(video_edges, video_edges, 0, 30, 3);

		imshow("edges", video_edges);*/
		if (waitKey(30) >= 0)
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

/** @function main */
int main(int argc, char** argv)
{
	// chose program type
	int choice = -1;

	while (choice != 3)
	{
		cout << "Chose program type:\n";
		cout << "1. Cany image edge detection\n";
		cout << "2. Test video input\n";
		cout << "3. Quit\n";
		cin >> choice;

		switch (choice)
		{
		case 1:
			canyImageTest();
			break;
		case 2:
			TestVideoCapture();
			break;
		}
	}

	return 0;
}
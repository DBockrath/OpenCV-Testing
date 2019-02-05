#include "pch.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

void saveImage(Mat image, string saveLocation) {

	imwrite(saveLocation, image); //write the image to a file as JPEG 

}

Mat changeBrightness(Mat image, int value) {

	Mat newImage;
	image.convertTo(newImage, -1, 1, value);
	return newImage;

}

Mat changeContrast(Mat image, int value) {

	Mat newImage;
	image.convertTo(newImage, -1, value, 0);
	return newImage;

}

Mat histogramGrayscale(Mat image) {

	cvtColor(image, image, COLOR_BGR2GRAY);
	Mat newImage;
	equalizeHist(image, newImage);
	return newImage;

}

Mat histogramColor(Mat image) {

	Mat newImage;
	cvtColor(image, newImage, COLOR_BGR2YCrCb);
	vector<Mat> vec_channels;
	split(newImage, vec_channels);
	equalizeHist(vec_channels[0], vec_channels[0]);
	merge(vec_channels, newImage);
	cvtColor(newImage, newImage, COLOR_YCrCb2BGR);
	return newImage;

}

Mat segment(Mat image) {

	if (image.type() == CV_8UC1) {

		Mat gray;
		cvtColor(image, gray, COLOR_BGR2GRAY);

		Mat otsu;
		threshold(gray, otsu, 0, 255, THRESH_BINARY + THRESH_OTSU);

		return otsu;

	}

	else {

		Mat newImage;
		image.convertTo(newImage, CV_8UC1);

		Mat gray;
		cvtColor(newImage, gray, COLOR_BGR2GRAY);

		Mat otsu;
		threshold(gray, otsu, 0, 255, THRESH_BINARY_INV + THRESH_OTSU); // Exception

		return otsu;

	}

}

Mat indentifyObjects(Mat image) {

	return image;

}

void takeVideo(string saveLocation) {

	//Open the default video camera
	VideoCapture cap(0);

	// if not success, exit program
	if (cap.isOpened() == false) {

		cout << "Cannot open the video camera" << endl;
		cin.get(); //wait for any key press

	}

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

	Size frame_size(frame_width, frame_height);
	int frames_per_second = 10;

	//Create and initialize the VideoWriter object 
	VideoWriter oVideoWriter(saveLocation, VideoWriter::fourcc('M', 'J', 'P', 'G'),
		frames_per_second, frame_size, true);

	//If the VideoWriter object is not initialized successfully, exit the program
	if (oVideoWriter.isOpened() == false) {

		cout << "Cannot save the video to a file" << endl;
		cin.get(); //wait for any key press

	}

	string window_name = "Camera Feed";
	namedWindow(window_name); //create a window called "My Camera Feed"

	while (true) {

		Mat frame;
		bool isSuccess = cap.read(frame); // read a new frame from the video camera

		//Breaking the while loop if frames cannot be read from the camera
		if (isSuccess == false) {

			cout << "Video camera is disconnected" << endl;
			cin.get(); //Wait for any key press
			break;

		}

		/*
		Make changes to the frame as necessary
		e.g.
		 1. Change brightness/contrast of the image
		 2. Smooth/Blur image
		 3. Crop the image
		 4. Rotate the image
		 5. Draw shapes on the image
		*/

		//write the video frame to the file
		oVideoWriter.write(frame);

		//show the frame in the created window
		imshow(window_name, frame);

		//Wait for for 10 milliseconds until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If any other key is pressed, continue the loop 
		//If any key is not pressed within 10 milliseconds, continue the loop 
		if (waitKey(10) == 27) {

			cout << "Esc key is pressed by the user. Stopping the video" << endl;
			break;

		}

	}

	//Flush and close the video file
	oVideoWriter.release();

}

void saveVideo(VideoCapture video, string saveLocation) {

	int frame_width = static_cast<int>(video.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(video.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

	Size frame_size(frame_width, frame_height);
	double frames_per_second = video.get(CAP_PROP_FPS);

	//Create and initialize the VideoWriter object 
	VideoWriter oVideoWriter(saveLocation, VideoWriter::fourcc('M', 'J', 'P', 'G'),
		frames_per_second, frame_size, true);

	//If the VideoWriter object is not initialized successfully, exit the program
	if (oVideoWriter.isOpened() == false) {

		cout << "Cannot save the video to a file" << endl;
		cin.get(); //wait for any key press

	}

	string window_name = "Save Video";
	namedWindow(window_name); //create a window called "Save Video"

	while (true) {

		Mat frame;
		bool isSuccess = video.read(frame); // read a new frame from the video camera

		//Breaking the while loop if frames cannot be read from the camera
		if (isSuccess == false) {

			cout << "Video camera is disconnected" << endl;
			cin.get(); //Wait for any key press
			break;

		}

		/*
		Make changes to the frame as necessary
		e.g.
		 1. Change brightness/contrast of the image
		 2. Smooth/Blur image
		 3. Crop the image
		 4. Rotate the image
		 5. Draw shapes on the image
		*/

		//write the video frame to the file
		oVideoWriter.write(frame);

		//show the frame in the created window
		imshow(window_name, frame);

		//Wait for for 10 milliseconds until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If any other key is pressed, continue the loop 
		//If any key is not pressed within 10 milliseconds, continue the loop 
		if (waitKey(10) == 27) {

			cout << "Esc key is pressed by the user. Stopping the video" << endl;
			break;

		}

	}

	//Flush and close the video file
	oVideoWriter.release();

}

void showVideo(string videoLocation) {

	//open the video file for reading
	VideoCapture cap(videoLocation);

	// if not success, exit program
	if (cap.isOpened() == false) {
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return;
	}

	//get the frames rate of the video
	double fps = cap.get(CAP_PROP_FPS);
	cout << "Frames per seconds : " << fps << endl;

	string window_name = "Video Player";

	namedWindow(window_name, WINDOW_NORMAL); //create a window

	while (true) {
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video 

		//Breaking the while loop at the end of the video
		if (bSuccess == false) {
			cout << "Found the end of the video" << endl;
			break;
		}

		//show the frame in the created window
		imshow(window_name, changeBrightness(frame, -100));

		//wait for for 10 ms until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If the any other key is pressed, continue the loop 
		//If any key is not pressed withing 10 ms, continue the loop
		if (waitKey(10) == 27) {
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}

}

void showVideo(VideoCapture video)
{

	// if not success, exit program
	if (video.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return;
	}

	//get the frames rate of the video
	double fps = video.get(CAP_PROP_FPS);
	cout << "Frames per seconds : " << fps << endl;

	string window_name = "Video Player";

	namedWindow(window_name, WINDOW_NORMAL); //create a window

	while (true)
	{
		Mat frame;
		bool bSuccess = video.read(frame); // read a new frame from video 

		//Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}

		//show the frame in the created window
		imshow(window_name, frame);

		//wait for for 10 ms until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If the any other key is pressed, continue the loop 
		//If any key is not pressed withing 10 ms, continue the loop
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}

}

void showCameraFeed()
{
	//Open the default video camera
	VideoCapture cap(0);

	// if not success, exit program
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video camera" << endl;
		cin.get(); //wait for any key press
		return;
	}

	double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

	string window_name = "My Camera Feed";
	namedWindow(window_name); //create a window called "My Camera Feed"

	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video 

		//Breaking the while loop if the frames cannot be captured
		if (bSuccess == false)
		{
			cout << "Video camera is disconnected" << endl;
			cin.get(); //Wait for any key press
			break;
		}

		//show the frame in the created window
		//imshow(window_name, histogramColor(changeContrast(changeBrightness(frame, -10), 1)));
		//imshow(window_name, segment(frame));
		imshow(window_name, frame);

		//wait for for 10 ms until any key is pressed.  
		//If the 'Esc' key is pressed, break the while loop.
		//If the any other key is pressed, continue the loop 
		//If any key is not pressed withing 10 ms, continue the loop 
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}

}

int main(int argc, char** argv) {

	showCameraFeed();
	return 0;

}
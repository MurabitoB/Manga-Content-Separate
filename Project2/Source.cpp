#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<math.h>
#include"image_information.h"
#include<utility>
using namespace std;
using namespace cv;
vector <pair<Point, Point>>horL;
vector <pair<Point, Point>>verL;
class FindLines
{
private:

	vector<Vec4i> lines;


	double rho;
	double theta;


	int minVote;

 
	double minLinesLength;

	double maxLineGap;

public:

	FindLines() : rho(1), theta(CV_PI / 360),
		minVote(20), minLinesLength(50), maxLineGap(10) {}
  
	void setAccResolution(double urho, double utheta)
	{
		rho = urho;
		theta = utheta;
	}

	void setThreshold(int uminVote)
	{
		minVote = uminVote;
	}

	void setLineLengthAndGap(double uminLinesLength, double umaxLineGap)
	{
		minLinesLength = uminLinesLength;
		maxLineGap = umaxLineGap;
	}

	vector<Vec4i> findLines(Mat &binary)
	{
		lines.clear();
		HoughLinesP(binary, lines, rho, theta, minVote, minLinesLength, maxLineGap);
		return lines;
	}

	void drawDetectedLines(Mat &image, Scalar color = Scalar(0, 0, 255), int thickness = 2)
	{
		vector<Vec4i>::const_iterator it_lines = lines.begin();
		while (it_lines != lines.end())
		{
			Point pt1((*it_lines)[0], (*it_lines)[1]);
			Point pt2((*it_lines)[2], (*it_lines)[3]);
			if (abs(pt1.x - pt2.x) < 5 )
			{
				verL.push_back(make_pair(pt1, pt2));
				line(image, pt1, pt2, color, thickness);
			}
			else if (abs(pt1.y - pt2.y) < 5)
			{
				horL.push_back(make_pair(pt1, pt2));
				line(image, pt1, pt2, color, thickness);
			}

			++it_lines;
		}
	}
};


void DetRoi()
{
	//統計出頂部
}
void getInformation(Mat src)
{
	src_height = src.rows;
	src_width = src.cols;
}

void testLine(Mat img)
{

}
int main()
{
	Mat img_src, img_gray, img_canny,img_bin;
	FindLines linefinder;
	img_src = imread("img040.jpg");
	getInformation(img_src);
	Mat Roi = img_src(Rect(100, 100, 600, 600));
	cvtColor(img_src, img_gray, CV_BGR2GRAY);
	threshold(img_gray, img_bin, 200, 255, THRESH_BINARY | THRESH_OTSU);
	namedWindow("Bin", WINDOW_FREERATIO);
	imshow("Bin", img_bin);
	Canny(img_bin, img_canny, 50, 150, 3,false);
	namedWindow("Canny", WINDOW_FREERATIO);
	imshow("Canny", img_canny);
	linefinder.setThreshold(20);
	linefinder.setLineLengthAndGap(100, 10);
	linefinder.findLines(img_canny);
	linefinder.drawDetectedLines(img_src);
	namedWindow("figure", WINDOW_FREERATIO);
	imwrite("ROI.jpg", Roi);
	imshow("figure", img_src);
	imshow("ROI", Roi);
	waitKey(0);
	return 0;
}
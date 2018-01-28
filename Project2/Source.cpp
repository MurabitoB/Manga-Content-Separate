#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;
using namespace std;
using namespace cv;

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

	void drawDetectedLines(Mat &image, Scalar color = Scalar(0, 0, 255), int thickness = 4)
	{
		vector<Vec4i>::const_iterator it_lines = lines.begin();
		while (it_lines != lines.end())
		{
			Point pt1((*it_lines)[0], (*it_lines)[1]);
			Point pt2((*it_lines)[2], (*it_lines)[3]);
			Point ptLeft(0, pt1.y);
			Point ptRight(image.cols, pt1.y);
			Point ptTop(pt1.x, 0);
			Point ptBottom(pt1.x, image.rows);
			/*if (abs(pt1.y - pt2.y) < 10.0)
			{
				line(image, ptLeft, ptRight, color, thickness);
			}
			if (abs(pt1.x - pt2.x) < 30.0)
			{
				line(image, ptTop, ptBottom, color, thickness);
			}*/
			line(image, pt1, pt2, color, thickness);
			++it_lines;
		}
	}
};

int main()
{
	Mat img_src, img_gray, img_canny;
	FindLines linefinder;
	img_src = imread("img020.jpg");

	cvtColor(img_src, img_gray, CV_BGR2GRAY);
	Canny(img_gray, img_canny, 1, 100, 3);
	linefinder.setThreshold(1);
	linefinder.setLineLengthAndGap(100, 0);
	linefinder.findLines(img_canny);
	linefinder.drawDetectedLines(img_src);
	namedWindow("figure", WINDOW_FREERATIO);
	imshow("figure", img_src);

	waitKey(0);
	return 0;
}
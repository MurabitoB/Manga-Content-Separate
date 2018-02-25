#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<math.h>
#include"image_information.h"
#include<utility>
#include<algorithm>
#include <stdlib.h>		
using namespace std;
using namespace cv;
#define lineSize src_height / 150
#define verDeviation  src_height / 200
vector <pair<Point, Point>>horL; 
vector <pair<Point, Point>>verL;
vector<vector<pair<Point, Point>>> horLcategory;
vector<vector<pair<Point, Point>>> verLcategory;
bool pointSortY(pair<Point, Point> point, pair<Point, Point> point2)
{
	return point.first.y < point2.first.y;
}
bool pointSortX(pair<Point, Point> point, pair<Point, Point> point2)
{
	return point.first.x < point2.first.x;
}
void pointSort(vector<pair<Point, Point>> &point, bool horizental)
{
	horizental ? sort(point.begin(), point.end(), pointSortY) : sort(point.begin(), point.end(), pointSortX);
}

void pointClassify(vector<pair<Point, Point>> &point, bool horizental)
{
	vector <pair<Point, Point>> tempPointPair;
	if (horizental)
	{
		tempPointPair.push_back(point.front());
		for (int i = 0; i < point.size() -1; i++)
		{
			if (abs((point[i].first.y - point[i + 1].first.y)) < lineSize)
			{
				tempPointPair.push_back(point[i + 1]);
			}
			else
			{
				horLcategory.push_back(tempPointPair), tempPointPair.clear(), tempPointPair.push_back(point[i + 1]);
			}
		}
		if (!tempPointPair.empty())
		{
			horLcategory.push_back(tempPointPair);
		}
	}
	else
	{
		tempPointPair.push_back(point.front());
		for (int i = 0; i < point.size() -1; i++)
		{
			if (abs((point[i].first.x - point[i + 1].first.x)) < lineSize)
			{
				tempPointPair.push_back(point[i + 1]);
			}
			else
			{
				verLcategory.push_back(tempPointPair), tempPointPair.clear(), tempPointPair.push_back(point[i + 1]);
			}
		}
		if (!tempPointPair.empty())
		{
			verLcategory.push_back(tempPointPair);
		}
	}
}

void pointFilter(vector<vector<pair<Point, Point>>> &pointCategory, bool horizental)
{
	for (int i = 0; i < pointCategory.size(); i++)
	{
		if (pointCategory.at(i).size() > 1)
		{
			pointSort(pointCategory.at(i), !horizental);
		}
	}
	if (horizental)
	{
		/*消掉水平線多餘的點*/
		for (int cateIndex = 0; cateIndex < pointCategory.size(); cateIndex++)
		{
			for (int pointIndex = 0; pointIndex < pointCategory.at(cateIndex).size() - 1; pointIndex++)
			{
				/*如果後面的線被含括在前面一個線裡面*/
				if (pointCategory.at(cateIndex).at(pointIndex).second.x >= pointCategory.at(cateIndex).at(pointIndex + 1).second.x)
				{
					pointCategory.at(cateIndex).erase(pointCategory.at(cateIndex).begin() + pointIndex + 1);
					pointIndex = -1;
				}
				/*如果第二個線的第一個點在第一個線裡面，而第二個點在第一個線外面*/
				else if ((pointCategory.at(cateIndex).at(pointIndex).second.x >= (pointCategory.at(cateIndex).at(pointIndex + 1).first.x)
					&& (pointCategory.at(cateIndex).at(pointIndex).second.x <= (pointCategory.at(cateIndex).at(pointIndex + 1).second.x)))
					)
				{
					pointCategory.at(cateIndex).at(pointIndex).second.x = pointCategory.at(cateIndex).at(pointIndex + 1).second.x;
					pointCategory.at(cateIndex).erase(pointCategory.at(cateIndex).begin() + pointIndex + 1);
					pointIndex = -1;
				}
				/*如果第二個線的第一個點在第一個線外面，而第二個點在第一個線外面，而且距離沒有太遠*/
				else if (pointCategory.at(cateIndex).at(pointIndex).second.x <= pointCategory.at(cateIndex).at(pointIndex + 1).first.x
					&& pointCategory.at(cateIndex).at(pointIndex + 1).first.x - pointCategory.at(cateIndex).at(pointIndex).second.x <= lineSize)
				{
					pointCategory.at(cateIndex).at(pointIndex).second.x = pointCategory.at(cateIndex).at(pointIndex + 1).second.x;
					pointCategory.at(cateIndex).erase(pointCategory.at(cateIndex).begin() + pointIndex + 1);
					pointIndex = -1;
				}
			}
		}
	}
	else
	{
		/*消掉垂直線多餘的點*/
		for (int cateIndex = 0; cateIndex < pointCategory.size(); cateIndex++)
		{
			for (int pointIndex = 0; pointIndex < pointCategory.at(cateIndex).size() - 1; pointIndex++)
			{
				/*如果後面的線被含括在前面一個線裡面*/
				if (pointCategory.at(cateIndex).at(pointIndex).second.y >= pointCategory.at(cateIndex).at(pointIndex + 1).second.y)
				{
					pointCategory.at(cateIndex).erase(pointCategory.at(cateIndex).begin() + pointIndex + 1);
					pointIndex = -1;
				}
				/*如果第二個線的第一個點在第一個線裡面，而第二個點在第一個線外面*/
				else if ((pointCategory.at(cateIndex).at(pointIndex).second.y >= (pointCategory.at(cateIndex).at(pointIndex + 1).first.y)
					&& (pointCategory.at(cateIndex).at(pointIndex).second.y <= (pointCategory.at(cateIndex).at(pointIndex + 1).second.y)))
					)
				{
					pointCategory.at(cateIndex).at(pointIndex).second.y = pointCategory.at(cateIndex).at(pointIndex + 1).second.y;
					pointCategory.at(cateIndex).erase(pointCategory.at(cateIndex).begin() + pointIndex + 1);
					pointIndex = -1;
				}
				/*如果第二個線的第一個點在第一個線外面，而第二個點在第一個線外面，而且距離沒有太遠*/
				else if (pointCategory.at(cateIndex).at(pointIndex).second.y <= pointCategory.at(cateIndex).at(pointIndex + 1).first.y
					&& pointCategory.at(cateIndex).at(pointIndex + 1).first.y - pointCategory.at(cateIndex).at(pointIndex).second.y <= lineSize * 2)
				{
					pointCategory.at(cateIndex).at(pointIndex).second.y = pointCategory.at(cateIndex).at(pointIndex + 1).second.y;
					pointCategory.at(cateIndex).erase(pointCategory.at(cateIndex).begin() + pointIndex + 1);
					pointIndex = -1;
				}
			}
		}

	}
}
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

	void DetectedLines()
	{
		vector<Vec4i>::const_iterator it_lines = lines.begin();
		while (it_lines != lines.end())
		{
			Point pt1((*it_lines)[0], (*it_lines)[1]);
			Point pt2((*it_lines)[2], (*it_lines)[3]);
			if (abs(pt1.x - pt2.x) < 5 && (pt1.x > 10 && pt2.x > 10) && (pt1.x < src_width - 10 && pt2.x < src_width - 10))
			{
				pt1.y < pt2.y ? pt2.x = pt1.x , verL.push_back(make_pair(pt1, pt2)) : verL.push_back(make_pair(pt2, pt1));
			//	line(image, pt1, pt2, color, thickness);
			}
			else if (abs(pt1.y - pt2.y) < 5 && (pt1.y > 10 && pt2.y > 10) && (pt1.y < src_height - 10 && pt2.y < src_height - 10))
			{
				pt1.x < pt2.x ? pt2.y = pt1.y, horL.push_back(make_pair(pt1, pt2)) : horL.push_back(make_pair(pt2, pt1));
			//	line(image, pt1, pt2, color, thickness);
			}

			++it_lines;
		}
	}
	void drawLines(Mat &image,vector<vector<pair<Point,Point>>> ptC, Scalar color = Scalar(0, 0, 255), int thickness = 3)
	{
		
		for (int i = 0; i < ptC.size(); i++)
		{
			for (int j = 0; j < ptC.at(i).size(); j++)
			{
				Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
				line(image, ptC.at(i).at(j).first, ptC.at(i).at(j).second, color, thickness);
			}
		}
	}
};

bool between(pair<Point, Point>top, pair<Point, Point> bottom, pair<Point, Point> src)
{
	int topX[2], topY[2], bottomX[2], bottomY[2], srcX[2], srcY[2];
	topX[0] = top.first.x;
	topY[0] = top.first.y;
	topX[1] = top.second.x;
	topY[1] = top.second.y;

	bottomX[0] = bottom.first.x;
	bottomY[1] = bottom.first.y;
	bottomX[0] = bottom.second.x;
	bottomY[1] = bottom.second.y;

	srcX[0] = src.first.x;
	srcY[1] = src.first.y;
	srcX[0] = src.second.x;
	srcY[1] = src.second.y;

	if (srcY[0] > topY[0] - verDeviation && srcY[1] < topY[1] + verDeviation)
	{
		return true;
	}
}
bool verRangeLegal(pair<Point, Point> &uB, pair<Point, Point> &lB, vector<vector<pair<Point, Point>>> &verC, vector<vector<pair<Point, Point>>> &horC)
{
	if (horC.at(1).at(0).first.y - horC.at(0).at(0).first.y <= src_height / 6)
	{
		horC.erase(horC.begin());
		uB = horC.at(0).at(0);
		if (horC.size() > 1)
		{
			lB = horC.at(1).at(0);
		}
		else
		{
			lB = horC.at(0).at(0);
			lB.first.y = 0;
			lB.second.y = 0;
		}
	}
	return true;
}
bool verDiff(vector<vector<pair<Point, Point>>> &ver)
{
	for (int i = 0; i < ver.size(); i++)
	{
		for (int j = 0; j < ver.at(i).size(); i++)
		{

		}
	}
}
void DetRoi(vector<vector<pair<Point, Point>>> &verC, vector<vector<pair<Point, Point>>> &horC)
{
	vector<Mat> roi;
	Mat tempRoi;
	pair<Point,Point> uB, lB; // UpperBound lowwerBound
	if (!horC.empty()) //check horL not empty
	{
		vector<pair<Point, Point>> possibleTop , leftBound, rightBound ,innerLine;
		//先預設有可能頂部沒有線，所以先在上面跟第一層一樣多條的線段
		for (int i = 0; i < horC.at(0).size(); i++)
		{
			possibleTop.push_back(horC.at(0).at(i));
			possibleTop.at(0).first.y = possibleTop.at(0).first.y = possibleTop.at(0).second.y = possibleTop.at(0).second.y = 0;
		}
		horC.insert(horC.begin(), possibleTop);
		//設定upperbound lowerbound
		uB = horC.at(0).at(0);//頂層
		lB = horC.at(1).at(0); //第一條水平線
		
		//假設頂部跟第一層的範圍夾得太小 則當作沒有頂部夾到東西的可能性
		if (horC.at(1).at(0).first.y - horC.at(0).at(0).first.y <= src_height / 6)
		{
			horC.erase(horC.begin());
			uB = horC.at(0).at(0);
			if (horC.size() > 1)
			{
				lB = horC.at(1).at(0);
			}
			else
			{
				lB = horC.at(0).at(0);
				lB.first.y = 0;
				lB.second.y = 0;
			}
		}

		//找upperbound 和 lowerbound 夾住的的左邊的線和右邊的線
		for (int i = 0; i < verC.size(); i++)
		{
			for (int j = 0; j < verC.at(i).size(); j++)
			{
				//如果在upperbound 和 lowerbound中間則加入可能清單
				if (between(uB, lB, verC.at(i).at(j)))
				{
					innerLine.push_back(verC.at(i).at(j));
				}
			}
		}
		//把innerLine按照x軸位置sort一遍
		if (!innerLine.empty())
		{
			pointSort(innerLine, false);
		}
		//根據innerLine 找出leftBound 和 rightBound
		for (int i = 0; i < innerLine.size(); i++)
		{
			if (leftBound.empty() && uB.first.x + lineSize > innerLine.at(i).first.x)//檢查是否真的是在左邊
			{
				leftBound.push_back(innerLine.at(i));
			}
			else if (!leftBound.empty() && abs(leftBound.at(0).first.x - innerLine.at(i).first.x) < lineSize)//檢查是否是在左邊那條同一個垂直線
			{
				leftBound.push_back(innerLine.at(i));
			}
			if (rightBound.empty() && uB.first.x - lineSize < innerLine.at(i).first.x)
			{
				rightBound.push_back(innerLine.at(i));
			}
			else if (!rightBound.empty() && abs(rightBound.at(0).first.x - innerLine.at(i).first.x) < lineSize)
			{
				rightBound.push_back(innerLine.at(i));
			}
		}
		//判斷這次的搜尋可不可用

	}
	else
	{
		return; 
	}
}
void horizonCut(vector<vector<pair<Point, Point>>> &verC, vector<vector<pair<Point, Point>>> &horC)
{
	
}
void getInformation(Mat src)
{
	src_height = src.rows;
	src_width = src.cols;
}

void testLine(Mat &img)
{
	img = imread("img020.jpg");
}
void getRegionofInterest(Mat &img)
{
	Mat Roi = img(Rect(100, 100, 600, 600));
}
void printLog()
{
	for (int i = 0; i < verLcategory.size(); i++)
	{
		for (int j = 0; j < verLcategory.at(i).size(); j++)
		{
			cout << "Line "<< i <<"  verL:(" << verLcategory.at(i).at(j).first.x << "," << verLcategory.at(i).at(j).first.y << ") , (" << verLcategory.at(i).at(j).second.x << "," << verLcategory.at(i).at(j).second.y<<")"<< endl;
		}
	}
	for (int i = 0; i < horLcategory.size(); i++)
	{
		for (int j = 0; j < horLcategory.at(i).size(); j++)
		{
			cout << "Line  " << i << "  horL:(" << horLcategory.at(i).at(j).first.x << "," << horLcategory.at(i).at(j).first.y << ") , (" << horLcategory.at(i).at(j).second.x << "," << horLcategory.at(i).at(j).second.y << ")" << endl;
		}
	}
}
void imgShowFunction(string windowname, Mat img)
{
	namedWindow(windowname.c_str(), WINDOW_FREERATIO);
	imshow(windowname.c_str(), img);
}
void setlinefinder(Mat &img_canny,Mat &img_src)
{
	FindLines linefinder;
	linefinder.setThreshold(20);
	linefinder.setLineLengthAndGap(100, 15);
	linefinder.findLines(img_canny);
	linefinder.DetectedLines();
	pointSort(verL, false);
	pointSort(horL, true);
	pointClassify(verL, false);
	pointClassify(horL, true);
	pointFilter(horLcategory, true);
	pointFilter(verLcategory, false);
	Mat img2(src_height, src_width, CV_8U, Scalar(100));
	linefinder.drawLines(img2,verLcategory);
	linefinder.drawLines(img2, horLcategory);
	imgShowFunction("預測線", img2);
	imgShowFunction("原圖", img_src);
	printLog();
	waitKey(0);
}

int main()
{
	Mat img_src, img_gray, img_canny,img_bin;
	testLine(img_src); //讀圖
	getInformation(img_src); // 拿原圖的width height
	cvtColor(img_src, img_gray, CV_BGR2GRAY); //轉灰階
	threshold(img_gray, img_bin, 200, 255, THRESH_BINARY | THRESH_OTSU); //轉binary
	blur(img_gray, img_gray, Size(3, 3)); // 消雜訊
	Canny(img_gray, img_canny, 500, 800, 3); // 繪製邊緣
	setlinefinder(img_canny, img_src); // 偵測長直線


	return 0;
}
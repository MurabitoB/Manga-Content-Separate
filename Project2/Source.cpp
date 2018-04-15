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
string seq;
struct roiStruct{
	Mat img;
	int height, width;
	int upL, downL;
	vector <pair<Point, Point>>verLine;
	vector<pair<Point, Point>> upLine;
	vector<pair<Point, Point>> downLine;
};
void imgShowFunction(string windowname, Mat img)
{
	namedWindow(windowname.c_str(), WINDOW_FREERATIO);
	imshow(windowname.c_str(), img);
}
bool pairSort(pair<int, int>a, pair<int, int>b)
{
	return a.first < b.first;
}
bool pointSortY(pair<Point, Point> point, pair<Point, Point> point2)
{
	return point.first.y < point2.first.y;
}
bool pointSortX(pair<Point, Point> point, pair<Point, Point> point2)
{
	return point.first.x < point2.first.x;
}
void pointSort(vector<pair<Point, Point>> &point, bool sortY)
{
	sortY ? sort(point.begin(), point.end(), pointSortY) : sort(point.begin(), point.end(), pointSortX);
}

void pointClassify(vector<pair<Point, Point>> &point, bool horizental)
{
	vector <pair<Point, Point>> tempPointPair;
	if (point.size() == 0)
	{
		return;
	}
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
string setName(string seqNum, string top, string order)
{
	string name = "000_00_00";
	if (seqNum.length() == 3)
	{
		name[0] = seqNum[0];
		name[1] = seqNum[1];
		name[2] = seqNum[2];
	}
	else if (seqNum.length() == 2)
	{
		name[1] = seqNum[0];
		name[2] = seqNum[1];
	}
	else if (seqNum.length() == 1)
	{
		name[2] = seqNum[0];
	}

	if (top.length() == 2)
	{
		name[4] = top[0];
		name[5] = top[1];
	}
	else if (top.length() == 1)
	{
		name[5] = top[0];
	}

	if (order.length() == 2)
	{
		name[7] = order[0];
		name[8] = order[1];
	}
	else if (top.length() == 1)
	{
		name[8] = order[0];
	}

	return name;
}
bool verDiff(vector<vector<pair<Point, Point>>> &ver)
{
	//選取某一個元素
	for (int i = 0; i < ver.size(); i++)
	{
		for (int j = 0; j < ver.at(i).size(); j++)
		{
			//跟不是同一類的元素做比較
			for (int k = 0; k < ver.size(); k++)
			{
				for (int l = 0; l < ver.at(k).size(); l++)
				{
					if ( (k != i) )
						//如果找到則把兩類合而為一
					{
						if ((ver.at(i).at(j).first.y >= ver.at(k).at(l).first.y && ver.at(i).at(j).first.y <= ver.at(k).at(l).second.y) ||
							(ver.at(i).at(j).second.y >= ver.at(k).at(l).first.y && ver.at(i).at(j).second.y <= ver.at(k).at(l).second.y))
						{
							ver.at(i).insert(ver.at(i).end(), ver.at(k).begin(), ver.at(k).end());
							ver.erase(ver.begin() + k);
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
vector<Mat> verticalCut(roiStruct &roiH)
{
	vector<Mat> roi;
	bool between = false;
	int index = 0, leftBorder = 0, rightBorder = 0;
#define left  roiH.verLine.at(index).first.x
#define right roiH.verLine.at(i).first.x
	for (int i = 1; i < roiH.verLine.size(); i++)
	{
		//判斷是否存在 up down 是否有存在一邊 夾在 left right 之間
		for (int k = 0; k < roiH.upLine.size(); k++)
		{
			if ((roiH.upLine.at(k).first.x + 10 > left) && (roiH.upLine.at(k).second.x - 10 < right))
			{
				between = true;
				leftBorder = roiH.upLine.at(k).first.x;
				rightBorder = roiH.upLine.at(k).second.x;
				break;
			}
		}
		if (!between)
		{
			for (int k = 0; k < roiH.downLine.size(); k++)
			{
				if ((roiH.downLine.at(k).first.x + 10 > left) && (roiH.downLine.at(k).second.x - 10 < right))
				{
					between = true;
					leftBorder = roiH.downLine.at(k).first.x;
					rightBorder = roiH.downLine.at(k).second.x;
					break;
				}
			}
		}
		//檢查可不可以夾得更緊
		if (between)
		{
			for (; index <= i; index++)
			{
				//如果超過就倒回來
				if (!(left - 10 < leftBorder && right + 10 > rightBorder))
				{
					index--;
					break;
				}
			}
			between = false;
			roi.push_back(roiH.img(Rect(left, 0, right - left, roiH.downL - roiH.upL)));
			index = i;
		}
	}
	return roi;
}
void horizonCut(vector<vector<pair<Point, Point>>> &verC, vector<vector<pair<Point, Point>>> &horC, Mat src)
{
	vector<vector<pair<Point, Point>>> ver;
	vector<pair<Point, Point>> verT;
	if (!horC.empty())
	{
		horC.insert(horC.begin(), horC.at(0));
		for (int i = 0; i < horC.at(0).size(); i++)
		{
			horC.at(0).at(i).first.y = 0;
			horC.at(0).at(i).second.y = 0;
		}
		horC.insert(horC.end(), horC.at(horC.size()-1));
		for (int i = 0; i < horC.at(horC.size() - 1).size(); i++)
		{
			horC.at(horC.size() - 1).at(i).first.y = src_height;
			horC.at(horC.size() - 1).at(i).second.y = src_height;
		}
	}
	for (int i = 0; i < verC.size(); i++)
	{
		for (int j = 0; j < verC.at(i).size(); j++)
		{
			verT.push_back(verC.at(i).at(j));
			ver.push_back(verT);
			verT.clear();
		}
	}
#define up horC.at(index).at(0).first.y
#define down horC.at(j).at(0).first.y
	while (verDiff(ver));
	//整理每個類別最高點最低點
	vector<pair<int, int>>vecY;
	vector<roiStruct>horY;
	vector<Mat> Roi;
	for (int i = 0; i < ver.size(); i++)
	{
		int minTemp = INT_MAX, maxTemp = 0;
		for (int j = 0; j < ver.at(i).size(); j++)
		{
			maxTemp = MAX(maxTemp, ver.at(i).at(j).second.y);
			minTemp = MIN(minTemp, ver.at(i).at(j).first.y);
		}
		vecY.push_back(make_pair(minTemp, maxTemp));
	}
	sort(vecY.begin(), vecY.end(), pairSort);
	int index = 0 , lastDown = 0;

	for (int j = 1; j < horC.size(); j++)
	{
		if (!vecY.empty() && up - 10 < vecY.at(0).first && down + 10 > vecY.at(0).second) //如果確定有夾到
		{
			for (; index <= j ; index++)
			{
			//如果超過就倒回來
				if (!(up - 10 < vecY.at(0).first && down + 10 > vecY.at(0).second))
				{
					index--;
					break;
				}
			}
			roiStruct roiT;
			roiT.upL = up;
			roiT.downL = down;
			horY.push_back(roiT);
			lastDown = down;
			//清除可能殘留的直線
			for (int k = 0; k < vecY.size(); k++)
			{
				if ((up - 10 < vecY.at(0).first && down + 10 > vecY.at(0).second))
				{
					vecY.erase(vecY.begin());
					k = 0;
				}
			}
			//將 夾在 up and down 的所有垂直線送到 horY
			for (int k = 0; k < ver.size(); k++)
			{
				for (int l = 0; l<ver.at(k).size(); l++)
				{
					if ((ver.at(k).at(l).first.y + lineSize > up) && (ver.at(k).at(l).second.y - lineSize < down))
					{
						horY.back().verLine.push_back(ver.at(k).at(l));
					}
				}
			}
			pair<Point, Point> tempP;
			Point a;
			a.x = 0, 
			a.y = 0;
			tempP.first = a;
			a.y = down - up;
			tempP.second = a;
			horY.back().verLine.insert(horY.back().verLine.begin(), tempP);
			a.x = src_width,
			a.y = 0;
			tempP.first = a;
			a.y = down - up;
			tempP.second = a;
			horY.back().verLine.push_back(tempP);
			horY.back().upLine = horC.at(index);
			horY.back().downLine = horC.at(j);
		}
	}
	if (src_height - lastDown > src_height / 4)
	{
		roiStruct roiT;
		roiT.upL =	lastDown;
		roiT.downL = src_height;
		horY.push_back(roiT);
	}
	for (int i = 0; i < horY.size(); i++)
	{
		horY.at(i).img= src(Rect(0, horY.at(i).upL, src_width, horY.at(i).downL - horY.at(i).upL));
		/*imgShowFunction("預測線" + (i + 1), horY.at(i).img);*/
	}
	for (int i = 0; i < horY.size(); i++)
	{
		pointSort(horY.at(i).upLine, false);
		pointSort(horY.at(i).downLine, false);
		pointSort(horY.at(i).verLine, false);
	}
	vector<vector<Mat>>roit;
	for (int i = 0; i < horY.size(); i++)
	{
		roit.push_back(verticalCut(horY.at(i)));
	}
	for (int i = 0; i < roit.size(); i++)
	{
		for (int j = 0; j < roit.at(i).size(); j++)
		{
			string name = setName(seq,to_string(i+1),to_string(j+1)); // 序列號_由上數下來第幾層_由左到右第幾章圖
			imwrite(name+".jpg", roit.at(i).at(j));
		}
	}
}

void getInformation(Mat src)
{
	src_height = src.rows;
	src_width = src.cols;
}

void testLine(Mat &img,string f)
{
	img = imread(f.c_str());
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
	horizonCut(verLcategory, horLcategory,img_src);
	imgShowFunction("預測線", img2);
	imgShowFunction("原圖", img_src);
	printLog();
	
}
//argv[1]為檔案路徑 argv[2]為圖片序列號
int main(int argc , char* argv[])
{
	Mat img_src, img_gray, img_canny,img_bin;
	string filename;
//	cout << filename;
	filename = argv[1];
	seq = argv[2];
	testLine(img_src,filename); //讀圖
	getInformation(img_src); // 拿原圖的width height
	cvtColor(img_src, img_gray, CV_BGR2GRAY); //轉灰階
	threshold(img_gray, img_bin, 200, 255, THRESH_BINARY | THRESH_OTSU); //轉binary
	blur(img_gray, img_gray, Size(3, 3)); // 消雜訊
	Canny(img_gray, img_canny, 500, 800, 3); // 繪製邊緣
	setlinefinder(img_canny, img_src); // 偵測長直線


	return 0;
}
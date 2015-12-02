#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv2\opencv.hpp>

#include <iostream>
#include <bitset>

using namespace cv;

//포커스 탐색 
class FindFocus
{

//생성, 소멸자
public:
	FindFocus(Mat image, int _angle, int _haralick);
	FindFocus(Mat image, int _angle, int _haralick, int _subWidth, int _subHeight, int _merge);
	~FindFocus();

public:
	//순차탐색, 이진탐색
	void Sequential(Mat image, Mat drawImage, int test);
	void BinaryTreeSearch(Mat image, Mat drawImage);	//폐기물수준
	void HillClimbing(Mat image, Mat drawImage);

	//포커스 영역 그리기
	void DrawFocus(Mat image, double featureMax_loc);

	//히스토그램
	void DrawHistogram(Mat image, double featureMax, int test);

public :
	//input image 크기
	int height;
	int width;

	//ROI 크기
	int subWidth;
	int subHeight;
	int subLeft;

	//상하여백
	int merge;	

	//특징값 저장
	int featureSize;		//특징 맵 크기
	double *featureValue;	//특징 맵 값
	
};

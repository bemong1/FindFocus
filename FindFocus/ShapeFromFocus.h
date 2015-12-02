#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <bitset>

using namespace cv;

//포커스 탐색 
class ShapeFromFocus
{

public:
	ShapeFromFocus();
	~ShapeFromFocus();

//OpenCV 제공 필터
public:

	void Sobel_in_OpenCV(Mat image, Mat *dstImage);
	void Laplacian_in_OpenCV(Mat image, Mat *dstImage);


public:
	void LaplacianFilter(Mat image, Mat *dstImage);
	void SumOfModuleDifferent(Mat image, Mat *dstImage);
	void ModifiedLaplacian(Mat image, Mat *dstImage);
	void EnergyOfLaplacianOfImage(Mat image, Mat *dstImage);
	void GrayLevelVariance(Mat image, Mat *dstImage);
	void HistogramEntropy(Mat image, Mat *dstImage);

};
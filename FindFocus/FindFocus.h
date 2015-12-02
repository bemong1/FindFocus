#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv2\opencv.hpp>

#include <iostream>
#include <bitset>

using namespace cv;

//��Ŀ�� Ž�� 
class FindFocus
{

//����, �Ҹ���
public:
	FindFocus(Mat image, int _angle, int _haralick);
	FindFocus(Mat image, int _angle, int _haralick, int _subWidth, int _subHeight, int _merge);
	~FindFocus();

public:
	//����Ž��, ����Ž��
	void Sequential(Mat image, Mat drawImage, int test);
	void BinaryTreeSearch(Mat image, Mat drawImage);	//��⹰����
	void HillClimbing(Mat image, Mat drawImage);

	//��Ŀ�� ���� �׸���
	void DrawFocus(Mat image, double featureMax_loc);

	//������׷�
	void DrawHistogram(Mat image, double featureMax, int test);

public :
	//input image ũ��
	int height;
	int width;

	//ROI ũ��
	int subWidth;
	int subHeight;
	int subLeft;

	//���Ͽ���
	int merge;	

	//Ư¡�� ����
	int featureSize;		//Ư¡ �� ũ��
	double *featureValue;	//Ư¡ �� ��
	
};

#include "FindFocus.h"

#include "GrayLevelCooccurrenceMatrix.h"


FindFocus::FindFocus(Mat image, int _angle, int _haralick)
{
	//input image�� ũ��
	width			= image.cols;
	height			= image.rows;

	//ROI ũ��� ���Ͽ���
	subLeft			= 0;
	subWidth		= width;
	subHeight		= 85;	
	merge			= 80;
	
	/*subLeft			= 33;
	subWidth		= width / 5.4;
	subHeight		= 110;	
	merge			= 10;
	*/
	
	//Ư¡ �� �޸� �Ҵ� �� �ʱ�ȭ
	featureSize = height - (merge * 2) -(subHeight - 1);
	featureValue	= new double[featureSize];

	for (int i = 0; i < featureSize; i++){
		featureValue[i] = 0;
	}
}

FindFocus::FindFocus(Mat image, int _angle, int _haralick, int _subWidth, int _subHeight, int _merge)
{


}

FindFocus::~FindFocus()
{
	delete[] featureValue;

}


void FindFocus::Sequential(Mat image, Mat drawImage, int test)
{
	double featureMax = 0;		//Ư¡ ���� �ִ� Ư¡ ��
	int featureMax_loc = 0;		//Ư¡ ���� �ִ� Ư¡ �� ��ġ
	
	GrayLevelCoOccurrenceMatrix grayLevelCooccurrenceMatrix
			(GLCM_ANGLE_ALL, HARALICK_CONTRAST);


	for (int k = 0; k < featureSize; k++)
	{
		//���ɿ����� GLCM�� Ư¡�� ����
		Mat roiImage;
		roiImage = image(Rect(subLeft, k+merge, subWidth, subHeight));
		featureValue[k] 
			= grayLevelCooccurrenceMatrix.CoOccurrence(roiImage, 1);		

		//�ִ� Ư¡ �� �� ��ġ �ľ�
		if (featureMax <= featureValue[k])
		{
			featureMax = featureValue[k];
			featureMax_loc = k+merge;
		}
		//std::cout << featureValue[k] << std::endl;
		
	}	
	//std::cout << std::endl;



	//ã�� ��Ŀ�� ǥ��
	DrawFocus(drawImage, featureMax_loc);
	//������׷� ����
	DrawHistogram(drawImage, featureMax, test);

	std::cout << "complete" << std::endl;

	
}


void FindFocus::BinaryTreeSearch(Mat image, Mat drawImage)
{
	int first = 0;
	int last = featureSize;
	int mid = 0;
	int n = 0;
	int N = log2(featureSize);	//�ݺ�Ƚ��

	GrayLevelCoOccurrenceMatrix grayLevelCooccurrenceMatrix
		(GLCM_ANGLE_ALL, HARALICK_CONTRAST);


	while (n <= N)
	{
		mid = (first + last) / 2;

		float featureValue_first = 0;	//���� ���� ��
		float featureValue_last = 0;	//�Ʒ� ���� ��

		Mat roiImage_first;
		Mat roiImage_last;

		roiImage_first	= image(Rect(subLeft, first + merge, subWidth, (mid - first)));
		roiImage_last	= image(Rect(subLeft, mid + merge, subWidth, (mid - first)));

		featureValue_first	= grayLevelCooccurrenceMatrix.CoOccurrence(roiImage_first, 1);
		featureValue_last	= grayLevelCooccurrenceMatrix.CoOccurrence(roiImage_last, 1);

		if (featureValue_first < featureValue_last)
			first = mid;
		else
			last = mid;

		n++;
	}


	int lineThickness = 1;
	CvScalar lineColor_focus = CV_RGB(0, 0, 255);

	//��Ŀ�� ����
	line(drawImage,
		Point(0, mid + merge),
		Point(width, mid + merge),
		lineColor_focus, lineThickness, CV_AA, 0);

	//std::cout << featureValue[mid] << std::endl;


}

void FindFocus::DrawFocus(Mat image, double featureMax_loc)
{
	int lineThickness = 1;
	CvScalar lineColor_focus = CV_RGB(255, 0, 0);
	CvScalar lineColor_region = CV_RGB(255, 255, 0);

	//��Ŀ�� ����
	line(image, 
		Point(0,		featureMax_loc),				
		Point(width,	featureMax_loc), 
		lineColor_focus, lineThickness, CV_AA, 0);
	line(image, 
		Point(0,		featureMax_loc + subHeight),	
		Point(width,	featureMax_loc + subHeight), 
		lineColor_focus, lineThickness, CV_AA, 0);

	//�¿� ���� ����
	line(image, 
		Point(subLeft,				0),						
		Point(subLeft,				height),
		lineColor_region, lineThickness, CV_AA, 0);
	line(image, 
		Point(subLeft + subWidth,	0),			
		Point(subLeft + subWidth,	height),	
		lineColor_region, lineThickness, CV_AA, 0);

	////merge�� ������ ��� ����
	//line(image,
	//	Point(20, merge),
	//	Point(20, merge + featureSize),
	//	CV_RGB(255, 255, 255), lineThickness, CV_AA, 0);
	//line(image,
	//	Point(20, merge + featureSize),
	//	Point(20, merge + featureSize + subHeight),
	//	CV_RGB(0, 0, 255), lineThickness, CV_AA, 0);

}

void FindFocus::DrawHistogram(Mat image, double featureMax, int test)
{
	//histo
	Mat glcmHist;
	glcmHist.create(Size(300, height), CV_8UC3);


	int plusLocation = merge;//+ (subHeight / 2);
	

	for (int k = 1; k < featureSize; k++)
	{		
		line(glcmHist, 
			Point((int)((featureValue[k-1] / featureMax) * 255),	k-1 + plusLocation), 
			Point((int)((featureValue[k]   / featureMax) * 255),	k	+ plusLocation), 
			CV_RGB(255, 255, 0), 1, CV_AA, 0);
		
		circle(glcmHist, 
			Point((int)((featureValue[k]   / featureMax) * 255),	k	+ plusLocation), 
			1, CV_RGB(255, 0, 0));		
	}

	imshow("histo", glcmHist);

	/*char tmp[4];

	string str1 = "C:\\Users\\kwk\\Documents\\����\\���� 3���� ��ĳ�ʸ� ���� Confocal ���и�� ��� ����\\dstImage\\histo";
	string str2 = _itoa(test+1, tmp, 10);
	string str3 = ".bmp";
	string str = str1 + str2 + str3;

	imwrite(str, glcmHist);*/
	
	glcmHist.setTo(0);
	glcmHist.release();

}













#include <iostream>
#include <bitset>

#include <opencv2/opencv.hpp>


using namespace cv;



//GLCM의 방향 flag
enum ANGLE
{
	GLCM_ANGLE_0	= 0x1,		
	GLCM_ANGLE_45	= 0x2,		
	GLCM_ANGLE_90	= 0x4,		
	GLCM_ANGLE_135	= 0x8,		
	GLCM_ANGLE_ALL	= GLCM_ANGLE_0 | GLCM_ANGLE_45 | GLCM_ANGLE_90 | GLCM_ANGLE_135
};

enum HARALICK
{
	HARALICK_CONTRAST		= 0x1,
	HARALICK_ASM			= 0x2,
	HARALICK_IDM			= 0x4,
	HARALICK_ENTROPY		= 0x8,
	HARALICK_CORRELATION	= 0x10,
	HARALICK_VARIANCE		= 0x20,	
};




//GLCM 생성 및 Haralick Texture Feature 계산 클래스
class GrayLevelCoOccurrenceMatrix 
{

//생성, 소멸자
public:
	GrayLevelCoOccurrenceMatrix	();
	GrayLevelCoOccurrenceMatrix	(int _flagAngle, int _flagHaralick);
	~GrayLevelCoOccurrenceMatrix();

//Gray Level Co-Occurrence Matrix
public:
	double CoOccurrence			(Mat image, int distance);


//Haralick Texture Features
private:
	
	double Contrast					(double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135);
	double AngularSecondMoment		(double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135);
	double InverseDifferenceMoment	(double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135);
	double Entropy					(double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135);
	double Correlation				(double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135);
	double Variance					(double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135);
	



//변수
public:
	//GLCM 플래그
	int flagAngle;
	int flagHaralick;

	//Haralick 플래그
	std::bitset<10> bitAngle;
	std::bitset<10> bitHaralick;

	//GLCM사이즈 = 픽셀크기 (0~255)
	const int glcmSize=256;

};

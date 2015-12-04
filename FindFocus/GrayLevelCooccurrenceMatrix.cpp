#include "GrayLevelCooccurrenceMatrix.h"

GrayLevelCoOccurrenceMatrix::GrayLevelCoOccurrenceMatrix ()
{
	//Defalut
	flagAngle		= GLCM_ANGLE_ALL;
	flagHaralick	= HARALICK_CONTRAST;

	bitAngle		= flagAngle;
	bitHaralick		= flagHaralick;

}

GrayLevelCoOccurrenceMatrix::GrayLevelCoOccurrenceMatrix ( int _angle, int _haralick )
{
	flagAngle		= _angle;
	flagHaralick	= _haralick;

	bitAngle		= flagAngle;
	bitHaralick		= flagHaralick;
}


GrayLevelCoOccurrenceMatrix::~GrayLevelCoOccurrenceMatrix ()
{

	
}



double GrayLevelCoOccurrenceMatrix::CoOccurrence ( Mat image, int distance )
{	

	//정규화 변수 ( 영상의 총 크기 )
	const double normalizeFactor 
		= ( image.cols - distance ) * ( image.rows - distance );
	
	//GLCM 맵 초기화
	double	**GLCM_0	= NULL;		GLCM_0		= new double*[glcmSize];
	double	**GLCM_45	= NULL;		GLCM_45		= new double*[glcmSize];
	double	**GLCM_90	= NULL;		GLCM_90		= new double*[glcmSize];
	double	**GLCM_135	= NULL;		GLCM_135	= new double*[glcmSize];
	
	for ( int i = 0; i < glcmSize; i++ )
	{
		GLCM_0[i]	= new double[glcmSize];
		GLCM_45[i]	= new double[glcmSize];
		GLCM_90[i]	= new double[glcmSize];
		GLCM_135[i]	= new double[glcmSize];

		for ( int j = 0; j < glcmSize; j++ )
		{
			GLCM_0[i][j] = 0;
			GLCM_45[i][j] = 0;
			GLCM_90[i][j] = 0;
			GLCM_135[i][j] = 0;
		}
	}

	//GLCM 맵 생성
	for ( int j = 0; j < image.rows - distance; j++ )
	{
		for ( int i = 0; i < image.cols - distance; i++ )
		{
			//p0
			//p1
			GLCM_0
				[image.data[(j)		* image.step + (i)]]
				[image.data[(j + 1) * image.step + (i)]]++;

			//   p1
			//p0
			GLCM_45
				[image.data[(j + 1) * image.step + (i)]]
				[image.data[(j)		* image.step + (i + 1)]]++;

			//p0 p1
			//
			GLCM_90
				[image.data[(j)		* image.step + (i)]]
				[image.data[(j)		* image.step + (i + 1)]]++;

			//p0
			//   p1
			GLCM_135
				[image.data[(j)		* image.step + (i)]]
				[image.data[(j + 1) * image.step + (i + 1)]]++;
		}
	}

	//GLCM 정규화( 영상의 크기로 나누어 정규화 )
	for ( int i = 0; i < glcmSize; i++ )
	{
		for ( int j = 0; j < glcmSize; j++ )
		{	
			GLCM_0[i][j]		= GLCM_0[i][j]		/ normalizeFactor;
			GLCM_45[i][j]		= GLCM_45[i][j]		/ normalizeFactor;
			GLCM_90[i][j]		= GLCM_90[i][j]		/ normalizeFactor;
			GLCM_135[i][j]		= GLCM_135[i][j]	/ normalizeFactor;

		}
	}

	//Haralick Texture Features	
	double allFeature = 0;

	if ( flagHaralick & HARALICK_CONTRAST )
		allFeature = Contrast				( GLCM_0, GLCM_45, GLCM_90, GLCM_135 );	//Contrast 측정
	
	if ( flagHaralick & HARALICK_ASM )
		allFeature = AngularSecondMoment	( GLCM_0, GLCM_45, GLCM_90, GLCM_135 );	//Contrast 측정

	if ( flagHaralick & HARALICK_IDM )
		allFeature = InverseDifferenceMoment( GLCM_0, GLCM_45, GLCM_90, GLCM_135 );	//Contrast 측정

	if ( flagHaralick & HARALICK_ENTROPY )
		allFeature = Entropy				( GLCM_0, GLCM_45, GLCM_90, GLCM_135 );	//Contrast 측정

	if ( flagHaralick & HARALICK_CORRELATION )
		allFeature = Correlation			( GLCM_0, GLCM_45, GLCM_90, GLCM_135 );	//Contrast 측정
	
	if ( flagHaralick & HARALICK_VARIANCE )
		allFeature = Variance				( GLCM_0, GLCM_45, GLCM_90, GLCM_135 );	//Contrast 측정
	
	

	//GLCM 메모리 반환
	for ( int i = 0; i < glcmSize; i++ )
	{
		delete[] GLCM_0[i];
		delete[] GLCM_45[i];
		delete[] GLCM_90[i];
		delete[] GLCM_135[i];
	}

	delete[] GLCM_0;
	delete[] GLCM_45;
	delete[] GLCM_90;
	delete[] GLCM_135;

	//총 특징 값 리턴
	return 
		allFeature 
		/ bitHaralick.count ();	
}



double GrayLevelCoOccurrenceMatrix::Contrast ( double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135 )
{
	//생성
	double contrastFeature_0	= 0;
	double contrastFeature_45	= 0;
	double contrastFeature_90	= 0;
	double contrastFeature_135	= 0;

	

	//GLCM으로부터 Contrast 계산
	for ( int i = 0; i < glcmSize; i++ )
	{
		for ( int j = 0; j < glcmSize; j++ )
		{			
			if ( flagAngle & GLCM_ANGLE_0 )
				contrastFeature_0		+= ( i - j ) * ( i - j ) * ( GLCM_0[i][j] * GLCM_0[i][j] );

			if ( flagAngle & GLCM_ANGLE_45 )
				contrastFeature_45		+= ( i - j ) * ( i - j ) * ( GLCM_45[i][j] * GLCM_45[i][j] );

			if ( flagAngle & GLCM_ANGLE_90 )
				contrastFeature_90		+= ( i - j ) * ( i - j ) * ( GLCM_90[i][j] * GLCM_90[i][j] );

			if ( flagAngle & GLCM_ANGLE_135 )
				contrastFeature_135		+= ( i - j ) * ( i - j ) * ( GLCM_135[i][j] * GLCM_135[i][j] );
		}
	}
	
	//특징 값 리턴
	return 
		( contrastFeature_0 + contrastFeature_45 + contrastFeature_90 + contrastFeature_135 ) 
		/ bitAngle.count ();
}


double GrayLevelCoOccurrenceMatrix::AngularSecondMoment ( double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135 )
{
	//생성
	double asmFeature_0		= 0;
	double asmFeature_45	= 0;
	double asmFeature_90	= 0;
	double asmFeature_135	= 0;
		

	//GLCM으로부터 ASM 계산
	for ( int i = 0; i < glcmSize; i++ )
	{
		for ( int j = 0; j < glcmSize; j++ )
		{			
			if ( flagAngle & GLCM_ANGLE_0 )
				asmFeature_0		+= ( GLCM_0[i][j] * GLCM_0[i][j] ) * ( GLCM_0[i][j] * GLCM_0[i][j] );

			if ( flagAngle & GLCM_ANGLE_45 )
				asmFeature_45		+= ( GLCM_45[i][j] * GLCM_45[i][j] ) * ( GLCM_0[i][j] * GLCM_0[i][j] );

			if ( flagAngle & GLCM_ANGLE_90 )
				asmFeature_90		+= ( GLCM_90[i][j] * GLCM_90[i][j] ) * ( GLCM_0[i][j] * GLCM_0[i][j] );

			if ( flagAngle & GLCM_ANGLE_135 )
				asmFeature_135		+= ( GLCM_135[i][j] * GLCM_135[i][j] ) * ( GLCM_0[i][j] * GLCM_0[i][j] );
		}
	}

	//특징 값 리턴
	return 
		( asmFeature_0 + asmFeature_45 + asmFeature_90 + asmFeature_135 ) 
		/ bitAngle.count ();
}

double GrayLevelCoOccurrenceMatrix::InverseDifferenceMoment ( double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135 )
{
	//생성
	double idmFeature_0		= 0;
	double idmFeature_45	= 0;
	double idmFeature_90	= 0;
	double idmFeature_135	= 0;


	//GLCM으로부터 ASM 계산
	for (int i = 0; i < glcmSize; i++)
	{
		for (int j = 0; j < glcmSize; j++)
		{			
			if (flagAngle & GLCM_ANGLE_0)
				idmFeature_0	+= ( GLCM_0[i][j] / (double)(1 + ( (i - j) * (i - j) )) );

			if (flagAngle & GLCM_ANGLE_45)
				idmFeature_45	+= ( GLCM_45[i][j] / (double)(1 + ( (i - j) * (i - j) )) );

			if (flagAngle & GLCM_ANGLE_90)
				idmFeature_90	+= ( GLCM_90[i][j] / (double)(1 + ( (i - j) * (i - j) )) );

			if (flagAngle & GLCM_ANGLE_135)
				idmFeature_135	+= ( GLCM_135[i][j] / (double)(1 + ( (i - j) * (i - j) )) );
		}
	}

	//특징 값 리턴
	return
		(idmFeature_0 + idmFeature_45 + idmFeature_90 + idmFeature_135)
		/ bitAngle.count();
}

double GrayLevelCoOccurrenceMatrix::Entropy ( double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135 )
{
	//생성
	double entropyFeature_0 = 0;
	double entropyFeature_45 = 0;
	double entropyFeature_90 = 0;
	double entropyFeature_135 = 0;


	//GLCM으로부터 ASM 계산
	for (int i = 0; i < glcmSize; i++)
	{
		for (int j = 0; j < glcmSize; j++)
		{
			if (flagAngle & GLCM_ANGLE_0)
				entropyFeature_0	+= GLCM_0[i][j] * log10(GLCM_0[i][j]);

			if (flagAngle & GLCM_ANGLE_45)
				entropyFeature_45	+= GLCM_45[i][j] * log10(GLCM_45[i][j]);

			if (flagAngle & GLCM_ANGLE_90)
				entropyFeature_90	+= GLCM_90[i][j] * log10(GLCM_90[i][j]);

			if (flagAngle & GLCM_ANGLE_135)
				entropyFeature_135	+= GLCM_135[i][j] * log10(GLCM_135[i][j]);
		}
	}

	//특징 값 리턴
	return
		(entropyFeature_0 + entropyFeature_45 + entropyFeature_90 + entropyFeature_135)
		/ bitAngle.count();
}

double GrayLevelCoOccurrenceMatrix::Correlation ( double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135 )
{
	//x, y방향 평균, 분산
	double meanX_0		= 0;	double meanY_0		= 0;
	double meanX_45		= 0;	double meanY_45		= 0;
	double meanX_90		= 0;	double meanY_90		= 0;
	double meanX_135	= 0;	double meanY_135	= 0;

	double varianceX_0		= 0;	double varianceY_0		= 0;
	double varianceX_45		= 0;	double varianceY_45		= 0;
	double varianceX_90		= 0;	double varianceY_90		= 0;
	double varianceX_135	= 0;	double varianceY_135	= 0;

	//Correlation 특징
	double correlationFeature_0		= 0;
	double correlationFeature_45	= 0;
	double correlationFeature_90	= 0;
	double correlationFeature_135	= 0;

	//GLCM으로부터 각 x, y방향의 평균
	for ( int i = 0; i < glcmSize; i++ )
	{
		for ( int j = 0; j < glcmSize; j++ )
		{
			if ( flagAngle & GLCM_ANGLE_0 )
			{
				meanX_0 += j * GLCM_0[i][j];
				meanY_0 += i * GLCM_0[i][j];			
			}

			if ( flagAngle & GLCM_ANGLE_45 )
			{
				meanX_45 += j * GLCM_45[i][j];
				meanY_45 += i * GLCM_45[i][j];
			}

			if ( flagAngle & GLCM_ANGLE_90 )
			{
				meanX_90 += j * GLCM_90[i][j];
				meanY_90 += i * GLCM_90[i][j];
			}

			if ( flagAngle & GLCM_ANGLE_135 )
			{
				meanX_135 += j * GLCM_135[i][j];
				meanY_135 += i * GLCM_135[i][j];
			}
		}
	}

	//GLCM으로부터 각 x, y방향의 분산
	for (int i = 0; i < glcmSize; i++)
	{
		for (int j = 0; j < glcmSize; j++)
		{
			if ( flagAngle & GLCM_ANGLE_0 )
			{
				varianceX_0 += sqrt ( (( j - meanX_0 ) *  ( j - meanX_0 )) * GLCM_0[i][j] );
				varianceY_0 += sqrt ( (( i - meanY_0 ) *  ( i - meanY_0 )) * GLCM_0[i][j] );
			}

			if ( flagAngle & GLCM_ANGLE_45 )
			{
				varianceX_45 += sqrt ( (( j - meanX_45 ) *  ( j - meanX_45 )) * GLCM_45[i][j] );
				varianceY_45 += sqrt ( (( i - meanY_45 ) *  ( i - meanY_45 )) * GLCM_45[i][j] );
			}

			if ( flagAngle & GLCM_ANGLE_90 )
			{
				varianceX_90 += sqrt ( (( j - meanX_90 ) *  ( j - meanX_90 )) * GLCM_90[i][j] );
				varianceY_90 += sqrt ( (( i - meanY_90 ) *  ( i - meanY_90 )) * GLCM_90[i][j] );
			}

			if ( flagAngle & GLCM_ANGLE_135 )
			{
				varianceX_135 += sqrt ( (( j - meanX_135 ) *  ( j - meanX_135 )) * GLCM_135[i][j] );
				varianceY_135 += sqrt ( (( i - meanY_135 ) *  ( i - meanY_135 )) * GLCM_135[i][j] );
			}
		}
	}

	//GLCM으로부터 Correlation 계산
	for (int i = 0; i < glcmSize; i++)
	{
		for (int j = 0; j < glcmSize; j++)
		{
			if ( flagAngle & GLCM_ANGLE_0 )
				correlationFeature_0	+= GLCM_0[i][j] * ( (( i - meanY_0 ) * ( j - meanX_0 )) / (varianceX_0 *  varianceY_0) );

			if ( flagAngle & GLCM_ANGLE_45 )
				correlationFeature_45	+= GLCM_45[i][j] * ( (( i - meanY_45 ) * ( j - meanX_45 )) / (varianceX_45 *  varianceY_45) );

			if ( flagAngle & GLCM_ANGLE_90 )
				correlationFeature_90	+= GLCM_90[i][j] * ( (( i - meanY_90 ) * ( j - meanX_90 )) / (varianceX_90 *  varianceY_90) );

			if ( flagAngle & GLCM_ANGLE_135 )
				correlationFeature_135	+= GLCM_135[i][j] * ( (( i - meanY_135 ) * ( j - meanX_135 )) / (varianceX_135 *  varianceY_135) );
		}
	}

	//특징 값 리턴
	return
		( correlationFeature_0 + correlationFeature_45 + correlationFeature_90 + correlationFeature_135 )
		/ bitAngle.count();

}

double GrayLevelCoOccurrenceMatrix::Variance ( double **GLCM_0, double **GLCM_45, double **GLCM_90, double **GLCM_135 )
{
	//x, y방향 평균
	double meanX_0		= 0;	double meanY_0		= 0;
	double meanX_45		= 0;	double meanY_45		= 0;
	double meanX_90		= 0;	double meanY_90		= 0;
	double meanX_135	= 0;	double meanY_135	= 0;

	//Variance 특징
	double varianceFeature_0	= 0;
	double varianceFeature_45	= 0;
	double varianceFeature_90	= 0;
	double varianceFeature_135	= 0;

	//GLCM으로부터 각 x, y방향의 평균
	for ( int i = 0; i < glcmSize; i++ )
	{
		for ( int j = 0; j < glcmSize; j++ )
		{
			if ( flagAngle & GLCM_ANGLE_0 )
			{
				meanX_0 += j * GLCM_0[i][j];
				meanY_0 += i * GLCM_0[i][j];			
			}

			if ( flagAngle & GLCM_ANGLE_45 )
			{
				meanX_45 += j * GLCM_45[i][j];
				meanY_45 += i * GLCM_45[i][j];
			}

			if ( flagAngle & GLCM_ANGLE_90 )
			{
				meanX_90 += j * GLCM_90[i][j];
				meanY_90 += i * GLCM_90[i][j];
			}

			if ( flagAngle & GLCM_ANGLE_135 )
			{
				meanX_135 += j * GLCM_135[i][j];
				meanY_135 += i * GLCM_135[i][j];
			}
		}
	}

	//GLCM으로부터 Variance 계산
	for (int i = 0; i < glcmSize; i++)
	{
		for (int j = 0; j < glcmSize; j++)
		{
			if ( flagAngle & GLCM_ANGLE_0 )
				varianceFeature_0	+= GLCM_0[i][j]		* ( (( i - meanX_0 ) * ( j - meanX_0 )) );

			if ( flagAngle & GLCM_ANGLE_45 )
				varianceFeature_45	+= GLCM_45[i][j]	* ( (( i - meanX_45 ) * ( j - meanX_45 )) );

			if ( flagAngle & GLCM_ANGLE_90 )
				varianceFeature_90	+= GLCM_90[i][j]	* ( (( i - meanX_90 ) * ( j - meanX_90 )) );

			if ( flagAngle & GLCM_ANGLE_135 )
				varianceFeature_135	+= GLCM_135[i][j]	* ( (( i - meanX_135 ) * ( j - meanX_135 )) );

		}
	}

	//특징 값 리턴
	return
		(varianceFeature_0 + varianceFeature_45 + varianceFeature_90 + varianceFeature_135)
		/ bitAngle.count();

}

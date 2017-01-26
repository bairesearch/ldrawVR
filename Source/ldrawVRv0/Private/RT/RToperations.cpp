/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: RToperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Raytracer Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "RToperations.hpp"

void RToperationsClass::createAdvancedVector(advancedVec* vec)
{
	vec->x = 0;
	vec->y = 0;
	vec->z = 0;
	vec->w = 1;
}

void RToperationsClass::createIdentityMatrixAdvanced(advancedMat* matx)
{
	matx->a.x = ONE;
	matx->b.x = ZERO;
	matx->c.x = ZERO;
	matx->d.x = ZERO;
	matx->a.y = ZERO;
	matx->b.y = ONE;
	matx->c.y = ZERO;
	matx->d.y = ZERO;
	matx->a.z = ZERO;
	matx->b.z = ZERO;
	matx->c.z = ONE;
	matx->d.z = ZERO;
	matx->a.w = ZERO;
	matx->b.w = ZERO;
	matx->c.w = ZERO;
	matx->d.w = ONE;
}

void RToperationsClass::makeAdvancedMatrix(advancedMat* matxAdv, mat* matx, int type)
{
	matxAdv->a.x = matx->a.x;
	matxAdv->b.x = matx->b.x;
	matxAdv->c.x = matx->c.x;
	matxAdv->d.x = ZERO;
	matxAdv->a.y = matx->a.y;
	matxAdv->b.y = matx->b.y;
	matxAdv->c.y = matx->c.y;
	matxAdv->d.y = ZERO;
	matxAdv->a.z = matx->a.z;
	matxAdv->b.z = matx->b.z;
	matxAdv->c.z = matx->c.z;
	matxAdv->d.z = ZERO;
	matxAdv->a.w = ZERO;
	matxAdv->b.w = ZERO;
	matxAdv->c.w = ZERO;
	matxAdv->d.w = (double)type;
}

void RToperationsClass::multAdvancedMatrixByVector(advancedVec* vecAdv1, advancedMat* matxAdv1, advancedVec* vecAdv)
{
	vecAdv->x =  matxAdv1->a.x* vecAdv1->x + matxAdv1->b.x* vecAdv1->y + matxAdv1->c.x* vecAdv1->z + matxAdv1->d.x* vecAdv1->w;
	vecAdv->y =  matxAdv1->a.y* vecAdv1->x + matxAdv1->b.y* vecAdv1->y + matxAdv1->c.y* vecAdv1->z + matxAdv1->d.y* vecAdv1->w;
	vecAdv->z =  matxAdv1->a.z* vecAdv1->x + matxAdv1->b.z* vecAdv1->y + matxAdv1->c.z* vecAdv1->z + matxAdv1->d.z* vecAdv1->w;
	vecAdv->w =  matxAdv1->a.w* vecAdv1->x + matxAdv1->b.w* vecAdv1->y + matxAdv1->c.w* vecAdv1->z + matxAdv1->d.w* vecAdv1->w;
}

void RToperationsClass::multAdvancedMatrix(advancedMat* matxAdv1, advancedMat* matxAdv2, advancedMat* matxAdv)
{
	matxAdv->a.x = matxAdv1->a.x*matxAdv2->a.x + matxAdv1->b.x*matxAdv2->a.y +  matxAdv1->c.x*matxAdv2->a.z + matxAdv1->d.x*matxAdv2->a.w;
	matxAdv->b.x = matxAdv1->a.x*matxAdv2->b.x + matxAdv1->b.x*matxAdv2->b.y +  matxAdv1->c.x*matxAdv2->b.z + matxAdv1->d.x*matxAdv2->b.w;
	matxAdv->c.x = matxAdv1->a.x*matxAdv2->c.x + matxAdv1->b.x*matxAdv2->c.y +  matxAdv1->c.x*matxAdv2->c.z + matxAdv1->d.x*matxAdv2->c.w;
	matxAdv->d.x = matxAdv1->a.x*matxAdv2->d.x + matxAdv1->b.x*matxAdv2->d.y +  matxAdv1->c.x*matxAdv2->d.z + matxAdv1->d.x*matxAdv2->d.w;

	matxAdv->a.y = matxAdv1->a.y*matxAdv2->a.x + matxAdv1->b.y*matxAdv2->a.y +  matxAdv1->c.y*matxAdv2->a.z + matxAdv1->d.y*matxAdv2->a.w;
	matxAdv->b.y = matxAdv1->a.y*matxAdv2->b.x + matxAdv1->b.y*matxAdv2->b.y +  matxAdv1->c.y*matxAdv2->b.z + matxAdv1->d.y*matxAdv2->b.w;
	matxAdv->c.y = matxAdv1->a.y*matxAdv2->c.x + matxAdv1->b.y*matxAdv2->c.y +  matxAdv1->c.y*matxAdv2->c.z + matxAdv1->d.y*matxAdv2->c.w;
	matxAdv->d.y = matxAdv1->a.y*matxAdv2->d.x + matxAdv1->b.y*matxAdv2->d.y +  matxAdv1->c.y*matxAdv2->d.z + matxAdv1->d.y*matxAdv2->d.w;

	matxAdv->a.z = matxAdv1->a.z*matxAdv2->a.x + matxAdv1->b.z*matxAdv2->a.y +  matxAdv1->c.z*matxAdv2->a.z + matxAdv1->d.z*matxAdv2->a.w;
	matxAdv->b.z = matxAdv1->a.z*matxAdv2->b.x + matxAdv1->b.z*matxAdv2->b.y +  matxAdv1->c.z*matxAdv2->b.z + matxAdv1->d.z*matxAdv2->b.w;
	matxAdv->c.z = matxAdv1->a.z*matxAdv2->c.x + matxAdv1->b.z*matxAdv2->c.y +  matxAdv1->c.z*matxAdv2->c.z + matxAdv1->d.z*matxAdv2->c.w;
	matxAdv->d.z = matxAdv1->a.z*matxAdv2->d.x + matxAdv1->b.z*matxAdv2->d.y +  matxAdv1->c.z*matxAdv2->d.z + matxAdv1->d.z*matxAdv2->d.w;

	matxAdv->a.w = matxAdv1->a.w*matxAdv2->a.x + matxAdv1->b.w*matxAdv2->a.y +  matxAdv1->c.w*matxAdv2->a.z + matxAdv1->d.w*matxAdv2->a.w;
	matxAdv->b.w = matxAdv1->a.w*matxAdv2->b.x + matxAdv1->b.w*matxAdv2->b.y +  matxAdv1->c.w*matxAdv2->b.z + matxAdv1->d.w*matxAdv2->b.w;
	matxAdv->c.w = matxAdv1->a.w*matxAdv2->c.x + matxAdv1->b.w*matxAdv2->c.y +  matxAdv1->c.w*matxAdv2->c.z + matxAdv1->d.w*matxAdv2->c.w;
	matxAdv->d.w = matxAdv1->a.w*matxAdv2->d.x + matxAdv1->b.w*matxAdv2->d.y +  matxAdv1->c.w*matxAdv2->d.z + matxAdv1->d.w*matxAdv2->d.w;
}



void RToperationsClass::createInverseTranslationMatrix(double xpos, double ypos, double zpos, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->d.x = -xpos;
	matx->d.y = -ypos;
	matx->d.z = -zpos;
	/*matx->d.w = 1 --> because it is a vertex not a vector*/
}

/*CHECK THIS ONE*/
void RToperationsClass::createTranslationMatrix(double xpos, double ypos, double zpos, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->d.x = xpos;
	matx->d.y = ypos;
	matx->d.z = zpos;
	/*matx->d.w = 1 --> because it is a vertex not a vector*/
}

void RToperationsClass::createScaleMatrix(double width, double length, double height, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->a.x = width;
	matx->b.y = length;
	matx->c.z = height;
	/*matx->d.w = 1 --> because it is a vertex not a vector*/
}

void RToperationsClass::createInverseScaleMatrix(const double width, const double length, const double height, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->a.x = 1/width;
	matx->b.y = 1/length;
	matx->c.z = 1/height;
}


void RToperationsClass::createRotationxMatrix(double rotation, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->b.y = cos(rotation);
	matx->b.z = -sin(rotation);
	matx->c.y = sin(rotation);
	matx->c.z = cos(rotation);
}
void RToperationsClass::createInverseRotationxMatrix(double rotation, advancedMat* matx)
{
	this->createRotationxMatrix(rotation, matx);
	this->invertAdvancedMatrix(matx);
}

void RToperationsClass::createRotationyMatrix(double rotation, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->a.x = cos(rotation);
	matx->a.z = sin(rotation);
	matx->c.x = -sin(rotation);
	matx->c.z = cos(rotation);
}
void RToperationsClass::createInverseRotationyMatrix(double rotation, advancedMat* matx)
{
	this->createRotationyMatrix(rotation, matx);
	this->invertAdvancedMatrix(matx);
}

void RToperationsClass::createRotationzMatrix(double rotation, advancedMat* matx)
{
	this->createIdentityMatrixAdvanced(matx);
	matx->a.x = cos(rotation);
	matx->a.y = -sin(rotation);
	matx->b.x = sin(rotation);
	matx->b.y = cos(rotation);
}
void RToperationsClass::createInverseRotationzMatrix(double rotation, advancedMat* matx)
{
	this->createRotationzMatrix(rotation, matx);
	this->invertAdvancedMatrix(matx);
}

void RToperationsClass::invertAdvancedMatrix(advancedMat* matx)
{
	/*since the matrix is orthogonal,it can be transposed instead*/
	this->transposeAdvancedMatrix(matx);
}

void RToperationsClass::transposeAdvancedMatrix(advancedMat* matx)
{
	double tmp;

	tmp = matx->a.y;
	matx->a.y = matx->b.x;
	matx->b.x = tmp;

	tmp = matx->a.z;
	matx->a.z = matx->c.x;
	matx->c.x = tmp;

	tmp = matx->a.w;
	matx->a.w = matx->d.x;
	matx->d.x = tmp;

	tmp = matx->b.z;
	matx->b.z = matx->c.y;
	matx->c.y = tmp;

	tmp = matx->b.w;
	matx->b.w = matx->d.y;
	matx->d.y = tmp;

	tmp = matx->c.w;
	matx->c.w = matx->d.z;
	matx->d.z = tmp;
}

double RToperationsClass::toRadians(const double degrees)
{
	return degrees/180*PI;
}

void RToperationsClass::toAdvancedVector(vec* vec, int type, advancedVec* vecAdv)
{
	vecAdv->x = vec->x;
	vecAdv->y = vec->y;
	vecAdv->z = vec->z;
	vecAdv->w = (double)type;
}

void RToperationsClass::fromAdvancedVector(advancedVec* vecAdv, vec* vec)
{
	vec->x = vecAdv->x;
	vec->y = vecAdv->y;
	vec->z = vecAdv->z;
}















double RToperationsClass::findSmallestValueAdvanced(double val1, const double val2)
{
	if((val1 > ZERO) && (val2 > ZERO))
	{
		return this->findSmallestValue(val1, val2);
	}
	else if(val1 > ZERO)
	{
		return val1;
	}
	else if(val2 > ZERO)
	{
		return val2;
	}
	else
	{
		return -1;
	}
}


double RToperationsClass::findSmallestValue(double val1, const double val2)
{
	if(val1 < val2)
	{
		return val1;
	}
	else
	{
		return val2;
	}
}

double RToperationsClass::findGreatestValueAdvanced(double val1, const double val2)
{
	if((val1 > ZERO) && (val2 > ZERO))
	{
		return this->findGreatestValue(val1, val2);
	}
	else if(val1 > ZERO)
	{
		return val1;
	}
	else if(val2 > ZERO)
	{
		return val2;
	}
	else
	{
		return -1;
	}
}


double RToperationsClass::findGreatestValue(double val1, const double val2)
{
	if(val1 > val2)
	{
		return val1;
	}
	else
	{
		return val2;
	}
}




double RToperationsClass::findSmallestValue(const double val1, const double val2, double val3)
{
	if(val1 > val2)
	{
		if(val2 > val3)
		{
			return val3;
		}
		else
		{
			return val2;
		}
	}
	else
	{
		if(val1 > val3)
		{
			return val3;
		}
		else
		{
			return val1;
		}
	}
}

double RToperationsClass::findGreatestValue(const double val1, const double val2, double val3)
{
	if(val1 < val2)
	{
		if(val2 < val3)
		{
			return val3;
		}
		else
		{
			return val2;
		}
	}
	else
	{
		if(val1 < val3)
		{
			return val3;
		}
		else
		{
			return val1;
		}
	}
}




int RToperationsClass::findPositionOfSmallestValueAdvanced(const double* array, const int size)
{
	int position = NOT_FOUND;
	double tmp = -1;
	int i;
	for(i = 0; i< size; i++)
	{
		if(tmp < ZERO)
		{

			if(array[i] > ZERO)
			{
				position = i;
				tmp = array[i];
			}

		}
		else
		{/*tmp != 0*/
			if((array[i] < tmp) && (array[i] > ZERO))
			{
				position = i;
				tmp = array[i];
			}
		}
	}
	return position;
}
int RToperationsClass::findPositionOfGreatestValueAdvanced(const double* array, const int size)
{
	int position = NOT_FOUND;
	double tmp = -1;
	int i;
	for(i = 0; i< size; i++)
	{
		if(tmp < ZERO)
		{

			if(array[i] > ZERO)
			{
				position = i;
				tmp = array[i];
			}
		}
		else
		{/*tmp != 0*/
			if((array[i] > tmp) && (array[i] > ZERO))
			{
				position = i;
				tmp = array[i];
			}
		}
	}
	return position;
}





int RToperationsClass::findPositionOfSmallestValueWhichHits(const double* array, const int* hitsArray, const int size)
{
	int position = NOT_FOUND;
	double tmp = REALLY_LARGE_DOUBLE;
	int i;
	for(i = 0; i< size; i++)
	{
		if((array[i] < tmp) && (hitsArray[i] == TRUE))
		{
			position = i;
			tmp = array[i];
		}
	}
	return position;
}


int RToperationsClass::findPositionOfGreatestValueWhichHits(const double* array, const int* hitsArray, const int size)
{
	int position = NOT_FOUND;
	double tmp = REALLY_SMALL_DOUBLE;
	int i;
	for(i = 0; i< size; i++)
	{
		if((array[i] > tmp) && (hitsArray[i] == TRUE))
		{
			position = i;
			tmp = array[i];
		}

	}
	return position;
}



int RToperationsClass::findIntersectLineWithLine(const vec* linept1, const vec* linept2, vec* povpt1, vec* povpt2, vec* pt_int, const vec* norm, double* t)
{
	if(this->findIntersectLineWithLine2D(povpt1, povpt2, linept1, linept2, pt_int))
	{

		bool boundaryCheck = true;

		double maxlinex;
		double minlinex;
		if(linept1->x > linept2->x)
		{
			maxlinex = linept1->x;
			minlinex = linept2->x;
		}
		else
		{
			maxlinex = linept2->x;
			minlinex = linept1->x;
		}

		double maxliney;
		double minliney;
		if(linept1->y > linept2->y)
		{
			maxliney = linept1->y;
			minliney = linept2->y;
		}
		else
		{
			maxliney = linept2->y;
			minliney = linept1->y;
		}

		double maxlinez;
		double minlinez;
		if(linept1->z > linept2->z)
		{
			maxlinez = linept1->z;
			minlinez = linept2->z;
		}
		else
		{
			maxlinez = linept2->z;
			minlinez = linept1->z;
		}

		if((pt_int->x > maxlinex+LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED)
		|| (pt_int->x < minlinex-LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED)
		|| (pt_int->y > maxliney+LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED)
		|| (pt_int->y < minliney-LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED)
		|| (pt_int->z > maxlinez+LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED)
		|| (pt_int->z < minlinez-LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED))
		{
			boundaryCheck = false;
		}

		if(boundaryCheck)
		{
			//p0 = povpt1
			//p1 = povpt2
			//pt_int = p0 + t(p1-p0)
			//t = (pt_int - p0) / (p1-p0)

			vec* p0 = povpt1;
			vec* p1 = povpt2;
			*t = (pt_int->x - p0->x) / (p1->x - p0->x);

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return FALSE;
	}
}





bool RToperationsClass::findIntersectLineWithLine2D(const vec* povpt1, const vec* povpt2, const vec* linept1, const vec* linept2, vec* pt_int)
{
	bool result;

	double xyXInt, zxXInt, xyYInt, yzYInt, yzZInt, zxZInt;

	bool xyinterceptionFound, xyinterceptionPointFound, yzinterceptionFound, yzinterceptionPointFound, zxinterceptionFound, zxinterceptionPointFound;

	xyinterceptionFound = false;
	xyinterceptionPointFound = false;
	yzinterceptionFound = false;
	yzinterceptionPointFound = false;
	zxinterceptionFound = false;
	zxinterceptionPointFound = false;

	double x1;
	double x2;
	double x3;
	double x4;
	double y1;
	double y2;
	double y3;
	double y4;

	x1 = povpt1->x;
	y1 = povpt1->y;
	x2 = linept1->x;
	y2 = linept1->y;
	x3 = povpt2->x;
	y3 = povpt2->y;
	x4 = linept2->x;
	y4 = linept2->y;

	SHAREDvector.find2DintersectionPoint(x1, y1, x2, y2, x3, y3, x4, y4, &xyXInt, &xyYInt, &xyinterceptionFound, &xyinterceptionPointFound);
	//xy plane

	x1 = povpt1->y;
	y1 = povpt1->z;
	x2 = linept1->y;
	y2 = linept1->z;
	x3 = povpt2->y;
	y3 = povpt2->z;
	x4 = linept2->y;
	y4 = linept2->z;

	SHAREDvector.find2DintersectionPoint(x1, y1, x2, y2, x3, y3, x4, y4, &yzYInt, &yzZInt, &yzinterceptionFound, &yzinterceptionPointFound);
	//yz plane

	x1 = povpt1->z;
	y1 = povpt1->x;
	x2 = linept1->z;
	y2 = linept1->x;
	x3 = povpt2->z;
	y3 = povpt2->x;
	x4 = linept2->z;
	y4 = linept2->x;

	SHAREDvector.find2DintersectionPoint(x1, y1, x2, y2, x3, y3, x4, y4, &zxZInt, &zxXInt, &zxinterceptionFound, &zxinterceptionPointFound);
	//zx plane

	bool xyzinterceptionPointFound = false;

	if(xyinterceptionFound && yzinterceptionFound && zxinterceptionFound)
	{
		result = true;

		if(xyinterceptionPointFound && zxinterceptionPointFound)
		{
			if(!SHAREDvector.compareDoublesRelaxed(xyXInt, zxXInt))
			{//xy plane
				result = false;
			}
			else
			{
				xyzinterceptionPointFound = true;
				pt_int->x = xyXInt;
				pt_int->y = xyYInt;
				pt_int->z = zxZInt;
				#ifdef RT_DEBUG
				//cout << "DEBUG: X intersection on xy plane = X intersection on zx plane detected" << endl;
				#endif
			}
		}
		else
		{
			#ifdef RT_DEBUG
			//cout << "DEBUG: X intersection on xy plane = X intersection on zx plane detected" << endl;
			#endif
		}

		if(xyinterceptionPointFound && yzinterceptionPointFound)
		{
			if(!SHAREDvector.compareDoublesRelaxed(xyYInt, yzYInt))
			{//yz plane
				result = false;
			}
			else
			{
				xyzinterceptionPointFound = true;

				pt_int->x = xyXInt;
				pt_int->y = xyYInt;
				pt_int->z = yzZInt;
				#ifdef RT_DEBUG
				//cout << "DEBUG: Y intersection on xy plane = Y intersection on yz plane detected" << endl;
				#endif
			}
		}
		else
		{
			#ifdef RT_DEBUG
			//cout << "DEBUG: Y intersection on xy plane = Y intersection on yz plane detected" << endl;
			#endif
		}

		if(yzinterceptionPointFound && zxinterceptionPointFound)
		{
			if(!SHAREDvector.compareDoublesRelaxed(yzZInt, zxZInt))
			{//zx plane
				result = false;
			}
			else
			{
				xyzinterceptionPointFound = true;

				pt_int->x = zxXInt;
				pt_int->y = yzYInt;
				pt_int->z = yzZInt;
				#ifdef RT_DEBUG
				//cout << "DEBUG: Z intersection on yz plane = Z intersection on zx plane detected" << endl;
				#endif
			}
		}
		else
		{
			#ifdef RT_DEBUG
			//cout << "DEBUG: Z intersection on yz plane = Z intersection on zx plane detected" << endl;
			#endif
		}


		if(!xyzinterceptionPointFound && result)
		{
			printf("error; !xyzinterceptionPointFound && result");
			exit(0);
		}
	}
	else
	{
		result = false;
	}

	return result;
}



/*Tri Intersection Code Copyright http://www.angelfire.com/fl/houseofbartlett/solutions/line2tri.html*/

#define SAME_CLOCKNESS 1
#define DIFF_CLOCKNESS 0

int RToperationsClass::findIntersectLineWithTri(const vec* pt1, const vec* pt2, const vec* pt3, vec* linept1, vec* linept2, vec* pt_int, vec* norm, double* t)
{
	double V1x, V1y, V1z;
	double V2x, V2y, V2z;
	double dotprod;

	vec vectorP1MinusP0;
	SHAREDvector.subtractVectorsRT(linept2, linept1, &vectorP1MinusP0);
	vec* vect = &vectorP1MinusP0;

	// vector form triangle pt1 to pt2
	V1x = pt2->x - pt1->x;
	V1y = pt2->y - pt1->y;
	V1z = pt2->z - pt1->z;

	// vector form triangle pt2 to pt3
	V2x = pt3->x - pt2->x;
	V2y = pt3->y - pt2->y;
	V2z = pt3->z - pt2->z;

	// vector normal of triangle
	norm->x = V1y*V2z - V1z*V2y;
	norm->y = V1z*V2x - V1x*V2z;
	norm->z = V1x*V2y - V1y*V2x;

	// dot product of normal and line's vector if zero line is parallel to triangle
	dotprod = norm->x* vect->x + norm->y* vect->y + norm->z* vect->z;


	/*
	if((norm->x != 0) || (norm->y != 0) || (norm->z != 0))
	{
		//printf("\n tegv");
	}
	printf("\n\n V1y= %f",V1y);
	printf("\n V2z = %f", V2z);
	printf("\n V1z = %f", V1z);
	printf("\n V2y = %f", V2y);
	printf("\n V2x = %f", V2x);
	printf("\n V1x = %f", V1x);
	printf("\n V1y = %f", V1y);

	printf("\n dotprod = %f", dotprod);
	printf("\n norm->x = %f", norm->x);
	printf("\n norm->y = %f", norm->y);
	printf("\n norm->z = %f", norm->z);
	printf("\n vect->x = %f", vect->x);
	printf("\n vect->x = %f", vect->y);
	printf("\n vect->x = %f", vect->z);
	*/

	//this condition removed by RBB October 08; required for negative points to work - maybe this condition should be; if dotprod == 0 instead?
	/*
	if(dotprod < 0)
	{
	*/
		//printf("mayyetpass");

		//Find point of intersect to triangle plane.
		//find t to intersect point
		*t = -(norm->x*(linept1->x-pt1->x) + norm->y*(linept1->y-pt1->y) + norm->z*(linept1->z-pt1->z))/
		     (norm->x*vect->x + norm->y*vect->y + norm->z*vect->z);

		// if ds is neg line started past triangle so can't hit triangle.

		if(*t < 0)
		{
			//printf("fail1");
			return 0;

		}

		pt_int->x = linept1->x + vect->x* (*t);
		pt_int->y = linept1->y + vect->y* (*t);
		pt_int->z = linept1->z + vect->z* (*t);

		if(this->checkSameClockDir(pt1, pt2, pt_int, norm) == SAME_CLOCKNESS)
		{
			if(this->checkSameClockDir(pt2, pt3, pt_int, norm) == SAME_CLOCKNESS)
			{
				if(this->checkSameClockDir(pt3, pt1, pt_int, norm) == SAME_CLOCKNESS)
				{
					/*
					norm->x = -norm->x;
					norm->y = -norm->z;
					norm->z = -norm->z;

					//or negativeVector(norm, norm);
					*/

					//printf("pass");
					// answer in pt_int is inside triangle
					return 1;
				}
				else
				{
					//printf("fail3");
				}
			}
			else
			{
				//printf("fail4");
			}
		}
		else
		{
			//printf("fail5");
		}
	/*
	}
	*/

	//printf("fail6");
	return 0;
}


int RToperationsClass::checkSameClockDir(const vec* pt1, const vec* pt2, const vec* pt3, const vec* norm)
{
	double testi, testj, testk;
	double dotprod;
	// normal of trinagle
	testi = (((pt2->y - pt1->y)*(pt3->z - pt1->z)) - ((pt3->y - pt1->y)*(pt2->z - pt1->z)));
	testj = (((pt2->z - pt1->z)*(pt3->x - pt1->x)) - ((pt3->z - pt1->z)*(pt2->x - pt1->x)));
	testk = (((pt2->x - pt1->x)*(pt3->y - pt1->y)) - ((pt3->x - pt1->x)*(pt2->y - pt1->y)));

	// Dot product with triangle normal
	dotprod = testi*norm->x + testj*norm->y + testk*norm->z;

	//answer
	if(dotprod < 0)
	{
		return DIFF_CLOCKNESS;
	}
	else
	{
		return SAME_CLOCKNESS;
	}
}



int RToperationsClass::findIntersectLineWithQuad(const vec* pt1, const vec* pt2, const vec* pt3, const vec* pt4, vec* linept1, vec* linept2, vec* pt_int, vec* norm, double* t)
{
	//NB a quad is formed out of 2 tris

	vec intersectionPointTriA;
	vec intersectionPointTriB;
	vec intersectionPoint;
	vec intersectionPointNormalTriA;
	vec intersectionPointNormalTriB;
	vec intersectionPointNormal;


	int triAIntersectionFound = FALSE;
	int triBIntersectionFound = FALSE;
	int quadIntersectionFound = FALSE;

	double triAtInAndOut;
	double triBtInAndOut;

	if(this->findIntersectLineWithTri(pt1, pt2, pt3, linept1, linept2, &intersectionPointTriA, &intersectionPointNormalTriA, &triAtInAndOut))
	{
		triAIntersectionFound = TRUE;
	}

	if(this->findIntersectLineWithTri(pt1, pt3, pt4, linept1, linept2, &intersectionPointTriB, &intersectionPointNormalTriB, &triBtInAndOut))
	{
		triBIntersectionFound = TRUE;
	}

	double tInAndOut;
	if(triAIntersectionFound && triBIntersectionFound)
	{
		quadIntersectionFound = TRUE;
		SHAREDvector.copyVectorRT(&intersectionPointNormal, &intersectionPointNormalTriA);
		SHAREDvector.copyVectorRT(&intersectionPoint, &intersectionPointTriA);
		tInAndOut = triAtInAndOut;
	}
	else if(triAIntersectionFound)
	{
		quadIntersectionFound = TRUE;
		SHAREDvector.copyVectorRT(&intersectionPointNormal, &intersectionPointNormalTriA);
		SHAREDvector.copyVectorRT(&intersectionPoint, &intersectionPointTriA);
		tInAndOut = triAtInAndOut;
	}
	else if(triBIntersectionFound)
	{
		quadIntersectionFound = TRUE;
		SHAREDvector.copyVectorRT(&intersectionPointNormal, &intersectionPointNormalTriB);
		SHAREDvector.copyVectorRT(&intersectionPoint, &intersectionPointTriB);
		tInAndOut = triBtInAndOut;
	}
	else
	{
		quadIntersectionFound = FALSE;
		tInAndOut = -1;
	}

	*t = tInAndOut;
	SHAREDvector.copyVectorRT(norm, &intersectionPointNormal);
	SHAREDvector.copyVectorRT(pt_int, &intersectionPoint);

	return 1;
}





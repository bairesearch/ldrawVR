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
 * File Name: SHAREDvector.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/

//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "SHAREDvector.hpp"



void SHAREDvectorClass::initialiseVector(vec* vect)
{
	vect->x = 0.0;
	vect->y = 0.0;
	vect->z = 0.0;
}

void SHAREDvectorClass::calculateNormalOfTri(vec* pt1, vec* pt2, vec* pt3, vec* normal)
{
	vec vec1;
	vec vec2;
	this->subtractVectorsRT(pt2, pt1 , &vec1);
	this->subtractVectorsRT(pt3, pt1, &vec2);
	this->calculateNormal(&vec1, &vec2, normal);
}

void SHAREDvectorClass::calculateRotationVectorFromDeformationMatrix(mat* deformationMatrix, vec* rotationVector)
{
	//http://planning.cs.uiuc.edu/node103.html
	rotationVector->x = atan2(deformationMatrix->a.y, deformationMatrix->a.x);
	rotationVector->y = atan2(deformationMatrix->a.z, sqrt(pow(deformationMatrix->b.z, 2.0) + pow(deformationMatrix->c.z, 2.0)));
	rotationVector->z = atan2(deformationMatrix->b.z, deformationMatrix->c.z);
}


//from; http://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
void SHAREDvectorClass::generateLookAtRotationMatrix(vec* at, vec* eye, vec* up, mat* rotationMatrix)
{
	/*
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)
	*/

	vec zaxis;
	this->subtractVectorsRT(at, eye, &zaxis);
	this->normaliseVector(&zaxis);

	vec xaxis;
	this->crossProduct(up, &zaxis, &xaxis);
	this->normaliseVector(&xaxis);

	vec yaxis;
	this->crossProduct(&zaxis, &xaxis, &yaxis);

	this->makeMatrix(&xaxis, &yaxis, &zaxis, rotationMatrix);

}

void SHAREDvectorClass::transposeMatrix(mat* matx)
{
	double tmp;

	tmp = matx->a.y;
	matx->a.y = matx->b.x;
	matx->b.x = tmp;

	tmp = matx->a.z;
	matx->a.z = matx->c.x;
	matx->c.x = tmp;

	tmp = matx->b.z;
	matx->b.z = matx->c.y;
	matx->c.y = tmp;
}





double SHAREDvectorClass::calculateInteriorAngleOfAPolygonVertex(vec* pt1Centre, vec* pt2, vec* pt3)
{
	vec side1;
	vec side2;
	this->subtractVectorsRT(pt1Centre, pt2, &side1);
	this->subtractVectorsRT(pt1Centre, pt3, &side2);
	//this->subtractVectorsRT(pt3, pt1Centre, &side2);
	double interiorAngle = acos(this->dotProduct(&side1, &side2) / (this->findMagnitudeOfVector(&side1)* this->findMagnitudeOfVector(&side2)));
	return interiorAngle;

}


double SHAREDvectorClass::calculateAreaOfTriangle3D(const vec* pt1, const vec* pt2, const vec* pt3)
{
	mat matrix;

	matrix.a.x = pt1->x;
	matrix.b.x = pt1->y;
	matrix.c.x = pt1->z;
	matrix.a.y = pt2->x;
	matrix.b.y = pt2->y;
	matrix.c.y = pt2->z;
	matrix.a.z = pt3->x;
	matrix.b.z = pt3->y;
	matrix.c.z = pt3->z;

	return this->absDouble2(0.5* this->calculateDeterminant3By3(&matrix));
}


double SHAREDvectorClass::calculateAreaOfTriangle(const vec* pt1, const vec* pt2, const vec* pt3)
{
	mat matrix;

	matrix.a.x = pt1->x;
	matrix.b.x = pt1->y;
	matrix.c.x = 1.0;
	matrix.a.y = pt2->x;
	matrix.b.y = pt2->y;
	matrix.c.y = 1.0;
	matrix.a.z = pt3->x;
	matrix.b.z = pt3->y;
	matrix.c.z = 1.0;

	return this->absDouble2(0.5* this->calculateDeterminant3By3(&matrix));
}


double SHAREDvectorClass::calculateDeterminant3By3(const mat* matrix)
{
	double a = matrix->a.x;
	double b = matrix->b.x;
	double c = matrix->c.x;
	double d = matrix->a.y;
	double e = matrix->b.y;
	double f = matrix->c.y;
	double g = matrix->a.z;
	double h = matrix->b.z;
	double i = matrix->c.z;

	return (a*((e*i)-(f*h)) - b*((d*i)-(f*g)) + c*((d*h)-(e*g)));
}

double SHAREDvectorClass::absDouble2(const double val)
{
	if(val < 0.0)
	{
		return (-val);
	}
	else
	{
		return val;
	}
}


void SHAREDvectorClass::calculateNormal(vec* pt1, vec* pt2, vec* normal)
{
	this->crossProduct(pt1, pt2, normal);
}

void SHAREDvectorClass::calculateMidDiffBetweenTwoPoints(vec* pt1, vec* pt2, vec* midDiff)
{
	midDiff->x = (pt2->x - pt1->x)/2.0;
	midDiff->y = (pt2->y - pt1->y)/2.0;
	midDiff->z = (pt2->z - pt1->z)/2.0;
}

void SHAREDvectorClass::calculateMidPointBetweenTwoPoints(vec* pt1, vec* pt2, vec* midPoint)
{
	vec midDiff;
	this->calculateMidDiffBetweenTwoPoints(pt1, pt2, &midDiff);

	midPoint->x = pt1->x + midDiff.x;
	midPoint->y = pt1->y + midDiff.y;
	midPoint->z = pt1->z + midDiff.z;
}




void SHAREDvectorClass::createRotationMatrix2D(mat* matrix, double rotation)
{
	matrix->a.x = cos(rotation);
	matrix->a.y = sin(rotation);
	matrix->a.z = 0.0;
	matrix->b.x = -sin(rotation);
	matrix->b.y = cos(rotation);
	matrix->b.z = 0.0;
	matrix->c.x = 0.0;
	matrix->c.y = 0.0;
	matrix->c.z = 1.0;
}

void SHAREDvectorClass::createXAxisShearMatrix2D(mat* matrix, double shear)
{
	matrix->a.x = 1.0;
	matrix->a.y = 0.0;
	matrix->a.z = 0.0;
	matrix->b.x = shear;
	matrix->b.y = 1.0;
	matrix->b.z = 0.0;
	matrix->c.x = 0.0;
	matrix->c.y = 0.0;
	matrix->c.z = 1.0;
}

void SHAREDvectorClass::createYAxisScaleMatrix2D(mat* matrix, double scaleFactor)
{
	matrix->a.x = 1.0;
	matrix->a.y = 0.0;
	matrix->a.z = 0.0;
	matrix->b.x = 0.0;
	matrix->b.y = scaleFactor;
	matrix->b.z = 0.0;
	matrix->c.x = 0.0;
	matrix->c.y = 0.0;
	matrix->c.z = 1.0;

}




double SHAREDvectorClass::calculateAngleBetweenVectors2D(const vec* vect1, const vec* vect2)
{
	double angleBetweenVectors2D;
	//tan angleBetweenVectors2D =  (y2-y1) / (x2 - x1)

	if(SHAREDvars.compareDoubles(vect2->x, vect1->x))
	{
		angleBetweenVectors2D = 0.0;
	}
	else
	{
		angleBetweenVectors2D =  atan((vect2->y - vect1->y) / (vect2->x - vect1->x));
	}

	return angleBetweenVectors2D;

}

double SHAREDvectorClass::calculateAngleOfVector3D(const vec* vect1, const int axis)
{
	double angle;

	if(axis == AXIS_X)
	{
		if(SHAREDvars.compareDoubles(vect1->z, 0.0))
		{
			angle = 0.0;
		}
		else
		{
			angle = atan(vect1->x / vect1->z);
		}
	}
	else if(axis == AXIS_Y)
	{
		if(SHAREDvars.compareDoubles(vect1->z, 0.0))
		{
			angle = 0.0;
		}
		else
		{
			angle =  atan(vect1->y / vect1->z);
		}
	}
	else if(axis == AXIS_Z)
	{
		if(SHAREDvars.compareDoubles(vect1->x, 0.0))
		{
			angle = 0.0;
		}
		else
		{
			angle = atan(vect1->z / vect1->x);
		}
	}
	else
	{
		exit(0);
	}

	return angle;

}




void SHAREDvectorClass::createScaleMatrix2D(mat* matrix, double scaleFactor)
{
	matrix->a.x = scaleFactor;
	matrix->a.y = 0.0;
	matrix->a.z = 0.0;
	matrix->b.x = 0.0;
	matrix->b.y = scaleFactor;
	matrix->b.z = 0.0;
	matrix->c.x = 0.0;
	matrix->c.y = 0.0;
	matrix->c.z = 1.0;

}

void SHAREDvectorClass::multiplyVectorByMatrix(vec* vecNew, vec* vecToMultiply, mat* matrix)
{
	vecNew->x = matrix->a.x*vecToMultiply->x + matrix->b.x*vecToMultiply->y +  matrix->c.x*vecToMultiply->z;
	vecNew->y = matrix->a.y*vecToMultiply->x + matrix->b.y*vecToMultiply->y +  matrix->c.y*vecToMultiply->z;
	vecNew->z = matrix->a.z*vecToMultiply->x + matrix->b.z*vecToMultiply->y +  matrix->c.z*vecToMultiply->z;
}






double SHAREDvectorClass::findMagnitudeOfVector(const vec* vect1)
{
	return sqrt(pow(vect1->x, 2) + pow(vect1->y,2) + pow(vect1->z,2));
}



void SHAREDvectorClass::normaliseVector(vec* vect1)
{
	double magnitude = this->findMagnitudeOfVector(vect1);
#ifdef OR_DO_NOT_CHECK_FOR_ZERO_DIVISION_DURING_POINT_NORMAL_CALC
	vect1->x = (vect1->x)/magnitude;
	vect1->y = (vect1->y)/magnitude;
	vect1->z = (vect1->z)/magnitude;
#else
	if(magnitude > DOUBLE_MIN_PRECISION)
	{
		vect1->x = (vect1->x)/magnitude;
		vect1->y = (vect1->y)/magnitude;
		vect1->z = (vect1->z)/magnitude;
	}
	else
	{
		vect1->x = 0.0;
		vect1->y = 0.0;
		vect1->z = 0.0;
	}
#endif
	return;
}

void SHAREDvectorClass::normaliseVectorRT(vec* vect1, vec* vect)
{
	double magnitude = this->findMagnitudeOfVector(vect1);
	vect->x = (vect1->x)/magnitude;
	vect->y = (vect1->y)/magnitude;
	vect->z = (vect1->z)/magnitude;
	return;
}


void SHAREDvectorClass::addVectorsRT(const vec* vect1, const vec* vect2, vec* vect)
{
	vect->x = vect1->x + vect2->x;
	vect->y = vect1->y + vect2->y;
	vect->z = vect1->z + vect2->z;
}

void SHAREDvectorClass::subtractVectorsRT(const vec* vect1, const vec* vect2, vec* vect)
{
	vect->x = vect1->x - vect2->x;
	vect->y = vect1->y - vect2->y;
	vect->z = vect1->z - vect2->z;
}

void SHAREDvectorClass::negativeVector(vec* vect1, vec* vect)
{
	vect->x = -(vect1->x);
	vect->y = -(vect1->y);
	vect->z = -(vect1->z);
}

double SHAREDvectorClass::dotProduct(const vec* vect1, const vec* vect2)
{
	double scalar = vect1->x*vect2->x + vect1->y*vect2->y + vect1->z*vect2->z;
	return scalar;
}

void SHAREDvectorClass::multiplyVectorByScalarRT(const vec* vect1, const double multiplyer, vec* vect)
{
	vect->x = vect1->x*multiplyer;
	vect->y = vect1->y*multiplyer;
	vect->z = vect1->z*multiplyer;
}

void SHAREDvectorClass::divideVectorByScalarRT(const vec* vect1, const double divisor, vec* vect)
{
	vect->x = vect1->x/divisor;
	vect->y = vect1->y/divisor;
	vect->z = vect1->z/divisor;
}





void SHAREDvectorClass::makeMatrix(vec* vect1, vec* vect2, vec* vect3, mat* matx)
{
	matx->a = *(vect1);
	matx->b = *(vect2);
	matx->c = *(vect3);
}

void SHAREDvectorClass::multMatrixByVector(mat* matx, vec* vect1, vec* vect)
{
	vect->x = matx->a.x* vect1->x + matx->b.x* vect1->y + matx->c.x* vect1->z;
	vect->y = matx->a.y* vect1->x + matx->b.y* vect1->y + matx->c.y* vect1->z;
	vect->z = matx->a.z* vect1->x + matx->b.z* vect1->y + matx->c.z* vect1->z;
}

void SHAREDvectorClass::crossProduct(vec* vect1, vec* vect2, vec* vect)
{
	vect->x = (vect1->y)*(vect2->z) - (vect2->y)*(vect1->z);
	vect->y = -((vect1->x)*(vect2->z) - (vect2->x)*(vect1->z));
	vect->z = (vect1->x)*(vect2->y) - (vect2->x)*(vect1->y);
}

void SHAREDvectorClass::createVector(vec* vec)
{
	vec->x = 0;
	vec->y = 0;
	vec->z = 0;
}

	/*method is not used*/
void SHAREDvectorClass::createIdentityMatrixRT(mat* matx)
{
	matx->a.x = ONE;
	matx->b.x = ZERO;
	matx->c.x = ZERO;
	matx->a.y = ZERO;
	matx->b.y = ONE;
	matx->c.y = ZERO;
	matx->a.z = ZERO;
	matx->b.z = ZERO;
	matx->c.z = ONE;
}

void SHAREDvectorClass::copyVectorRT(vec* vecNew, const vec* vecToCopy)
{

	vecNew->x = vecToCopy->x;
	vecNew->y = vecToCopy->y;
	vecNew->z = vecToCopy->z;
}




bool SHAREDvectorClass::compareVectorsArbitraryError(const vec* vecA, const vec* vecB, const double error)
{
	bool result = true;
	if(!SHAREDvars.compareDoublesArbitraryError(vecA->x, vecB->x, error))
	{
		result = false;
	}
	if(!SHAREDvars.compareDoublesArbitraryError(vecA->y, vecB->y, error))
	{
		result = false;
	}
	if(!SHAREDvars.compareDoublesArbitraryError(vecA->z, vecB->z, error))
	{
		result = false;
	}
	return result;
}




bool SHAREDvectorClass::compareVectors(const vec* vecA, const vec* vecB)
{
	bool result = true;
	if(!SHAREDvars.compareDoubles(vecA->x, vecB->x))
	{
		result = false;
	}
	if(!SHAREDvars.compareDoubles(vecA->y, vecB->y))
	{
		result = false;
	}
	if(!SHAREDvars.compareDoubles(vecA->z, vecB->z))
	{
		result = false;
	}
	return result;
}

bool SHAREDvectorClass::compareMatricies(const mat* matA, const mat* matB)
{
	bool result = true;
	if(!this->compareVectors(&(matA->a), &(matB->a)))
	{
		result = false;
	}
	if(!this->compareVectors(&(matA->b), &(matB->b)))
	{
		result = false;
	}
	if(!this->compareVectors(&(matA->c), &(matB->c)))
	{
		result = false;
	}
	return result;
}

void SHAREDvectorClass::copyVectors(vec* vecNew, const vec* vecToCopy)
{

	vecNew->x = vecToCopy->x;
	vecNew->y = vecToCopy->y;
	vecNew->z = vecToCopy->z;
}

void SHAREDvectorClass::copyMatricies(mat* matNew, const mat* matToCopy)
{
	this->copyVectors(&(matNew->a), &(matToCopy->a));
	this->copyVectors(&(matNew->b), &(matToCopy->b));
	this->copyVectors(&(matNew->c), &(matToCopy->c));
}

void SHAREDvectorClass::multiplyMatricies(mat* matNew, mat* mat1, mat* mat2)
{
	matNew->a.x = mat1->a.x*mat2->a.x + mat1->b.x*mat2->a.y +  mat1->c.x*mat2->a.z;
	matNew->b.x = mat1->a.x*mat2->b.x + mat1->b.x*mat2->b.y +  mat1->c.x*mat2->b.z;
	matNew->c.x = mat1->a.x*mat2->c.x + mat1->b.x*mat2->c.y +  mat1->c.x*mat2->c.z;

	matNew->a.y = mat1->a.y*mat2->a.x + mat1->b.y*mat2->a.y +  mat1->c.y*mat2->a.z;
	matNew->b.y = mat1->a.y*mat2->b.x + mat1->b.y*mat2->b.y +  mat1->c.y*mat2->b.z;
	matNew->c.y = mat1->a.y*mat2->c.x + mat1->b.y*mat2->c.y +  mat1->c.y*mat2->c.z;

	matNew->a.z = mat1->a.z*mat2->a.x + mat1->b.z*mat2->a.y +  mat1->c.z*mat2->a.z;
	matNew->b.z = mat1->a.z*mat2->b.x + mat1->b.z*mat2->b.y +  mat1->c.z*mat2->b.z;
	matNew->c.z = mat1->a.z*mat2->c.x + mat1->b.z*mat2->c.y +  mat1->c.z*mat2->c.z;
}


void SHAREDvectorClass::subtractVectors(vec* vecNew, vec* a, vec* b)
{

	vecNew->x = a->x - b->x;
	vecNew->y = a->y - b->y;
	vecNew->z = a->z - b->z;
}

void SHAREDvectorClass::addVectors(vec* vecNew, vec* a, vec* b)
{

	vecNew->x = a->x + b->x;
	vecNew->y = a->y + b->y;
	vecNew->z = a->z + b->z;
}


void SHAREDvectorClass::multiplyVectorByScalar(vec* vec, double scalar)
{
	vec->x = vec->x* scalar;
	vec->y = vec->y* scalar;
	vec->z = vec->z* scalar;
}


double SHAREDvectorClass::calculateTheDistanceBetweenTwoPoints(const vec* positionOfUnit1, const vec* positionOfUnit2)
{
	return this->calculateTheDistanceBetweenTwoPoints(positionOfUnit1->x, positionOfUnit2->x, positionOfUnit1->y, positionOfUnit2->y, positionOfUnit1->z, positionOfUnit2->z);
}

double SHAREDvectorClass::calculateTheDistanceBetweenTwoPointsXYOnly(const vec* positionOfUnit1, const vec* positionOfUnit2)
{
	return this->calculateTheDistanceBetweenTwoPoints(positionOfUnit1->x, positionOfUnit2->x, positionOfUnit1->y, positionOfUnit2->y, 0, 0);
}

double SHAREDvectorClass::calculateTheDistanceBetweenTwoPoints(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2, const double positionZOfUnit1, const double positionZOfUnit2)
{
	double xDistanceBetweenTheTwoUnits = positionXOfUnit1 - positionXOfUnit2; //fabs((positionXOfUnit1) - (positionXOfUnit2));
	double yDistanceBetweenTheTwoUnits = positionYOfUnit1 - positionYOfUnit2; //fabs((positionYOfUnit1) - (positionYOfUnit2));
	double zDistanceBetweenTheTwoUnits = positionZOfUnit1 - positionZOfUnit2; //fabs((positionZOfUnit1) - (positionZOfUnit2));
	double distanceBetweenTheTwoUnits = sqrt(pow(xDistanceBetweenTheTwoUnits, 2) + pow(yDistanceBetweenTheTwoUnits, 2) + pow(zDistanceBetweenTheTwoUnits, 2));

	return distanceBetweenTheTwoUnits;
}

double SHAREDvectorClass::calculateTheDistanceBetweenTwoPoints2D(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2)
{
	double xDistanceBetweenTheTwoUnits = positionXOfUnit1 - positionXOfUnit2; //fabs((positionXOfUnit1) - (positionXOfUnit2));
	double yDistanceBetweenTheTwoUnits = positionYOfUnit1 - positionYOfUnit2; //fabs((positionYOfUnit1) - (positionYOfUnit2));
	double distanceBetweenTheTwoUnits = sqrt(pow(xDistanceBetweenTheTwoUnits, 2) + pow(yDistanceBetweenTheTwoUnits, 2));

	return distanceBetweenTheTwoUnits;
}









void SHAREDvectorClass::createRotationMatrixX(mat* matrix, double rotation)
{
	matrix->b.y = cos(rotation);
	matrix->b.z = -sin(rotation);
	matrix->c.y = sin(rotation);
	matrix->c.z = cos(rotation);
}

void SHAREDvectorClass::createRotationMatrixY(mat* matrix, double rotation)
{
	matrix->a.x = cos(rotation);
	matrix->a.z = sin(rotation);
	matrix->c.x = -sin(rotation);
	matrix->c.z = cos(rotation);
}

void SHAREDvectorClass::createRotationMatrixZ(mat* matrix, double rotation)
{
	matrix->a.x = cos(rotation);
	matrix->a.y = -sin(rotation);
	matrix->b.x = sin(rotation);
	matrix->b.y = cos(rotation);
}

double SHAREDvectorClass::convertDegreesToRadian(const double degrees)
{
	return (degrees/180*PI);
}

double SHAREDvectorClass::convertRadianToDegrees(const double radian)
{
	return (radian*180/PI);
}


void SHAREDvectorClass::createIdentityMatrix(mat* matrix)
{
	matrix->a.x = 1;
	matrix->b.x = 0;
	matrix->c.x = 0;
	matrix->a.y = 0;
	matrix->b.y = 1;
	matrix->c.y = 0;
	matrix->a.z = 0;
	matrix->b.z = 0;
	matrix->c.z = 1;
}

void SHAREDvectorClass::copyMatrixTwoIntoMatrixOne(mat* mat1, mat* mat2)
{
	mat1->a.x = mat2->a.x;
	mat1->b.x = mat2->b.x;
	mat1->c.x = mat2->c.x;
	mat1->a.y = mat2->a.y;
	mat1->b.y = mat2->b.y;
	mat1->c.y = mat2->c.y;
	mat1->a.z = mat2->a.z;
	mat1->b.z = mat2->b.z;
	mat1->c.z = mat2->c.z;
}


void SHAREDvectorClass::scaleMatrix(mat* matrix, double scaleFactor)
{
	matrix->a.x = matrix->a.x* scaleFactor;
	matrix->b.y = matrix->b.y* scaleFactor;
	matrix->c.z = matrix->c.z* scaleFactor;
}


void SHAREDvectorClass::createRotatationMatrix(mat* matrix, const int rotationAxis, double rotationRadians)
{
	if(rotationAxis == AXIS_X)
	{
		this->createRotationMatrixX(matrix, rotationRadians);
	}
	else if(rotationAxis == AXIS_Y)
	{
		this->createRotationMatrixY(matrix, rotationRadians);
	}
	else if(rotationAxis == AXIS_Z)
	{
		this->createRotationMatrixZ(matrix, rotationRadians);
	}
}




























//void find2DintersectionPoint(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double* intersectionX, double* intersectionY, bool* interceptionFound, bool* xinterceptionPointFound, bool* yinterceptionPointFound)	//NB an interception may be a line and not a point

//NB uses 0 is the centre of the quad drawn by the 4 points. a, b, c, d.
void SHAREDvectorClass::find2DintersectionPoint(double ax, double ay, double bx, double by, const double cx, const double cy, const double dx, const double dy, double* intersectionX, double* intersectionY, bool* interceptionFound, bool* interceptionPointFound)	//NB an interception may be a line and not a point
{
	//try for analytical solution

	double mEdge = 0.0;
	double iEdge = 0.0;
	double mOptionalLine = 0.0;
	double iOptionalLine = 0.0;
	bool edgeGradientInfinityDetected = false;
	bool optionalLineGradientInfinityDetected = false;
	bool edgeNoLineDetected = false;
	bool optionalLineNoLineDetected = false;

	if(!this->twoPointsAreTheSame2D(ax, ay, cx, cy))
	{
		if(this->solve2DlineEquationWithTwoPoints(ax, ay, cx, cy, &mEdge, &iEdge))
		{//m != infinity

		}
		else
		{//m = infinity detected, y=iEdge
			edgeGradientInfinityDetected = true;
		}
	}
	else
	{
		edgeNoLineDetected = true;
	}

	if(!this->twoPointsAreTheSame2D(bx, by, dx, dy))
	{
		if(this->solve2DlineEquationWithTwoPoints(bx, by, dx, dy, &mOptionalLine, &iOptionalLine))
		{//m != infinity

		}
		else
		{//m = infinity detected, y=iOptionalLine
			optionalLineGradientInfinityDetected = true;
		}
	}
	else
	{
		optionalLineNoLineDetected = true;
	}

	if(!edgeGradientInfinityDetected && !optionalLineGradientInfinityDetected && !edgeNoLineDetected && !optionalLineNoLineDetected)
	{
		if(this->find2DintersectionPointOfTwoLines(mEdge, iEdge, mOptionalLine, iOptionalLine, intersectionX, intersectionY))
		{
			*interceptionPointFound = true;
		}
		else
		{//edge and optional lines are parallel as their gradients are equal, therefore no intersection occurs unless their y intercepts are equal also

		#ifdef USE_RT
			if(this->compareDoublesRelaxed(iEdge, iOptionalLine))
		#else
			if(SHAREDvars.compareDoubles(iEdge, iOptionalLine))
		#endif
			{
				//return from function that there is no precise interception point on this plane (Eg xy plane).
				*interceptionFound = true;
			}
		}
	}
	else if(!edgeNoLineDetected && !optionalLineNoLineDetected)
	{

		if(edgeGradientInfinityDetected && !optionalLineGradientInfinityDetected)
		{
			*interceptionPointFound = true;

			*intersectionX  = ax;
			*intersectionY = mOptionalLine*ax + iOptionalLine;	//y = mx + i
		}
		else if(!edgeGradientInfinityDetected && optionalLineGradientInfinityDetected)
		{
			*interceptionPointFound = true;

			*intersectionX = bx;
			*intersectionY = mEdge*bx + iEdge;	//y = mx + i
		}
		else if(edgeGradientInfinityDetected && optionalLineGradientInfinityDetected)
		{//edge and optional lines are parallel as their gradients are equal, therefore no intersection occurs unless their x intercepts are equal also
		#ifdef USE_RT
			if(this->compareDoublesRelaxed(ax, bx))
		#else
			if(SHAREDvars.compareDoubles(ax, bx))
		#endif
			{
				//return from function that there is no precise interception point on this plane (Eg xy plane).
				*interceptionFound = true;

				/*removed by RBB oct 08*/
			#ifdef USE_ORIGINAL_OLC_CODE
				*intersectionX = ax;
				*intersectionY = ay;
			#endif
			}
		}

	}
	else
	{
		if((edgeNoLineDetected) && (optionalLineNoLineDetected))
		{
			//in this case all 4 points must be the same for an intersection to occur a, b, c, d
		#ifdef USE_RT
			if(this->twoPointsAreTheSame2Drelaxed(ax, ay, bx, by))
		#else
			if(this->twoPointsAreTheSame2D(ax, ay, bx, by))
		#endif
			{
				*interceptionPointFound = true;
				*intersectionX = ax;
				*intersectionY = ay;
			}
		}
		else if(edgeNoLineDetected)
		{
			if(!optionalLineGradientInfinityDetected)
			{
			#ifdef USE_RT
				if(this->determineIfPointLiesOnAKnownLine2Drelaxed(mOptionalLine, iOptionalLine, ax, ay))
			#else
				if(this->determineIfPointLiesOnAKnownLine2D(mOptionalLine, iOptionalLine, ax, ay))
			#endif
				{
					*interceptionPointFound = true;
					*intersectionX = ax;
					*intersectionY = ay;
				}
			}
			else
			{
			#ifdef USE_RT
				if(this->compareDoublesRelaxed(ax, bx))
			#else
				if(SHAREDvars.compareDoubles(ax, bx))
			#endif
				{
					*interceptionPointFound = true;
					*intersectionX = ax;
					*intersectionY = ay;
				}
			}
		}
		else if(optionalLineNoLineDetected)
		{
			if(!edgeGradientInfinityDetected)
			{
			#ifdef USE_RT
				if(this->determineIfPointLiesOnAKnownLine2Drelaxed(mEdge, iEdge, bx, by))
			#else
				if(this->determineIfPointLiesOnAKnownLine2D(mEdge, iEdge, bx, by))
			#endif
				{
					*interceptionPointFound = true;
					*intersectionX = bx;
					*intersectionY = by;
				}
			}
			else
			{
			#ifdef USE_RT
				if(this->compareDoublesRelaxed(bx, ax))
			#else
				if(SHAREDvars.compareDoubles(bx, ax))
			#endif
				{
					*interceptionPointFound = true;
					*intersectionX = bx;
					*intersectionY = by;
				}
			}
		}
	}

	if(*interceptionPointFound)
	{
		*interceptionFound = true;
	}
}




bool SHAREDvectorClass::find2DintersectionPointOfTwoLines(double m1, double i1, const double m2, const double i2, double* xIntersection, double* yIntersection)
{
	bool result;

	/*
	/*theory
	m1, m2, i1, i2 are known, find x, and y, in y = m1x + i1, and y = m2x + i2

	y = m1x + i1
	y = m2x + i2
	---

	0 = (m1 - m2)x + (i1 - i2)
	x = -(i1 - i2)/(m1-m2)
	y = m1*x + i1
	*/

	double mNumerator = - (i1-i2);
	double mDenomenator = (m1-m2);

	if(mDenomenator == 0)
	{
		*xIntersection = 0;	//no intersection found
		result = false;
	}
	else
	{
		*xIntersection = mNumerator/mDenomenator;
		result = true;
	}

	*yIntersection = m1*(*xIntersection) + i1;

	return result;
}


double SHAREDvectorClass::calcDistanceBetweenTwoPoints2D(const double x1, const double y1, const double x2, const double y2)
{
	double xDistanceBetweenTheTwoPoints = x1 -  x2;
	double yDistanceBetweenTheTwoPoints = y1 -  y2;
	return sqrt(pow(xDistanceBetweenTheTwoPoints, 2) + pow(yDistanceBetweenTheTwoPoints, 2));
}


bool SHAREDvectorClass::solve2DlineEquationWithTwoPoints(double x1, double y1, const double x2, const double y2, double* m, double* i)
{
	bool result;

	/*theory
	find m and i, in y = mx + i

	y1 = mx1 + i
	y2 = mx2 + i
	--
	y1 - y2 = mx1 - mx2
	m = (y1-y2)/(x1-x2)
	---
	i = y1 - ((y1-y2)/(m1-x2)* x1)

	*/

	double mNumerator = (y1-y2);
	double mDenomenator = (x1-x2);
	if(mDenomenator == 0)
	{
		#define VERY_LARGE_DOUBLE 999999999999.0;
		*m = VERY_LARGE_DOUBLE;
		result = false;
	}
	else
	{
		*m = mNumerator/mDenomenator;
		result = true;
	}
	*i = y1 - ((*m)* x1);
	return result;
}

bool SHAREDvectorClass::determineIfPointLiesOnLine2D(double x1, double y1, const double x2, const double y2, const double x3, const double y3)
{
	bool result = false;

	double m, i;
	if(this->solve2DlineEquationWithTwoPoints(x1, y1, x2, y2, &m, &i))
	{//m != infinity
		double RHS = m*(x3) + i;
		if(SHAREDvars.compareDoubles(y3, RHS))
		{
			result = true;
		}
	}
	else
	{//m = infinity detected
		if(SHAREDvars.compareDoubles(x1, x3))
		{
			result = true;
		}
	}

	return result;
}


bool SHAREDvectorClass::determineIfPointLiesOnAKnownLine2D(const double m1, const double i1, const double x3, const double y3)
{//preconditions: m != infinity
	bool result = false;


	double RHS = m1*(x3) + i1;

	if(SHAREDvars.compareDoubles(y3, RHS))
	{
		result = true;
	}

	return result;
}



bool SHAREDvectorClass::twoPointsAreTheSame2D(const double x1, const double y1, const double x2, const double y2)
{
	bool result;

	if(SHAREDvars.compareDoubles(this->calcDistanceBetweenTwoPoints2D(x1, y1, x2, y2), 0.0))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


#ifdef USE_RT


bool SHAREDvectorClass::compareDoublesRelaxed(const double a, const double b)	//required for raytracing operations
{
	bool result;
	if((a < (b+RELAXED_DOUBLE_MIN_PRECISION)) && (a > (b-RELAXED_DOUBLE_MIN_PRECISION)))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


bool SHAREDvectorClass::determineIfPointLiesOnAKnownLine2Drelaxed(const double m1, const double i1, const double x3, const double y3)
{//preconditions: m != infinity
	bool result = false;


	double RHS = m1*(x3) + i1;

	if(this->compareDoublesRelaxed(y3, RHS))
	{
		result = true;
	}

	return result;
}

bool SHAREDvectorClass::twoPointsAreTheSame2Drelaxed(const double x1, const double y1, const double x2, const double y2)
{
	bool result;

	if(this->compareDoublesRelaxed(this->calcDistanceBetweenTwoPoints2D(x1, y1, x2, y2), 0.0))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}

#endif



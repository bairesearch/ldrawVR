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
 * File Name: SHAREDvector.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_SHARED_VECTOR
#define HEADER_SHARED_VECTOR

#include "SHAREDvars.hpp"
#include "SHAREDglobalDefs.hpp"		//needed so as to know if USE_RT is defined

#ifdef USE_RT
#define RELAXED_DOUBLE_MIN_PRECISION 0.03
#endif

#define PI (3.14159265)
#define ZERO 0.0
#define ONE 1.0
#define NOT_FOUND -1

#define AXIS_X (1)
#define AXIS_Y (2)
#define AXIS_Z (3)

class SHAREDvectorClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void initialiseVector(vec* vect);

	public: void calculateNormalOfTri(vec* pt1, vec* pt2, vec* pt3, vec* normal);

	public: void calculateRotationVectorFromDeformationMatrix(mat* deformationMatrix, vec* rotationVector);
	public: void generateLookAtRotationMatrix(vec* at, vec* eye, vec* up, mat* rotationMatrix);
	public: void transposeMatrix(mat* matx);

	public: double calculateInteriorAngleOfAPolygonVertex(vec* pt1Centre, vec* pt2, vec* pt3);

	private: double calculateAreaOfTriangle3D(const vec* pt1, const vec* pt2, const vec* pt3);
	public: double calculateAreaOfTriangle(const vec* pt1, const vec* pt2, const vec* pt3);
		private: double calculateDeterminant3By3(const mat* matrix);

	private: double absDouble2(const double val);


	public: void createRotationMatrix2D(mat* matrix, double rotation);
	public: void createXAxisShearMatrix2D(mat* matrix, double shear);
	public: double calculateAngleBetweenVectors2D(const vec* vect1, const vec* vect2);
	public: void createScaleMatrix2D(mat* matrix, double scaleFactor);
	public: void createYAxisScaleMatrix2D(mat* matrix, double scaleFactor);

	public: void calculateMidPointBetweenTwoPoints(vec* pt1, vec* pt2, vec* midPoint);
		private: void calculateMidDiffBetweenTwoPoints(vec* pt1, vec* pt2, vec* midDiff);
	public: void calculateNormal(vec* pt1, vec* pt2, vec* normal);

	//from RToperations.h

	public: void subtractVectorsRT(const vec* vect1, const vec* vect2, vec* vect);
	/*vect = vect1 - vect2*/

	public: void addVectorsRT(const vec* vect1, const vec* vect2, vec* vect);
	/*adds 2 vectors together*/

	public: void multiplyVectorByScalarRT(const vec* vect1, const double multiplyer, vec* vect);
	/*multiply a vector by a scalar*/

	public: void divideVectorByScalarRT(const vec* vect1, const double divisor, vec* vect);

	public: void copyVectorRT(vec* vecNew, const vec* vecToCopy);

	public: void normaliseVectorRT(vec* vect1, vec* vect);


	public: void normaliseVector(vec* vect1);
	/*normalises a vector*/

	public: void negativeVector(vec* vect1, vec* vect);
	/*finds the negative of a vector*/

	public: double dotProduct(const vec* vect1, const vec* vect2);
	/*finds the dot product of 2 vectors*/

	public: void crossProduct(vec* vect1, vec* vect2, vec* vect);
	/*vect = vect1 x vect2; finds the cross product of 2 vectors*/

	public: void makeMatrix(vec* vect1, vec* vect2, vec* vect3, mat* matx);
	/*creates a 3x3 matrix from 3 vectors*/

	public: void multMatrixByVector(mat* matx, vec* vect1, vec* vect);
	/*multiplies a matrix by a vector*/

	public: void createVector(vec* vec);

	public: void createIdentityMatrixRT(mat* matx);

	public: double findMagnitudeOfVector(const vec* vect1);








	public: bool compareVectors(const vec* vecA, const vec* vecB);
	public: bool compareVectorsArbitraryError(const vec* vecA, const vec* vecB, const double error);

	public: bool compareMatricies(const mat* matA, const mat* matB);
	public: void copyVectors(vec* vecNew, const vec* vecToCopy);
	public: void copyMatricies(mat* matNew, const mat* matToCopy);
	public: void multiplyVectorByMatrix(vec* vecNew, vec* vecToMultiply, mat* matrix);

	//semi shared
	public: void multiplyMatricies(mat* matNew, mat* mat1, mat* mat2);
	public: void subtractVectors(vec* vecNew, vec* a, vec* b);
	public: void addVectors(vec* vecNew, vec* a, vec* b);
	public: void multiplyVectorByScalar(vec* vec, double scalar);
	public: void createIdentityMatrix(mat* matrix);

	public: double calculateTheDistanceBetweenTwoPointsXYOnly(const vec* positionOfUnit1, const vec* positionOfUnit2);
	public: double calculateTheDistanceBetweenTwoPoints(const vec* positionOfUnit1, const vec* positionOfUnit2);
		public: double calculateTheDistanceBetweenTwoPoints(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2, const double positionZOfUnit1, const double positionZOfUnit2);
		private: double calculateTheDistanceBetweenTwoPoints2D(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2);


	public: void createRotatationMatrix(mat* matrix, const int rotationAxis, double rotationRadians);
	public: void createRotationMatrixX(mat* matrix, double rotation);
	public: void createRotationMatrixY(mat* matrix, double rotation);
	public: void createRotationMatrixZ(mat* matrix, double rotation);
	public: double convertDegreesToRadian(const double degrees);
	public: double convertRadianToDegrees(const double radian);
//void rotateMatrix(mat* matrix, char rotationAxis, double rotationDegrees);
	public: void copyMatrixTwoIntoMatrixOne(mat* mat1, mat* mat2);
	public: void scaleMatrix(mat* matrix, double scaleFactor);

	public: double calculateAngleOfVector3D(const vec* vect1, const int axis);



//From OLCoperations.h

	public: void find2DintersectionPoint(double ax, double ay, double bx, double by, const double cx, const double cy, const double dx, const double dy, double* intersectionX, double* intersectionY, bool* interceptionFound, bool* interceptionPointFound);	//NB an interception may be a line and not a point
		private: double calcDistanceBetweenTwoPoints2D(const double x1, const double y1, const double x2, const double y2);
		private: bool solve2DlineEquationWithTwoPoints(double x1, double y1, const double x2, const double y2, double* m, double* i);
		private: bool determineIfPointLiesOnLine2D(double x1, double y1, const double x2, const double y2, const double x3, const double y3);
		private: bool find2DintersectionPointOfTwoLines(double m1, double i1, const double m2, const double i2, double* xIntersection, double* yIntersection);
		private: bool twoPointsAreTheSame2D(const double x1, const double y1, const double x2, const double y2);
		private: bool determineIfPointLiesOnAKnownLine2D(const double m1, const double i1, const double x3, const double y3);

#ifdef USE_RT
	//required for raytracing operations
	public: bool compareDoublesRelaxed(const double a, const double b);
	private: bool determineIfPointLiesOnAKnownLine2Drelaxed(const double m1, const double i1, const double x3, const double y3);
	private: bool twoPointsAreTheSame2Drelaxed(const double x1, const double y1, const double x2, const double y2);
#endif

};

#endif





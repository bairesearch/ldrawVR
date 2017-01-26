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
 * File Name: RToperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Raytracer Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_RT_OPERATIONS
#define HEADER_RT_OPERATIONS

#include "SHAREDvars.hpp"
#include "SHAREDvector.hpp"
//#include "math.h"

//#define MISS (-1)
#define LINE_INTERSECTION_BOUNDARY_ERROR_ALLOWED (0.01)

	/*4x4 matrix manipulation methods*/

typedef struct { double x; double y; double z; double w;} advancedVec;

typedef struct { advancedVec a; advancedVec b; advancedVec c; advancedVec d;} advancedMat;

class RToperationsClass
{
	private: SHAREDvectorClass SHAREDvector;
	public: void makeAdvancedMatrix(advancedMat* matxAdv, mat* matx, int type);
	public: void multAdvancedMatrix(advancedMat* matxAdv1, advancedMat* matxAdv2, advancedMat* matxAdv);
	private: void createIdentityMatrixAdvanced(advancedMat* matx);
	public: void createInverseTranslationMatrix(double xpos, double ypos, double zpos, advancedMat* matx);
	public: void createTranslationMatrix(double xpos, double ypos, double zpos, advancedMat* matx);
	public: void createScaleMatrix(double width, double length, double height, advancedMat* matx);
	public: void createInverseScaleMatrix(const double width, const double length, const double height, advancedMat* matx);
	public: void createRotationxMatrix(double rotation, advancedMat* matx);
	public: void createInverseRotationxMatrix(double rotation, advancedMat* matx);
	public: void createRotationyMatrix(double rotation, advancedMat* matx);
	public: void createInverseRotationyMatrix(double rotation, advancedMat* matx);
	public: void createRotationzMatrix(double rotation, advancedMat* matx);
	public: void createInverseRotationzMatrix(double rotation, advancedMat* matx);
	private: void invertAdvancedMatrix(advancedMat* matx);
	public: void transposeAdvancedMatrix(advancedMat* matx);
	public: void multAdvancedMatrixByVector(advancedVec* vecAdv1, advancedMat* matxAdv1, advancedVec* vecAdv);
	public: double toRadians(const double degrees);
	public: void toAdvancedVector(vec* vec, int type, advancedVec* vecAdv);
	public: void fromAdvancedVector(advancedVec* vecAdv, vec* vec);
	public: void createAdvancedVector(advancedVec* vec);


	public: double findSmallestValue(const double val1, const double val2, double val3);
	public: double findGreatestValue(const double val1, const double val2, double val3);
	/*finds the smallest/largest value of 3 given scalars*/

	private: int findPositionOfSmallestValueAdvanced(const double* array, const int size);
	private: int findPositionOfGreatestValueAdvanced(const double* array, const int size);

	public: int findIntersectLineWithTri(const vec* pt1, const vec* pt2, const vec* pt3, vec* linept1, vec* linept2, vec* pt_int, vec* norm, double* t);
		private: int checkSameClockDir(const vec* pt1, const vec* pt2, const vec* pt3, const vec* norm);
	public: int findIntersectLineWithLine(const vec* linept1, const vec* linept2, vec* povpt1, vec* povpt2, vec* pt_int, const vec* norm, double* t);
		private: bool findIntersectLineWithLine2D(const vec* povpt1, const vec* povpt2, const vec* linept1, const vec* linept2, vec* pt_int);
	public: int findIntersectLineWithQuad(const vec* pt1, const vec* pt2, const vec* pt3, const vec* pt4, vec* linept1, vec* linept2, vec* pt_int, vec* norm, double* t);



	private: double findSmallestValueAdvanced(double val1, const double val2);
	public: double findSmallestValue(double val1, const double val2);
	private: double findGreatestValueAdvanced(double val1, const double val2);
	public: double findGreatestValue(double val1, const double val2);

	public: int findPositionOfSmallestValueWhichHits(const double* array, const int* hitsArray, const int size);
	public: int findPositionOfGreatestValueWhichHits(const double* array, const int* hitsArray, const int size);
};

#endif

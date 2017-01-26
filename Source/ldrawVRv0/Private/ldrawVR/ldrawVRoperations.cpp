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
* File Name: ldrawVRoperations.cpp
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#include "ldrawVRv0.h"
#include "ldrawVRoperations.h"

#define _USE_MATH_DEFINES	//required for visual C++	//http://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
#include <math.h>

static int LDgridResolution;
static vec LDgridSnapPos;
static double LDgridSnapRotDegrees;

ldrawVRoperations::ldrawVRoperations()
{
}

ldrawVRoperations::~ldrawVRoperations()
{
}

vec ldrawVRoperations::getLDgridSnapPos()
{
	return LDgridSnapPos;
}
void ldrawVRoperations::setLDgridSnapPos(vec LDgridSnapPosNew)
{
	LDgridSnapPos = LDgridSnapPosNew;
}
double ldrawVRoperations::getLDgridSnapRotDegrees()
{
	return LDgridSnapRotDegrees;
}
void ldrawVRoperations::setLDgridSnapRotDegrees(double LDgridSnapRotDegreesNew)
{
	LDgridSnapRotDegrees = LDgridSnapRotDegreesNew;
}
int ldrawVRoperations::getLDgridResolution()
{
	return LDgridResolution;
}
void ldrawVRoperations::setLDgridResolution(int LDgridResolutionNew)
{
	LDgridResolution = LDgridResolutionNew;
	if(LDgridResolution == LDRAW_VR_LD_GRID_SNAP_COARSE)
	{
		LDgridSnapPos.x = LDRAW_VR_LD_GRID_SNAP_COARSE_POS_X;
		LDgridSnapPos.y = LDRAW_VR_LD_GRID_SNAP_COARSE_POS_Y;
		LDgridSnapPos.z = LDRAW_VR_LD_GRID_SNAP_COARSE_POS_Z;
		LDgridSnapRotDegrees = LDRAW_VR_LD_GRID_SNAP_COARSE_ROT_DEGREES;
	}
	else if(LDgridResolution == LDRAW_VR_LD_GRID_SNAP_MEDIUM)
	{
		LDgridSnapPos.x = LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_X;
		LDgridSnapPos.y = LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_Y;
		LDgridSnapPos.z = LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_Z;
		LDgridSnapRotDegrees = LDRAW_VR_LD_GRID_SNAP_MEDIUM_ROT_DEGREES;
	}
	#ifdef LDRAW_VR_LD_GRID_SNAP_FINE_ENABLED
	else if(LDgridResolution == LDRAW_VR_LD_GRID_SNAP_FINE)
	{
		LDgridSnapPos.x = LDRAW_VR_LD_GRID_SNAP_FINE_POS_X;
		LDgridSnapPos.y = LDRAW_VR_LD_GRID_SNAP_FINE_POS_Y;
		LDgridSnapPos.z = LDRAW_VR_LD_GRID_SNAP_FINE_POS_Z;
		LDgridSnapRotDegrees = LDRAW_VR_LD_GRID_SNAP_FINE_ROT_DEGREES;
	}
	#endif
}

void ldrawVRoperations::calculateSnapPosition(FVector* worldPositionUnreal, FRotator* worldRotationUnreal, FVector* worldPositionUnrealSnapped, FRotator* worldRotationUnrealSnapped)
{
	vec translationVectorUnreal = convertFVectorToVector(worldPositionUnreal);
	vec absolutePositionLD;
	convertTranslationVectorUnrealToLD(&absolutePositionLD, &translationVectorUnreal);
	vec absolutePositionLDsnapped;
	absolutePositionLDsnapped.x = floor(absolutePositionLD.x/LDgridSnapPos.x + 0.5) * LDgridSnapPos.x;
	absolutePositionLDsnapped.y = floor(absolutePositionLD.y/LDgridSnapPos.y + 0.5) * LDgridSnapPos.y;
	absolutePositionLDsnapped.z = floor(absolutePositionLD.z/LDgridSnapPos.z + 0.5) * LDgridSnapPos.z;
	vec translationVectorUnrealSnapped;
	convertTranslationVectorLDtoUnreal(&absolutePositionLDsnapped, &translationVectorUnrealSnapped, LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT);
	*worldPositionUnrealSnapped = convertVectorToFVector(&translationVectorUnrealSnapped);

	worldRotationUnrealSnapped->Roll = floor(worldRotationUnreal->Roll/LDgridSnapRotDegrees + 0.5) * LDgridSnapRotDegrees;
	worldRotationUnrealSnapped->Pitch = floor(worldRotationUnreal->Pitch/LDgridSnapRotDegrees + 0.5) * LDgridSnapRotDegrees;
	worldRotationUnrealSnapped->Yaw = floor(worldRotationUnreal->Yaw/LDgridSnapRotDegrees + 0.5) * LDgridSnapRotDegrees;
}









void ldrawVRoperations::convertTransformationMatrixCoordinateSystemUnrealToLD(vec* translationVectorLD, mat* rotationMatrixLD, FVector* translatorUnreal, FRotator* rotatorUnreal)
{
	convertRotationMatrixUnrealToLD(rotationMatrixLD, rotatorUnreal);

	vec translationVectorUnreal = convertFVectorToVector(translatorUnreal);
	convertTranslationVectorUnrealToLD(translationVectorLD, &translationVectorUnreal);

	string translationVectorLDstring = convertDoubleToString(translationVectorLD->x) + " " + convertDoubleToString(translationVectorLD->y) + " " + convertDoubleToString(translationVectorLD->z);
	string rotationMatrixLDstring = convertDoubleToString(rotationMatrixLD->a.x) + " " + convertDoubleToString(rotationMatrixLD->b.x) + " " + convertDoubleToString(rotationMatrixLD->c.x) + " " + convertDoubleToString(rotationMatrixLD->a.y) + " " + convertDoubleToString(rotationMatrixLD->b.y) + " " + convertDoubleToString(rotationMatrixLD->c.y) + " " + convertDoubleToString(rotationMatrixLD->a.z) + " " + convertDoubleToString(rotationMatrixLD->b.z) + " " + convertDoubleToString(rotationMatrixLD->c.z);
	//printMessage("end: translationVectorLD = " + translationVectorLDstring);
	//printMessage("end: rotationMatrixLD = " + rotationMatrixLDstring);
}

void ldrawVRoperations::convertTranslationVectorUnrealToLD(vec* translationVectorLD, vec* translationVectorUnreal)
{
	translationVectorLD->x = -convertTranslationValueUnrealToLD(translationVectorUnreal->x);	//parameters depend on desired coordinate system transformation (this is just an example)
	translationVectorLD->y = -convertTranslationValueUnrealToLD(translationVectorUnreal->z);	//parameters depend on desired coordinate system transformation (this is just an example)
	translationVectorLD->z = convertTranslationValueUnrealToLD(translationVectorUnreal->y);	//parameters depend on desired coordinate system transformation (this is just an example)
}

double ldrawVRoperations::convertTranslationValueUnrealToLD(double translationValueUnreal)
{
	double translationValueLD = translationValueUnreal / LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_SCALING_FACTOR;
	return translationValueLD;
}

void ldrawVRoperations::convertRotationMatrixUnrealToLD(mat* rotationMatrixLD, FRotator* rotatorUnreal)
{
	vec eulerUnreal = convertRotatorToEuler(rotatorUnreal);
	convertEulerToRotationMatrix(-(eulerUnreal.x-M_PI), -(eulerUnreal.z), -(eulerUnreal.y-M_PI), MATH_EULER_YZX, rotationMatrixLD); //parameters depend on desired coordinate system transformation (this is just an example)
	//convertEulerToRotationMatrix(-eulerUnreal.x, -eulerUnreal.z + M_PI, eulerUnreal.y, MATH_EULER_YZX, rotationMatrixLD);	//ORIGINAL (these are valid also) //parameters depend on desired coordinate system transformation (this is just an example)
}

#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
void ldrawVRoperations::convertRotationMatrixUnrealToLDcalibrate(mat* rotationMatrixLD, FRotator* rotatorUnreal)
{
	vec eulerUnreal = convertRotatorToEuler(rotatorUnreal);

	#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT_MODEL1
	//testModelRotatedSmall1.ldr
	mat rotationMatrixLDexpected;
	rotationMatrixLDexpected.a.x = 0.683013;
	rotationMatrixLDexpected.b.x = -0.707107;
	rotationMatrixLDexpected.c.x = -0.183013;
	rotationMatrixLDexpected.a.y = 0.726727;
	rotationMatrixLDexpected.b.y = 0.683013;
	rotationMatrixLDexpected.c.y = 0.0732233;
	rotationMatrixLDexpected.a.z = 0.0732233;
	rotationMatrixLDexpected.b.z = -0.183013;
	rotationMatrixLDexpected.c.z = 0.98038;
	#else
	//testModelRotatedSmall2.ldr
	mat rotationMatrixLDexpected;
	rotationMatrixLDexpected.a.x = 0.683013;
	rotationMatrixLDexpected.b.x = -0.183013;
	rotationMatrixLDexpected.c.x = -0.707107;
	rotationMatrixLDexpected.a.y = 0.592752;
	rotationMatrixLDexpected.b.y = -0.426777;
	rotationMatrixLDexpected.c.y = 0.683013;
	rotationMatrixLDexpected.a.z = -0.426777;
	rotationMatrixLDexpected.b.z = -0.885646;
	rotationMatrixLDexpected.c.z = -0.183013;
	#endif
	/*
	printMessage("eulerUnreal.x = " + convertDoubleToString(eulerUnreal.x));
	printMessage("eulerUnreal.y = " + convertDoubleToString(eulerUnreal.y));
	printMessage("eulerUnreal.z = " + convertDoubleToString(eulerUnreal.z));
	*/

	bool foundIdeal = false;
	for(int order=1; order<=MATH_EULER_NUM_VARIATIONS; order++)
	{
		for(int param1axis=1; param1axis<=AXIS_NUMBER_OF_TYPES; param1axis++)
		{
			for(int param2axis=1; param2axis<=AXIS_NUMBER_OF_TYPES; param2axis++)
			{
				for(int param3axis=1; param3axis<=AXIS_NUMBER_OF_TYPES; param3axis++)
				{
					if((param2axis != param1axis) && (param3axis != param1axis) && (param3axis != param2axis))
					{
						for(int param1axisPhaseOffset=-180; param1axisPhaseOffset<=180; param1axisPhaseOffset=param1axisPhaseOffset+180)
						{
							for(int param2axisPhaseOffset=-180; param2axisPhaseOffset<=180; param2axisPhaseOffset=param2axisPhaseOffset+180)
							{
								for(int param3axisPhaseOffset=-180; param3axisPhaseOffset<=180; param3axisPhaseOffset=param3axisPhaseOffset+180)
								{
									for(int param1axisSign=-1; param1axisSign<=1; param1axisSign=param1axisSign+2)
									{
										for(int param2axisSign=-1; param2axisSign<=1; param2axisSign=param2axisSign+2)
										{
											for(int param3axisSign=-1; param3axisSign<=1; param3axisSign=param3axisSign+2)
											{
												/*OLD:
												double param1axisValue = (getVecValue(&eulerUnreal, param1axis))*double(param1axisSign) + convertDegreesToRadians(double(param1axisPhaseOffset));
												double param2axisValue = (getVecValue(&eulerUnreal, param2axis))*double(param2axisSign) + convertDegreesToRadians(double(param2axisPhaseOffset));
												double param3axisValue = (getVecValue(&eulerUnreal, param3axis))*double(param3axisSign) + convertDegreesToRadians(double(param3axisPhaseOffset));
												*/
												double param1axisValue = (getVecValue(&eulerUnreal, param1axis) + convertDegreesToRadians(double(param1axisPhaseOffset)))*double(param1axisSign);
												double param2axisValue = (getVecValue(&eulerUnreal, param2axis) + convertDegreesToRadians(double(param2axisPhaseOffset)))*double(param2axisSign);
												double param3axisValue = (getVecValue(&eulerUnreal, param3axis) + convertDegreesToRadians(double(param3axisPhaseOffset)))*double(param3axisSign);

												convertEulerToRotationMatrix(param1axisValue, param2axisValue, param3axisValue, order, rotationMatrixLD);	//ORIGINAL //parameters depend on desired coordinate system transformation (this is just an example)
												if(compareMatricies(&rotationMatrixLDexpected, rotationMatrixLD))
												{
													foundIdeal = true;
													printMessage("LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT convertRotationMatrixUnrealToLD{}: found ideal transformation parameters");
													printMessage("order = " + convertIntToString(order));
													printMessage("param1axis = " + convertIntToString(param1axis));
													printMessage("param2axis = " + convertIntToString(param2axis));
													printMessage("param3axis = " + convertIntToString(param3axis));
													printMessage("param1axisPhaseOffset = " + convertIntToString(param1axisPhaseOffset));
													printMessage("param2axisPhaseOffset = " + convertIntToString(param2axisPhaseOffset));
													printMessage("param3axisPhaseOffset = " + convertIntToString(param3axisPhaseOffset));
													printMessage("param1axisSign = " + convertIntToString(param1axisSign));
													printMessage("param2axisSign = " + convertIntToString(param2axisSign));
													printMessage("param3axisSign = " + convertIntToString(param3axisSign));
													printMessage("param1axisValue = " + convertDoubleToString(param1axisValue));
													printMessage("param2axisValue = " + convertDoubleToString(param2axisValue));
													printMessage("param3axisValue = " + convertDoubleToString(param3axisValue));
													//return;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if(!foundIdeal)
	{
		//do nothing (leave rotationMatrixLD as identity matrix)
	}
}
#endif




void ldrawVRoperations::convertTransformationMatrixCoordinateSystemLDtoUnreal(vec* translationVectorLD, mat* rotationMatrixLD, FVector* translatorUnreal, FRotator* rotatorUnreal, int order)
{
	vec translationVectorUnreal;
	mat rotationMatrixUnreal;
	vec eulerUnreal;

	string translationVectorLDstring = convertDoubleToString(translationVectorLD->x) + " " + convertDoubleToString(translationVectorLD->y) + " " + convertDoubleToString(translationVectorLD->z);
	string rotationMatrixLDstring = convertDoubleToString(rotationMatrixLD->a.x) + " " + convertDoubleToString(rotationMatrixLD->b.x) + " " + convertDoubleToString(rotationMatrixLD->c.x) + " " + convertDoubleToString(rotationMatrixLD->a.y) + " " + convertDoubleToString(rotationMatrixLD->b.y) + " " + convertDoubleToString(rotationMatrixLD->c.y) + " " + convertDoubleToString(rotationMatrixLD->a.z) + " " + convertDoubleToString(rotationMatrixLD->b.z) + " " + convertDoubleToString(rotationMatrixLD->c.z);
	//printMessage("convertTransformationMatrixCoordinateSystemLDtoUnreal: translationVectorLD = " + translationVectorLDstring);
	//printMessage("convertTransformationMatrixCoordinateSystemLDtoUnreal: rotationMatrixLD = " + rotationMatrixLDstring);

	convertTransformationMatrixCoordinateSystemLDtoUnreal(translationVectorLD, rotationMatrixLD, &translationVectorUnreal, &rotationMatrixUnreal, &eulerUnreal, order);

	//unreal translation creation based on euler angles:
	*translatorUnreal = convertVectorToFVector(&translationVectorUnreal);
	//unreal rotator creation based on euler angles:
	vec eulerUnrealDegrees = convertRadiansToDegrees(&eulerUnreal);	//required for Unreal FRotator definition
	rotatorUnreal->Roll = eulerUnrealDegrees.x;
	rotatorUnreal->Pitch = eulerUnrealDegrees.y;
	rotatorUnreal->Yaw = eulerUnrealDegrees.z;

	//printMessage("roll = " + convertDoubleToString(eulerUnrealDegrees.x));
	//printMessage("pitch = " + convertDoubleToString(eulerUnrealDegrees.y));
	//printMessage("yaw = " + convertDoubleToString(eulerUnrealDegrees.z));
}

void ldrawVRoperations::convertTransformationMatrixCoordinateSystemLDtoUnreal(vec* translationVectorLD, mat* rotationMatrixLD, vec* translationVectorUnreal, mat* rotationMatrixUnreal, vec* eulerUnreal, int order)
{
	convertTranslationVectorLDtoUnreal(translationVectorLD, translationVectorUnreal, order);
	convertRotationMatrixLDtoUnreal(rotationMatrixLD, rotationMatrixUnreal, eulerUnreal, order);
}

void ldrawVRoperations::convertTranslationVectorLDtoUnreal(vec* translationVectorLD, vec* translationVectorUnreal, int order)
{
	//translation:
	translationVectorUnreal->x = -translationVectorLD->x * LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_SCALING_FACTOR;	//parameters depend on desired coordinate system transformation (this is just an example)
	translationVectorUnreal->y = translationVectorLD->z * LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_SCALING_FACTOR;	//parameters depend on desired coordinate system transformation (this is just an example)
	translationVectorUnreal->z = -translationVectorLD->y * LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_SCALING_FACTOR;	//parameters depend on desired coordinate system transformation (this is just an example)
}

void ldrawVRoperations::convertRotationMatrixLDtoUnreal(mat* rotationMatrixLD, mat* rotationMatrixUnreal, vec* eulerUnreal, int order)
{
	//rotation:
	vec eulerLD = rotationMatrixToEulerAngles(rotationMatrixLD);
	if(order == LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT)
	{
		convertEulerToRotationMatrix(eulerLD.x, eulerLD.z, -(eulerLD.y+M_PI), MATH_EULER_YZX, rotationMatrixUnreal);	//parameters depend on desired coordinate system transformation (this is just an example)
	}
	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
	else if(order == LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_INTERMEDIATE)
	{
		convertEulerToRotationMatrix(eulerLD.x, eulerLD.z, -(eulerLD.y+M_PI), MATH_EULER_YZX, rotationMatrixUnreal);	//parameters depend on desired coordinate system transformation (this is just an example)
		//ORIG: convertEulerToRotationMatrix(eulerLD.x, eulerLD.z, -(eulerLD.y+M_PI), MATH_EULER_YZX, rotationMatrixUnreal);	//parameters depend on desired coordinate system transformation (this is just an example)
	}
	#endif
	*eulerUnreal = rotationMatrixToEulerAngles(rotationMatrixUnreal);
}

void ldrawVRoperations::convertRotatorLDtoUnreal(FRotator& worldRotationUnreal, FRotator& worldRotationLD)
{
	worldRotationUnreal.Yaw = worldRotationLD.Yaw;
	worldRotationUnreal.Pitch = worldRotationLD.Pitch;
	worldRotationUnreal.Roll = worldRotationLD.Roll + 90.0;
}


FRotator ldrawVRoperations::convertEulerToRotator(vec* eulerUnrealRadians)
{
	vec eulerUnrealDegrees = convertRadiansToDegrees(eulerUnrealRadians);
	FRotator rotatorUnreal;
	rotatorUnreal.Roll = eulerUnrealDegrees.x;
	rotatorUnreal.Pitch = eulerUnrealDegrees.y;
	rotatorUnreal.Yaw = eulerUnrealDegrees.z;
	return rotatorUnreal;
}

vec ldrawVRoperations::convertRotatorToEuler(FRotator* rotatorUnreal)
{
	vec eulerUnrealDegrees;
	eulerUnrealDegrees.x = rotatorUnreal->Roll;
	eulerUnrealDegrees.y = rotatorUnreal->Pitch;
	eulerUnrealDegrees.z = rotatorUnreal->Yaw;
	vec eulerUnrealRadians = convertDegreesToRadians(&eulerUnrealDegrees);
	return eulerUnrealRadians;
}


FVector ldrawVRoperations::convertVectorToFVector(vec* v)
{
	FVector fvec;
	fvec.X = v->x;
	fvec.Y = v->y;
	fvec.Z = v->z;
	return fvec;
}

vec ldrawVRoperations::convertFVectorToVector(FVector* fvec)
{
	vec v;
	v.x = fvec->X;
	v.y = fvec->Y;
	v.z = fvec->Z;
	return v;
}




vec ldrawVRoperations::rotationMatrixToEulerAngles(mat* R)
{
	//https://www.learnopencv.com/rotation-matrix-to-euler-angles/

	double sy = sqrt((R->a.x * R->a.x) + (R->b.x * R->b.x));

	bool singular = false;
	if(sy < 1e-6)
	{
		singular = true;
	}

	double x, y, z;
	if(!singular)
	{
		x = atan2(R->c.y , R->c.z);
		y = atan2(-(R->c.x), sy);
		z = atan2(R->b.x, R->a.x);
	}
	else
	{
		x = atan2(-(R->b.z), R->b.y);
		y = atan2(-(R->c.x), sy);
		z = 0.0;
	}
	vec euler;
	euler.x = x;
	euler.y = y;
	euler.z = z;
	return euler;
}

vec ldrawVRoperations::convertRadiansToDegrees(vec* v)
{
	vec vDegrees;
	vDegrees.x = convertRadiansToDegrees(v->x);
	vDegrees.y = convertRadiansToDegrees(v->y);
	vDegrees.z = convertRadiansToDegrees(v->z);
	return vDegrees;
}

double ldrawVRoperations::convertRadiansToDegrees(double value)
{
	double degrees = value/M_PI*180.0;
	return degrees;
}

vec ldrawVRoperations::convertDegreesToRadians(vec* v)
{
	vec vRadians;
	vRadians.x = convertDegreesToRadians(v->x);
	vRadians.y = convertDegreesToRadians(v->y);
	vRadians.z = convertDegreesToRadians(v->z);
	return vRadians;
}

double ldrawVRoperations::convertDegreesToRadians(double value)
{
	double radians = value/180.0*M_PI;
	return radians;
}





/*
#ifdef LDRAWVR_LOAD_MATERIALS_AT_START
TArray<UObject*> materialAssets;
#ifdef LDRAWVR_LOAD_MATERIALS_AT_START
EngineUtils::FindOrLoadAssetsByPath(TEXT(UNREAL_MATERIAL_ASSET_LOCATION), materialAssets, EngineUtils::ATL_Regular);
#endif

bool getMaterial(TArray<UObject*> materialAssets, string name, UMaterial** materialFound)
{
	bool result = false;
	for (auto asset : materialAssets)
	{
		UMaterial materialTemp;
		if(ClassIsChildOf(asset->GetClass(), materialTemp)
		{
			UMaterial* material = Cast<UMaterial>(asset);
			if(material != nullptr)
			{
				string materialName = convertFStringToString(material->GetName());
				if(materialName == name)
				{
					result = true;
					*materialFound = material;
				}
			}
		}
	}
}
#endif
*/

void ldrawVRoperations::convertEulerToRotationMatrix(double x, double y, double z, int mode, mat *matrix)
{
	mat matrixX;
	mat matrixY;
	mat matrixZ;
	SHAREDvector.createIdentityMatrix(&matrixX);
	SHAREDvector.createIdentityMatrix(&matrixY);
	SHAREDvector.createIdentityMatrix(&matrixZ);
	SHAREDvector.createRotationMatrixX(&matrixX, x);
	SHAREDvector.createRotationMatrixY(&matrixY, y);
	SHAREDvector.createRotationMatrixZ(&matrixZ, z);

	mat result;
	if(mode == MATH_EULER_XYZ)
	{
		multiplyMatricies2(&matrixX, &matrixY, &result);
		multiplyMatricies2(&result, &matrixZ, matrix);
	}
	else if(mode == MATH_EULER_XZY)
	{
		multiplyMatricies2(&matrixX, &matrixZ, &result);
		multiplyMatricies2(&result, &matrixY, matrix);
	}
	else if(mode == MATH_EULER_YXZ)
	{
		multiplyMatricies2(&matrixY, &matrixX, &result);
		multiplyMatricies2(&result, &matrixZ, matrix);
	}
	else if(mode == MATH_EULER_YZX)
	{
		multiplyMatricies2(&matrixY, &matrixZ, &result);
		multiplyMatricies2(&result, &matrixX, matrix);
	}
	else if(mode == MATH_EULER_ZXY)
	{
		multiplyMatricies2(&matrixZ, &matrixX, &result);
		multiplyMatricies2(&result, &matrixY, matrix);
	}
	else if(mode == MATH_EULER_ZYX)
	{
		multiplyMatricies2(&matrixZ, &matrixY, &result);
		multiplyMatricies2(&result, &matrixX, matrix);
	}
}

void ldrawVRoperations::multiplyMatricies2(mat* mat1, mat* mat2, mat* matNew)
{
	SHAREDvector.multiplyMatricies(matNew, mat1, mat2);
}


void ldrawVRoperations::printMessage(string msg)
{
	FString msgF = convertStringToFString(msg);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *msgF);
}


FString ldrawVRoperations::convertStringToFString(string str)
{
	FString fstring = FString(str.c_str());
	return fstring;
}

string ldrawVRoperations::convertFStringToString(FString fstring)
{
	string str = TCHAR_TO_UTF8(*fstring);
	return str;
}

string ldrawVRoperations::convertDoubleToString(double number)
{
	return convertFStringToString(FString::SanitizeFloat(number));
}

FName ldrawVRoperations::convertStringToFName(string meshComponentName)
{
	return FName(*(convertStringToFString(meshComponentName)));
}





//moved from AldrawVRpartActor;

UMaterialInterface* ldrawVRoperations::getReplacementMaterial(LDreference* reference, bool highlighted, bool allowColourChange)
{
	int relativeColour = reference->colour;
	int absoluteColour = reference->absoluteColour;
	bool inheritedMaterial = false;
	if(relativeColour == DAT_FILE_DEFAULT_COLOUR)
	{
		if(relativeColour == absoluteColour)
		{
			inheritedMaterial = true;		//if the part colour is defined as default (16), and this not been assigned a different (ie absolute) colour in a higher level model, then set the material to inherited (ie allow its colour to be changed by the user)
		}
	}
	if(allowColourChange)
	{
		inheritedMaterial = true;
	}
	return getReplacementMaterial(absoluteColour, inheritedMaterial, highlighted);
}

UMaterialInterface* ldrawVRoperations::getReplacementMaterial(int absoluteColour, bool inheritedMaterial, bool highlighted)
{
	string materialNameIndexPrepend = string(LDRAWVR_NUMBER_MATERIAL_NAME_INDEX_PREPEND);
	string materialName  = materialNameIndexPrepend + SHAREDvars.convertIntToString(absoluteColour);
	#ifdef LDRAW_VR_OLD_INHERITED_MATERIAL_NAMES
	if(absoluteColour == DAT_FILE_DEFAULT_COLOUR)
	{
		materialName = materialNameIndexPrepend + LDRAWVR_MATERIAL_NAME_INDEX_DEFAULT;
	}
	#endif
	return getReplacementMaterial(materialName, inheritedMaterial, highlighted);
}

UMaterialInterface* ldrawVRoperations::getReplacementMaterial(string materialName, bool inheritedMaterial, bool highlighted)
{
	string materialAssetLocation = "";
	string materialNameIndexPrepend = string(LDRAWVR_NUMBER_MATERIAL_NAME_INDEX_PREPEND);

	#ifdef LDRAW_VR_ASSIGN_UNIQUE_MATERIAL_NAMES_FOR_INHERITED_MESHES
	if(!inheritedMaterial)
	{
	#endif
		materialAssetLocation = UNREAL_MATERIAL_ASSET_LOCATION;
	#ifdef LDRAW_VR_ASSIGN_UNIQUE_MATERIAL_NAMES_FOR_INHERITED_MESHES
	}
	else
	{
		//ldrawVRops.printMessage("UNREAL_MATERIAL_INHERITED_ASSET_LOCATION: materialName = " + materialName);
		materialAssetLocation = UNREAL_MATERIAL_INHERITED_ASSET_LOCATION;
	}
	#endif
	#ifdef LDRAW_VR_REASSIGN_MATERIALS_HIGHLIGHTED_WHEN_SELECTED
	if(highlighted)
	{
		materialAssetLocation = materialAssetLocation + UNREAL_MATERIAL_HIGHLIGHTED_ASSET_LOCATION_APPEND;
	}
	#endif

	FString pathNameMaterial = generateMaterialPathName(materialName, materialAssetLocation);
	printMessage("pathNameMaterial = " + convertFStringToString(pathNameMaterial));

	UMaterialInterface* replacementMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL, *pathNameMaterial));
	return replacementMaterial;
}


//not robust (colour index cannot be assigned for custom material colours)
int ldrawVRoperations::getColourFromMaterialName(string materialName)
{
	int colour = DAT_FILE_COLOUR_BLACK;	//colour to display for custom material name
	int indexOfColourString = materialName.rfind(LDRAWVR_NUMBER_MATERIAL_NAME_DELIMITER);
	if(indexOfColourString != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		if(materialName.length() > indexOfColourString+1)
		{
			string colourString = materialName.substr(indexOfColourString+1);
			colour = SHAREDvars.convertStringToInt(colourString);
		}
	}
	return colour;
}

FString ldrawVRoperations::generateMaterialPathName(string assetName)
{
	string assetDescription = string("Material");
	string assetLocation = UNREAL_MATERIAL_ASSET_LOCATION;
	return generateUnrealAssetPathName(assetDescription, assetLocation, assetName);
}
FString ldrawVRoperations::generateMaterialPathName(string assetName, string assetLocation)
{
	string assetDescription = string("Material");
	return generateUnrealAssetPathName(assetDescription, assetLocation, assetName);
}
FString ldrawVRoperations::generateUnrealAssetPathName(string assetDescription, string assetLocation, string assetName)
{
	string pathName = assetDescription + "'" + assetLocation + assetName + "." + assetName + "'";	//eg StaticMesh' / Game / MyCharacter / Meshes / weapons / BlameGT_Mesh.BlameGT_Mesh'
	FString pathNameF = convertStringToFString(pathName);
	return pathNameF;
}

bool ldrawVRoperations::submodelIsPart(string name, string* partName)
{
	bool result = false;
	if(name.find(LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		result = true;
		*partName = name.substr(0, name.find(LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION));
	}
	else if(name.find(LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION_UPPER) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		result = true;
		*partName = name.substr(0, name.find(LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION_UPPER));
	}
	else if(name.find(LD_DEFAULT_LDRAW_MODEL_FILE_EXTENSION) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		*partName = name.substr(0, name.find(LD_DEFAULT_LDRAW_MODEL_FILE_EXTENSION));
	}
	else if(name.find(LD_DEFAULT_LDRAW_MODEL_FILE_EXTENSION_UPPER) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		*partName = name.substr(0, name.find(LD_DEFAULT_LDRAW_MODEL_FILE_EXTENSION_UPPER));
	}
	return result;
}




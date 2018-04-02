#pragma once

#include "AxisAngle.h"
#include "DirectionAngle.h"
#include "EulerAngles.h"
#include "EulerConstants.h"
#include "HMatrix.h"
#include "Quaternion.h"
#include "RotationMatrix.h"
#include "Vector.h"
#include "YawPitchRoll.h"

class Rotation {
private:
	Quaternion QuaternionRotation;

	Quaternion AxisAngleToQuaternion(AxisAngle axisAngle);
	Quaternion DirectionAngleToQuaternion(DirectionAngle directionAngle);
	Quaternion RotationMatrixToQuaternion(RotationMatrix rotationMatrix);
	Quaternion EulerAnglesToQuaternion(EulerAngles eulerAngles);
	Quaternion HierarchicalMatrixToQuaternion(HMatrix hMatrix);
	EulerAngles HierarchicalMatrixToEulerAngles(HMatrix hM, EulerOrder order);
	HMatrix EulerAnglesToHierarchicalMatrix(EulerAngles eulerAngles);
	Quaternion QuaternionFromDirectionVectors(Vector3D initial, Vector3D target);
	Quaternion YawPitchRollToQuaternion(YawPitchRoll ypr);

public:
	Rotation();
	Rotation(Quaternion quaternion);
	Rotation(AxisAngle axisAngle);
	Rotation(DirectionAngle directionAngle);
	Rotation(RotationMatrix rotationMatrix);
	Rotation(EulerAngles eulerAngles);
	Rotation(HMatrix hMatrix);
	Rotation(Vector3D initial, Vector3D target);
	Rotation(YawPitchRoll ypr);

	Quaternion GetQuaternion();
	AxisAngle GetAxisAngle();
	DirectionAngle GetDirectionAngle();
	RotationMatrix GetRotationMatrix();
	EulerAngles GetEulerAngles(EulerOrder order);
	HMatrix GetHierarchicalMatrix();
	YawPitchRoll GetYawPitchRoll();

};

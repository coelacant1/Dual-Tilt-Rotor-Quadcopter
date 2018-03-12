#include <Rotation.h>

Rotation::Rotation() {
	QuaternionRotation = Quaternion();
}

Rotation::Rotation(Quaternion quaternion) {
	QuaternionRotation = quaternion;
}

Rotation::Rotation(AxisAngle axisAngle) {
	QuaternionRotation = AxisAngleToQuaternion(axisAngle);
}

Rotation::Rotation(DirectionAngle directionAngle) {
	QuaternionRotation = DirectionAngleToQuaternion(directionAngle);
}

Rotation::Rotation(RotationMatrix rotationMatrix) {
	QuaternionRotation = RotationMatrixToQuaternion(rotationMatrix);
}

Rotation::Rotation(EulerAngles eulerAngles) {
	QuaternionRotation = EulerAnglesToQuaternion(eulerAngles);
}

Rotation::Rotation(HMatrix hMatrix) {
	QuaternionRotation = HierarchicalMatrixToQuaternion(hMatrix);
}

Quaternion Rotation::AxisAngleToQuaternion(AxisAngle axisAngle) {
		double rotation = Math::DegreesToRadians(axisAngle.Rotation);
		double scale = sin(rotation / 2);

		return Quaternion(
			cos(rotation / 2),
			axisAngle.Axis.X * scale,
			axisAngle.Axis.Y * scale,
			axisAngle.Axis.Z * scale);
}

Quaternion Rotation::DirectionAngleToQuaternion(DirectionAngle directionAngle) {
	Vector3D right = Vector3D(1, 0, 0);
	Vector3D up = Vector3D(0, 1, 0);
	Vector3D forward = Vector3D(0, 0, 1);

	Vector3D rotatedRight;
	Vector3D rotatedUp = Vector3D(directionAngle.Direction);
	Vector3D rotatedForward;

	Quaternion rotationChange = Quaternion::QuaternionFromDirectionVectors(up, rotatedUp);

	Vector3D rightAngleRotated = RotationMatrix::RotateVector(Vector3D(0, -directionAngle.Rotation, 0), right);
	Vector3D forwardAngleRotated = RotationMatrix::RotateVector(Vector3D(0, -directionAngle.Rotation, 0), forward);

	rotatedRight = rotationChange.RotateVector(rightAngleRotated);
	rotatedForward = rotationChange.RotateVector(forwardAngleRotated);

	return RotationMatrixToQuaternion(RotationMatrix(rotatedRight, rotatedUp, rotatedForward));
}

Quaternion Rotation::RotationMatrixToQuaternion(RotationMatrix rM) {
	Quaternion q = Quaternion();

	Vector3D X = Vector3D(rM.XAxis);
	Vector3D Y = Vector3D(rM.YAxis);
	Vector3D Z = Vector3D(rM.ZAxis);

	double matrixTrace = X.X + Y.Y + Z.Z;
	double square;

	if (matrixTrace > 0)//standard procedure
	{
		square = sqrt(1.0 + matrixTrace) * 2;//4 * qw

		q.W = 0.25 * square;
		q.X = (Z.Y - Y.Z) / square;
		q.Y = (X.Z - Z.X) / square;
		q.Z = (Y.X - X.Y) / square;
	}
	else if ((X.X > Y.Y) && (X.X > Z.Z))
	{
		square = sqrt(1.0 + X.X - Y.Y - Z.Z) * 2;//4 * qx

		q.W = (Z.Y - Y.Z) / square;
		q.X = 0.25 * square;
		q.Y = (X.Y + Y.X) / square;
		q.Z = (X.Z + Z.X) / square;
	}
	else if (Y.Y > Z.Z)
	{
		square = sqrt(1.0 + Y.Y - X.X - Z.Z) * 2;//4 * qy

		q.W = (X.Z - Z.X) / square;
		q.X = (X.Y + Y.X) / square;
		q.Y = 0.25 * square;
		q.Z = (Y.Z + Z.Y) / square;
	}
	else
	{
		square = sqrt(1.0 + Z.Z - X.X - Y.Y) * 2;//4 * qz

		q.W = (Y.X - X.Y) / square;
		q.X = (X.Z + Z.X) / square;
		q.Y = (Y.Z + Z.Y) / square;
		q.Z = 0.25 * square;
	}

	return q.UnitQuaternion().Conjugate();
}

Quaternion Rotation::EulerAnglesToQuaternion(EulerAngles eulerAngles) {
	Quaternion q = Quaternion(1, 0, 0, 0);
	double sx, sy, sz, cx, cy, cz, cc, cs, sc, ss;

	eulerAngles.Angles.X = Math::DegreesToRadians(eulerAngles.Angles.X);
	eulerAngles.Angles.Y = Math::DegreesToRadians(eulerAngles.Angles.Y);
	eulerAngles.Angles.Z = Math::DegreesToRadians(eulerAngles.Angles.Z);

	if (eulerAngles.Order.FrameTaken == EulerOrder::AxisFrame::Rotating)
	{
		double t = eulerAngles.Angles.X;
		eulerAngles.Angles.X = eulerAngles.Angles.Z;
		eulerAngles.Angles.Z = t;
	}

	if (eulerAngles.Order.AxisPermutation == EulerOrder::Parity::Odd)
	{
		eulerAngles.Angles.Y = -eulerAngles.Angles.Y;
	}

	sx = sin(eulerAngles.Angles.X * 0.5);
	sy = sin(eulerAngles.Angles.Y * 0.5);
	sz = sin(eulerAngles.Angles.Z * 0.5);

	cx = cos(eulerAngles.Angles.X * 0.5);
	cy = cos(eulerAngles.Angles.Y * 0.5);
	cz = cos(eulerAngles.Angles.Z * 0.5);

	cc = cx * cz;
	cs = cx * sz;
	sc = sx * cz;
	ss = sx * sz;

	if (eulerAngles.Order.InitialAxisRepetition == EulerOrder::AxisRepetition::Yes)
	{
		q.X = cy * (cs + sc);
		q.Y = sy * (cc + ss);
		q.Z = sy * (cs - sc);
		q.W = cy * (cc - ss);
	}
	else
	{
		q.X = cy * sc - sy * cs;
		q.Y = cy * ss + sy * cc;
		q.Z = cy * cs - sy * sc;
		q.W = cy * cc + sy * ss;
	}

	q.Permutate(eulerAngles.Order.Permutation);

	if (eulerAngles.Order.AxisPermutation == EulerOrder::Parity::Odd)
	{
		q.Y = -q.Y;
	}

	return q;
}

Quaternion Rotation::HierarchicalMatrixToQuaternion(HMatrix hMatrix) {
	return EulerAnglesToQuaternion(HierarchicalMatrixToEulerAngles(hMatrix, EulerConstants::EulerOrderXYZS));
}

EulerAngles Rotation::HierarchicalMatrixToEulerAngles(HMatrix hM, EulerOrder order) {
	EulerAngles eulerAngles = EulerAngles(Vector3D(0, 0, 0), order);
	Vector3D p = order.Permutation;

	if (order.InitialAxisRepetition == EulerOrder::AxisRepetition::Yes)
	{
		double sy = sqrt(pow(hM(p.X, p.Y), 2) + pow(hM(p.X, p.Z), 2));

		if (sy > 32 * numeric_limits<double>::epsilon())//16 * float.Epsilon
		{
			eulerAngles.Angles.X = atan2(hM(p.X, p.Y), hM(p.X, p.Z));
			eulerAngles.Angles.Y = atan2(sy, hM(p.X, p.X));
			eulerAngles.Angles.Z = atan2(hM(p.Y, p.X), -hM(p.Z, p.X));
		}
		else
		{
			eulerAngles.Angles.X = atan2(-hM(p.Y, p.Z), hM(p.Y, p.Y));
			eulerAngles.Angles.Y = atan2(sy, hM(p.X, p.X));
			eulerAngles.Angles.Z = 0;
		}
	}
	else
	{
		double cy = sqrt(pow(hM(p.X, p.X), 2) + pow(hM(p.Y, p.X), 2));

		if (cy > 32 * numeric_limits<double>::epsilon())
		{
			eulerAngles.Angles.X = atan2(hM(p.Z, p.Y), hM(p.Z, p.Z));
			eulerAngles.Angles.Y = atan2(-hM(p.Z, p.X), cy);
			eulerAngles.Angles.Z = atan2(hM(p.Y, p.X), hM(p.X, p.X));
		}
		else
		{
			eulerAngles.Angles.X = atan2(-hM(p.Y, p.Z), hM(p.Y, p.Y));
			eulerAngles.Angles.Y = atan2(-hM(p.Z, p.X), cy);
			eulerAngles.Angles.Z = 0;
		}
	}

	if (order.AxisPermutation == EulerOrder::Parity::Odd)
	{
		eulerAngles.Angles.X = -eulerAngles.Angles.X;
		eulerAngles.Angles.Y = -eulerAngles.Angles.Y;
		eulerAngles.Angles.Z = -eulerAngles.Angles.Z;
	}

	if (order.FrameTaken == EulerOrder::AxisFrame::Rotating)
	{
		double temp = eulerAngles.Angles.X;
		eulerAngles.Angles.X = eulerAngles.Angles.Z;
		eulerAngles.Angles.Z = temp;
	}

	eulerAngles.Angles.X = Math::RadiansToDegrees(eulerAngles.Angles.X);
	eulerAngles.Angles.Y = Math::RadiansToDegrees(eulerAngles.Angles.Y);
	eulerAngles.Angles.Z = Math::RadiansToDegrees(eulerAngles.Angles.Z);

	return eulerAngles;
}

HMatrix Rotation::EulerAnglesToHierarchicalMatrix(EulerAngles eulerAngles) {
	HMatrix hM = HMatrix();
	double sx, sy, sz, cx, cy, cz, cc, cs, sc, ss; Vector3D p = eulerAngles.Order.Permutation;

	eulerAngles.Angles.X = Math::DegreesToRadians(eulerAngles.Angles.X);
	eulerAngles.Angles.Y = Math::DegreesToRadians(eulerAngles.Angles.Y);
	eulerAngles.Angles.Z = Math::DegreesToRadians(eulerAngles.Angles.Z);

	if (eulerAngles.Order.FrameTaken == EulerOrder::AxisFrame::Rotating)
	{
		double t = eulerAngles.Angles.X;
		eulerAngles.Angles.X = eulerAngles.Angles.Z;
		eulerAngles.Angles.Z = t;
	}

	if (eulerAngles.Order.AxisPermutation == EulerOrder::Parity::Odd)
	{
		eulerAngles.Angles.X = -eulerAngles.Angles.X;
		eulerAngles.Angles.Y = -eulerAngles.Angles.Y;
		eulerAngles.Angles.Z = -eulerAngles.Angles.Z;
	}

	sx = sin(eulerAngles.Angles.X);
	sy = sin(eulerAngles.Angles.Y);
	sz = sin(eulerAngles.Angles.Z);
	cx = cos(eulerAngles.Angles.X);
	cy = cos(eulerAngles.Angles.Y);
	cz = cos(eulerAngles.Angles.Z);

	cc = cx * cz;
	cs = cx * sz;
	sc = sx * cz;
	ss = sx * sz;

	if (eulerAngles.Order.InitialAxisRepetition == EulerOrder::AxisRepetition::Yes)
	{
		hM(p.X, p.X) = cy; hM(p.X, p.Y) = sy * sx; hM(p.X, p.Z) = sy * cx; hM(0, 3) = 0;
		hM(p.Y, p.X) = sy * sz; hM(p.Y, p.Y) = -cy * ss + cc; hM(p.Y, p.Z) = -cy * cs - sc; hM(1, 3) = 0;
		hM(p.Z, p.X) = -sy * cz; hM(p.Z, p.Y) = cy * sc + cs; hM(p.Z, p.Z) = cy * cc - ss; hM(2, 3) = 0;
		hM(3, 0) = 0; hM(3, 1) = 0; hM(3, 2) = 0; hM(3, 3) = 1;
	}
	else
	{
		hM(p.X, p.X) = cy * cz; hM(p.X, p.Y) = sy * sc - cs; hM(p.X, p.Z) = sy * cc + ss; hM(0, 3) = 0;
		hM(p.Y, p.X) = cy * sz; hM(p.Y, p.Y) = sy * ss + cc; hM(p.Y, p.Z) = sy * cs - sc; hM(1, 3) = 0;
		hM(p.Z, p.X) = -sy; hM(p.Z, p.Y) = cy * sx; hM(p.Z, p.Z) = cy * cx; hM(2, 3) = 0;
		hM(3, 0) = 0; hM(3, 1) = 0; hM(3, 2) = 0; hM(3, 3) = 1;
	}

	return hM;
}

Quaternion Rotation::GetQuaternion() {
	return QuaternionRotation;
}

AxisAngle Rotation::GetAxisAngle() {
	AxisAngle axisAngle = AxisAngle(0, 0, 1, 0);
	Quaternion q = QuaternionRotation;

	q = (abs(q.W) > 1.0) ? q.UnitQuaternion() : q;

	axisAngle.Rotation = Math::RadiansToDegrees(2.0 * acos(q.W));

	double quaternionCheck = sqrt(1.0 - pow(q.W, 2.0));//Prevents rotation jumps, and division by zero

	if (quaternionCheck >= 0.001)//Prevents division by zero
	{
		//Normalizes axis
		axisAngle.Axis;
		axisAngle.Axis.X = q.X / quaternionCheck;
		axisAngle.Axis.Y = q.Y / quaternionCheck;
		axisAngle.Axis.Z = q.Z / quaternionCheck;
	}
	else
	{
		//If X is close to zero the axis doesn't matter
		axisAngle.Axis;
		axisAngle.Axis.X = 0.0;
		axisAngle.Axis.Y = 1.0;
		axisAngle.Axis.Z = 0.0;
	}

	return axisAngle;
}

DirectionAngle Rotation::GetDirectionAngle() {
	Quaternion q = QuaternionRotation;
	Vector3D up = Vector3D(0, 1, 0);//up vector
	Vector3D right = Vector3D(1, 0, 0);
	Vector3D rotatedUp = q.RotateVector(up);//new direction vector
	Vector3D rotatedRight = q.RotateVector(right);
	Quaternion rotationChange = Quaternion::QuaternionFromDirectionVectors(up, rotatedUp);

	//rotate forward vector by direction vector rotation
	Vector3D rightXZCompensated = rotationChange.UnrotateVector(rotatedRight);//should only be two points on circle, compare against right

																			  //define angles that define the forward vector, and the rotated then compensated forward vector
	double rightAngle = Math::RadiansToDegrees(atan2(right.Z, right.X));//forward as zero
	double rightRotatedAngle = Math::RadiansToDegrees(atan2(rightXZCompensated.Z, rightXZCompensated.X));//forward as zero

																										 //angle about the axis defined by the direction of the object
	double angle = rightAngle - rightRotatedAngle;

	//returns the angle rotated about the rotated up vector as an axis
	return DirectionAngle(angle, rotatedUp);
}

RotationMatrix Rotation::GetRotationMatrix() {
	Vector3D X = Vector3D(1, 0, 0);
	Vector3D Y = Vector3D(0, 1, 0);
	Vector3D Z = Vector3D(0, 0, 1);

	return RotationMatrix(
		QuaternionRotation.RotateVector(X),
		QuaternionRotation.RotateVector(Y),
		QuaternionRotation.RotateVector(Z)
	);
}

EulerAngles Rotation::GetEulerAngles(EulerOrder order) {
	Quaternion q = QuaternionRotation;
	double norm = q.Normal();
	double scale = norm > 0.0 ? 2.0 / norm : 0.0;
	HMatrix hM = HMatrix();

	Vector3D s = Vector3D(q.X * scale, q.Y * scale, q.Z * scale);
	Vector3D w = Vector3D(q.W * s.X, q.W * s.Y, q.W * s.Z);
	Vector3D x = Vector3D(q.X * s.X, q.X * s.Y, q.X * s.Z);
	Vector3D y = Vector3D(0.0, q.Y * s.Y, q.Y * s.Z);
	Vector3D z = Vector3D(0.0, 0.0, q.Z * s.Z);

	//0X, 1Y, 2Z, 3W
	hM(0, 0) = 1.0 - (y.Y + z.Z);   hM(0, 1) = x.Y - w.Z;           hM(0, 2) = x.Z + w.Y;           hM(0, 3) = 0.0;
	hM(1, 0) = x.Y + w.Z;           hM(1, 1) = 1.0 - (x.X + z.Z);   hM(1, 2) = y.Z - w.X;           hM(1, 3) = 0.0;
	hM(2, 0) = x.Z - w.Y;           hM(2, 1) = y.Z + w.X;           hM(2, 2) = 1.0 - (x.X + y.Y);   hM(2, 3) = 0.0;
	hM(3, 0) = 0.0;                 hM(3, 1) = 0.0;                 hM(3, 2) = 0.0;                 hM(3, 3) = 1.0;

	return HierarchicalMatrixToEulerAngles(hM, order);
}

HMatrix Rotation::GetHierarchicalMatrix() {
	return EulerAnglesToHierarchicalMatrix(GetEulerAngles(EulerConstants::EulerOrderXYZS));
}
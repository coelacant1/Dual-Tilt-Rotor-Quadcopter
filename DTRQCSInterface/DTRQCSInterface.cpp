#include "stdafx.h"
#include "DTRQCSInterface.h"
#include <iostream>

#include "Quadcopter.h"
#include "Vector.h"
#include "Rotation.h"
#include "Thruster.h"

#include <SVector.h>
#include <SQuaternion.h>
#include <SThrust.h>
#include <SQuad.h>

using namespace System;

namespace DTRQCSInterface {
	public ref class DTRQQuadcopter {
	private:
		Quadcopter* quadcopter;
		SQuad^ quadData;
		double dT;
	public:
		DTRQQuadcopter() {
			VectorFeedbackController *pos = new VectorFeedbackController{
				new PID{ 10, 0, 12.5 },
				new PID{ 1, 0, 0.2 },
				new PID{ 10, 0, 12.5 }
			};

			VectorFeedbackController *rot = new VectorFeedbackController{
				new PID{ 0.05, 0, 0.325 },
				new PID{ 0.05, 0, 0.325 },
				new PID{ 0.05, 0, 0.325 }
			};

			quadcopter = new Quadcopter(true, 0.3, 55, 0.05, pos, rot);
			
			GetQuadcopter();
		}
		~DTRQQuadcopter() { delete quadcopter; }

		DTRQQuadcopter(bool simulation, double armLength, double armAngle, double dT) {
			std::cout << "Initializing feedback controllers..." << std::endl;
			
			/*VectorFeedbackController positionController = VectorFeedbackController {
				new ADRC { 50.0, 200.0, 4.0, 10.0,
					PID { 10, 0, 12.5 }
				},
				new ADRC { 10.0, 10.0, 1.5, 0.05,
					PID { 1, 0, 0.2 }
				},
				new ADRC { 50.0, 200.0, 4.0, 10.0,
					PID { 10, 0, 12.5 }
				}
			};
			*/
			/*VectorFeedbackController rotationController = VectorFeedbackController {
				new ADRC { 20.0, 200.0, 4.0, 10.0,
					PID { 5, 0, 7.5 }
				},
				new ADRC { 10.0, 10.0, 1.5, 64,
					PID { 10, 0, 25 }
				},
				new ADRC { 20.0, 200.0, 4.0, 10.0,
					PID { 5, 0, 7.5 }
				}
			};*/

			//CHANGE THESE
			VectorFeedbackController *pos = new VectorFeedbackController{
				new PID{ 10, 0, 12.5 },
				new PID{ 1, 0, 0.2 },
				new PID{ 10, 0, 12.5 }
			};

			VectorFeedbackController *rot = new VectorFeedbackController{
				new PID{ 0.05, 0, 0.325 },
				new PID{ 0.05, 0, 0.325 },
				new PID{ 0.05, 0, 0.325 }
			};

			std::cout << "Quadcopter initializing..." << std::endl;

			quadcopter = new Quadcopter(simulation, armLength, armAngle, dT, pos, rot);
			this->dT = dT;

			std::cout << "Quadcopter initialized." << std::endl;
		}

		void CalculateCombinedThrustVector() {
			if (quadcopter == NULL) {
				std::cout << "ONE" << std::endl;
			}

			quadcopter->CalculateCombinedThrustVector();
		}

		void SetTarget(SVector^ pos, SQuaternion^ q) {
			if (quadcopter == NULL) {
				std::cout << "TWO" << std::endl;
			}

			quadcopter->SetTarget(Vector3D(pos->X, pos->Y, pos->Z), 
				Rotation(Quaternion(q->W, q->X, q->Y, q->Z)));
		}

		void SimulateCurrent(SVector^ extAcc) {
			if (quadcopter == NULL) {
				std::cout << "THREE" << std::endl;
			}

			quadcopter->SimulateCurrent(Vector3D(extAcc->X, extAcc->Y, extAcc->Z));
		}

		SQuad^ GetQuadcopter() {
			if (quadcopter == NULL) {
				std::cout << "FOUR" << std::endl;
			}

			quadData = gcnew SQuad{
				gcnew SVector(quadcopter->CurrentPosition),
				gcnew SQuaternion(quadcopter->CurrentRotation.GetQuaternion()),
				gcnew SVector(quadcopter->TargetPosition),
				gcnew SThrust(quadcopter->TB->CurrentPosition, quadcopter->TB->CurrentRotation, quadcopter->TB->TargetPosition),
				gcnew SThrust(quadcopter->TC->CurrentPosition, quadcopter->TC->CurrentRotation, quadcopter->TC->TargetPosition),
				gcnew SThrust(quadcopter->TD->CurrentPosition, quadcopter->TD->CurrentRotation, quadcopter->TD->TargetPosition),
				gcnew SThrust(quadcopter->TE->CurrentPosition, quadcopter->TE->CurrentRotation, quadcopter->TE->TargetPosition),
				dT
			};

			return quadData;
		}
	};
}

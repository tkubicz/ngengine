#include "catch.hpp"
#include "NGE/Math/Matrix4.hpp"

namespace m = NGE::Math;

SCENARIO("Matrix4 rotation", "[math][matrix]") {
    
	GIVEN("Yaw, pitch, roll parameters in radians") {
		float yaw = 1.5, pitch = 1.2, roll = 2.1;

		WHEN("Matrix is rotated") {
			m::mat4f matXYZ, matX, matY, matZ;
			matX.RotateX(roll);
			matY.RotateY(pitch);
			matZ.RotateZ(yaw);
			matXYZ.RotateXYZ(roll, pitch, yaw);

			THEN("Matrices are the same") {
				REQUIRE(matXYZ == matZ * matY * matX);
			}
		}
	}
}
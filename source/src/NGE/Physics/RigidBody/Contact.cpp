#include "NGE/Physics/RigidBody/Contact.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Physics::RigidBody;

void Contact::SetBodyData(RigidBody* one, RigidBody* two, float friction, float restitution) {
	body[0] = one;
	body[1] = two;
	this->friction = friction;
	this->restitution = restitution;
}

void Contact::MatchAwakeState() {
	// Kolizja ze scenerią (światem nie powoduje obudzenia się obiektu)
	if (!body[1])
		return;

	bool body0awake = body[0]->GetAwake();
	bool body1awake = body[1]->GetAwake();

	// Obudzenie tylko śpiącego obiektu
	if (body0awake ^ body1awake) {
		if (body0awake)
			body[1]->SetAwake();
		else
			body[0]->SetAwake();
	}
}

/**
 * W przypadku ręcznego wywołania tej metody, należy po niej wywołać metodę
 * CalculateInternals.
 */
void Contact::SwapBodies() {
	contactNormal *= -1;

	RigidBody* temp = body[0];
	body[0] = body[1];
	body[1] = temp;
}

inline void Contact::CalculateContactBasis() {
	Math::vec3f contactTangent[2];

	// Sprawdzenie czy oś Z jest blisko osi X lub Y
	if (std::abs(contactNormal.x) > std::abs(contactNormal.y)) {
		// Współczynnik skalowania by zapewnić znormalizowane wyniki
		const float s = 1.0f / sqrt(contactNormal.z * contactNormal.z + contactNormal.x * contactNormal.x);

		// Nowa oś X jest 
		contactTangent[0].x = contactNormal.z * s;
		contactTangent[0].y = 0;
		contactTangent[0].z = -contactNormal.x * s;

		// Nowa oś Y
		contactTangent[1].x = contactNormal.y * contactTangent[0].x;
		contactTangent[1].y = contactNormal.z * contactTangent[0].x - contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = -contactNormal.y * contactTangent[0].x;
	} else {
		// Współczynnik skalowania by zapewnić znormalizowane wyniki
		const float s = 1.0f / sqrt(contactNormal.z * contactNormal.z + contactNormal.y * contactNormal.y);

		// Nowa oś X
		contactTangent[0].x = 0;
		contactTangent[0].y = -contactNormal.z * s;
		contactTangent[0].z = contactNormal.y * s;

		// Nowa oś Y
		contactTangent[1].x = contactNormal.y * contactTangent[0].z - contactNormal.z * contactTangent[0].y;
		contactTangent[1].y = contactNormal.x * contactTangent[0].z;
		contactTangent[1].z = contactNormal.x * contactTangent[0].y;
	}

	// Utworzenie macierzy z trzech wektorów
	contactToWorld[0] = contactNormal.x;
	contactToWorld[3] = contactTangent[0].x;
	contactToWorld[6] = contactTangent[1].x;

	contactToWorld[1] = contactNormal.y;
	contactToWorld[4] = contactTangent[0].y;
	contactToWorld[7] = contactTangent[1].y;

	contactToWorld[2] = contactNormal.z;
	contactToWorld[5] = contactTangent[0].z;
	contactToWorld[8] = contactTangent[1].z;
}

NGE::Math::vec3f Contact::CalculateLocalVelocity(unsigned bodyIndex, float duration) {
	RigidBody* thisBody = body[bodyIndex];

	// Obliczenie prędkości w punkcie kontaktu
	Math::vec3f velocity = thisBody->GetRotation().CrossProduct(relativeContactPosition[bodyIndex]);
	velocity += thisBody->GetVelocity();

	// Konwersja prędkości do współrzędnych kontaktu
	Math::vec3f contactVelocity = contactToWorld.TransformTranspose(velocity);

	Math::vec3f accVelocity = thisBody->GetLastFrameAcceleration() * duration;

	// Obliczenie prędkości w przestrzeni kontaktu
	accVelocity = contactToWorld.TransformTranspose(accVelocity);

	accVelocity.x = 0;

	contactVelocity += accVelocity;

	return contactVelocity;
}

void Contact::CalculateDesiredDeltaVelocity(float duration) {
	const static float velocityLimit = 0.25f;

	float velocityFromAcc = 0;

	if (body[0]->GetAwake())
		velocityFromAcc += (body[0]->GetLastFrameAcceleration() * duration).DotProduct(contactNormal);

	if (body[1] && body[1]->GetAwake())
		velocityFromAcc -= (body[1]->GetLastFrameAcceleration() * duration).DotProduct(contactNormal);

	float thisRestitution = restitution;
	if (std::abs(contactVelocity.x) < velocityLimit)
		thisRestitution = 0.0f;

	desiredDeltaVelocity = -contactVelocity.x - thisRestitution * (contactVelocity.x - velocityFromAcc);
}

void Contact::CalculateInternals(float duration) {
	// Sprawdzenie czy pierwszy obiekt jest NULLem, jeżeli tak to zamiana.
	if (!body[0])
		SwapBodies();

	if (!body[0])
		return;

	// Wyliczenie i ustawienie osi w miejscu kontaktu.
	CalculateContactBasis();

	// Przechowanie pozycji względem drugiego kontaktu
	relativeContactPosition[0] = contactPoint - body[0]->GetPosition();

	if (body[1])
		relativeContactPosition[1] = contactPoint - body[1]->GetPosition();

	// Wyszukanie względnej prędkości ciał w miejscu kontaktu
	contactVelocity = CalculateLocalVelocity(0, duration);

	if (body[1])
		contactVelocity -= CalculateLocalVelocity(1, duration);

	// Wyliczenie zmiany prędkości dla rozwiązania
	CalculateDesiredDeltaVelocity(duration);
}

void Contact::ApplyVelocityChange(NGE::Math::vec3f velocityChange[], NGE::Math::vec3f rotationChange[]) {
	// Pobranie odwróconej masy i odwróconego tensora bezwładności, obu we przestrzeni świata.
	Math::mat3f inverseInertiaTensor[2];
	body[0]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
	if (body[1])
		body[1]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

	// Obliczenie impulsu dla każdej z osi kontaktu
	Math::vec3f impulseContact;

	if (friction == 0.0f)
		// Użycie metody dla kontaktów bez tarcia
		impulseContact = CalculateFricionlessImpulse(inverseInertiaTensor);
	else
		// Bardziej skomplikowana wersja
		impulseContact = CalculateFrictionImpulse(inverseInertiaTensor);

	// Konwersja impulsu do przestrzeni świata
	Math::vec3f impulse = contactToWorld * impulseContact;

	// Podzielenie impulsu na komponenty: liniowy i rotację
	Math::vec3f impulsiveTorque = relativeContactPosition[0].CrossProduct(impulse);
	rotationChange[0] = inverseInertiaTensor[0] * impulsiveTorque;
	velocityChange[0].Clear();
	velocityChange[0] += impulse * body[0]->GetInverseMass();

	// Zastosowanie zmian
	body[0]->AddVelocity(velocityChange[0]);
	body[0]->AddRotation(rotationChange[0]);

	if (body[1]) {
		// Obliczenie liniowej i kątowej zmiany dla drugiego obiektu
		Math::vec3f impulsiveTorque = impulse.CrossProduct(relativeContactPosition[1]);
		rotationChange[1] = inverseInertiaTensor[1] * impulsiveTorque;
		velocityChange[1].Clear();
		velocityChange[1] += impulse * -body[1]->GetInverseMass();

		// Zastosowanie zmian
		body[1]->AddVelocity(velocityChange[1]);
		body[1]->AddRotation(rotationChange[1]);
	}
}

inline NGE::Math::vec3f Contact::CalculateFricionlessImpulse(NGE::Math::mat3f* inverseInertiaTensor) {
	Math::vec3f impulseContact;

	// Zbudowanie wektora będącego zmianą prędkości w przestrzeni świata,
	// dla pojedynczego impulsu w kierunku normalnej impulsu.
	Math::vec3f deltaVelWorld = relativeContactPosition[0].CrossProduct(contactNormal);
	deltaVelWorld = inverseInertiaTensor[0] * deltaVelWorld;
	deltaVelWorld = deltaVelWorld.CrossProduct(relativeContactPosition[0]);

	// Obliczenie zmiany prędkości w przestrzeni kontaktu
	float deltaVelocity = deltaVelWorld.DotProduct(contactNormal);

	// Dodanie liniowej części zmiany prędkości
	deltaVelocity += body[0]->GetInverseMass();

	// Sprawdzenie czy potrzebujemy informacji z drugiego obiektu
	if (body[1]) {
		// Przejście przez transformację raz jeszcze.
		Math::vec3f deltaVelWorld = relativeContactPosition[1].CrossProduct(contactNormal);
		deltaVelWorld = inverseInertiaTensor[1] * deltaVelWorld;
		deltaVelWorld = deltaVelWorld.CrossProduct(relativeContactPosition[1]);

		// Dodanie zmiany w prędkości względem rotacji
		deltaVelocity += deltaVelWorld.DotProduct(contactNormal);

		// Dodanie zmiany w prędkości względem liniowego ruchu
		deltaVelocity += body[1]->GetInverseMass();
	}

	// Obliczenie wymaganej wielkości impulsu
	impulseContact.x = desiredDeltaVelocity / deltaVelocity;
	impulseContact.y = 0;
	impulseContact.z = 0;
	return impulseContact;
}

inline NGE::Math::vec3f Contact::CalculateFrictionImpulse(NGE::Math::mat3f* inverseInertiaTensor) {
	Math::vec3f impulseContact;
	float inverseMass = body[0]->GetInverseMass();

	// Odpowiednikiem iloczynu wektorowego (cross product) w macierzach
	// jest pomnożenie przez macierz niesymetryczną (skew symetric).
	Math::mat3f impulseToTorque;
	impulseToTorque.SetSkewSymmetric(relativeContactPosition[0]);

	// Zbudowanie macierzy do konwersji impulsu kontaktu do zmiany w prędkości w przestrzeni świata
	Math::mat3f deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	// Sprawdzenie czy potrzebujemy informacji z drugiego obiektu
	if (body[1]) {
		// Ustawienie macierzy
		impulseToTorque.SetSkewSymmetric(relativeContactPosition[1]);

		// Obliczenie macierzy zmiany prędkości
		Math::mat3f deltaVelWorld2 = impulseToTorque;
		deltaVelWorld2 *= inverseInertiaTensor[1];
		deltaVelWorld2 *= impulseToTorque;
		deltaVelWorld2 *= -1;

		deltaVelWorld += deltaVelWorld2;

		// Dodanie odwróconej masy
		inverseMass += body[1]->GetInverseMass();
	}

	Math::mat3f deltaVelocity = contactToWorld.Transpose();
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= contactToWorld;

	// Dodanie liniowej zmiany prędkości
	deltaVelocity[0] += inverseMass;
	deltaVelocity[4] += inverseMass;
	deltaVelocity[8] += inverseMass;

	Math::mat3f impulseMatrix = deltaVelocity.Invserse();

	Math::vec3f velKill(desiredDeltaVelocity, -contactVelocity.y, -contactVelocity.z);

	impulseContact = impulseMatrix * velKill;

	float planarImpulse = sqrt(impulseContact.y * impulseContact.y + impulseContact.z * impulseContact.z);

	if (planarImpulse > impulseContact.x * friction) {
		// Użycie dynamicznego tarcia
		impulseContact.y /= planarImpulse;
		impulseContact.z /= planarImpulse;

		impulseContact.x = deltaVelocity[0] +
				deltaVelocity[3] * friction * impulseContact.y +
				deltaVelocity[6] * friction * impulseContact.z;

		impulseContact.x = desiredDeltaVelocity / impulseContact.x;
		impulseContact.y *= friction * impulseContact.x;
		impulseContact.z *= friction * impulseContact.x;
	}

	return impulseContact;
}

void Contact::ApplyPositionChange(NGE::Math::vec3f linearChange[], NGE::Math::vec3f angularChange[], float penetration) {
	const float angularLimit = 0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	// Obliczenie bezwładności każdego obiektu w kierunku normalnej kontaku, względem bezwładności kątowej.
	for (unsigned i = 0; i < 2; ++i) {
		if (body[i]) {
			Math::mat3f inverseInertiaTensor;
			body[i]->GetInverseInertiaTensor(&inverseInertiaTensor);

			Math::vec3f angularInertiaWorld = relativeContactPosition[i].CrossProduct(contactNormal);
			angularInertiaWorld = inverseInertiaTensor * angularInertiaWorld;
			angularInertiaWorld = angularInertiaWorld.CrossProduct(relativeContactPosition[i]);
			angularInertia[i] = angularInertiaWorld.DotProduct(contactNormal);

			// Liniowy komponent to odwrócona masa
			linearInertia[i] = body[i]->GetInverseMass();

			totalInertia += linearInertia[i] + angularInertia[i];
		}
	}

	// Zastosowanie zmian
	for (unsigned i = 0; i < 2; ++i) {
		if (body[i]) {
			float sign = (i == 0) ? 1.0f : -1.0f;
			angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
			linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

			Math::vec3f projection = relativeContactPosition[i];
			projection += contactNormal * (-relativeContactPosition[i].DotProduct(contactNormal));

			float maxMagnitude = angularLimit * projection.Length();

			if (angularMove[i] < -maxMagnitude) {
				float totalMove = angularMove[i] + linearMove[i];
				angularMove[i] = -maxMagnitude;
				linearMove[i] = totalMove - angularMove[i];
			} else if (angularMove[i] > maxMagnitude) {
				float totalMove = angularMove[i] + linearMove[i];
				angularMove[i] = maxMagnitude;
				linearMove[i] = totalMove - angularMove[i];
			}

			if (angularMove[i] == 0)
				angularChange[i].Clear();
			else {
				// Wyliczenie kierunku w którym chcemy obracać obiekt
				Math::vec3f targetAngularDirection = relativeContactPosition[i].CrossProduct(contactNormal);

				Math::mat3f inverseInertiaTensor;
				body[i]->GetInverseInertiaTensor(&inverseInertiaTensor);

				angularChange[i] = (inverseInertiaTensor * targetAngularDirection) * (angularMove[i] / angularInertia[i]);
			}

			// Zmiana prędkości
			linearChange[i] = contactNormal * linearMove[i];

			// Zastosowanie zmian w ruchu
			Math::vec3f pos;
			body[i]->GetPosition(&pos);
			pos += contactNormal * linearMove[i];
			body[i]->SetPosition(pos);

			// Zastosowanie zmian w orientacji
			Math::quaternionf q;
			body[i]->GetOrientation(&q);
			q.AddScaledVector(angularChange[i], 1.0f);
			body[i]->SetOrientation(q);

			if (!body[i]->GetAwake())
				body[i]->CalculateDerivedData();
		}
	}
}
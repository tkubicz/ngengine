#include <cfloat>

#include "NGE/Physics/RigidBody/RigidBody.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::Physics::RigidBody;

const float RigidBody::sleepEpsilon = 0.1f;

void RigidBody::TransformInertiaTensor(NGE::Math::mat3f& iitWorld, const NGE::Math::quaternionf& q, const NGE::Math::mat3f& iitBody, const NGE::Math::mat4f& rotmat) {
    float t4 = rotmat[0] * iitBody[0] + rotmat[4] * iitBody[1] + rotmat[8] * iitBody[2];
    float t9 = rotmat[0] * iitBody[3] + rotmat[4] * iitBody[4] + rotmat[8] * iitBody[5];
    float t14 = rotmat[0] * iitBody[6] + rotmat[4] * iitBody[7] + rotmat[8] * iitBody[8];
    float t28 = rotmat[1] * iitBody[0] + rotmat[5] * iitBody[1] + rotmat[9] * iitBody[2];
    float t33 = rotmat[1] * iitBody[3] + rotmat[5] * iitBody[4] + rotmat[9] * iitBody[5];
    float t38 = rotmat[1] * iitBody[6] + rotmat[5] * iitBody[7] + rotmat[9] * iitBody[8];
    float t52 = rotmat[2] * iitBody[0] + rotmat[6] * iitBody[1] + rotmat[10] * iitBody[2];
    float t57 = rotmat[2] * iitBody[3] + rotmat[6] * iitBody[4] + rotmat[10] * iitBody[5];
    float t62 = rotmat[2] * iitBody[6] + rotmat[6] * iitBody[7] + rotmat[10] * iitBody[8];

    iitWorld[0] = t4 * rotmat[0] + t9 * rotmat[4] + t14 * rotmat[8];
    iitWorld[3] = t4 * rotmat[1] + t9 * rotmat[5] + t14 * rotmat[9];
    iitWorld[6] = t4 * rotmat[2] + t9 * rotmat[6] + t14 * rotmat[10];
    iitWorld[1] = t28 * rotmat[0] + t33 * rotmat[4] + t38 * rotmat[8];
    iitWorld[4] = t28 * rotmat[1] + t33 * rotmat[5] + t38 * rotmat[9];
    iitWorld[7] = t28 * rotmat[2] + t33 * rotmat[6] + t38 * rotmat[10];
    iitWorld[2] = t52 * rotmat[0] + t57 * rotmat[4] + t62 * rotmat[8];
    iitWorld[5] = t52 * rotmat[1] + t57 * rotmat[5] + t62 * rotmat[9];
    iitWorld[8] = t52 * rotmat[2] + t57 * rotmat[6] + t62 * rotmat[10];
}

void RigidBody::CalculateTransformMatrix(NGE::Math::mat4f& transformMatrix, const NGE::Math::vec3f& position, const NGE::Math::quaternionf& orientation) {
    transformMatrix[0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
    transformMatrix[4] = 2 * orientation.i * orientation.j - 2 * orientation.r * orientation.k;
    transformMatrix[8] = 2 * orientation.i * orientation.k + 2 * orientation.r * orientation.j;
    transformMatrix[12] = position.x;

    transformMatrix[1] = 2 * orientation.i * orientation.j + 2 * orientation.r * orientation.k;
    transformMatrix[5] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k;
    transformMatrix[9] = 2 * orientation.j * orientation.k - 2 * orientation.r * orientation.i;
    transformMatrix[13] = position.y;

    transformMatrix[2] = 2 * orientation.i * orientation.k - 2 * orientation.r * orientation.j;
    transformMatrix[6] = 2 * orientation.j * orientation.k + 2 * orientation.r * orientation.i;
    transformMatrix[10] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j;
    transformMatrix[14] = position.z;

    transformMatrix[3] = transformMatrix[7] = transformMatrix[11] = 0;
    transformMatrix[15] = 1;
}

void RigidBody::CalculateDerivedData() {
    orientation.Normalize();

    // Wyliczenie macierzy transformacji dla ciała.
    CalculateTransformMatrix(transformMatrix, position, orientation);

    // Wyliczenie tensora bezwładności w przestrzeni świata.
    TransformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
}

void RigidBody::Integrate(float duration) {
    if (!isAwake)
        return;

    // Wyliczenie liniowego przyśpieszenia z podanych sił.
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration += forceAccum * inverseMass;

    // Wyliczenie przyśpieszenia kątowego z tarcia.
    Math::vec3f angularAcceleration = inverseInertiaTensorWorld * torqueAccum;

    // Modyfikacja prędkości
    // Aktualizacja liniowej prędkości z przyśpieszenia i impulsu.
    velocity += lastFrameAcceleration * duration;

    // Aktualizacja prędkości kątowej z przyśpieszenia i impulsu.
    rotation += angularAcceleration * duration;

    // Opór
    velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);

    // Modyfikacja pozycji
    // Aktualizcja liniowej pozycji.
    position += velocity * duration;

    // Aktualizacja pozycji kątowej.
    orientation.AddScaledVector(rotation, duration);

    // Normalizacja orientacji i aktualizacja macierzy nową pozycją i orientacją.
    CalculateDerivedData();

    // Wyczyszczenie akumulatorów.
    ClearAccumulators();

    // Aktualizacja energii kinetycznej i jeżeli to możliwe uśpienie ciała
    if (canSleep) {
        float currentMotion = velocity.DotProduct(velocity) + rotation.DotProduct(rotation);
        float bias = std::pow(0.5f, duration);
        motion = bias * motion + (1 - bias) * currentMotion;

        if (motion < sleepEpsilon)
            SetAwake(false);
        else if (motion > 10 * sleepEpsilon)
            motion = 10 * sleepEpsilon;
    }
}

void RigidBody::SetMass(const float mass) {
    if (mass == 0.0f) {
        log_error("Mass cannot equals zero");
        return;
    }

    inverseMass = 1.0f / mass;
}

float RigidBody::GetMass() const {
    if (inverseMass == 0) {
        return FLT_MAX;
    } else {
        return 1.0f / inverseMass;
    }
}

void RigidBody::SetInverseMass(const float inverseMass) {
    this->inverseMass = inverseMass;
}

float RigidBody::GetInverseMass() const {
    return inverseMass;
}

bool RigidBody::HasFiniteMass() const {
    return inverseMass >= 0.0f;
}

void RigidBody::SetInertiaTensor(const NGE::Math::mat3f& inertiaTensor) {
    inverseInertiaTensor = inertiaTensor;
    inverseInertiaTensor.SetInverse();
}

void RigidBody::GetInertiaTensor(NGE::Math::mat3f* inertiaTensor) const {
    inertiaTensor->Set(inverseInertiaTensor);
    inertiaTensor->SetInverse();
}

NGE::Math::mat3f RigidBody::GetInertiaTensor() const {
    Math::mat3f it;
    GetInertiaTensor(&it);
    return it;
}

void RigidBody::GetInertiaTensorWorld(NGE::Math::mat3f* inertiaTensor) const {
    inertiaTensor->Set(inverseInertiaTensorWorld);
    inertiaTensor->SetInverse();
}

NGE::Math::mat3f RigidBody::GetInertiaTensorWorld() const {
    Math::mat3f it;
    GetInertiaTensorWorld(&it);
    return it;
}

void RigidBody::SetInverseInertiaTensor(const NGE::Math::mat3f& inverseInertiaTensor) {
    this->inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::GetInverseInertiaTensor(NGE::Math::mat3f* inverseInertiaTensor) const {
    *inverseInertiaTensor = this->inverseInertiaTensor;
}

NGE::Math::mat3f RigidBody::GetInverseInertiaTensor() const {
    return inverseInertiaTensor;
}

void RigidBody::GetInverseInertiaTensorWorld(NGE::Math::mat3f* inverseInertiaTensor) const {
    *inverseInertiaTensor = inverseInertiaTensorWorld;
}

NGE::Math::mat3f RigidBody::GetInverseInertiaTensorWorld() const {
    return inverseInertiaTensorWorld;
}

void RigidBody::SetDamping(const float linearDamping, const float angularDamping) {
    this->linearDamping = linearDamping;
    this->angularDamping = angularDamping;
}

void RigidBody::SetLinearDamping(const float linearDamping) {
    this->linearDamping = linearDamping;
}

float RigidBody::GetLinearDamping() const {
    return linearDamping;
}

void RigidBody::SetAngularDamping(const float angularDamping) {
    this->angularDamping = angularDamping;
}

float RigidBody::GetAngularDamping() const {
    return angularDamping;
}

void RigidBody::SetPosition(const NGE::Math::vec3f& position) {
    this->position = position;
}

void RigidBody::SetPosition(const float x, const float y, const float z) {
    position.Set(x, y, z);
}

void RigidBody::GetPosition(NGE::Math::vec3f* position) const {
    *position = this->position;
}

NGE::Math::vec3f RigidBody::GetPosition() const {
    return position;
}

void RigidBody::SetOrientation(const NGE::Math::quaternionf& orientation) {
    this->orientation = orientation;
    this->orientation.Normalize();
}

void RigidBody::SetOrientation(const float r, const float i, const float j, const float k) {
    orientation.r = r;
    orientation.i = i;
    orientation.j = j;
    orientation.k = k;
    orientation.Normalize();
}

void RigidBody::GetOrientation(NGE::Math::quaternionf* orientation) const {
    *orientation = this->orientation;
}

NGE::Math::quaternionf RigidBody::GetOrientation() const {
    return orientation;
}

void RigidBody::GetOrientation(NGE::Math::mat3f* matrix) const {
    // TODO: Uzupełnić pobieranie orientacji
}

void RigidBody::GetOrientation(float matrix[9]) const {
    matrix[0] = transformMatrix[0];
    matrix[1] = transformMatrix[1];
    matrix[2] = transformMatrix[2];

    matrix[3] = transformMatrix[4];
    matrix[4] = transformMatrix[5];
    matrix[5] = transformMatrix[6];

    matrix[6] = transformMatrix[8];
    matrix[7] = transformMatrix[9];
    matrix[8] = transformMatrix[10];
}

void RigidBody::GetTransform(NGE::Math::mat4f* transform) const {
    *transform = this->transformMatrix;
}

void RigidBody::GetTransform(float matrix[16]) const {
    // TODO: Dopisać
}

NGE::Math::mat4f RigidBody::GetTransform() const {
    return transformMatrix;
}

NGE::Math::vec3f RigidBody::GetPointInLocalSpace(const NGE::Math::vec3f& point) const {
    return transformMatrix.TransformInverse(point);
}

NGE::Math::vec3f RigidBody::GetPointInWorldSpace(const NGE::Math::vec3f& point) const {
    return transformMatrix * point;
}

NGE::Math::vec3f RigidBody::GetDirectionInLocalSpace(const NGE::Math::vec3f& direction) const {
    return transformMatrix.TransformInverseDirection(direction);
}

NGE::Math::vec3f RigidBody::GetDirectionInWorldSpace(const NGE::Math::vec3f& direction) const {
    return transformMatrix.TransformDirection(direction);
}

void RigidBody::SetVelocity(const NGE::Math::vec3f& velocity) {
    this->velocity = velocity;
}

void RigidBody::SetVelocity(const float x, const float y, const float z) {
    velocity.Set(x, y, z);
}

void RigidBody::GetVelocity(NGE::Math::vec3f* velocity) const {
    *velocity = this->velocity;
}

NGE::Math::vec3f RigidBody::GetVelocity() const {
    return velocity;
}

void RigidBody::AddVelocity(const NGE::Math::vec3f& deltaVelocity) {
    velocity += deltaVelocity;
}

void RigidBody::SetRotation(const NGE::Math::vec3f& rotation) {
    this->rotation = rotation;
}

void RigidBody::SetRotation(const float x, const float y, const float z) {
    rotation.Set(x, y, z);
}

void RigidBody::GetRotation(NGE::Math::vec3f* rotation) const {
    *rotation = this->rotation;
}

NGE::Math::vec3f RigidBody::GetRotation() const {
    return rotation;
}

void RigidBody::AddRotation(const NGE::Math::vec3f& deltaRotation) {
    rotation += deltaRotation;
}

bool RigidBody::GetAwake() const {
    return isAwake;
}

void RigidBody::SetAwake(const bool awake) {
    if (awake) {
        isAwake = true;

        // Dodanie trochę ruchu w celu uniknięcia uśpienia natchmiastowo.
        motion = sleepEpsilon * 2.0f;
    } else {
        isAwake = false;
        velocity.Clear();
        rotation.Clear();
    }
}

void RigidBody::SetCanSleep(const bool canSleep) {
    this->canSleep = canSleep;

    if (!canSleep && !isAwake)
        SetAwake();
}

void RigidBody::GetLastFrameAcceleration(NGE::Math::vec3f* linearAcceleration) const {
    *linearAcceleration = lastFrameAcceleration;
}

NGE::Math::vec3f RigidBody::GetLastFrameAcceleration() const {
    return lastFrameAcceleration;
}

void RigidBody::ClearAccumulators() {
    forceAccum.Clear();
    torqueAccum.Clear();
}

void RigidBody::AddForce(const NGE::Math::vec3f& force) {
    forceAccum += force;
    isAwake = true;
}

void RigidBody::AddForceAtBodyPoint(const NGE::Math::vec3f& force, const NGE::Math::vec3f& point) {
    // Konwersja współrzędnych względem środka ciężkości
    Math::vec3f pt = GetPointInWorldSpace(point);
    AddForceAtPoint(force, pt);

    isAwake = true;
}

void RigidBody::AddForceAtPoint(const NGE::Math::vec3f& force, const NGE::Math::vec3f& point) {
    // Konwersja współrzędnych względem środka ciężkości
    Math::vec3f pt = point;
    pt -= position;

    forceAccum += force;
    torqueAccum += pt.CrossProduct(force);

    isAwake = true;
}

void RigidBody::AddTorque(const NGE::Math::vec3f& torque) {
    torqueAccum += torque;
    isAwake = true;
}

void RigidBody::SetAcceleration(const NGE::Math::vec3f& acceleration) {
    this->acceleration = acceleration;
}

void RigidBody::SetAcceleration(const float x, const float y, const float z) {
    acceleration.Set(x, y, z);
}

void RigidBody::GetAcceleration(NGE::Math::vec3f* acceleration) const {
    *acceleration = this->acceleration;
}

NGE::Math::vec3f RigidBody::GetAcceleration() const {
    return acceleration;
}




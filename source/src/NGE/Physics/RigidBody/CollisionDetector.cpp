#include <float.h>

#include "NGE/Physics/RigidBody/CollisionDetector.hpp"
#include "NGE/Physics/RigidBody/IntersectionTests.hpp"

using namespace NGE::Physics::RigidBody;

unsigned CollisionDetector::SphereAndTruePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
    // Mamy kontakty
    if (data->contactsLeft <= 0)
        return 0;

    // Pozycja kuli
    Math::vec3f position = sphere.GetAxis(3);

    // Odległość od płaszczyzny
    float centreDistance = plane.direction.DotProduct(position) - plane.offset;

    // Sprawdzenie czy jesteśmy w promieniu
    if (centreDistance * centreDistance > sphere.radius * sphere.radius)
        return 0;

    // Sprawdzenie po której stronie płaszczyzny się znajdujemy
    Math::vec3f normal = plane.direction;
    float penetration = -centreDistance;
    if (centreDistance < 0)
    {
        normal *= -1;
        penetration = -penetration;
    }
    penetration += sphere.radius;

    // Utworzenie kontaktu
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = position - plane.direction * centreDistance;
    contact->SetBodyData(sphere.body, NULL, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
    // Sprawdzenie czy mamy kontakty
    if (data->contactsLeft <= 0)
        return 0;

    // Pozycja kuli
    Math::vec3f position = sphere.GetAxis(3);

    // Znalezienie odleglosci od płaszczyzny
    float ballDistance = plane.direction.DotProduct(position) - sphere.radius - plane.offset;

    if (ballDistance >= 0)
        return 0;

    // Utworzenie kontaktu
    Contact* contact = data->contacts;
    contact->contactNormal = plane.direction;
    contact->penetration = -ballDistance;
    contact->contactPoint = position - plane.direction * (ballDistance + sphere.radius);
    contact->SetBodyData(sphere.body, NULL, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data)
{

    // Sprawdzenie czy mamy kontakty
    if (data->contactsLeft <= 0)
        return 0;

    // Pozycje kul
    Math::vec3f positionOne = one.GetAxis(3);
    Math::vec3f positionTwo = two.GetAxis(3);

    // Znalezienie wektora pomiędzy obiektami
    Math::vec3f midline = positionOne - positionTwo;
    float size = midline.Length();

    // Czy wektor jest wystarczająco duży
    if (size <= 0.0f || size >= one.radius + two.radius)
        return 0;

    // Ręczne utworzenie normalnej
    Math::vec3f normal = midline * (1.0f / size);

    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = positionOne + midline * 0.5f;
    contact->penetration = (one.radius + two.radius - size);
    contact->SetBodyData(one.body, two.body, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::BoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    // Check for intersection
    if (!IntersectionTests::BoxAndHalfSpace(box, plane))
    {
        return 0;
    }

    // We have an intersection, so find the intersection points. We can make
    // do with only checking vertices. If the box is resting on a plane
    // or on an edge, it will be reported as four or two contact points.

    // Go through each combination of + and - for each half-size
    static float mults[8][3] = {
        {1, 1, 1},
        {-1, 1, 1},
        {1, -1, 1},
        {-1, -1, 1},
        {1, 1, -1},
        {-1, 1, -1},
        {1, -1, -1},
        {-1, -1, -1}
    };

    Contact* contact = data->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++)
    {

        // Calculate the position of each vertex
        Math::vec3f vertexPos(mults[i][0], mults[i][1], mults[i][2]);
        vertexPos.ComponentProductUpdate(box.halfSize);
        vertexPos = box.transform * vertexPos;

        // Calculate the distance from the plane
        float vertexDistance = vertexPos.DotProduct(plane.direction);

        // Compare this to the plane's distance
        if (vertexDistance <= plane.offset)
        {
            // Create the contact data.

            // The contact point is halfway between the vertex and the
            // plane - we multiply the direction by half the separation
            // distance and add the vertex location.
            contact->contactPoint = plane.direction;
            contact->contactPoint *= (vertexDistance - plane.offset);
            contact->contactPoint = vertexPos;
            contact->contactNormal = plane.direction;
            contact->penetration = plane.offset - vertexDistance;

            // Write the appropriate data
            contact->SetBodyData(box.body, NULL,
                                 data->friction, data->restitution);

            // Move onto the next contact
            contact++;
            contactsUsed++;
            if (contactsUsed == (unsigned) data->contactsLeft) 
                return contactsUsed;
        }
    }

    data->AddContacts(contactsUsed);
    return contactsUsed;
}

unsigned CollisionDetector::BoxAndBox(const CollisionBox& one, const CollisionBox& two, CollisionData* data)
{
    //if (!IntersectionTests::BoxAndBox(one, two))
    //    return 0;

    // Znalezienie wektora pomiędzy dwoma środkami
    Math::vec3f toCenter = two.GetAxis(3) - one.GetAxis(3);

    // Zaczynamy zakładając, że nie ma kontaktu
    float pen = FLT_MAX;
    unsigned best = 0xffffff;

    // Sprawdzamy każdą oś
    if (!TryAxis(one, two, one.GetAxis(0), toCenter, 0, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(1), toCenter, 1, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(2), toCenter, 2, pen, best)) return 0;

    if (!TryAxis(one, two, two.GetAxis(0), toCenter, 3, pen, best)) return 0;
    if (!TryAxis(one, two, two.GetAxis(1), toCenter, 4, pen, best)) return 0;
    if (!TryAxis(one, two, two.GetAxis(2), toCenter, 5, pen, best)) return 0;

    unsigned bestSingleAxis = best;

    if (!TryAxis(one, two, one.GetAxis(0).CrossProduct(two.GetAxis(0)), toCenter, 6, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(0).CrossProduct(two.GetAxis(1)), toCenter, 7, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(0).CrossProduct(two.GetAxis(2)), toCenter, 8, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(1).CrossProduct(two.GetAxis(0)), toCenter, 9, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(1).CrossProduct(two.GetAxis(1)), toCenter, 10, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(1).CrossProduct(two.GetAxis(2)), toCenter, 11, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(2).CrossProduct(two.GetAxis(0)), toCenter, 12, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(2).CrossProduct(two.GetAxis(1)), toCenter, 13, pen, best)) return 0;
    if (!TryAxis(one, two, one.GetAxis(2).CrossProduct(two.GetAxis(2)), toCenter, 14, pen, best)) return 0;

    // Upewnienie się, że mamy wynik
    if (best == 0xffffff)
        return -1;

    // Wiemy, że istnieje kolizja i wiemy, na której osi występuje najmniejsza penetracja.
    if (best < 3)
    {
        // Wierzchołek pudełka dwa na powierzchni pudełka jeden.
        FillPointFaceBoxBox(one, two, toCenter, data, best, pen);
        data->AddContacts(1);
        return 1;
    }
    else if (best < 6)
    {
        // Wierzchołek pudełka jeden na powierzchni pudełka dwa.
        // Używamy tego samego algorytmu co powyżej, ale zamieniamy
        // pudełka (więc również wektor pomiędzy nimi).
        FillPointFaceBoxBox(two, one, toCenter * -1.0f, data, best - 3, pen);
        data->AddContacts(1);
        return 1;
    }
    else
    {
        // Kontakt krawędziami. Znaleźnienie na których osiach.
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        Math::vec3f oneAxis = one.GetAxis(oneAxisIndex);
        Math::vec3f twoAxis = two.GetAxis(twoAxisIndex);
        Math::vec3f axis = oneAxis.CrossProduct(twoAxis);
        axis.Normalize();

        // Oś powinna wskazywać z pudełka jeden do pudełka dwa.
        if (axis.DotProduct(toCenter) > 0)
            axis = axis * -1.0f;

        // Mamy osie, ale nie krawędzie: każda oś posiada 4 krawędzie równoległe
        // do niej. Musimy znaleźć które 4 dla każdego obiektu. Robimy to przez
        // znalezienie punktu w środku krawędzi. TODO: Dalszy komentarz
        Math::vec3f ptOnOneEdge = one.halfSize;
        Math::vec3f ptOnTwoEdge = two.halfSize;
        for (unsigned i = 0; i < 3; ++i)
        {
            if (i == oneAxisIndex)
                ptOnOneEdge[i] = 0;
            else if (one.GetAxis(i).DotProduct(axis) > 0)
                ptOnOneEdge[i] = -ptOnOneEdge[i];

            if (i == twoAxisIndex)
                ptOnTwoEdge[i] = 0;
            else if (two.GetAxis(i).DotProduct(axis) < 0)
                ptOnTwoEdge[i] = -ptOnTwoEdge[i];
        }

        // Przeniesienie ich do przestrzeni świata (są w odpowiedniej orientacji
        // ponieważ są pochodnymi osi).
        ptOnOneEdge = one.transform * ptOnOneEdge;
        ptOnTwoEdge = two.transform * ptOnTwoEdge;

        // TODO: Komentarz
        Math::vec3f vertex = ContactPoint(ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex],
                                          ptOnTwoEdge, twoAxis, two.halfSize[twoAxisIndex],
                                          bestSingleAxis > 2);

        // Wypełnienie struktury kontaktu
        Contact* contact = data->contacts;

        contact->penetration = pen;
        contact->contactNormal = axis;
        contact->contactPoint = vertex;
        contact->SetBodyData(one.body, two.body, data->friction, data->restitution);
        data->AddContacts(1);
        return 1;
    }

    return 0;
}

unsigned CollisionDetector::BoxAndPoint(const CollisionBox& box, const NGE::Math::vec3f& point, CollisionData* data)
{
    // Transformacja współrzędnych punktu do przestrzeni pudełka
    Math::vec3f relPt = box.transform.TransformInverse(point);
    Math::vec3f normal;

    // Sprawdzenie każdej z osi, szukając tej na której penetracja jest najmniejsza
    float min_depth = box.halfSize.x - std::abs(relPt.x);
    if (min_depth < 0)
        return 0;
    normal = box.GetAxis(0) * (relPt.x < 0 ? -1.0f : 1.0f);

    float depth = box.halfSize.y - std::abs(relPt.y);
    if (depth < 0)
        return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.GetAxis(1) * (relPt.y < 0 ? -1.0f : 1.0f);
    }

    depth = box.halfSize.z - std::abs(relPt.z);
    if (depth < 0)
        return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.GetAxis(2) * (relPt.z < 0 ? -1.0f : 1.0f);
    }

    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->penetration = min_depth;

    // Nie wiemy do jakiego ciała sztywnego należy punkt,
    // wiec używamy NULL. 
    contact->SetBodyData(box.body, NULL, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::BoxAndSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data)
{
    // Transformacja centrum kuli w przestrzeń pudełka
    Math::vec3f center = sphere.GetAxis(3);
    Math::vec3f relCenter = box.transform.TransformInverse(center);
    
    // Sprawdzenie czy możemy wykluczyć kontakt
    if (std::abs(relCenter.x) - sphere.radius > box.halfSize.x ||
        std::abs(relCenter.y) - sphere.radius > box.halfSize.y ||
        std::abs(relCenter.z) - sphere.radius > box.halfSize.z)
        return 0;
    
    Math::vec3f closestPt(0, 0, 0);
    float dist;
    
    dist = relCenter.x;
    closestPt.x = Math::MathUtils::Clamp(dist, -box.halfSize.x, box.halfSize.x);
    
    dist = relCenter.y;
    closestPt.y = Math::MathUtils::Clamp(dist, -box.halfSize.y, box.halfSize.y);
    
    dist = relCenter.z;
    closestPt.z = Math::MathUtils::Clamp(dist, -box.halfSize.z, box.halfSize.z);
    
    // Sprawdzenie czy mamy kontakt
    dist = (closestPt - relCenter).LengthSquared();
    if (dist > sphere.radius * sphere.radius)
        return 0;
    
    Math::vec3f closestPtWorld = box.transform * closestPt;
    
    Contact* contact = data->contacts;
    contact->contactNormal = (closestPtWorld - center);
    contact->contactNormal.Normalize();
    contact->contactPoint = closestPtWorld;
    contact->penetration = sphere.radius - std::sqrt(dist);
    contact->SetBodyData(box.body, sphere.body, data->friction, data->restitution);
    
    data->AddContacts(1);
    return 1;
}

float CollisionDetector::PenetrationOnAxis(const CollisionBox& one, const CollisionBox& two, const NGE::Math::vec3f& axis, const NGE::Math::vec3f& toCenter)
{
    float oneProject = IntersectionTests::TransformToAxis(one, axis);
    float twoProject = IntersectionTests::TransformToAxis(two, axis);

    float distance = std::abs(toCenter.DotProduct(axis));

    return oneProject + twoProject - distance;
}

bool CollisionDetector::TryAxis(const CollisionBox& one, const CollisionBox& two, const NGE::Math::vec3f& axis2, const NGE::Math::vec3f& toCenter,
                                unsigned index, float& smallestPentration, unsigned &smallestCase)
{
    Math::vec3f axis = axis2;

    // Nie sprawdzamy prawie równoległych osi
    if (axis.LengthSquared() < 0.0001)
        return true;

    axis.Normalize();

    float penetration = PenetrationOnAxis(one, two, axis, toCenter);

    if (penetration < 0) return false;
    if (penetration < smallestPentration)
    {
        smallestPentration = penetration;
        smallestCase = index;
    }

    return true;
}

void CollisionDetector::FillPointFaceBoxBox(const CollisionBox& one, const CollisionBox& two, const NGE::Math::vec3f& toCenter, CollisionData* data, unsigned best, float pen)
{
    Contact* contact = data->contacts;

    Math::vec3f normal = one.GetAxis(best);
    if (one.GetAxis(best).DotProduct(toCenter) > 0)
        normal = normal * -1.0f;

    Math::vec3f vertex = two.halfSize;
    if (two.GetAxis(0).DotProduct(normal) < 0)
        vertex.x = -vertex.x;
    if (two.GetAxis(1).DotProduct(normal) < 0)
        vertex.y = -vertex.y;
    if (two.GetAxis(2).DotProduct(normal) < 0)
        vertex.z = -vertex.z;

    contact->contactNormal = normal;
    contact->penetration = pen;
    contact->contactPoint = two.GetTransform() * vertex;
    contact->SetBodyData(one.body, two.body, data->friction, data->restitution);
}

NGE::Math::vec3f CollisionDetector::ContactPoint(const NGE::Math::vec3f& pOne, const NGE::Math::vec3f& dOne, float oneSize, const NGE::Math::vec3f& pTwo, const NGE::Math::vec3f& dTwo,
                                                 float twoSize, bool useOne)
{
    Math::vec3f toSt, cOne, cTwo;
    float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    float denom, mua, mub;

    smOne = dOne.LengthSquared();
    smTwo = dTwo.LengthSquared();
    dpOneTwo = dTwo.DotProduct(dOne);

    toSt = pOne - pTwo;
    dpStaOne = dOne.DotProduct(toSt);
    dpStaTwo = dTwo.DotProduct(toSt);

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    if (std::abs(denom) < 0.0001f)
        return useOne ? pOne : pTwo;

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    if (mua > oneSize || mua < -oneSize || mub > twoSize || mub < -twoSize)
        return useOne ? pOne : pTwo;
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;
        return cOne * 0.5 + cTwo * 0.5;
    }
}






template <typename T> AABox<T>::AABox()
{
    origin = Math::Vector3<T>(0, 0, 0);
    halfSize = Math::Vector3<T>(1, 1, 1);
}

template <typename T> AABox<T>::AABox(const Math::Vector3<T>& origin, const Math::Vector3<T>& halfSize)
{
    SetBox(origin, halfSize);
}

template <typename T> void AABox<T>::SetBox(const Math::Vector3<T>& origin, const Math::Vector3<T>& halfSize)
{
    this->origin = origin;
    this->halfSize = halfSize;
}

template <typename T> Math::Vector3<T> AABox<T>::GetPositiveVertex(const Math::Vector3<T>& normal) const
{
    Math::Vector3<T> result(origin.x - halfSize.x, origin.y - halfSize.y, origin.z - halfSize.z);
    
    if (normal.x > 0)
        result.x += halfSize.x * 2.0;
    
    if (normal.y > 0)
        result.y += halfSize.y * 2.0;
    
    if (normal.z > 0)
        result.z += halfSize.z * 2.0;

    return result;
}

template <typename T> Math::Vector3<T> AABox<T>::GetNegativeVertex(const Math::Vector3<T>& normal) const
{
    Math::Vector3<T> result(origin.x - halfSize.x, origin.y - halfSize.y, origin.z - halfSize.z);
    
    if (normal.x < 0)
        result.x += halfSize.x * 2.0;
    
    if (normal.y < 0)
        result.y += halfSize.y * 2.0;
    
    if (normal.z < 0)
        result.z += halfSize.z * 2.0;
    
    return result;
}

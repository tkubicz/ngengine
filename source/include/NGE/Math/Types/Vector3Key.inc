template <typename T> bool Vector3Key<T>::operator ==(const Vector3Key<T>& other) const
{
    return value == other.GetValue();
}

template <typename T> bool Vector3Key<T>::operator !=(const Vector3Key<T>& other) const
{
    return value != other.GetValue();
}

template <typename T> bool Vector3Key<T>::operator<(const Vector3Key<T>& other) const
{
    return time < other.GetTime();
}

template <typename T> bool Vector3Key<T>::operator>(const Vector3Key<T>& other) const
{
    return time > other.GetTime();
}

template <typename T> T Vector3Key<T>::GetTime() const
{
    return time;
}

template <typename T> Vector3<T> Vector3Key<T>::GetValue() const
{
    return value;
}

template <typename T> void Vector3Key<T>::SetTime(T time)
{
    this->time = time;
}

template <typename T> void Vector3Key<T>::SetValue(const Vector3<T>& value)
{
    this->value = value;
}
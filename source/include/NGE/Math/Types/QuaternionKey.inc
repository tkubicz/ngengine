template <typename T> bool QuaternionKey<T>::operator ==(const QuaternionKey<T>& other) const
{
    return value == other.GetValue();
}

template <typename T> bool QuaternionKey<T>::operator !=(const QuaternionKey<T>& other) const
{
    return value != other.GetValue();
}

template <typename T> bool QuaternionKey<T>::operator<(const QuaternionKey<T>& other) const
{
    return time < other.GetTime();
}

template <typename T> bool QuaternionKey<T>::operator>(const QuaternionKey<T>& other) const
{
    return time > other.GetTime();
}

template <typename T> T QuaternionKey<T>::GetTime() const
{
    return time;
}

template <typename T> Quaternion<T> QuaternionKey<T>::GetValue() const
{
    return value;
}

template <typename T> void QuaternionKey<T>::SetTime(T time)
{
    this->time = time;
}

template <typename T> void QuaternionKey<T>::SetValue(const Quaternion<T>& value)
{
    this->value = value;
}
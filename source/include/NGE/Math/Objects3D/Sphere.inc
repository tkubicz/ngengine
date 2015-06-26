template <typename T> Sphere<T>::Sphere() {
	center.Set(0, 0, 0);
	radius = 1;
}

template <typename T> Sphere<T>::Sphere(const Vector3<T>& center, T radius) : center(center), radius(radius) { }

template <typename T> Sphere<T>::~Sphere() { }

template <typename T> Math::Vector3<T> Sphere<T>::GetCenter() {
	return center;
}

template <typename T> Sphere<T>& Sphere<T>::operator=(const Sphere<T>& copy) {
	this->center = copy.center;
	this->radius = copy.radius;

	return *this;
}

template <typename T> void Sphere<T>::SetCenter(const Vector3<T>& center) {
	this->center = center;
}

template <typename T> T Sphere<T>::GetRadius() {
	return radius;
}

template <typename T> void Sphere<T>::SetRadius(const T radius) {
	this->radius = radius;
}
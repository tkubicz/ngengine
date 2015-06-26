template <typename T> Plane<T>::Plane(T a, T b, T c, T d) {
	SetCoefficients(a, b, c, d);
}

template <typename T> Plane<T>::Plane(const Math::Vector3<T>& p1, const Math::Vector3<T>& p2, const Math::Vector3<T>& p3) {
	SetPoints(p1, p2, p3);
}

template <typename T> Plane<T>::Plane(const Math::Vector3<T>& normal, const Math::Vector3<T>& point) {
	SetNormalAndPoint(normal, point);
}

template<typename T> Plane<T>& Plane<T>::operator=(const Plane<T>& p) {
	a = p[0];
	b = p[1];
	c = p[2];
	d = p[3];
	return* this;
}

template<typename T> const T Plane<T>::operator[](unsigned i) const {
	return ((T*) & a)[i];
}

template<typename T> T& Plane<T>::operator[](unsigned i) {
	return ((T*) & a)[i];
}

template <typename T> void Plane<T>::SetPoints(const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3) {
	// Calculate normal vector from three points.
	Math::Vector3<T> aux1, aux2;
	aux1 = p1 - p2;
	aux2 = p3 - p2;

	Math::Vector3<T> normal = aux2.CrossProduct(aux1);
	normal.Normalize();

	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = -(normal.DotProduct(p2));
}

template<typename T> void Plane<T>::SetNormalAndPoint(const Math::Vector3<T>& normal, const Math::Vector3<T>& point) {
	Math::Vector3<T> copy = normal;
	copy.Normalize();
	a = copy.x;
	b = copy.y;
	c = copy.z;
	d = -(copy.DotProduct(point));
}

template<typename T> void Plane<T>::SetCoefficients(T a, T b, T c, T d) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

template<typename T> T Plane<T>::GetDistance(const Math::Vector3<T>& p) const {
	Math::Vector3<T> normal(a, b, c);
	return (d + normal.DotProduct(p));
}

template <typename T> NGE::Math::Vector3<T> Plane<T>::GetNormal() const {
	return Math::Vector3<T>(a, b, c);
}

template <typename T> std::ostream &operator<<(std::ostream &out, const Plane<T> &p) {
	return out << "[" << p[0] << "," << p[1] << "," << p[2] << "," << p[3] << "]";
}




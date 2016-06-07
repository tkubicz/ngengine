template <typename T> void Frustum<T>::SetCameraInternals(T fieldOfView, T ratio, T nearDistance, T farDistance) {
	this->ratio = ratio;
	this->fieldOfView = fieldOfView;
	this->nearDistance = nearDistance;
	this->farDistance = farDistance;

	tang = (T) std::tan(Math::MathUtils::DegToRad(fieldOfView) * 0.5);

	dimensions.y = nearDistance * tang;
	dimensions.x = dimensions.y * ratio;
	dimensions.w = farDistance * tang;
	dimensions.z = dimensions.w * ratio;
}

template <typename T> void Frustum<T>::SetCameraDefinition(const Math::Vector3<T>& position, const Math::Vector3<T>& focus, const Math::Vector3<T>& upVector) {
	Math::Vector3<T> dir, nearCenter, farCenter, x, y, z;

	z = position - focus;
	z.Normalize();

	x = upVector.CrossProduct(z);
	x.Normalize();

	y = z.CrossProduct(x);

	nearCenter = position - z * nearDistance;
	farCenter = position - z * farDistance;

	this->near[0] = nearCenter + (y * dimensions.y) - (x * dimensions.x);
	this->near[1] = nearCenter + y * dimensions.y + x * dimensions.x;
	this->near[2] = nearCenter - y * dimensions.y - x * dimensions.x;
	this->near[3] = nearCenter - y * dimensions.y + x * dimensions.x;

	this->far[0] = farCenter + y * dimensions.w - x * dimensions.z;
	this->far[1] = farCenter + y * dimensions.w + x * dimensions.z;
	this->far[2] = farCenter - y * dimensions.w - x * dimensions.z;
	this->far[3] = farCenter - y * dimensions.w + x * dimensions.z;

	plane[TOP].SetPoints(near[1], near[0], far[0]);
	plane[BOTTOM].SetPoints(near[2], near[3], far[3]);
	plane[LEFT].SetPoints(near[0], near[2], far[2]);
	plane[RIGHT].SetPoints(near[3], near[1], far[3]);
	plane[NEAR].SetPoints(near[0], near[1], near[3]);
	plane[FAR].SetPoints(far[1], far[0], far[2]);

	// TODO: Remove this code after testing.
	InitialiseVBO();
	InitialiseVBA();
}

template <typename T> int Frustum<T>::PointInFrustum(const Math::Vector3<T>& point) {
	for (int i = 0; i < 6; ++i) {
		if (plane[i].GetDistance(point) < 0.0)
			return OUTSIDE;
	}
	return INSIDE;
}

template <typename T> int Frustum<T>::SphereInFrustum(const Math::Vector3<T>& position, T radius) {
	int result = INSIDE;
	float distance;

	for (int i = 0; i < 6; ++i) {
		distance = plane[i].GetDistance(position);
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			result = INTERSECT;
	}

	return result;
}

template <typename T> int Frustum<T>::AABoxInFrustum(const Math::Objects3D::AABox<T>& box) const {
	int result = INSIDE;
	for (int i = 0; i < 6; ++i) {
		Math::Vector3<T> positiveVertex = box.GetPositiveVertex(plane[i].GetNormal());
		Math::Vector3<T> negativeVertex = box.GetNegativeVertex(plane[i].GetNormal());
		if (plane[i].GetDistance(positiveVertex) < 0)
			return OUTSIDE;
		else if (plane[i].GetDistance(negativeVertex) < 0)
			result = INTERSECT;
	}

	return result;
}

template <typename T> void Frustum<T>::InitialiseVBO() {
	Math::Vector3<T> vertices[] = {
		// Near plane.
		Math::Vector3<T>(near[0].x, near[0].y, near[0].z),
		Math::Vector3<T>(near[1].x, near[1].y, near[1].z),
		Math::Vector3<T>(near[3].x, near[3].y, near[3].z),
		Math::Vector3<T>(near[2].x, near[2].y, near[2].z),

		// Far plane.
		Math::Vector3<T>(far[1].x, far[1].y, far[1].z),
		Math::Vector3<T>(far[0].x, far[0].y, far[0].z),
		Math::Vector3<T>(far[2].x, far[2].y, far[2].z),
		Math::Vector3<T>(far[3].x, far[3].y, far[3].z),

		// Bottom plane.
		Math::Vector3<T>(near[2].x, near[2].y, near[2].z),
		Math::Vector3<T>(near[3].x, near[3].y, near[3].z),
		Math::Vector3<T>(far[3].x, far[3].y, far[3].z),
		Math::Vector3<T>(far[2].x, far[2].y, far[2].z),

		// Top plane.
		Math::Vector3<T>(near[1].x, near[1].y, near[1].z),
		Math::Vector3<T>(near[0]),
		Math::Vector3<T>(far[0]),
		Math::Vector3<T>(far[1]),

		// Left plane.
		Math::Vector3<T>(near[0]),
		Math::Vector3<T>(near[2]),
		Math::Vector3<T>(far[2]),
		Math::Vector3<T>(far[0]),

		// Right plane.
		Math::Vector3<T>(near[3]),
		Math::Vector3<T>(near[1]),
		Math::Vector3<T>(far[1]),
		Math::Vector3<T>(far[3]),

		/* Normals */
		// Near normal.
		Math::Vector3<T>((near[1] + near[0] + near[3] + near[2]) * 0.25),
		Math::Vector3<T>((near[1] + near[0] + near[3] + near[2]) * 0.25 + plane[NEAR].GetNormal()),

		// Far normal.
		Math::Vector3<T>((far[1] + far[0] + far[3] + far[2]) * 0.25),
		Math::Vector3<T>((far[1] + far[0] + far[3] + far[2]) * 0.25 + plane[FAR].GetNormal()),

		// Left normal.
		Math::Vector3<T>((far[0] + far[2] + near[2] + near[0]) * 0.25),
		Math::Vector3<T>((far[0] + far[2] + near[2] + near[0]) * 0.25 + plane[LEFT].GetNormal()),

		// Right normal.
		Math::Vector3<T>((far[1] + near[3] + far[3] + near[1]) * 0.25),
		Math::Vector3<T>((far[1] + near[3] + far[3] + near[1]) * 0.25 + plane[RIGHT].GetNormal()),

		// Top normal.
		Math::Vector3<T>((far[1] + far[0] + near[1] + near[0]) * 0.25),
		Math::Vector3<T>((far[1] + far[0] + near[1] + near[0]) * 0.25 + plane[TOP].GetNormal()),

		//Bottom normal.
		Math::Vector3<T>((far[3] + far[2] + near[3] + near[2]) * 0.25),
		Math::Vector3<T>((far[3] + far[2] + near[3] + near[2]) * 0.25 + plane[BOTTOM].GetNormal())
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * 36, vertices, GL_STATIC_DRAW);

	//shader = Media::MediaManager::GetInstance().GetShaderManager().GetProgram("solidColorShader");
}

template <typename T> void Frustum<T>::InitialiseVBA() {
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

template <typename T> void Frustum<T>::Render() {
	shader->BindShader();

	Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
	//Rendering::Renderer::GetInstance().GetMatrixStack().Scale(15.0f, 15.0f, 15.0f);

	shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->sendUniform("color", Math::vec4f(0.0f, 0.0f, 1.f));

	glBindVertexArray(vertexArray);
	// TODO: Fix that, change GL_QUADS to something else. GL_QUADS is depricated.
#ifndef ANDROID
	glDrawArrays(GL_QUADS, 0, 24);
#endif

	shader->sendUniform("color", Math::vec4f(1.0f, 0.0f, 0.f));
	glDrawArrays(GL_LINES, 24, 36);

	glBindVertexArray(0);

	Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();

	shader->UnbindShader();
}
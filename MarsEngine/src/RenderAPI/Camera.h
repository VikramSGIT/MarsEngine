#pragma once

#include "MarsHeader.h"

#include "Vender/OGLM/oglm.h"

class Camera
{
public:
	Camera();
	~Camera();

	inline oglm::Matrix4& GetProjectionViewMat() { return m_PV; }
	inline void SetProjectionView(const oglm::Matrix4& pv) { m_PV = pv; }
private:
	oglm::Matrix4 m_PV;
};

#pragma once

#include "MarsHeader.h"

#include "Vender/OGLM/oglm.h"

class Camera
{
public:
	Camera();
	~Camera();

	inline oglm::Matrix4<ME_DATATYPE>& GetProjectionViewMat() { return m_PV; }
	inline void SetProjectionViewMat(const oglm::Matrix4<ME_DATATYPE>& pv) { m_PV = pv; }
private:
	oglm::Matrix4<ME_DATATYPE> m_PV;
};

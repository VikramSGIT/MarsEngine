#pragma once

#include <string>

#include "MarsHeader.h"
#include "Vender/OGLM/oglm.h"

class Shader
{
public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void unBind() const = 0;

	virtual void SetUniforms4f(const std::string& name, ME_DATATYPE f1, ME_DATATYPE f2, ME_DATATYPE f3, ME_DATATYPE f4) = 0;
	virtual void SetUniforms1i(const std::string& name, int i1) = 0;
	virtual void SetUniformsMat4f(const std::string& name, const oglm::Matrix4<float>& matrix) = 0;
};

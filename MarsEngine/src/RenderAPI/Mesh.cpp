#include "Mesh.h"

namespace ME
{
//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	Mesh::Mesh(const Mesh& mesh)
		:m_Ready(mesh.m_Ready), m_Destroyied(false), MemoryBound(false), m_Vertices(mesh.m_Vertices),
		m_Indices(mesh.m_Indices), m_ResetVertices(mesh.m_ResetVertices), m_Name(mesh.m_Name)
	{

		ME_PROFILE_TRACE_CALL();

		m_data.vsize = mesh.m_data.vsize;
		m_data.isize = mesh.m_data.isize;
		if (MemoryBound)
		{
			m_data.vertices = alloc<VERTEX>(mesh.m_data.vsize);
			m_data.indices = alloc<unsigned int>(mesh.m_data.isize);
			memcpy(m_data.vertices, mesh.m_data.vertices, mesh.m_data.vsize);
			memcpy(m_data.indices, mesh.m_data.indices, mesh.m_data.isize);
		}
	}

	Mesh::Mesh(Mesh&& mesh) noexcept
		:m_Ready(mesh.m_Ready), m_Destroyied(false), MemoryBound(false), m_Vertices(mesh.m_Vertices),
		m_Indices(mesh.m_Indices), m_ResetVertices(mesh.m_ResetVertices), m_Name(mesh.m_Name)
	{

		ME_PROFILE_TRACE_CALL();

		m_data.vsize = mesh.m_data.vsize;
		m_data.isize = mesh.m_data.isize;
		if (MemoryBound)
		{
			m_data.vertices = alloc<VERTEX>(mesh.m_data.vsize);
			m_data.indices = alloc<unsigned int>(mesh.m_data.isize);
			memcpy(m_data.vertices, mesh.m_data.vertices, mesh.m_data.vsize);
			memcpy(m_data.indices, mesh.m_data.indices, mesh.m_data.isize);
		}
	}

	Mesh::~Mesh()
	{
		//dealloc(m_data.vertices, m_data.vsize);
		//dealloc(m_data.indices, m_data.isize);
	}

	void Mesh::BufferVertices(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_Vertices.clear();
		m_ResetVertices.clear();
		for (unsigned int i = 0; i < count; i++)
		{
			m_Vertices.emplace_back(vertex[i]);
			m_ResetVertices.emplace_back(vertex[i]);
		}
		m_data.vertices = m_Vertices.begin()._Ptr;
		m_data.vsize = m_Vertices.size();
		m_Ready = false;
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_Indices.clear();
		for (unsigned int i = 0; i < count; i++)
			m_Indices.emplace_back(data[i]);
		m_data.indices = m_Indices.begin()._Ptr;
		m_data.isize = m_Indices.size();
		m_Ready = false;
	}

	void Mesh::SetReset(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_ResetVertices.clear();
		for (unsigned int i = 0; i < count; i++)
			m_ResetVertices.emplace_back(vertex[i]);
	}

	void Mesh::SetReset(const std::vector<VERTEX>& vertices)
	{
		m_ResetVertices.clear();
		m_ResetVertices = vertices;
	}

	void Mesh::Reset()
	{

		ME_PROFILE_TRACE_CALL();

		m_Vertices = m_ResetVertices;
		m_Ready = false;
	}

	void Mesh::Transform(const glm::mat4& matrix)
	{

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (size_t i = 0; i < m_data.vsize; i++)
		{
			VERTEX& vertex = m_data.vertices[i];

			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] , 1.0f };
			out = trans * out;
			out = matrix * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
			vertex.vertices[2] = out.z;
		}
		m_Ready = false;
	}

	void Mesh::Translate(const glm::vec3& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		for (size_t i = 0; i < m_data.vsize; i++)
		{
			VERTEX& vertex = m_data.vertices[i];
			vertex.vertices[0] += XYZ.x;
			vertex.vertices[1] += XYZ.y;
			vertex.vertices[2] += XYZ.z;
		}
		m_Ready = false;
	}

	void Mesh::TranslateTo(const glm::vec3& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 centroid = GetCentroid();

		glm::vec3 distance = XYZ - centroid;

		for (size_t i = 0; i < m_data.vsize; i++)
		{
			VERTEX& vertex = m_data.vertices[i];
			vertex.vertices[0] += distance.x;
			vertex.vertices[1] += distance.y;
			vertex.vertices[2] += distance.z;
		}
		m_Ready = false;
	}

	void Mesh::Rotate(const glm::vec3& degreeXYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (size_t i = 0; i < m_data.vsize; i++)
		{
			VERTEX& vertex = m_data.vertices[i];
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] , 1.0f };
			out = trans * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.x), glm::vec3(1.0f, 0.0f, 0.0f))) * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.y), glm::vec3(0.0f, 1.0f, 0.0f))) * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.z), glm::vec3(0.0f, 0.0f, 1.0f))) * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
			vertex.vertices[2] = out.z;
		}
		m_Ready = false;
	}

	void Mesh::Scale(const glm::vec3& XYZ)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (size_t i = 0; i < m_data.vsize; i++)
		{
			VERTEX& vertex = m_data.vertices[i];
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] , 1.0f };
			out = trans * out;
			out = glm::scale(glm::identity<glm::mat4>(), glm::vec3(XYZ.x, XYZ.y, XYZ.z)) * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
			vertex.vertices[2] = out.z;
		}
		m_Ready = false;
	}

	const glm::vec3 Mesh::GetCentroid() const
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 Result = { 0.0f, 0.0f, 0.0f };
		for (size_t i = 0; i < m_data.vsize; i++)
		{
			VERTEX& vertex = m_data.vertices[i];
			Result.x += vertex.vertices[0];
			Result.y += vertex.vertices[1];
			Result.z += vertex.vertices[2];
		}
		Result.x = Result.x / static_cast<ME_DATATYPE>(m_data.vsize);
		Result.y = Result.y / static_cast<ME_DATATYPE>(m_data.vsize);
		Result.z = Result.z / static_cast<ME_DATATYPE>(m_data.vsize);
		return Result;
	}

////////////////////////////////////////// Mesh Queue ////////////////////////////////////////////////

	MeshQueue::MeshQueue()
	{
		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}
	}

	void MeshQueue::PushMesh(const Ref<Mesh>& mesh)
	{

		ME_PROFILE_TRACE_CALL();

//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//
		size_t tempvbuffer = total_vertices, tempibuffer = total_indices;
		if (m_AllocationMode == ALLOCMODE::ALLATONE)
		{
			if (!mesh->MemoryBound)
			{
				m_Meshes.emplace_back(mesh);
				total_vertices += static_cast<unsigned int>(mesh->m_data.vsize) * m_Layout->GetTotalCount();
				total_indices += static_cast<unsigned int>(mesh->m_data.isize);
			}
			else
				ME_CORE_ERROR(mesh->m_Name + " has already been added to a memory bound Queue " + mesh->m_Name + " can only be added to distributed Queue!!");
		}
		else if(m_AllocationMode == ALLOCMODE::DISTRIBUTED)
		{
			m_Meshes.emplace_back(mesh);
			total_vertices += static_cast<unsigned int>(mesh->m_data.vsize) * m_Layout->GetTotalCount();
			total_indices += static_cast<unsigned int>(mesh->m_data.isize);
		}

		if (m_AllocationMode == ALLOCMODE::ALLATONE)
		{
			if (m_Allocation == ALLOCAT::ONMESHPUSH)
			{
				float* tempvertexbuffer = vertexbuffer;
				unsigned int* tempindexbuffer = indexbuffer;

				vertexbuffer = alloc<ME_DATATYPE>(total_vertices);
				indexbuffer = alloc<unsigned int>(total_indices);

				unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
				for (Ref<Mesh> ms : m_Meshes)
				{
//
// Filling up of vertexbuffer with datas
//
					std::memcpy(&vertexbuffer[voffset], ms->m_data.vertices, ms->m_data.vsize * m_Layout->GetTotalCount() * sizeof(ME_DATATYPE));
					ms->m_data.vertices = (VERTEX*)&vertexbuffer[voffset];
					voffset += static_cast<unsigned int>(ms->m_data.vsize) * m_Layout->GetTotalCount();

					ms->m_Vertices.clear();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
					for (unsigned __int64 j = 0; j < ms->m_data.isize; j++)
						indexbuffer[j + ioffset] = ms->m_data.indices[j] + indexoffset;
					ioffset += ms->m_data.isize;
					indexoffset += *std::max_element(ms->m_data.indices, ms->m_data.indices + ms->m_data.isize) + 1;

					ms->MemoryBound = true;
					ms->SetReady(true);
				}
				dealloc(tempvertexbuffer, tempvbuffer);
				dealloc(tempindexbuffer, tempibuffer);
			}
		}
	}

	void MeshQueue::PushMeshes(const std::vector<Ref<Mesh>>& meshes)
	{

		ME_PROFILE_TRACE_CALL();

		size_t tempvbuffer = total_vertices, tempibuffer = total_indices;
		for (Ref<Mesh> mesh : meshes)
		{
			if (m_AllocationMode == ALLOCMODE::ALLATONE)
			{
				if (!mesh->MemoryBound)
				{
					m_Meshes.emplace_back(mesh);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//
					total_vertices += static_cast<unsigned int>(mesh->m_data.vsize) * m_Layout->GetTotalCount();
					total_indices += static_cast<unsigned int>(mesh->m_data.isize);
				}
				else
					ME_CORE_ERROR(mesh->m_Name + " has already been bound to Queue. " + mesh->m_Name + " can only be added to distributed Queue!!");
			}
			else if (m_AllocationMode == ALLOCMODE::DISTRIBUTED)
			{
				m_Meshes.emplace_back(mesh);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//
				total_vertices += static_cast<unsigned int>(mesh->m_data.vsize) * m_Layout->GetTotalCount();
				total_indices += static_cast<unsigned int>(mesh->m_data.isize);
			}
		}

		if (m_AllocationMode == ALLOCMODE::ALLATONE)
		{
			if (m_Allocation == ALLOCAT::ONMESHPUSH)
			{

				float* tempvertexbuffer = vertexbuffer;
				unsigned int* tempindexbuffer = indexbuffer;
				vertexbuffer = alloc<ME_DATATYPE>(total_vertices);
				indexbuffer = alloc<unsigned int>(total_indices);

				unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
				for (Ref<Mesh> ms : m_Meshes)
				{
//
// Filling up of vertexbuffer with datas
//
					std::memcpy(&vertexbuffer[voffset], ms->m_data.vertices, ms->m_data.vsize * m_Layout->GetTotalCount() * sizeof(ME_DATATYPE));
					ms->m_data.vertices = (VERTEX*)&vertexbuffer[voffset];
					voffset += static_cast<unsigned int>(ms->m_data.vsize) * m_Layout->GetTotalCount();

					ms->m_Vertices.clear();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
					for (size_t j = 0; j < ms->m_data.isize; j++)
						indexbuffer[j + ioffset] = ms->m_data.indices[j] + indexoffset;
					ioffset += static_cast<unsigned int>(ms->m_data.isize);
					indexoffset += *std::max_element(ms->m_data.indices, ms->m_data.indices + ms->m_data.isize) + 1;

					ms->MemoryBound = true;
					ms->SetReady(true);
				}
				dealloc(tempvertexbuffer, tempvbuffer);
				dealloc(tempindexbuffer, tempibuffer);
			}
		}
	}

	void MeshQueue::Allocate()
	{
		if (m_AllocationMode == ALLOCMODE::ALLATONE)
		{
			if (m_Allocation == ALLOCAT::CUSTOM)
			{

				float* tempvertexbuffer = vertexbuffer;
				unsigned int* tempindexbuffer = indexbuffer;

				vertexbuffer = alloc<ME_DATATYPE>(total_vertices);
				indexbuffer = alloc<unsigned int>(total_indices);

				unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
				for (Ref<Mesh> ms : m_Meshes)
				{
//
// Filling up of vertexbuffer with datas
//
					std::memcpy(&vertexbuffer[voffset], ms->m_data.vertices, ms->m_data.vsize * m_Layout->GetTotalCount() * sizeof(ME_DATATYPE));
					ms->m_data.vertices = (VERTEX*)&vertexbuffer[voffset];
					voffset += static_cast<unsigned int>(ms->m_data.vsize) * m_Layout->GetTotalCount();

					//ms->m_Vertices.clear();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
					for (size_t j = 0; j < ms->m_data.isize; j++)
						indexbuffer[j + ioffset] = ms->m_data.indices[j] + indexoffset;
					ioffset += static_cast<unsigned int>(ms->m_data.isize);
					indexoffset += *std::max_element(ms->m_data.indices, ms->m_data.indices + ms->m_data.isize) + 1;

					ms->MemoryBound = true;
					ms->SetReady(true);
				}
				dealloc(tempvertexbuffer, total_vertices);
				dealloc(tempindexbuffer, total_indices);
			}
		}
		else
			ME_CORE_ERROR("Can only allocate if Bound Queue");
	}

	void MeshQueue::ClearBuffer()
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(vertexbuffer, total_vertices);
		dealloc(indexbuffer, total_indices);
	}

	std::vector<glm::vec<2, unsigned int>> MeshQueue::GetUpdate()
	{

		ME_PROFILE_TRACE_CALL();

		std::vector<glm::vec<2, unsigned int>> ranges;
		if (m_AllocationMode == ALLOCMODE::ALLATONE)
		{
			unsigned int offset = 0;
			for (Ref<Mesh> ms : m_Meshes)
			{
				glm::vec2 range;
				if (!ms->m_Ready)
				{
					range.x = offset;
					range.y = offset + ms->m_data.vsize * m_Layout->GetTotalCount();
					ranges.emplace_back(range);
					ms->SetReady(true);
				}
				offset += ms->m_data.vsize * m_Layout->GetTotalCount();
			}
		}
		else if (m_AllocationMode == ALLOCMODE::DISTRIBUTED)
		{
			size_t i = 0u;
			glm::vec2 range;
			unsigned int offset = 0;

			for (; i < m_Meshes.size(); i++)
			{
				if (!m_Meshes[i]->m_Ready)
				{
					range.x = offset;
					range.y = i;
					ranges.emplace_back(range);
					m_Meshes[i]->SetReady(true);
				}
				offset += m_Meshes[i]->m_data.vsize * m_Layout->GetTotalCount();
			}
		}
		return ranges;
	}

	void MeshQueue::PushAddon(ME::Addon::MeshAddon& addon)
	{

		ME_PROFILE_TRACE_CALL();
		
		if (m_AllocationMode == ALLOCMODE::ALLATONE)
		{
			size_t tempvbuffer = total_vertices, tempibuffer = total_indices;

			for (Ref<Mesh> mesh : addon.GetMeshes())
			{
				if (!mesh->MemoryBound)
				{
					m_Meshes.emplace_back(mesh);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//
					total_vertices += static_cast<unsigned int>(mesh->m_data.vsize) * m_Layout->GetTotalCount();
					total_indices += static_cast<unsigned int>(mesh->m_data.isize);
				}
				else
				{
					ME_CORE_ERROR(mesh->m_Name + " addon has already been bound to a Queue. Can only be added to a distributed Queue");
					break;
				}
			}

			if (m_Allocation == ALLOCAT::ONMESHPUSH)
			{
				float* tempvertexbuffer = vertexbuffer;
				unsigned int* tempindexbuffer = indexbuffer;

				vertexbuffer = alloc<ME_DATATYPE>(total_vertices);
				indexbuffer = alloc<unsigned int>(total_indices);

				unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
				for (Ref<Mesh> ms : m_Meshes)
				{
//
// Filling up of vertexbuffer with datas
//			
					std::memcpy(&vertexbuffer[voffset], ms->m_data.vertices, ms->m_data.vsize * m_Layout->GetTotalCount() * sizeof(ME_DATATYPE));
					ms->m_data.vertices = (VERTEX*)&vertexbuffer[voffset];
					voffset += static_cast<unsigned int>(ms->m_Vertices.size()) * m_Layout->GetTotalCount();

					//ms->m_Vertices.clear();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
					for (size_t j = 0; j < ms->m_data.isize; j++)
						indexbuffer[j + ioffset] = ms->m_data.indices[j] + indexoffset;
					ioffset += static_cast<unsigned int>(ms->m_data.isize);
					indexoffset += *std::max_element(ms->m_data.indices, ms->m_data.indices + ms->m_data.isize) + 1;

					ms->MemoryBound = true;
					ms->SetReady(true);
				}
				dealloc(tempvertexbuffer, tempvbuffer);
				dealloc(tempindexbuffer, tempibuffer);
			}
		}
		else if (m_AllocationMode == ALLOCMODE::DISTRIBUTED)
		{
			std::vector<Ref<Mesh>> temp;
			temp = m_Meshes;
			m_Meshes.clear();

			for (Ref<Mesh> mesh : addon.GetMeshes())
			{
				m_Meshes.emplace_back(mesh);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//
				total_vertices += static_cast<unsigned int>(mesh->m_data.vsize) * m_Layout->GetTotalCount();
				total_indices += static_cast<unsigned int>(mesh->m_data.isize);
			}

			for (Ref<Mesh> ms : temp)
				m_Meshes.emplace_back(ms);
		}
	}
}
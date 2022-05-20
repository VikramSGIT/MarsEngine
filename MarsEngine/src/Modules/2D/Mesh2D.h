#ifndef ME_MESH2D
#define ME_MESH2D

#pragma once

#include "Core/Entity.h"
#include "RenderAPI/Buffers.h"
#include "RenderAPI/Shader.h"
#include "Core/Utilites/Vector.h"
#include "Addons/Addon.h"
#include "Core/Utilites/Ref.h"

#include <string>
#include <functional>

#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "Vender/glm/glm/glm.hpp"

/*
* TODO: Plan virtually exposed functions
*/
namespace ME
{
	struct VERTEX2D
	{
		ME_DATATYPE vertices[2] = { 0.0f, 0.0f };
		ME_DATATYPE texturecoord[2] = { 0.0f, 0.0f };
		ME_DATATYPE index = 0.0f;
	};

	struct MeshData2D
	{
		MeshData2D()
			:vertex(Vertex()), index(Index()) {}
		class Vertex
		{
		public:
			Vertex()
				:vertex(nullptr), reset_vertex(nullptr), m_Size(0), m_Offset(0) {}

			VERTEX2D* begin() { return vertex; }
			VERTEX2D* end() { return (vertex + m_Size); }

			const VERTEX2D* begin() const { return vertex; }
			const VERTEX2D* end() const { return (vertex + m_Size); }

			VERTEX2D* operator++() { return vertex++; }

			inline const unsigned int Size() const { return m_Size; }
			inline VERTEX2D* GetReset() { return reset_vertex; }
			inline const unsigned int GetOffset() const { return m_Offset; }

			void SetOffset(const unsigned int& offset) { m_Offset = offset; }

		private:
			VERTEX2D* vertex, * reset_vertex;
			unsigned int m_Size, m_Offset;

			friend class Mesh2D;
		};

		class Index
		{
		public:
			Index()
				:index(nullptr), m_Size(0), m_Offset(0) {}

			unsigned int* begin() { return index; }
			unsigned int* end() { return (index + m_Size); }

			const unsigned* begin() const { return index; }
			const unsigned* end() const { return (index + m_Size);; }

			inline const unsigned int Size() const { return m_Size; }
			inline const unsigned int GetOffset() const { return m_Offset; }

			void SetOffset(const unsigned int& offset) { m_Offset = offset; }

		private:
			unsigned int* index;
			unsigned int m_Size, m_Offset;

			friend class Mesh2D;
		};

		Vertex vertex;
		Index index;
	};

	class Mesh2D : public Entity
	{	ME_CLASS_CONNECT(Mesh2D)
	public:

		Mesh2D(const std::string& name)
			:m_Name(name), m_MeshData(MeshData2D()), Ready(true) {}
		Mesh2D(const Mesh2D& mesh);
		Mesh2D(Mesh2D&& mesh) noexcept;
		~Mesh2D();

		void BufferVertices(const VERTEX2D* vertex, const unsigned int& count);
		void BufferIndices(const unsigned int* data, const unsigned int& count);

		void SetReset(const VERTEX2D* vertex);
		void SetReset(const std::vector<VERTEX2D>& vertices);
		void SetReady(const bool& ready) { Ready = ready; }
		void Reset();

		void Transform(const glm::mat4& matrix);
		void Translate(const glm::vec2& XY);
		void TranslateTo(const glm::vec2& XY); //translate the mesh's centroid to the point specified
		void Rotate(const ME_DATATYPE& degree);
		void Scale(const glm::vec2& XY);

		inline const MeshData2D& GetMeshData() const { return m_MeshData; }
		inline MeshData2D& GetMeshData() { return m_MeshData; }
		inline const std::string GetName() const { return m_Name; }
		inline const bool IsReady() { return Ready; }
		const glm::vec2 GetCentroid() const;

		Mesh2D operator* (const glm::mat4 &mat)
		{

			ME_PROFILE_TRACE_CALL();

			glm::mat4 matrix = mat;
			for (VERTEX2D& vertex : m_MeshData.vertex)
			{
				glm::vec4 out(vertex.vertices[0], vertex.vertices[1], 1, 1.0f);
				out = matrix * out;
				vertex.vertices[0] = out.x;
				vertex.vertices[1] = out.y;
			}
			return *this;

			callback(this, m_MeshData.vertex.m_Offset);
		}

	private:
		MeshData2D m_MeshData;

		std::function<void(Mesh2D*, const unsigned int&)> callback;
		std::string m_Name;
		bool Ready;

		friend class MeshQueue2D;
	};

	class MeshQueue2D // Render-Call-batcher
	{
	public:

		MeshQueue2D();

		virtual void PushMesh(const Ref<Mesh2D>& mesh);
		virtual void PushMeshes(const Vector<Ref<Mesh2D>>& meshes);
		virtual void PushMeshes(const std::vector<Ref<Mesh2D>>& meshes);
		virtual void PushAddon(ME::Addon::MeshAddon2D& addon);

		virtual inline std::vector<Ref<Mesh2D>> GetMeshes() const { return m_Meshes; }

		virtual inline size_t GetTotalVertices() const { return total_vertices; }
		virtual inline size_t GetTotalIndices() const { return total_indices; }
		virtual inline Ref<Renderer::VertexBufferLayout> GetLayout() const { return m_Layout; }
		virtual const std::vector<std::pair<Mesh2D*, unsigned int>> GetUpdate()
		{
			if (m_MeshUpdates.size())
			{
				std::vector<std::pair<Mesh2D*, unsigned int>> out = m_MeshUpdates;
				m_MeshUpdates.clear();
				return out;
			}
			m_MeshUpdates.clear();
			return m_MeshUpdates;
		}

		std::vector<Ref<Mesh2D>>::iterator begin() { return m_Meshes.begin(); }
		std::vector<Ref<Mesh2D>>::iterator end() { return m_Meshes.end(); }

		const std::vector<Ref<Mesh2D>>::const_iterator begin() const { return m_Meshes.begin(); }
		const std::vector<Ref<Mesh2D>>::const_iterator end() const { return m_Meshes.end(); }

	private:

		void MeshCallback(Mesh2D* mesh, const unsigned int& offset)
		{
			if (mesh->Ready)
			{
				m_MeshUpdates.emplace_back(mesh, offset);
				mesh->Ready = true;
			}
		}

		Ref<Renderer::VertexBufferLayout> m_Layout;
		std::vector <Ref<Mesh2D>> m_Meshes;
		std::vector<std::pair<Mesh2D*, unsigned int>> m_MeshUpdates;

		size_t total_vertices;
		unsigned int total_indices;
	};
//
// Commenly used meshes
//
	static Ref<Mesh2D> GenQuad2D(const std::string& name, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& v4, const unsigned int& index = 0)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX2D vertexbuffer[] =
		{
			{v1.x, v1.y, 0.0f, 0.0f, (float)index},
			{v2.x, v2.y, 1.0f, 0.0f, (float)index},
			{v3.x, v3.y, 1.0f, 1.0f, (float)index},
			{v4.x, v4.y, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh2D> out = CreateRef<Mesh2D>(name);
		out->BufferVertices(vertexbuffer, 4);
		out->BufferIndices(indexbuffer, 6);
		return out;
	}
//
// Generate Rectangle using diagonal points
	static Ref<Mesh2D> GenRect2D(const std::string& name, const glm::vec2& point1, const glm::vec2& point2, const unsigned int& index = 0)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX2D vertexbuffer[] =
		{
			{point1.x, point1.y, 0.0f, 0.0f, (float)index},
			{point2.x, point1.y, 1.0f, 0.0f, (float)index},
			{point2.x, point2.y, 1.0f, 1.0f, (float)index},
			{point1.x, point2.y, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh2D> out = CreateRef<Mesh2D>(name);
		out->BufferVertices(vertexbuffer, 4);
		out->BufferIndices(indexbuffer, 6);
		return out;
	}
//
// Generates a quad using lenght and breath
	static Ref<Mesh2D> GenRect2D(const std::string& name, const glm::vec2& lb, const unsigned int& index = 0)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX2D vertexbuffer[] =
		{
			{0.0f, 0.0f, 0.0f, 0.0f, (float)index},
			{lb.x, 0.0f, 1.0f, 0.0f, (float)index},
			{lb.x, lb.y, 1.0f, 1.0f, (float)index},
			{0.0f, lb.y, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh2D> out = CreateRef<Mesh2D>(name);
		out->BufferVertices(vertexbuffer, 4);
		out->BufferIndices(indexbuffer, 6);
		return out;
	}
//
// Quickest quad generation with 1x1 size
	static Ref<Mesh2D> QuickQuad2D(const unsigned int& index = 0) { return GenRect2D("Quad", {1.0f, 1.0f}, index); }
}

#endif
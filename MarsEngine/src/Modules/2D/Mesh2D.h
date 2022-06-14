#ifndef ME_MESH2D
#define ME_MESH2D

#pragma once

#include "Core/Entity.h"
#include "RenderAPI/Buffers.h"
#include "RenderAPI/Shader.h"
#include "Core/Utilites/Vector.h"
#include "Addons/Addon.h"
#include "Core/Utilites/Ref.h"

/*
* TODO: Plan virtually exposed functions
*/
namespace ME
{
	struct VERTEX2D
	{
		ME_DATATYPE x = 0.0f, y = 0.0f, uv_x = 0.0f, uv_y = 0.0f, index = 0.0f;
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
				:index(nullptr), m_Size(0), m_Offset(0), m_IndexOffset(0) {}

			unsigned int* begin() { return index; }
			unsigned int* end() { return (index + m_Size); }

			const unsigned* begin() const { return index; }
			const unsigned* end() const { return (index + m_Size);; }

			inline const unsigned int Size() const { return m_Size; }
			inline const unsigned int GetOffset() const { return m_Offset; }
			inline const unsigned int GetIndexOffset() const { return m_IndexOffset; }

			void SetOffset(const unsigned int& offset) { m_Offset = offset; }
			void SetIndexOffset(const unsigned int& offset) { m_IndexOffset = offset; }

		private:
			unsigned int* index;
			unsigned int m_Size, m_Offset, m_IndexOffset;

			friend class Mesh2D;
		};

		Vertex vertex;
		Index index;
	};

	class Mesh2D : public Entity
	{	ME_CLASS_CONNECT(Mesh2D)
	public:

		Mesh2D(const string& name);
		Mesh2D(const Mesh2D& mesh);
		Mesh2D(Mesh2D&& mesh) noexcept;
		~Mesh2D();

		void BufferVertices(const VERTEX2D* vertex, const unsigned int& count);
		void BufferIndices(const unsigned int* data, const unsigned int& count);

		void SetReset(const VERTEX2D* vertex);
		void SetReset(const std::vector<VERTEX2D>& vertices);
		void Reset();

		void Transform(const glm::mat4& matrix);
		void Translate(const glm::vec2& XY);
		void TranslateTo(const glm::vec2& XY); //translate the mesh's centroid to the point specified
		void Rotate(const ME_DATATYPE& degree);
		void Scale(const glm::vec2& XY);

		inline MeshData2D& GetMeshData() { return m_MeshData; }
		inline const MeshData2D& GetMeshData() const { return m_MeshData; }
		inline const string GetName() const { return m_Name; }
		const glm::vec2 GetCentroid() const;

		Mesh2D operator* (const glm::mat4& mat);

	private:
		MeshData2D m_MeshData;

		string m_Name;
		friend class MeshQueue2D;
	};

	class Rectangle : public Mesh2D
	{
		ME_CLASS_CONNECT(Rectangle)
	public:
		Rectangle(const string& name, const unsigned int& index = 0);
		Rectangle(const string& name, const glm::vec2& lb, const unsigned int& index = 0);
		Rectangle(const string& name, const glm::vec2& edge1, glm::vec2 edge2, const unsigned int& index = 0);
		virtual ~Rectangle() = default;

		void Grow(const glm::vec2& lb);
		void Set(const glm::vec2& lb);
	};
//
// Commenly used meshes
//
	static Ref<Mesh2D> GenQuad2D(const string& name, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& v4, const unsigned int& index = 0)
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
}

#endif
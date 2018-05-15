#pragma once

#include <glm.hpp>
#include "mfTetraCell.h"

using glm::ivec3;

namespace mf
{
	template<class Traits>
	class CybTetraCell : public mfTetraCell<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sMesh sMesh;
		typedef typename Traits::sVertex sVertex;

		/** Constructor */
		CybTetraCell()
		{

		}

		/** Destructor */
		virtual ~CybTetraCell()
		{

		}
		
		void getFaceVerticesIndex(int faceId, ivec3& vertices)
		{
			if (faceId == 0) {
				vertices[0] = 1;
				vertices[1] = 2;
				vertices[2] = 3;
				return;
			}
			
			if (faceId == 1) {
				vertices[0] = 0;
				vertices[1] = 3;
				vertices[2] = 2;
				return;
			}
			
			if (faceId == 2) {
				vertices[0] = 0;
				vertices[1] = 1;
				vertices[2] = 3;
				return;
			}

			if (faceId == 3) {
				vertices[0] = 0;
				vertices[1] = 2;
				vertices[2] = 1;
				return;
			}
		}

		void getFaceVerticesId(int faceId, ivec3& vertices)
		{
			if (faceId == 0) {
				vertices[0] = getVertexId(1);
				vertices[1] = getVertexId(2);
				vertices[2] = getVertexId(3);
				return;
			}

			if (faceId == 1) {
				vertices[0] = getVertexId(0);
				vertices[1] = getVertexId(3);
				vertices[2] = getVertexId(2);
				return;
			}

			if (faceId == 2) {
				vertices[0] = getVertexId(0);
				vertices[1] = getVertexId(1);
				vertices[2] = getVertexId(3);
				return;
			}

			if (faceId == 3) {
				vertices[0] = getVertexId(0);
				vertices[1] = getVertexId(2);
				vertices[2] = getVertexId(1);
				return;
			}
		}
	};
}

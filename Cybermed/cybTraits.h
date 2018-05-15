#pragma once

#include "cybTetraCell.h"
#include "cybTriCell.h"
#include "cybVertex.h"
#include "mfGeometric2D.h"
#include "mfMeshTetra.h"
#include "mfMeshTriSurf.h"
#include "mfTopology.h"

namespace mf
{
	struct cybSurfaceTriTraits
	{
		typedef double									space;
		typedef int										ids;
		typedef CybVertex<cybSurfaceTriTraits>			sVertex;
		typedef CybTriCell<cybSurfaceTriTraits>			sCell;
		typedef mfMesh<cybSurfaceTriTraits>				sMesh;
		typedef mfMeshTriSurface<cybSurfaceTriTraits>	sOper;
		typedef mfGeometric2D<cybSurfaceTriTraits>		sGeometric;
		typedef mfTopology<cybSurfaceTriTraits>			sTopology;
	};

	struct cybTetraTraits
	{
		typedef double							space;
		typedef int								ids;
		typedef CybVertex<cybTetraTraits>		sVertex;
		typedef CybTetraCell<cybTetraTraits>	sCell;
		typedef mfMesh<cybTetraTraits>			sMesh;
		typedef mfMeshTetra<cybTetraTraits>	sOper;
		typedef mfGeometric2D<cybTetraTraits>	sGeometric;
		typedef mfTopology<cybTetraTraits>		sTopology;
	};
}
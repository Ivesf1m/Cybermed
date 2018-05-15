#pragma once

#include "mfVertex.h"
#include "mfTriCell.h"
#include "mfQuadCell.h"
#include "mfHybridCell2D.h"
#include "mfTetraCell.h"
#include "mfHexaCell.h"
#include "mfMesh2D.h"
#include "mfMeshTriSurf.h"
#include "mfMeshQuadSurface.h"
#include "mfMeshHybridSurface.h"
#include "mfMeshTetra.h"
#include "mfMeshHexa.h"
#include "mfMeshNOSurface.h"
#include "mfGeometric2D.h"
#include "mfTopology.h"

namespace mf
{
	struct mfDefault2D {
		typedef mfDefault2D				sTraits;
		typedef double					space;
		typedef int						ids;
		typedef mfVertex<2, sTraits>	sVertex;
		typedef mfTriCell<sTraits>		sCell;
		typedef mfMesh<sTraits>			sMesh;
		typedef mfMesh2D<sTraits>		sOper;
		typedef mfGeometric2D<sTraits>	sGeometric;
		typedef mfTopology<sTraits>		sTopology;
	};

	struct mfDefaultTetrahedra {
		typedef mfDefaultTetrahedra		sTraits;
		typedef double					space;
		typedef int						ids;
		typedef mfVertex<3, sTraits>	sVertex;
		typedef mfTetraCell<sTraits>	sCell;
		typedef mfMesh<sTraits>			sMesh;
		typedef mfMeshTetra<sTraits>	sOper;
		typedef mfGeometric2D<sTraits>	sGeometric;
		typedef mfTopology<sTraits>		sTopology;
	};

	struct mfDefaultHexahedra {
		typedef mfDefaultHexahedra		sTraits;
		typedef double					space;
		typedef int						ids;
		typedef mfVertex<3, sTraits>	sVertex;
		typedef mfHexaCell<sTraits>		sCell;
		typedef mfMesh<sTraits>			sMesh;
		typedef mfMeshHexa<sTraits>		sOper;
		typedef mfGeometric2D<sTraits>	sGeometric;
		typedef mfTopology<sTraits>		sTopology;
	};

	struct mfDefaultTriSurface {
		typedef mfDefaultTriSurface			sTraits;
		typedef double						space;
		typedef int							ids;
		typedef mfVertex<3, sTraits>		sVertex;
		typedef mfTriCell<sTraits>			sCell;
		typedef mfMesh<sTraits>				sMesh;
		typedef mfMeshTriSurface<sTraits>	sOper;
		typedef mfGeometric2D<sTraits>		sGeometric;
		typedef mfTopology<sTraits>			sTopology;
	};

	struct mfDefaultQuadSurface {
		typedef mfDefaultQuadSurface		sTraits;
		typedef double						space;
		typedef int							ids;
		typedef mfVertex<3, sTraits>		sVertex;
		typedef mfQuadCell<sTraits>			sCell;
		typedef mfMesh<sTraits>				sMesh;
		typedef mfMeshQuadSurface<sTraits>	sOper;
		typedef mfGeometric2D<sTraits>		sGeometric;
		typedef mfTopology<sTraits>			sTopology;
	};

	struct mfDefaultHybridSurface {
		typedef mfDefaultHybridSurface			sTraits;
		typedef double							space;
		typedef int								ids;
		typedef mfVertex<3, sTraits>			sVertex;
		typedef mfHybridCell2D<sTraits>			sCell;
		typedef mfMesh<sTraits>					sMesh;
		typedef mfMeshHybridSurface<sTraits>	sOper;
		typedef mfGeometric2D<sTraits>			sGeometric;
		typedef mfTopology<sTraits>				sTopology;
	};

	struct mfDefaultNOSurface {
		typedef mfDefaultNOSurface			sTraits;
		typedef double						space;
		typedef int							ids;
		typedef mfVertex<3, sTraits>		sVertex;
		typedef mfTriCell<sTraits>			sCell;
		typedef mfMesh<sTraits>				sMesh;
		typedef mfMeshNOSurface<sTraits>	sOper;
		typedef mfGeometric2D<sTraits>		sGeometric;
		typedef mfTopology<sTraits>			sTopology;
	};
}

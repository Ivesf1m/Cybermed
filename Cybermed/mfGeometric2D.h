#pragma once

#include <cmath>
#include "mfMesh.h"
#include "mfSing.h"

using std::fabs;
using std::pow;
using std::sqrt;

namespace mf
{
#define SPACE typename mfGeometric2D<Traits>::space

	template<class Traits>
	class mfGeometric2D
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename mfSing<Traits> sSing;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfGeometric2D(sMesh* _mesh = NULL, double _erro = MF_ERRO);

		/** Destructor */
		~mfGeometric2D();

		/**
		*	Sets the mesh reference that this class will manipulate
		*	@param mesh: reference to the mesh
		*/
		void setMesh(sMesh* mesh);

		int inLeft(sVertex* p0, sVertex* p1, sVertex* p2);
		int inLeft(sVertex* p0, sVertex* p1, space coord0, space coord1);
		int inLeft(ids p0, ids p1, ids p2);
		int inLeft(ids p0, ids p1, space coord0, space coord1);
		space dist(sVertex* p0, sVertex* p1);
		space dist(space* p0, space* p1);
		int inCircle(sCell* c, sVertex* p);
		int inCircle(sVertex* p1, sVertex* p2, sVertex* p3, sVertex* p);
		int pointInTetrahedron(sCell* c, sVertex* p);
		int pointInTetrahedron(sVertex* p0, sVertex* p1, sVertex* p2,
			sVertex* p3, sVertex* p);
		bool inDiametralCircle(sVertex* v1, sVertex* v2, sVertex* v);
		bool sameVertices(sVertex* v1, sVertex* v2);
		int orientation2D(ids* idvertices, sVertex** vertices);
		int orientation3D(ids* idvertices, sVertex** vertices);
		bool isBadCell(sCell* c, space beta);
		void getCircuncircle(sCell* c, space* coords);
		void getCircuncircle(space* c0, space* c1, space* c2, space* coords);
		space det3(space matrix[3][3]);
		space det(space matrix[4][4]);
		space dot(ids p1, ids p2, space* coords);
		space vecAngle(ids p1, ids p_ang, ids p2);
		space norm2d(space* coords);
		space areaTriangle(space* x, space* y, space* z);
		bool isDelaunay(ids idcell);
		space areaTriangle(sCell* c);
		//void flip2D(ids c1, ids c2);

	protected:
		double error;
		int temp;
		sMesh* mesh;

	};

	template<class Traits>
	mfGeometric2D<Traits>::mfGeometric2D(sMesh* _mesh, double _error)
		: mesh(_mesh), error(_error), temp(0)
	{

	}

	template<class Traits>
	mfGeometric2D<Traits>::~mfGeometric2D()
	{

	}

	template<class Traits>
	void mfGeometric2D<Traits>::setMesh(sMesh* mesh)
	{
		this->mesh = mesh;
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::dist(sVertex* p0, sVertex* p1)
	{
		return sqrt(pow(p0->getCoord(0) - p1->getCoord(0), 2) +
			pow(p0->getCoord(1) - p1->getCoord(1), 2));
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::dist(space* p0, space* p1)
	{
		return sqrt((p0[0] - p1[0]) * (p0[0] - p1[0]) +
			(p0[1] - p1[1]) * (p0[1] - p1[1]));
	}

	template<class Traits>
	int mfGeometric2D<Traits>::inLeft(sVertex* p0, sVertex* p1, sVertex* p2)
	{
		/*
		*	Returns 1 if it's left
		*	Returns 0 if it's right
		*	Returns 2 if it's on
		*/
		space v[2] = { p1->getCoord(0) - p0->getCoord(0), p1->getCoord(1)
			- p0->getCoord(1) };
		space w[2] = { p2->getCoord(0) - p0->getCoord(0), p2->getCoord(1)
			- p0->getCoord(1) };

		space i = (v[0] * w[1] - w[0] * v[1]);

		if (i > error)
			return 1;
		else {
			if (i < -error)
				return 0;
			else
				return 2;
		}
	}

	template<class Traits>
	int mfGeometric2D<Traits>::inLeft(sVertex* p0, sVertex* p1, space coord0,
		space coord1)
	{
		/*
		*	Returns 1 if it's left
		*	Returns 0 if it's right
		*	Returns 2 if it's on
		*/

		space i = ((p1->getCoord(0) - p0->getCoord(0)) * (coord1 - p0->getCoord(1))
			- (coord0 - p0->getCoord(0)) * (p1->getCoord(1) - p0->getCoord(1)));

		if (i > erro)
			return 1;
		else {
			if (i < -error)
				return 0;
			else
				return 2;
		}
	}

	template<class Traits>
	int mfGeometric2D<Traits>::inLeft(ids p0, ids p1, ids p2)
	{
		/*
		*	Returns 1 if it's left
		*	Returns 0 if it's right
		*	Returns 2 if it's on
		*/
		sVertex* v0 = mesh->getVertex(p0);
		sVertex* v1 = mesh->getVertex(p1);
		sVertex* v2 = mesh->getVertex(p2);
		return inLeft(v0, v1, v2);
	}

	template<class Traits>
	int mfGeometric2D<Traits>::inLeft(ids p0, ids p1, space coord0, space coord1)
	{
		sVertex* v0 = mesh->getVertex(p0);
		sVertex* v1 = mesh->getVertex(p1);
		return inLeft(v0, v1, coord0, coord1);
	}

	template<class Traits>
	int mfGeometric2D<Traits>::inCircle(sVertex* p1, sVertex* p2, sVertex* p3,
		sVertex* p)
	{
		space a[3][3];
		a[0][0] = p2->getCoord(0) - p1->getCoord(0);
		a[0][1] = p2->getCoord(1) - p1->getCoord(1);
		a[0][2] = a[0][0] * a[0][0] + a[0][1] * a[0][1];

		a[1][0] = p3->getCoord(0) - p1->getCoord(0);
		a[1][1] = p3->getCoord(1) - p1->getCoord(1);
		a[1][2] = a[1][0] * a[1][0] + a[1][1] * a[1][1];

		a[2][0] = p->getCoord(0) - p1->getCoord(0);
		a[2][1] = p->getCoord(1) - p1->getCoord(1);
		a[2][2] = a[2][0] * a[2][0] + a[2][1] * a[2][1];

		space i = det3(a);

		if (i > MF_ERRO_INCIRCLE)
			return 0;
		else {
			if (i < -MF_ERRO_INCIRCLE)
				return 1;
			else
				return 2;
		}
	}

	template<class Traits>
	int mfGeometric2D<Traits>::inCircle(sCell* c, sVertex* p)
	{
		sVertex* p1 = mesh->getVertex(c->getVertexId(0));
		sVertex* p2 = mesh->getVertex(c->getVertexId(1));
		sVertex* p3 = mesh->getVertex(c->getVertexId(2));
		return inCircle(p1, p2, p3, p);
	}

	template<class Traits>
	int mfGeometric2D<Traits>::pointInTetrahedron(sVertex* p0, sVertex* p1,
		sVertex* p2, sVertex* p3, sVertex* p)
	{
		int sign0, sign1, sign2, sign3;
		space dotProdValue;
		space n0[3], n1[3], n2[3], n3[3]; //normal vectors

		n0[0] = ((p1->getCoord(1) - p3->getCoord(1)) * (p2->getCoord(2) -
			p3->getCoord(2))) - ((p1->getCoord(2) - p3->getCoord(2)) *
			(p2->getCoord(1) - p3->getCoord(1)));
		n0[1] = ((p1->getCoord(2) - p3->getCoord(2)) * (p2->getCoord(0) -
			p3->getCoord(0))) - ((p1->getCoord(0) - p3->getCoord(0)) *
			(p2->getCoord(2) - p3->getCoord(2)));
		n0[2] = ((p1->getCoord(0) - p3->getCoord(0)) * (p2->getCoord(1) -
			p3->getCoord(1))) - ((p1->getCoord(1) - p3->getCoord(1)) *
			(p2->getCoord(0) - p3->getCoord(0)));

		n1[0] = ((p0->getCoord(1) - p2->getCoord(1)) * (p3->getCoord(2) - 
			p2->getCoord(2))) - ((p0->getCoord(2) - p2->getCoord(2)) *
			(p3->getCoord(1) - p2->getCoord(1)));
		n1[1] = ((p0->getCoord(2) - p2->getCoord(2)) * (p3->getCoord(0) -
			p2->getCoord(0))) - ((p0->getCoord(0) - p2->getCoord(0)) *
			(p3->getCoord(2) - p2->getCoord(2)));
		n1[2] = ((p0->getCoord(0) - p2->getCoord(0)) * (p3->getCoord(1) -
			p2->getCoord(1))) - ((p0->getCoord(1) - p2->getCoord(1)) *
			(p3->getCoord(0) - p2->getCoord(0)));

		n2[0] = ((p3->getCoord(1) - p1->getCoord(1)) * (p0->getCoord(2) -
			p1->getCoord(2))) - ((p3->getCoord(2) - p1->getCoord(2)) *
			(p0->getCoord(1) - p1->getCoord(1)));
		n2[1] = ((p3->getCoord(2) - p1->getCoord(2)) * (p0->getCoord(0) -
			p1->getCoord(0))) - ((p3->getCoord(0) - p1->getCoord(0)) *
			(p0->getCoord(2) - p1->getCoord(2)));
		n2[2] = ((p3->getCoord(0) - p1->getCoord(0)) * (p0->getCoord(1) -
			p1->getCoord(1))) - ((p3->getCoord(1) - p1->getCoord(1)) *
			(p0->getCoord(0) - p1->getCoord(0)));

		n3[0] = ((p2->getCoord(1) - p0->getCoord(1)) * (p1->getCoord(2) -
			p0->getCoord(2))) - ((p2->getCoord(2) - p0->getCoord(2)) *
			(p1->getCoord(1) - p0->getCoord(1)));
		n3[1] = ((p2->getCoord(2) - p0->getCoord(2)) * (p1->getCoord(0) -
			p0->getCoord(0))) - ((p2->getCoord(0) - p0->getCoord(0)) *
			(p1->getCoord(2) - p0->getCoord(2)));
		n3[2] = ((p2->getCoord(0) - p0->getCoord(0)) * (p1->getCoord(1) -
			p0->getCoord(1))) - ((p2->getCoord(1) - p0->getCoord(1)) *
			(p1->getCoord(0) - p0->getCoord(0)));

		//p in n0
		dotProdValue = (n0[0] * (p->getCoord(0) - p3->getCoord(0))) +
			(n0[1] * (p->getCoord(1) - p3->getCoord(1))) +
			(n0[2] * (p->getCoord(2) - p3->getCoord(2)));
		if (dotProdValue < 0)
			sign0 = -1;
		else if (dotProdValue > 0)
			sign0 = 1;
		else
			sign0 = 0;

		//p in n1
		dotProdValue = (n1[0] * (p->getCoord(0) - p2->getCoord(0))) +
			(n1[1] * (p->getCoord(1) - p2->getCoord(1))) +
			(n1[2] * (p->getCoord(2) - p2->getCoord(2)));
		if (dotProdValue < 0)
			sign1 = -1;
		else if (dotProdValue > 0)
			sign1 = 1;
		else
			sign1 = 0;

		//p in n2
		dotProdValue = (n2[0] * (p->getCoord(0) - p1->getCoord(0))) +
			(n2[1] * (p->getCoord(1) - p1->getCoord(1))) +
			(n2[2] * (p->getCoord(2) - p1->getCoord(2)));
		if (dotProdValue < 0)
			sign0 = -1;
		else if (dotProdValue > 0)
			sign0 = 1;
		else
			sign0 = 0;

		//p in n3
		dotProdValue = (n3[0] * (p->getCoord(0) - p0->getCoord(0))) +
			(n3[1] * (p->getCoord(1) - p0->getCoord(1))) +
			(n3[2] * (p->getCoord(2) - p0->getCoord(2)));
		if (dotProdValue < 0)
			sign0 = -1;
		else if (dotProdValue > 0)
			sign0 = 1;
		else
			sign0 = 0;

		if (sign0 == -1 && sign0 == sign1 && sign0 == sign2 && sign0 == sign3)
			return 1; //inside
		else if (!sign0 || !sign1 || !sign2 || !sign3)
			return 2; //border
		else
			return 0; //outside
	}

	template<class Traits>
	int mfGeometric2D<Traits>::pointInTetrahedron(sCell* c, sVertex* p)
	{
		sVertex* p0 = mesh->getVertex(c->getVertexId(0));
		sVertex* p1 = mesh->getVertex(c->getVertexId(1));
		sVertex* p2 = mesh->getVertex(c->getVertexId(2));
		sVertex* p3 = mesh->getVertex(c->getVertexId(3));
		return pointInTetrahedron(p0, p1, p2, p3, p);
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::det(space matrix[4][4])
	{
		space p1, p2, p3, p4;
		p1 = (matrix[0][0] * (
			matrix[1][1] * matrix[2][2] * matrix[3][3] +
			matrix[1][2] * matrix[2][3] * matrix[3][1] +
			matrix[1][3] * matrix[2][1] * matrix[3][2] -
			matrix[1][3] * matrix[2][2] * matrix[3][1] -
			matrix[1][2] * matrix[2][1] * matrix[3][3] -
			matrix[1][1] * matrix[2][3] * matrix[3][2]));

		p2 = ((-1)*matrix[1][0] * (
			matrix[0][1] * matrix[2][2] * matrix[3][3] +
			matrix[0][2] * matrix[2][3] * matrix[3][1] +
			matrix[0][3] * matrix[2][1] * matrix[3][2] -
			matrix[0][3] * matrix[2][2] * matrix[3][1] -
			matrix[0][2] * matrix[2][1] * matrix[3][3] -
			matrix[0][1] * matrix[2][3] * matrix[3][2]));

		p3 = (matrix[2][0] * (
			matrix[0][1] * matrix[1][2] * matrix[3][3] +
			matrix[0][2] * matrix[1][3] * matrix[3][1] +
			matrix[0][3] * matrix[1][1] * matrix[3][2] -
			matrix[0][3] * matrix[1][2] * matrix[3][1] -
			matrix[0][2] * matrix[1][1] * matrix[3][3] -
			matrix[0][1] * matrix[1][3] * matrix[3][2]));

		p4 = ((-1)*matrix[3][0] * (
			matrix[0][1] * matrix[1][2] * matrix[2][3] +
			matrix[0][2] * matrix[1][3] * matrix[2][1] +
			matrix[0][3] * matrix[1][1] * matrix[2][2] -
			matrix[0][3] * matrix[1][2] * matrix[2][1] -
			matrix[0][2] * matrix[1][1] * matrix[2][3] -
			matrix[0][1] * matrix[1][3] * matrix[2][2]));

		return p1 + p2 + p3 + p4;
	}

	template<class Traits>
	bool mfGeometric2D<Traits>::inDiametralCircle(sVertex* v1, sVertex*v2,
		sVertex* v)
	{
		space x, y, r, d;
		x = (v1->getCoord(0) + v2->getCoord(0)) / 2.0;
		x = (v1->getCoord(1) + v2->getCoord(1)) / 2.0;
		r = sqrt((x - v1->getCoord(0)) * (x - v1->getCoord(0)) +
			(y - v1->getCoord(1)) * (y - v1->getCoord(1)));
		d = sqrt((x - v->getCoord(0)) * (x - v->getCoord(0)) +
			(y - v->getCoord(1)) * (y - v->getCoord(1)));
		if (r < (d - error))
			return false;
		else
			return true;
	}

	template<class Traits>
	bool mfGeometric2D<Traits>::sameVertices(sVertex* v1, sVertex* v2)
	{
		return (fabs(v1->getCoord(0) - v2->getCoord(0)) < erro) &&
			(fabs(v1->getCoord(1) - v2->getCoord(1)) < erro);
	}

	template<class Traits>
	int mfGeometric2D<Traits>::orientation2D(ids* idvertices, sVertex** vertices)
	{
		//Returns 0 if vertices are colinear
		//Returns 1 if orientation has been fixed
		//Returns 2 if orientation was already correct

		space x0, y0, x1, y1, p, n0, n1;
		x0 = vertices[1]->getCoord(0) - vertices[0]->getCoord(0);
		y0 = vertices[1]->getCoord(1) - vertices[0]->getCoord(1);
		n0 = sqrt(x0 * x0 + y0 * y0);
		x0 /= n0;
		y0 /= n0;

		x1 = vertices[2]->getCoord(0) - vertices[0]->getCoord(0);
		y1 = vertices[2]->getCoord(1) - vertices[0]->getCoord(1);
		n1 = sqrt(x1 * x1 + y1 * y1);
		x1 /= n1;
		y1 /= n1;

		p = (x0 * y1 - x1 * y0);

		if (p < -error) {
			sVertex* vaux = vertices[0];
			ids temp = idsvertices[0];

			vertices[0] = vertices[2];
			idvertices[0] = idvertices[2];

			vertices[2] = vaux;
			idvertices[2] = temp;
			return 1;
		}
		else {
			if (p > erro)
				return 2;
			else
				return 0;
		}
	}

	template<class Traits>
	int mfGeometric2D<Traits>::orientation3D(ids* idvertices, sVertex** vertices)
	{
		//Returns 0 if vertices are colinear
		//Returns 1 if orientation has been fixed
		//Returns 2 if orientation was already correct
		space x0, y0, z0, x1, y1, z1, x2, y2, z2;
		double det;

		x0 = vertices[1]->getCoord(0) - vertices[0]->getCoord(0);
		y0 = vertices[1]->getCoord(1) - vertices[0]->getCoord(1);
		z0 = vertices[1]->getCoord(2) - vertices[0]->getCoord(2);

		x1 = vertices[2]->getCoord(0) - vertices[0]->getCoord(0);
		y1 = vertices[2]->getCoord(1) - vertices[0]->getCoord(1);
		z1 = vertices[2]->getCoord(2) - vertices[0]->getCoord(2);

		x2 = vertices[3]->getCoord(0) - vertices[0]->getCoord(0);
		y2 = vertices[3]->getCoord(1) - vertices[0]->getCoord(1);
		z2 = vertices[3]->getCoord(2) - vertices[0]->getCoord(2);

		det = x0 * y1 * z2 + y0 * z1 * x2 + z0 * x1 * y2 -
			z0 * y1 * x2 - y0 * x1 * z2 - x0 * z1 * y2;

		if (det > 0)
			return 2;
		else if (det < 0) {
			sVertex* vaux = vertices[0];
			ids temp = idsvertices[0];

			vertices[0] = vertices[2];
			idvertices[0] = idvertices[2];

			vertices[2] = vaux;
			idvertices[2] = temp;
			return 1;
		}
		else
			return 0;
	}

	template<class Traits>
	bool mfGeometric2D<Traits>::isBadCell(sCell* c, space beta)
	{
		sVertex* v0 = mesh->getVertex(c->getVertexId(0));
		sVertex* v1 = mesh->getVertex(c->getVertexId(1));
		sVertex* v2 = mesh->getVertex(c->getVertexId(2));

		space r, circ[2], d1, d2, d3, resp = 0;
		getCircuncircle(c, circ);
		r = sqrt((v0->getCoord(0) - circ[0]) * (v0->getCoord(0) - circ[0]) +
			(v0->getCoord(1) - circ[1]) * (v0->getCoord(1) - circ[1]));
		d1 = dist(v0, v1);
		d2 = dist(v0, v2);
		d3 = dist(v2, v1);

		if ((d1 < d2) && (d1 < d3))
			resp = r / d1;
		else if ((d2 < d1) && (d2 < d3))
			resp = r / d2;
		else if ((d3 < d1) && (d3 < d2))
			resp = r / d3;

		return (resp > beta);
	}

	template<class Traits>
	void mfGeometric2D<Traits>::getCircuncircle(space* c0, space* c1, space* c2,
		space* coords)
	{
		space a[3][3], bx[3][3], by[3][3];

		a[0][0] = c0[0];
		a[0][1] = c0[1];
		a[0][2] = 1;

		a[1][0] = c1[0];
		a[1][1] = c1[1];
		a[1][2] = 1;

		a[2][0] = c2[0];
		a[2][1] = c2[1];
		a[2][2] = 1;

		space deta = det3(a);

		bx[0][0] = (c0[0] * c0[0]) + (c0[1] * c0[1]);
		bx[0][1] = c0[1];
		bx[0][2] = 1;

		bx[1][0] = (c1[0] * c1[0]) + (c1[1] * c1[1]);
		bx[1][1] = c1[1];
		bx[1][2] = 1;

		bx[2][0] = (c2[0] * c2[0]) + (c2[1] * c2[1]);
		bx[2][1] = c2[1];
		bx[2][2] = 1;

		space detbx = det3(bx);

		by[0][0] = (c0[0] * c0[0]) + (c0[1] * c0[1]);
		by[0][1] = c0[0];
		by[0][2] = 1;

		by[1][0] = (c1[0] * c1[0]) + (c1[1] * c1[1]);
		by[1][1] = c1[0];
		by[1][2] = 1;

		by[2][0] = (c2[0] * c2[0]) + (c2[1] * c2[1]);
		by[2][1] = c2[0];
		by[2][2] = 1;

		space detby = -det3(by);

		coords[0] = (detbx / (2 * deta));
		coords[1] = (detby / (2 * deta));
	}

	template<class Traits>
	void mfGeometric2D<Traits>::getCircuncircle(sCell* c, space* coords)
	{
		sVertex* v0 = mesh->getVertex(c->getVertexId(0));
		sVertex* v1 = mesh->getVertex(c->getVertexId(1));
		sVertex* v2 = mesh->getVertex(c->getVertexId(2));
		
		space* c0 = v0->getCoords();
		space* c1 = v1->getCoords();
		space* c2 = v2->getCoords();

		getCircuncircle(c0, c1, c2, coords);
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::det3(space matrix[3][3])
	{
		return((matrix[0][0] * matrix[1][1] * matrix[2][2]) +
			(matrix[1][0] * matrix[2][1] * matrix[0][2]) +
			(matrix[0][1] * matrix[1][2] * matrix[2][0]) -
			(matrix[0][2] * matrix[1][1] * matrix[2][0]) -
			(matrix[0][1] * matrix[1][0] * matrix[2][2]) -
			(matrix[0][0] * matrix[2][1] * matrix[1][2]));
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::dot(ids p1, ids p2, space* coords)
	{
		sVertex* v1 = mesh->getVertex(p1);
		sVertex* v2 = mesh->getVertex(p2);
		double vv1x, vv1y, vv2x, vv2y, dp;

		vv1x = v1->getCoord(0) - coords[0];
		vv1x = v1->getCoord(1) - coords[1];
		vv2x = v2->getCoord(0) - coords[0];
		vv2x = v2->getCoord(1) - coords[1];
		dp = (vv1x * vv2x) + (vv1y * vv2y);
		return dp;
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::vecAngle(ids p1, ids p_ang, ids p2)
	{
		sVertex* v1 = mesh->getVertex(p1);
		sVertex* v2 = mesh->getVertex(p2);
		sVertex* v_ang = mesh->getVertex(p_ang);
		double vv1x, vv1y, vv2x, vv2y, dp, aux;

		vv1x = v1->getCoord(0) - coords[0];
		vv1x = v1->getCoord(1) - coords[1];
		vv2x = v2->getCoord(0) - coords[0];
		vv2x = v2->getCoord(1) - coords[1];
		dp = (vv1x * vv2x) + (vv1y * vv2y);

		aux = sqrt(vv1x * vv1x + vv1y * vv1y) * sqrt(vv2x * vv2x + vv2y * vv2y);
		return (dp / aux);
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::norm2d(space* coords)
	{
		return sqrt(coords[0] * coords[0] + coords[1] * coords[1]);
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::areaTriangle(space* x, space* y, space* z)
	{
		return fabs((x[1] - z[1]) * (y[2] - z[2]) - (x[2] - z[2]) *
			(y[1] - z[1])) / 2;
	}

	template<class Traits>
	SPACE mfGeometric2D<Traits>::areaTriangle(sCell* c)
	{
		return areaTriangle(mesh->getVertex(c->getVertexId(0))->getCoords(),
			mesh->getVertex(c->getVertexId(1))->getCoords(),
			mesh->getVertex(c->getVertexId(2))->getCoords());
	}

	template<class Traits>
	bool mfGeometric2D<Traits>::isDelaunay(ids idcell)
	{
		ids mates[3] = { mesh->getCell(idcell)->getMateId(0),
			mesh->getCell(idcell)->getMateId(1),
			mesh->getCell(idcell)->getMateId(2) };
		sVertex* v;
		for (int i = 0; i < 3; ++i) {
			if (mates[i] != -1) {
				v = mesh->getVertex(mesh->getCell(mates[i])->getVertexId(
					mesh->getCell(mates[i])->getMateIndex(idcell)));
				if (inCircle(mesh->getCell(idcell), v) != 0)
					return false;
			}
		}
		return true;
	}

#undef SPACE
}
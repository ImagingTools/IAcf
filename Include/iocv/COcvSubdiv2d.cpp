#include <iocv/COcvSubdiv2d.h>


namespace iocv
{


// public methods

void COcvSubdiv2d::GetTrianglesIndices(std::vector<Triangle>& triangleList) const
{
	triangleList.clear();

	int total = (int)(qedges.size() * 4);
	std::vector<bool> edgemask(total, false);

	for (int i = 4; i < total; i += 2){
		if (edgemask[i]){
			continue;
		}

		cv::Point2f a, b, c;
		int edge_a = i;

		int indexA = edgeOrg(edge_a, &a) - 4;
		int edge_b = getEdge(edge_a, NEXT_AROUND_LEFT);

		int indexB = edgeOrg(edge_b, &b) - 4;
		int edge_c = getEdge(edge_b, NEXT_AROUND_LEFT);

		int indexC = edgeOrg(edge_c, &c) - 4;

		edgemask[edge_a] = true;
		edgemask[edge_b] = true;
		edgemask[edge_c] = true;

		Triangle t;
		t.a = indexA;
		t.b = indexB;
		t.c = indexC;

		triangleList.push_back(t);
	}
}


} // namespace iocv



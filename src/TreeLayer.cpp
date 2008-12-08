#include "stdafx.h"
#include "TreeLayer.h"
#include "ActionQueueRenderInstance.h"
#include "Frustum.h"
#include "Renderer.h"

extern Frustum g_Frustum;

TreeLayer::~TreeLayer() {
	delete [] cells;
}

TreeLayer::TreeLayer() {
	numCellColumns = 0;
	numCellRows = 0;
	cells = 0;
	
	// tree planting parameters
	spacing = vec2(15.0f, 15.0f);
	displacement = 5.0f;
}

void TreeLayer::generate(const vec2 &center,
                         float size,
                         function<tuple<float,vec3>(vec2)> elevationFunc) {
	int s = (int)ceilf(size / 8.0f);
	numCellColumns = s;
	numCellRows = s;
	cells = new struct TreeCell[numCellRows * numCellColumns];
	
	float cellSpacing = size / s;
	float cellSize = cellSpacing / 2.0f;
	
	for (int col = 0; col < numCellColumns; ++col) {
		for (int row = 0; row < numCellRows; ++row) {
			TreeCell &cell = cells[numCellRows*col + row];
			generateCell(cell,
			             vec2(col + 0.5f, row + 0.5f) * cellSpacing,
			             cellSize,
			             elevationFunc);
		}
	}
}

void TreeLayer::generateCell(struct TreeCell &cell,
                             const vec2 &center,
                             float size,
                             function<tuple<float,vec3>(vec2)> elevationFunc) {
	vec2 bounds_min = center - vec2(size, size);
	vec2 bounds_max = center + vec2(size, size);
	
	cell.size = size;
	cell.center = center;
	
	vec2 pos;
	for (pos.y = bounds_min.y; pos.y < bounds_max.y; pos.y += spacing.y) {
		for (pos.x = bounds_min.x; pos.x < bounds_max.x; pos.x += spacing.x) {
			float angle = FRAND_RANGE(0.0f, (float)(M_PI * 2.0));
			vec2 offset = vec2(cos(angle), sin(angle)) * SampleNormal(displacement, 0.0f);
			vec2 tuftPos =  pos + offset;
			tuple<float, vec3> zn = elevationFunc(tuftPos);
			
			TreeData tree;
			tree.position = vec3(tuftPos, zn.get<0>());
			tree.seed = rand();
			cell.trees.push_back(tree);
		}
	}
}

void TreeLayer::queueForRender(vector<ActionQueueTreeForRender> &actions) const {
	for (int col = 0; col < numCellColumns; ++col)
		for (int row = 0; row < numCellRows; ++row) {
			const TreeCell &cell = cells[numCellRows*col + row];
			
			if (g_Frustum.isCubeWithin(vec3(cell.center.x, cell.center.y, 0.0), cell.size)) {
				for (vector<TreeData>::const_iterator i=cell.trees.begin(); i!=cell.trees.end(); ++i)
					actions.push_back(ActionQueueTreeForRender(i->position, i->seed));
			}
		}
}

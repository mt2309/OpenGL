#ifndef _CG_RENDER_H

#define _CG_RENDER_H

#include <GL/glut.h>
#include <stdlib.h>
#include <vector>

using namespace std;

struct vtk_file
{
	int point_count;
	int polygon_size;
	int polygon_no;

	vector<float>* points;
	vector<int>* polygons;
	vector<float>* point_data;

};

#endif

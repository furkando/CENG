#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

Vec3 *translation(Vec3 *input, Translation *translation)
{
	Vec3 *r = new Vec3();
	r->x = input->x + translation->tx;
	r->y = input->y + translation->ty;
	r->z = input->z + translation->tz;
	r->colorId = input->colorId;
	return r;
}

Vec3 *rotation(Vec3 *input, Rotation *rotation)
{
	double x = rotation->ux;
	double y = rotation->uy;
	double z = rotation->uz;
	double ix = input->x;
	double iy = input->y;
	double iz = input->z;
	double squx = x * x;
	double squy = y * y;
	double squz = z * z;
	double sqrtL = sqrt(squx + squy + squz);
	double angle = (rotation->angle * 3.14159) / 180;
	double cosa = cos(angle);
	double sina = sin(angle);
	double divisionL = 1.0f / (squx + squy + squz);
	double m[4][4] = {{(squx + (squy + squz) * cosa) * divisionL, (x * y * (1 - cosa) - z * sqrtL * sina) * divisionL, (x * z * (1 - cosa) + y * sqrtL * sina) * divisionL, 0},
					  {(x * y * (1 - cosa) + z * sqrtL * sina) * divisionL, (squy + (squx + squz) * cosa) * divisionL, (y * z * (1 - cosa) - x * sqrtL * sina) * divisionL, 0},
					  {(x * z * (1 - cosa) - y * sqrtL * sina) * divisionL, (y * z * (1 - cosa) + x * sqrtL * sina) * divisionL, (squz + (squx + squy) * cosa) * divisionL, 0},
					  {0, 0, 0, 1}};
	Matrix4 matrix = Matrix4(m);
	Vec4 v = Vec4(ix, iy, iz, 1, input->colorId);
	Vec4 r = multiplyMatrixWithVec4(matrix, v);
	Vec3 *result = new Vec3();
	result->x = r.x;
	result->y = r.y;
	result->z = r.z;
	result->colorId = input->colorId;
	return result;
}

Vec3 *scale(Vec3 *input, Scaling *scaling)
{
	Vec3 *r = new Vec3();
	r->x = input->x * scaling->sx;
	r->y = input->y * scaling->sy;
	r->z = input->z * scaling->sz;
	r->colorId = input->colorId;
	return r;
}

Vec3 Scene::viewingTransformations(Camera *camera, double worldp[4])
{
	double cameraTransformation[4][4] = {{camera->u.x, camera->u.y, camera->u.z, (-1) * (camera->u.x * camera->pos.x + camera->u.y * camera->pos.y + camera->u.z * camera->pos.z)},
										 {camera->v.x, camera->v.y, camera->v.z, (-1) * (camera->v.x * camera->pos.x + camera->v.y * camera->pos.y + camera->v.z * camera->pos.z)},
										 {camera->w.x, camera->w.y, camera->w.z, (-1) * (camera->w.x * camera->pos.x + camera->w.y * camera->pos.y + camera->w.z * camera->pos.z)},
										 {0, 0, 0, 1}};

	double orthogonalProjection[4][4] = {{(2) / (camera->right - camera->left), 0, 0, (-1) * ((camera->right + camera->left) / (camera->right - camera->left))},
										 {0, (2) / (camera->top - camera->bottom), 0, (-1) * ((camera->top + camera->bottom) / (camera->top - camera->bottom))},
										 {0, 0, (-1) * ((2) / (camera->far - camera->near)), (-1) * ((camera->far + camera->near) / (camera->far - camera->near))},
										 {0, 0, 0, 1}};

	double perspectiveProjection[4][4] = {{(2 * camera->near) / (camera->right - camera->left), 0, (camera->right + camera->left) / (camera->right - camera->left), 0},
										  {0, (2 * camera->near) / (camera->top - camera->bottom), (camera->top + camera->bottom) / (camera->top - camera->bottom), 0},
										  {0, 0, (-1) * ((camera->far + camera->near) / (camera->far - camera->near)), (-1) * ((2 * camera->far * camera->near) / (camera->far - camera->near))},
										  {0, 0, -1, 0}};

	double viewPort[4][4] = {{(double(camera->horRes) / 2), 0, 0, ((double(camera->horRes) - 1) / 2)},
							 {0, (double(camera->verRes) / 2), 0, ((double(camera->verRes) - 1) / 2)},
							 {0, 0, 0.5, 0.5},
							 {0, 0, 0, 1}};

	Vec4 realWorld = Vec4(worldp[0], worldp[1], worldp[2], worldp[3], 0);
	Vec4 realToCamera = multiplyMatrixWithVec4(cameraTransformation, realWorld);
	Vec4 cameraToProjection, divided, viewportTrans;
	if (projectionType)
	{
		cameraToProjection = multiplyMatrixWithVec4(perspectiveProjection, realToCamera);
		divided = divideVec4WithScalar(cameraToProjection, cameraToProjection.t);
		viewportTrans = multiplyMatrixWithVec4(viewPort, divided);
	}
	else
	{
		cameraToProjection = multiplyMatrixWithVec4(orthogonalProjection, realToCamera);
		viewportTrans = multiplyMatrixWithVec4(viewPort, cameraToProjection);
	}

	Vec3 newPoint;
	newPoint.x = viewportTrans.x;
	newPoint.y = viewportTrans.y;
	newPoint.z = viewportTrans.z;
	return newPoint;
}

void Scene::coloring(Vec3 first, Vec3 second, Vec3 third, int horSize, int verSize)
{

	int x1, x2, y1, y2;
	double dip;
	Color *color1, *color2;
	for (int mid = 0; mid < 3; mid++)
	{
		if (mid == 0)
		{
			x1 = (int)first.x;
			x2 = (int)second.x;
			y1 = (int)first.y;
			y2 = (int)second.y;
			dip = (second.y - first.y) / (second.x - first.x);
			color1 = colorsOfVertices[first.colorId - 1];
			color2 = colorsOfVertices[second.colorId - 1];
		}
		else if (mid == 1)
		{
			x1 = (int)second.x;
			x2 = (int)third.x;
			y1 = (int)second.y;
			y2 = (int)third.y;
			dip = (third.y - second.y) / (third.x - second.x);
			color1 = colorsOfVertices[second.colorId - 1];
			color2 = colorsOfVertices[third.colorId - 1];
		}
		else if (mid == 2)
		{
			x1 = (int)third.x;
			x2 = (int)first.x;
			y1 = (int)third.y;
			y2 = (int)first.y;
			dip = (first.y - third.y) / (first.x - third.x);
			color1 = colorsOfVertices[third.colorId - 1];
			color2 = colorsOfVertices[first.colorId - 1];
		}

		int dx2x1 = x2 - x1;
		int dx1x2 = x1 - x2;
		int dy2y1 = y2 - y1;
		int dy1y2 = y1 - y2;

		double color1r = color1->r;
		double color1g = color1->g;
		double color1b = color1->b;

		double color2r = color2->r;
		double color2g = color2->g;
		double color2b = color2->b;

		bool difr = color1r < color2r;
		bool difg = color1g < color2g;
		bool difb = color1b < color2b;

		bool x1gx2 = x1 > x2;
		bool x2gx1 = x2 > x1;
		bool y1gy2 = y1 > y2;
		bool y2gy1 = y2 > y1;

		double dr = abs(color1r - color2r);
		double dg = abs(color1g - color2g);
		double db = abs(color1b - color2b);

		double rrx1 = (dr) / (dx1x2);
		double rgx1 = (dg) / (dx1x2);
		double rbx1 = (db) / (dx1x2);

		double rrx2 = (dr) / (dx2x1);
		double rgx2 = (dg) / (dx2x1);
		double rbx2 = (db) / (dx2x1);

		double rry1 = (dr) / (dy1y2);
		double rgy1 = (dg) / (dy1y2);
		double rby1 = (db) / (dy1y2);

		double rry2 = (dr) / (dy2y1);
		double rgy2 = (dg) / (dy2y1);
		double rby2 = (db) / (dy2y1);
		// cout << "dip:" << dip << endl;
		// cout << x1gx2 << endl
		// 	 << x2gx1 << endl
		// 	 << difr << endl
		// 	 << dx1x2 << "--" << dx2x1 << "--" << color1r << "x1:" << x1 << endl;
		if (dip < (-1))
		{
			if (x1gx2)
			{
				int x = x1;
				int d = 2 * (dx2x1) + (dy2y1);
				double rr = rry2;
				double rg = rgy2;
				double rb = rby2;
				for (int i = 0; i <= (dy2y1); i++)
				{
					if (x >= 0 && x < horSize && y1 + i >= 0 && y1 + i < verSize)
					{
						if (difr)
						{
							image[x][y1 + i].r = color1r + (i * rr);
						}
						else
						{
							image[x][y1 + i].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x][y1 + i].g = color1g + (i * rg);
						}
						else
						{
							image[x][y1 + i].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x][y1 + i].b = color1b + (i * rb);
						}
						else
						{
							image[x][y1 + i].b = color1b - (i * rb);
						}
					}
					if (d < 0)
					{ //NW
						x -= 1;
						d += 2 * ((dx2x1) + (dy2y1));
					}
					else
					{ //N
						d += 2 * (dx2x1);
					}
				}
			}
			else if (x2gx1)
			{
				int x = x1;
				int d = 2 * (dx1x2) + (dy1y2);
				double rr = rry1;
				double rg = rgy1;
				double rb = rby1;
				for (int i = 0; i <= (dy1y2); i++)
				{
					if (x >= 0 && x < horSize && y1 - i >= 0 && y1 - i < verSize)
					{
						if (difr)
						{
							image[x][y1 - i].r = color1r + (i * rr);
						}
						else
						{
							image[x][y1 - i].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x][y1 - i].g = color1g + (i * rg);
						}
						else
						{
							image[x][y1 - i].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x][y1 - i].b = color1b + (i * rb);
						}
						else
						{
							image[x][y1 - i].b = color1b - (i * rb);
						}
					}
					if (d < 0)
					{
						x += 1;
						d += 2 * ((dx1x2) + (dy1y2));
					}
					else
					{
						d += 2 * (dx1x2);
					}
				}
			}
		}
		else if (dip == 0)
		{
			if (x1gx2)
			{
				double rr = rrx1;
				double rg = rgx1;
				double rb = rbx1;
				for (int i = 0; i <= (dx1x2); i++)
				{
					if (x1 - i >= 0 && x1 - i < horSize && y1 >= 0 && y1 < verSize)
					{
						if (difr)
						{
							image[x1 - i][y1].r = color1r + (i * rr);
						}
						else
						{
							image[x1 - i][y1].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1 - i][y1].g = color1g + (i * rg);
						}
						else
						{
							image[x1 - i][y1].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1 - i][y1].b = color1b + (i * rb);
						}
						else
						{
							image[x1 - i][y1].b = color1b - (i * rb);
						}
					}
				}
			}
			else if (x2gx1)
			{
				double rr = rrx2;
				double rg = rgx2;
				double rb = rbx2;
				for (int i = 0; i <= (dx2x1); i++)
				{
					if (x1 + i >= 0 && x1 + i < horSize && y1 >= 0 && y1 < verSize)
					{
						if (difr)
						{
							image[x1 + i][y1].r = color1r + (i * rr);
						}
						else
						{
							image[x1 + i][y1].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1 + i][y1].g = color1g + (i * rg);
						}
						else
						{
							image[x1 + i][y1].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1 + i][y1].b = color1b + (i * rb);
						}
						else
						{
							image[x1 + i][y1].b = color1b - (i * rb);
						}
					}
				}
			}
		}
		else if (dip > 1)
		{
			if (x1gx2)
			{
				int x = x1;
				int d = 2 * (dx1x2) + (dy2y1);
				double rr = rry1;
				double rg = rgy1;
				double rb = rby1;
				for (int i = 0; i <= (dy1y2); i++)
				{
					if (x >= 0 && x < horSize && y1 - i >= 0 && y1 - i < verSize)
					{
						if (difr)
						{
							image[x][y1 - i].r = color1r + (i * rr);
						}
						else
						{
							image[x][y1 - i].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x][y1 - i].g = color1g + (i * rg);
						}
						else
						{
							image[x][y1 - i].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x][y1 - i].b = color1b + (i * rb);
						}
						else
						{
							image[x][y1 - i].b = color1b - (i * rb);
						}
					}
					if (d < 0)
					{ //S
						d += 2 * (dx1x2);
					}
					else
					{ //SW
						x -= 1;
						d += 2 * ((dx1x2) + (dy2y1));
					}
				}
			}
			else if (x2gx1)
			{
				int x = x1;
				int d = 2 * (dx2x1) + (dy1y2);
				double rr = rry2;
				double rg = rgy2;
				double rb = rby2;
				for (int i = 0; i <= (dy2y1); i++)
				{
					if (x >= 0 && x < horSize && y1 + i >= 0 && y1 + i < verSize)
					{
						if (difr)
						{
							image[x][y1 + i].r = color1r + (i * rr);
						}
						else
						{
							image[x][y1 + i].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x][y1 + i].g = color1g + (i * rg);
						}
						else
						{
							image[x][y1 + i].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x][y1 + i].b = color1b + (i * rb);
						}
						else
						{
							image[x][y1 + i].b = color1b - (i * rb);
						}
					}
					if (d < 0) //N
					{
						d += 2 * (dx2x1);
					}
					else //NE
					{
						x += 1;
						d += 2 * ((dx2x1) + (dy1y2));
					}
				}
			}
		}
		else if (x1 == x2)
		{
			if (y1gy2)
			{
				double rr = rry1;
				double rg = rgy1;
				double rb = rby1;
				for (int i = 0; i <= (dy1y2); i++)
				{
					if (x1 >= 0 && x1 < horSize && y1 - i >= 0 && y1 - i < verSize)
					{
						if (difr)
						{
							image[x1][y1 - i].r = color1r + (i * rr);
						}
						else
						{
							image[x1][y1 - i].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1][y1 - i].g = color1g + (i * rg);
						}
						else
						{
							image[x1][y1 - i].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1][y1 - i].b = color1b + (i * rb);
						}
						else
						{
							image[x1][y1 - i].b = color1b - (i * rb);
						}
					}
				}
			}
			else if (y2gy1)
			{
				double rr = rry2;
				double rg = rgy2;
				double rb = rby2;
				for (int i = 0; i <= (dy2y1); i++)
				{
					if (x1 >= 0 && x1 < horSize && y1 + i >= 0 && y1 + i < verSize)
					{
						if (difr)
						{
							image[x1][y1 + i].r = color1r + (i * rr);
						}
						else
						{
							image[x1][y1 + i].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1][y1 + i].g = color1g + (i * rg);
						}
						else
						{
							image[x1][y1 + i].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1][y1 + i].b = color1b + (i * rb);
						}
						else
						{
							image[x1][y1 + i].b = color1b - (i * rb);
						}
					}
				}
			}
		}
		else if (dip > 0 && dip <= 1)
		{
			if (y1gy2)
			{
				int y = y1;
				int d = 2 * (dy2y1) + (dx1x2);
				double rr = rrx1;
				double rg = rgx1;
				double rb = rbx1;
				for (int i = 0; i <= (dx1x2); i++)
				{
					if (x1 - i >= 0 && x1 - i < horSize && y >= 0 && y < verSize)
					{
						if (difr)
						{
							image[x1 - i][y].r = color1r + (i * rr);
						}
						else
						{
							image[x1 - i][y].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1 - i][y].g = color1g + (i * rg);
						}
						else
						{
							image[x1 - i][y].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1 - i][y].b = color1b + (i * rb);
						}
						else
						{
							image[x1 - i][y].b = color1b - (i * rb);
						}
					}
					if (d < 0) //SW
					{
						y -= 1;
						d += 2 * ((dy2y1) + (dx1x2));
					}
					else //W
					{
						d += 2 * (dy2y1);
					}
				}
			}
			else if (y2gy1)
			{
				int y = y1;
				int d = 2 * (dy1y2) + (dx2x1);
				double rr = rrx2;
				double rg = rgx2;
				double rb = rbx2;
				for (int i = 0; i <= (dx2x1); i++)
				{
					if (x1 + i >= 0 && x1 + i < horSize && y >= 0 && y < verSize)
					{
						if (difr)
						{
							image[x1 + i][y].r = color1r + (i * rr);
						}
						else
						{
							image[x1 + i][y].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1 + i][y].g = color1g + (i * rg);
						}
						else
						{
							image[x1 + i][y].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1 + i][y].b = color1b + (i * rb);
						}
						else
						{
							image[x1 + i][y].b = color1b - (i * rb);
						}
					}
					if (d < 0) //NE
					{
						y += 1;
						d += 2 * ((dy1y2) + (dx2x1));
					}
					else //E
					{
						d += 2 * (dy1y2);
					}
				}
			}
		}
		else if (dip >= (-1) && dip < 0)
		{
			if (x1gx2)
			{
				int y = y1;
				int d = 2 * (dy2y1) + (dx2x1);
				double rr = rrx1;
				double rg = rgx1;
				double rb = rbx1;
				for (int i = 0; i <= (dx1x2); i++)
				{
					if (x1 - i >= 0 && x1 - i < horSize && y >= 0 && y < verSize)
					{
						if (difr)
						{
							image[x1 - i][y].r = color1r + (i * rr);
						}
						else
						{
							image[x1 - i][y].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1 - i][y].g = color1g + (i * rg);
						}
						else
						{
							image[x1 - i][y].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1 - i][y].b = color1b + (i * rb);
						}
						else
						{
							image[x1 - i][y].b = color1b - (i * rb);
						}
					}
					if (d < 0)
					{ //W
						d += 2 * (dy2y1);
					}
					else
					{ //NW
						y += 1;
						d += 2 * ((dy2y1) + (dx2x1));
					}
				}
			}
			else if (x2gx1)
			{
				int y = y1;
				int d = 2 * (dy1y2) + (dx1x2);
				double rr = rrx2;
				double rg = rgx2;
				double rb = rbx2;
				for (int i = 0; i <= (dx2x1); i++)
				{
					if (x1 + i >= 0 && x1 + i < horSize && y >= 0 && y < verSize)
					{
						if (difr)
						{
							image[x1 + i][y].r = color1r + (i * rr);
						}
						else
						{
							image[x1 + i][y].r = color1r - (i * rr);
						}

						if (difg)
						{
							image[x1 + i][y].g = color1g + (i * rg);
						}
						else
						{
							image[x1 + i][y].g = color1g - (i * rg);
						}

						if (difb)
						{
							image[x1 + i][y].b = color1b + (i * rb);
						}
						else
						{
							image[x1 + i][y].b = color1b - (i * rb);
						}
					}
					if (d < 0)
					{ //E
						d += 2 * (dy1y2);
					}
					else
					{ //SE
						y -= 1;
						d += 2 * ((dy1y2) + (dx1x2));
					}
				}
			}
		}
	}
}

bool backfaceCulling(Vec3 *first, Vec3 *second, Vec3 *third, Vec3 v)
{

	Vec3 firstv;
	Vec3 secondv;

	firstv.x = second->x - first->x;
	firstv.y = second->y - first->y;
	firstv.z = second->z - first->z;

	secondv.x = third->x - first->x;
	secondv.y = third->y - first->y;
	secondv.z = third->z - first->z;

	Vec3 normalVector = crossProductVec3(firstv, secondv);
	Vec3 normalized = normalizeVec3(normalVector);

	double culling = dotProductVec3(normalized, v);

	if (culling >= 0.00005)
	{
		return false;
	}
	return true;
}

Color colorMaker(double alpha, double beta, double gamma, Color *color1, Color *color2, Color *color3)
{

	double newr = (alpha * color1->r + beta * color2->r + gamma * color3->r);
	double newg = (alpha * color1->g + beta * color2->g + gamma * color3->g);
	double newb = (alpha * color1->b + beta * color2->b + gamma * color3->b);
	Color color;
	color.r = newr;
	color.g = newg;
	color.b = newb;
	return color;
}

double triangleRasterizer(Vec3 first, Vec3 second, Vec3 third, Vec3 point)
{

	double func1 = point.x * (first.y - second.y) + point.y * (second.x - first.x) + first.x * second.y - first.y * second.x;
	double func2_div = 1.0f / (third.x * (first.y - second.y) + third.y * (second.x - first.x) + first.x * second.y - first.y * second.x);
	double res = func1 * func2_div;
	return res;
}

void Scene::forwardRenderingPipeline(Camera *camera)
{
	//cout << 1 << endl;
	int horSize = camera->horRes;
	int verSize = camera->verRes;
	for (auto curModel : models)
	{
		int triangleNumber = curModel->numberOfTriangles;
		int transformationNumber = curModel->numberOfTransformations;
		for (int i = 0; i < triangleNumber; i++)
		{
			//cout << 2 << endl;
			Vec3 *p1 = vertices[curModel->triangles[i].getFirstVertexId() - 1];
			Vec3 *p2 = vertices[curModel->triangles[i].getSecondVertexId() - 1];
			Vec3 *p3 = vertices[curModel->triangles[i].getThirdVertexId() - 1];

			// p1->colorId = curModel->triangles[i].getFirstVertexId();
			// p2->colorId = curModel->triangles[i].getSecondVertexId();
			// p3->colorId = curModel->triangles[i].getThirdVertexId();

			// cout << "triangle number :" << triangleNumber << endl;
			// cout << "cur triangle number :" << i << endl;
			// cout << "vertex id:" << curModel->triangles[i].getFirstVertexId() << endl;
			// cout << "color of 1:" << vertices[curModel->triangles[i].getFirstVertexId() - 1]->colorId << endl;
			// cout << "vertex id:" << curModel->triangles[i].getSecondVertexId() << endl;
			// cout << "color of 2:" << vertices[curModel->triangles[i].getSecondVertexId() - 1]->colorId << endl;
			// cout << "vertex id:" << curModel->triangles[i].getThirdVertexId() << endl;
			// cout << "color of 3:" << vertices[curModel->triangles[i].getThirdVertexId() - 1]->colorId << endl;
			// cout << "image size :" << camera->horRes << "-" << camera->verRes << endl;
			cout << "basla1:" << i << endl;
			cout << p1->x << "--" << p1->y << "--" << p1->z << "--" << p1->colorId << endl;
			cout << p2->x << "--" << p2->y << "--" << p2->z << "--" << p2->colorId << endl;
			cout << p3->x << "--" << p3->y << "--" << p3->z << "--" << p3->colorId << endl;
			cout << "bitir:" << i << endl;

			for (int k = 0; k < transformationNumber; k++)
			{
				if (curModel->transformationTypes[k] == 't')
				{
					// cout << 4 << endl;
					// cout << "beforet:" << p1->colorId << "--" << p2->colorId << "--" << p3->colorId << endl;
					p3 = translation(p3, translations[curModel->transformationIds[k] - 1]);
					p2 = translation(p2, translations[curModel->transformationIds[k] - 1]);
					p1 = translation(p1, translations[curModel->transformationIds[k] - 1]);

					cout << "baslatttt:" << i << endl;
					cout << p1->x << "--" << p1->y << "--" << p1->z << "--" << p1->colorId << endl;
					cout << p2->x << "--" << p2->y << "--" << p2->z << "--" << p2->colorId << endl;
					cout << p3->x << "--" << p3->y << "--" << p3->z << "--" << p3->colorId << endl;
					cout << "bitir:" << i << endl;

					// cout << "aftert:" << p1->colorId << "--" << p2->colorId << "--" << p3->colorId << endl;
					// cout << 5 << endl;
				}
				else if (curModel->transformationTypes[k] == 'r')
				{
					// cout << 6 << endl;
					// cout << "beforer:" << p1->colorId << "--" << p2->colorId << "--" << p3->colorId << endl;
					p3 = rotation(p3, rotations[curModel->transformationIds[k] - 1]);
					p2 = rotation(p2, rotations[curModel->transformationIds[k] - 1]);
					p1 = rotation(p1, rotations[curModel->transformationIds[k] - 1]);

					cout << "baslarrr:" << i << endl;
					cout << p1->x << "--" << p1->y << "--" << p1->z << "--" << p1->colorId << endl;
					cout << p2->x << "--" << p2->y << "--" << p2->z << "--" << p2->colorId << endl;
					cout << p3->x << "--" << p3->y << "--" << p3->z << "--" << p3->colorId << endl;
					cout << "bitir:" << i << endl;

					// cout << "afterr:" << p1->colorId << "--" << p2->colorId << "--" << p3->colorId << endl;

					// cout << 7 << endl;
				}
				else if (curModel->transformationTypes[k] == 's')
				{
					// cout << 8 << endl;
					// cout << "befores:" << p1->colorId << "--" << p2->colorId << "--" << p3->colorId << endl;

					p3 = scale(p3, scalings[curModel->transformationIds[k] - 1]);
					p2 = scale(p2, scalings[curModel->transformationIds[k] - 1]);
					p1 = scale(p1, scalings[curModel->transformationIds[k] - 1]);

					cout << "baslasss:" << i << endl;
					cout << p1->x << "--" << p1->y << "--" << p1->z << "--" << p1->colorId << endl;
					cout << p2->x << "--" << p2->y << "--" << p2->z << "--" << p2->colorId << endl;
					cout << p3->x << "--" << p3->y << "--" << p3->z << "--" << p3->colorId << endl;
					cout << "bitir:" << i << endl;

					// cout << "afters:" << p1->colorId << "--" << p2->colorId << "--" << p3->colorId << endl;

					// cout << 9 << endl;
				}
			}

			cout << "basla:" << i << endl;
			cout << p1->x << "--" << p1->y << "--" << p1->z << "--" << p1->colorId << endl;
			cout << p2->x << "--" << p2->y << "--" << p2->z << "--" << p2->colorId << endl;
			cout << p3->x << "--" << p3->y << "--" << p3->z << "--" << p3->colorId << endl;
			cout << "bitir:" << i << endl;

			if (cullingEnabled)
			{
				// cout << 10 << endl;

				Vec3 tempp1 = Vec3(p1->x, p1->y, p1->z, p1->colorId);
				Vec3 tempp2 = Vec3(p2->x, p2->y, p2->z, p2->colorId);
				Vec3 tempp3 = Vec3(p3->x, p3->y, p3->z, p3->colorId);
				Vec3 temp = addVec3(tempp1, tempp2);
				Vec3 temp2 = addVec3(temp, tempp3);
				Vec3 mid = divideVec3WithScalar(temp2, 3);
				Vec3 vector = subtractVec3(mid, camera->pos);
				bool flag = backfaceCulling(p1, p2, p3, vector);
				// cout << 11 << endl;

				if (flag == false)
				{
					continue;
				}
			}

			double worldp1[4];
			double worldp2[4];
			double worldp3[4];

			worldp1[0] = p1->x;
			worldp1[1] = p1->y;
			worldp1[2] = p1->z;
			worldp1[3] = 1;

			worldp2[0] = p2->x;
			worldp2[1] = p2->y;
			worldp2[2] = p2->z;
			worldp2[3] = 1;

			worldp3[0] = p3->x;
			worldp3[1] = p3->y;
			worldp3[2] = p3->z;
			worldp3[3] = 1;

			// cout << 12 << endl;

			Vec3 res1 = viewingTransformations(camera, worldp1);
			Vec3 res2 = viewingTransformations(camera, worldp2);
			Vec3 res3 = viewingTransformations(camera, worldp3);
			// cout << "basla:" << i << endl;
			// cout << res1.x << "--" << res1.y << "--" << res1.z << "--" << res1.colorId << endl;
			// cout << res2.x << "--" << res2.y << "--" << res2.z << "--" << res2.colorId << endl;
			// cout << res3.x << "--" << res3.y << "--" << res3.z << "--" << res3.colorId << endl;
			// cout << "bitir:" << i << endl;

			// cout << 13 << endl;

			res1.x = (round(res1.x));
			res1.y = (round(res1.y));
			res1.colorId = p1->colorId;

			res2.x = (round(res2.x));
			res2.y = (round(res2.y));
			res2.colorId = p2->colorId;

			res3.x = (round(res3.x));
			res3.y = (round(res3.y));
			res3.colorId = p3->colorId;
			// cout << "p3color" << res3.colorId << p3->colorId << endl;
			if (curModel->type == 0)
			{
				// cout << 14 << endl;
				coloring(res1, res2, res3, horSize, verSize);
				// cout << 15 << endl;
			}

			else
			{
				int minx = min(res1.x, min(res2.x, res3.x));
				int maxx = max(res1.x, max(res2.x, res3.x));
				int miny = min(res1.y, min(res2.y, res3.y));
				int maxy = max(res1.y, max(res2.y, res3.y));

				if (maxx > camera->horRes)
				{
					maxx = camera->horRes;
				}
				if (maxy > camera->verRes)
				{
					maxy = camera->verRes;
				}
				if (minx < 0)
				{
					minx = 0;
				}
				if (miny < 0)
				{
					miny = 0;
				}

				// cout << minx << "--" << miny << "--" << maxx << "--" << maxy << "--" << endl;
				for (int j1 = miny; j1 < maxy; j1++)
				{
					for (int i1 = minx; i1 < maxx; i1++)
					{
						Vec3 point;
						point.x = i1;
						point.y = j1;
						// cout << 16 << endl;

						double alpha = triangleRasterizer(res2, res3, res1, point);
						double beta = triangleRasterizer(res3, res1, res2, point);
						double gamma = triangleRasterizer(res1, res2, res3, point);
						// cout << 17 << endl;

						if (alpha >= 0 && beta >= 0 && gamma >= 0 && alpha <= 1 && beta <= 1 && gamma <= 1)
						{
							// cout << 18 << endl;
							// cout << "i1:" << i1 << "---j1:" << j1 << "---rest---" << res1.colorId << "---" << res2.colorId << "---" << res3.colorId << endl;
							Color color = colorMaker(alpha, beta, gamma, colorsOfVertices[res1.colorId - 1], colorsOfVertices[res2.colorId - 1], colorsOfVertices[res3.colorId - 1]);
							// cout << 18.5 << endl;
							image[i1][j1].r = color.r;
							image[i1][j1].g = color.g;
							image[i1][j1].b = color.b;
							// cout << 19 << endl;
						}
					}
				}
			}
		}
	}
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"
#include <limits>

using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void)
{
	Vector3f zeroVector;

	for (int c = 0; c < cameras.size(); c++)
	{
		ImagePlane imgPlane = cameras[c]->imgPlane;
		Image image = Image(imgPlane.nx, imgPlane.ny);

		for (int i = 0; i < imgPlane.nx; i++)
		{
			for (int j = 0; j < imgPlane.ny; j++)
			{
				Ray ray = cameras[c]->getPrimaryRay(i, j);
				Vector3f color;
				Vector3f pixel;
				Vector3f mirror(1, 1, 1);
				int maxRecursion = pScene->maxRecursionDepth;
				bool isHit = false;

				for (int i = 0; i < maxRecursion; i++)
				{
					float t = std::numeric_limits<float>::infinity();

					ReturnVal retVal, tmp;
					retVal.matIndex = -1;

					for (int ob = 0; ob < objects.size(); ob++)
					{
						tmp = objects[ob]->intersect(ray);
						if (tmp.matIndex != -1)
						{
							if (tmp.t < t && tmp.t > 0)
							{
								t = tmp.t;
								retVal = tmp;
							}
						}
					}

					if (retVal.matIndex != -1)
					{
						isHit = true;
						Vector3f tColor = ambientLight * materials[retVal.matIndex - 1]->ambientRef;
						// cout << "--Start--" << endl;
						// cout << tColor.r << endl;
						bool isShadow;
						for (int l = 0; l < lights.size(); l++)
						{

							Vector3f lightDirection = lights[l]->position - retVal.point;
							Vector3f normalOfLight = lightDirection.Normalize();
							Ray shadowRay(retVal.point + retVal.normal * shadowRayEps, normalOfLight);

							isShadow = false;
							for (int o = 0; o < objects.size(); o++)
							{
								ReturnVal tmpp;
								tmpp = objects[o]->intersect(shadowRay);

								if (tmpp.matIndex != -1)
								{
									if (shadowRay.gett(lights[l]->position) > tmpp.t)
									{
										// cout << "check for shadow" << endl;
										isShadow = true;
										break;
									}
								}
							}

							if (!isShadow)
							{

								float lightDistance = lightDirection.Dot(lightDirection);
								float cost = max(.0f, retVal.normal.Dot(normalOfLight));

								Vector3f diffuse = materials[retVal.matIndex - 1]->diffuseRef * cost;
								tColor += lights[l]->computeLightContribution(diffuse) / lightDistance;
								// cout << tColor.r << endl;

								Vector3f halfVector = (normalOfLight - ray.direction).Normalize();
								float normalHalf = pow(retVal.normal.Dot(halfVector), materials[retVal.matIndex - 1]->phongExp);
								float cosa = max(.0f, normalHalf);
								Vector3f specular = materials[retVal.matIndex - 1]->specularRef * cosa;
								tColor += lights[l]->computeLightContribution(specular) / lightDistance;
								// cout << tColor.r << endl;
							}
						}

						color += tColor * mirror;
						if (materials[retVal.matIndex - 1]->mirrorRef.IsEqual(zeroVector))
						{
							break;
						}
						else
						{
							// cout << color.r << endl;
							// cout << tColor.r << endl;
							// cout << "--End--" << endl;
							mirror *= materials[retVal.matIndex - 1]->mirrorRef;
							ray.origin = retVal.point + retVal.normal * shadowRayEps;
							ray.direction = (ray.direction + retVal.normal * -2 * ray.direction.Dot(retVal.normal)).Normalize();
						}
					}
					else
					{
						break;
					}
				}
				if (isHit)
				{
					pixel = color;
				}
				else
				{
					pixel = backgroundColor;
				}
				pixel.r = pixel.r > 255 ? 255 : pixel.r;
				pixel.g = pixel.g > 255 ? 255 : pixel.g;
				pixel.b = pixel.b > 255 ? 255 : pixel.b;

				Color colorToSet = {(unsigned char)pixel.r, (unsigned char)pixel.g, (unsigned char)pixel.b};
				image.setPixelValue(i, j, colorToSet);
			}
		}
		image.saveImage(cameras[c]->imageName);
	}
}

// Parses XML file.
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if (pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if (pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if (pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != nullptr)
	{
		int id;
		char imageName[64];
		Vector3f pos, gaze, up;
		ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while (pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;

		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if (materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
		else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if (materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while (str[cursor] != '\0')
	{
		for (int cnt = 0; cnt < 3; cnt++)
		{
			if (cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if (cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++;
			while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");

	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while (str[cursor] != '\0')
		{
			for (int cnt = 0; cnt < 3; cnt++)
			{
				if (cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if (cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++;
				while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while (pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

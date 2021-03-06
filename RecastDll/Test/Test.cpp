// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <direct.h>
#include "../RecastDll/stdafx.h"
using namespace std;

extern "C" void SetEnvLog(Envlog log);
extern "C" RecastPtr CreateSoloRecast();
extern "C" RecastPtr CreateTileRecast();
extern "C" RecastPtr CreateTileCacheRecast();
extern "C" void AddConvexPoint(RecastPtr recast, float* p);
extern "C" bool MakeConvexPolygon(RecastPtr recast, int areaType);
extern "C" bool LoadMeshGeometry(RecastPtr recast, const char* objPath);
extern "C" bool BuildBinary(RecastPtr recast, const char* binPath);
extern "C" void LoadMeshBin(RecastPtr recast, const char* filePath);
extern "C" void Release(RecastPtr recast);
extern "C" int FindSmoothPath(RecastPtr recast, float* startPos, float* endPos, float* smoothPath, int* nsmooth);
extern "C" bool AddCylinderObstacle(RecastPtr recast, float* pos, float radius, float height, dtObstacleRef& result);
extern "C" bool RemoveObstacle(RecastPtr recast, dtObstacleRef ref);

void Log(int categary, const char* msg)
{
	if(categary == RC_LOG_PROGRESS)
	{
		printf("[INFO]: %s\n", msg);
	}
	else if(categary == RC_LOG_WARNING)
	{
		printf("[WARNING]: %s\n", msg);
	}
	else if(categary == RC_LOG_ERROR)
	{
		printf("[ERROR]: %s\n", msg);
	}
}

void MakeConvex(RecastPtr recast)
{
	AddConvexPoint(recast, new float[3]{ -3.24209833, 9.998181, -24.5476284 });
	AddConvexPoint(recast, new float[3]{ -0.9683833, 9.998181, -25.0214272 });
	AddConvexPoint(recast, new float[3]{ 0.771945655, 9.998181, -22.9526768 });
	AddConvexPoint(recast, new float[3]{ 0.5242688, 9.998181, -20.67718 });
	MakeConvexPolygon(recast, POLYAREA_WATER);
}

void SoloBuild(RecastPtr recast)
{
	LoadMeshGeometry(recast, "../x64/Debug/Meshes/dungeon.obj");
	BuildBinary(recast, "../x64/Debug/Bins/solo_navmesh");
}

void SoloLoadBin(RecastPtr recast)
{
	LoadMeshBin(recast, "../x64/Debug/Bins/solo_navmesh");
}

void SoloFindPath(RecastPtr recast)
{
	float* startPos = new float[3]{ 43.552830f, 9.998184f, 1.683649f };
	float* endPos = new float[3]{ 23.345955f, 16.084068f, -79.066635f };
	float smoothPath[2048 * 3];
	int nsmooth = 0;
	FindSmoothPath(recast, startPos, endPos, smoothPath, &nsmooth);

	for (int i = 0; i < nsmooth; i++)
	{
		int index = i * 3;
		printf("smooth path point %d: (%f, %f, %f)\n", i + 1, smoothPath[index], smoothPath[index + 1], smoothPath[index + 2]);
	}
}

void TileFindPath(RecastPtr recast)
{
	float* startPos = new float[3]{ -0.072342, 9.998177, -23.939409 };
	float* endPos = new float[3]{ 19.976967, 13.210732, -60.942467 };
	float smoothPath[300];
	int nsmooth = 0;
	FindSmoothPath(recast, startPos, endPos, smoothPath, &nsmooth);

	for (int i = 0; i < nsmooth; i++)
	{
		int index = i * 3;
		printf("smooth path point %d: (%f, %f, %f)\n", i + 1, smoothPath[index], smoothPath[index + 1], smoothPath[index + 2]);
	}
}

void TileBuild(RecastPtr recast)
{
	LoadMeshGeometry(recast, "../x64/Debug/Meshes/dungeon.obj");
	BuildBinary(recast, "../x64/Debug/Bins/tile_navmesh");
}

void TileLoadBin(RecastPtr recast)
{
	LoadMeshBin(recast, "../x64/Debug/Bins/tile_navmesh");
}

void TileCacheBuild(RecastPtr recast)
{
	LoadMeshGeometry(recast, "../x64/Debug/Meshes/dungeon.obj");
	MakeConvex(recast);
	BuildBinary(recast, "../x64/Debug/Bins/tile_cache_navmesh");
}

void TileCacheLoadBin(RecastPtr recast)
{
	LoadMeshBin(recast, "../x64/Debug/Bins/tile_cache_navmesh");
}

dtObstacleRef TileCacheAddObstacle(RecastPtr recast)
{
	dtObstacleRef result = 0;
	float* pos = new float[3]{ 9.465790, 9.498177, -40.617744 };
	if(!AddCylinderObstacle(recast, pos, 1.0f, 2.0f, result))
	{
		cout << "add obstacle failed" << endl;
	}
	return result;
}

void RemoveTileCacheObstacle(RecastPtr recast, dtObstacleRef result)
{
	if(!RemoveObstacle(recast, result))
	{
		cout << "remove obstacle failed" << endl;
	}
}

void TileCacheFindPath(RecastPtr recast)
{
	float* startPos = new float[3]{ -0.423388, 9.998178, -33.546574 };
	float* endPos = new float[3]{ 20.373758, 9.998180, -50.225769 };
	float smoothPath[300];
	int nsmooth = 0;
	FindSmoothPath(recast, startPos, endPos, smoothPath, &nsmooth);

	for (int i = 0; i < nsmooth; i++)
	{
		int index = i * 3;
		printf("smooth path point %d: (%f, %f, %f)\n", i + 1, smoothPath[index], smoothPath[index + 1], smoothPath[index + 2]);
	}
}

int main()
{
	char buff[1000];
	_getcwd(buff, 1000);
	cout << "当前路径是：" << buff << endl;

	SetEnvLog(Log);

	RecastBase* recast = CreateTileCacheRecast();
	TileCacheLoadBin(recast);
	dtObstacleRef result = TileCacheAddObstacle(recast);
	//TileCacheFindPath(recast);
	RemoveTileCacheObstacle(recast, result);
	//TileCacheBuild(recast);

	//RecastPtr recast = CreateTileRecast();
	//TileBuild();
	//TileLoadBin(recast);
	//TileFindPath(recast);

	Release(recast);
	std::cout << "Hello World!\n";
}



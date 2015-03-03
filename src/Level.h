#ifndef LEVEL_H
#define LEVEL_H

#include <list>
#include "IwMaterial.h"
#include "IwModel.h"

class CRenderModel
{
public:

	CRenderModel(){};
	~CRenderModel(){};

	void Init();

	int m_imaterial_id;
	int m_inum_verts;
	int m_inum_indices;
	int m_inum_normals;

	CIwFVec3 *m_pVertStream;
	CIwFVec2 *m_pUVStream1;
	CIwFVec2 *m_pUVStream2;
	CIwFVec3 *m_pNormStream;
	CIwFVec3 *m_pTangentStream;
	CIwFVec3 *m_pBiTangentStream;
	uint16 	*m_pIndexStream;

	CIwMaterial *m_pMaterial;

	CIwFVec3 m_vTransform;
	CIwFVec3 m_vRotation;

};

struct sModel
{
	int m_Id;
	CIwModel *m_pModel;
};


struct sMaterial
{
	int m_Id;
	CIwMaterial *m_pMaterial;
};



class CLevel
{

public:

	CLevel(){};
	~CLevel(){};

	int m_ilevel_id;
	int m_iTotal_object_counts;
	int m_iLevel_size;

	void Render(float xoffset, float yoffset, float zoffset);

	char *m_txtptr;

	std::list<CRenderModel> RenderList;

};


class CLevelManager
{
public:

	CLevelManager();
	~CLevelManager();

	bool LoadLevel(char *name, int id);
	void RenderLevel(int id, float xoffset, float yoffset, float zoffset);
	void Release(); 

	void AddModel(CIwModel *model, int id);
	void AddMaterial(CIwMaterial *material, int id);

	int GetLevelTotalObjectCounts(int id_level);
	int GetLevelMemoryUsed(int id_level);
	int GetLevelCounts();
	int GetModelCounts();
	int GetMaterialCounts();

	std::list<sModel> ModelList;
	std::list<sMaterial> MaterialList;
	std::list<CLevel> LevelList;

	int m_ilevel_counts;

private:

	void PrepareLevel(char *ptr, int id_level);

	int Parse_GetInt();
	CIwFVec3 Parse_GetVecFloat();

	char *m_tmpptr;

};

#endif
#include "IwModelBlockGL.h"
#include "IwModelBlockGen.h"
#include "Level.h"
#include <math.h>

CLevelManager::CLevelManager()
{
	m_ilevel_counts = 0;
	ModelList.clear();
	MaterialList.clear();
	LevelList.clear();
}

CLevelManager::~CLevelManager()
{
//	s3eFreeBase((void*)txtptr); // free loadel txt buffer
}


void CRenderModel::Init()
{
	m_pVertStream = NULL;
	m_pUVStream1 = NULL;
	m_pUVStream2 = NULL;
	m_pNormStream = NULL;
	m_pTangentStream = NULL;
	m_pBiTangentStream = NULL;
	m_pIndexStream = NULL;
	m_pMaterial = NULL;
}

bool CLevelManager::LoadLevel(char *name, int id_level)
{
	int32 fileSize = 0;
	s3eFile*    hFileHandle;
	char*       pDataReadFile;

	hFileHandle = s3eFileOpen(name, "rb");

	if (hFileHandle != NULL)
    {
            
	//	s3eFreeBase((void*)pDataReadFile);
		fileSize = s3eFileGetSize(hFileHandle);
		pDataReadFile = (char*)s3eMallocBase(fileSize); 
		memset(pDataReadFile, 0, fileSize);

		if (s3eFileRead(&pDataReadFile[0], fileSize, 1, hFileHandle) != 1)
		{
			pDataReadFile = NULL;
		}
		
    }
	else 
	{
		pDataReadFile = NULL;
	}
	
	s3eFileClose(hFileHandle);


	if (pDataReadFile != NULL)
	{
		PrepareLevel(pDataReadFile, id_level);
		m_ilevel_counts++;

		return true;
	} 
	else 
	{
		return false; // level not loaded
	}


}


void CLevelManager::AddModel(CIwModel *model, int id)
{
	ModelList.begin();
	sModel tmpModel;

	tmpModel.m_pModel = model;
	tmpModel.m_Id = id;

	ModelList.push_back(tmpModel);

}


void CLevelManager::AddMaterial(CIwMaterial *material, int id)
{
	MaterialList.begin();
	sMaterial tmpMaterial;

	tmpMaterial.m_pMaterial = material;
	tmpMaterial.m_Id = id;

	MaterialList.push_back(tmpMaterial);

}


int CLevelManager::Parse_GetInt()
{
	int var;

	while (m_tmpptr[0] != 0x3d) m_tmpptr++;  // symbol =
	m_tmpptr++;

	sscanf(m_tmpptr, "%d", &var);

	while (m_tmpptr[0] != 0x0a) m_tmpptr++; // symbol next string
	m_tmpptr++;

	return var;
}

CIwFVec3 CLevelManager::Parse_GetVecFloat()
{
	CIwFVec3 vector;

	while (m_tmpptr[0] != 0x3d) m_tmpptr++;  // symbol =
	m_tmpptr++;

	sscanf(m_tmpptr, "%f", &vector.x);
	
	while (m_tmpptr[0] != 0x2c) m_tmpptr++;  // symbol ,
	m_tmpptr++;

	sscanf(m_tmpptr, "%f", &vector.y);

	while (m_tmpptr[0] != 0x2c) m_tmpptr++;  // symbol ,
	m_tmpptr++;

	sscanf(m_tmpptr, "%f", &vector.z);

	while (m_tmpptr[0] != 0x0a) m_tmpptr++; // symbol next string
	m_tmpptr++;

	return vector;

}



void CLevelManager::PrepareLevel(char *ptr, int id_level)
{
	m_tmpptr = ptr;
	LevelList.begin();

	CLevel tmpLevel;
	CRenderModel tmpRenderModel;

	int model_counts, material_id, model_id;
	CIwMaterial *pMat;
	int level_size = 0;

	tmpLevel.m_txtptr = ptr;
	tmpLevel.m_ilevel_id = id_level; 

	/// parse total object counts in the level
	int total_objects = Parse_GetInt();
	tmpLevel.m_iTotal_object_counts = total_objects;


	while (total_objects > 0)
	{
		/// model counts in current batch
		model_counts = Parse_GetInt();

		// parse id material
		material_id = Parse_GetInt();
		std::list<sMaterial>::iterator material_list = MaterialList.begin();
		while (material_list->m_Id != material_id) material_list++;
		pMat = material_list->m_pMaterial;

		for (int i=0; i<model_counts; i++)
		{
			tmpRenderModel.Init();

			/// parse id model
			model_id = Parse_GetInt();
			std::list<sModel>::iterator model_list = ModelList.begin();
			while (model_list->m_Id != model_id) model_list++;

			/// parse mpdel transform and rotation
			CIwFVec3 model_transform = Parse_GetVecFloat();
			CIwFVec3 model_rotation = Parse_GetVecFloat();

			/// copy vertices data
			int num_verts = model_list->m_pModel->GetNumVerts();
			tmpRenderModel.m_pVertStream = (CIwFVec3*)s3eMallocBase(num_verts * sizeof(CIwFVec3)); 
			memcpy(tmpRenderModel.m_pVertStream, model_list->m_pModel->GetVerts(), num_verts * sizeof(CIwFVec3));
			level_size = level_size + num_verts * sizeof(CIwFVec3);

			/// copy indices data
			CIwModelBlockGLTriList* block_verts = (CIwModelBlockGLTriList*)(model_list->m_pModel->GetBlockNamed("CIwModelBlockGLTriList"));
			int num_indices = block_verts->m_NumItems;
			tmpRenderModel.m_pIndexStream = (uint16*)s3eMallocBase(num_indices * sizeof (uint16)); 
			memcpy(tmpRenderModel.m_pIndexStream, block_verts->m_Inds, num_indices * sizeof(uint16));
			level_size = level_size + num_indices * sizeof(uint16);


			/// copy uvs data (chanel 1 and chanel 2 only)
			int num_uvs = model_list->m_pModel->GetNumUVs();
			tmpRenderModel.m_pUVStream1 = (CIwFVec2*)s3eMallocBase(num_uvs * sizeof(CIwFVec2)); 
			memcpy(tmpRenderModel.m_pUVStream1, model_list->m_pModel->GetUVs(0), num_uvs * sizeof(CIwFVec2));
			level_size = level_size + num_uvs * sizeof(CIwFVec2);

			// check 2nd uv channel if present
			if (model_list->m_pModel->GetUVs(1) != NULL)
			{
				tmpRenderModel.m_pUVStream2 = (CIwFVec2*)s3eMallocBase(num_uvs * sizeof(CIwFVec2)); 
				memcpy(tmpRenderModel.m_pUVStream2, model_list->m_pModel->GetUVs(1), num_uvs * sizeof(CIwFVec2));
				level_size = level_size + num_uvs * sizeof(CIwFVec2);
			}

			/// copy normals data
			int num_normals = model_list->m_pModel->GetNumNorms();
			tmpRenderModel.m_pNormStream = (CIwFVec3*)s3eMallocBase(num_normals * sizeof(CIwFVec3));
			memcpy(tmpRenderModel.m_pNormStream, model_list->m_pModel->GetNorms(), num_normals * sizeof(CIwFVec3));
			level_size = level_size + num_normals * sizeof(CIwFVec3);

			/// copy tangents & bitangents data if they are exist
			CIwModelBlockTangents* block_tangents = (CIwModelBlockTangents*)(model_list->m_pModel->GetBlockNamed("CIwModelBlockTangents"));
			
			if (block_tangents != NULL)
			{

				int num_tangents = block_tangents->m_NumItems;
				int tangent_size = block_tangents->GetItemSize();

				tmpRenderModel.m_pTangentStream = (CIwFVec3*)s3eMallocBase(num_tangents * tangent_size); 
				memcpy(tmpRenderModel.m_pTangentStream, block_tangents->m_Tangents, num_tangents * tangent_size);
				level_size = level_size + num_tangents * tangent_size;

				CIwModelBlockBiTangents* block_bitangents = (CIwModelBlockBiTangents*)(model_list->m_pModel->GetBlockNamed("CIwModelBlockBiTangents"));

				int num_bitangents = block_bitangents->m_NumItems;
				int bitangent_size = block_bitangents->GetItemSize();

				tmpRenderModel.m_pBiTangentStream = (CIwFVec3*)s3eMallocBase(num_bitangents * bitangent_size); 
				memcpy(tmpRenderModel.m_pBiTangentStream, block_bitangents->m_BiTangents, num_bitangents * bitangent_size);
				level_size = level_size + num_bitangents * bitangent_size;

			}

			tmpRenderModel.m_inum_verts = num_verts;
			tmpRenderModel.m_inum_indices = num_indices;
			tmpRenderModel.m_inum_normals = num_normals;
			tmpRenderModel.m_vRotation = model_rotation;
			tmpRenderModel.m_vTransform = model_transform;
			tmpRenderModel.m_imaterial_id = material_id;
			tmpRenderModel.m_pMaterial = pMat;

			tmpLevel.RenderList.push_back(tmpRenderModel);

			total_objects--;
		}
	}

	tmpLevel.m_iLevel_size = level_size;
	LevelList.push_back(tmpLevel);

}


void CLevelManager::RenderLevel(int id_level, float xoffset, float yoffset, float zoffset)
{
	
	std::list<CLevel>::iterator level_list = LevelList.begin();

	while (id_level != level_list->m_ilevel_id) level_list++;

	level_list->Render(xoffset, yoffset, zoffset);

}



void CLevel::Render(float xoffset, float yoffset, float zoffset)
{

	CIwFMat rotX, rotY, rotZ;
	CIwFMat ModelMat;


	std::list<CRenderModel>::iterator render_list = RenderList.begin();

	int material_id = render_list->m_imaterial_id;
	IwGxSetMaterial(render_list->m_pMaterial);

	for (int i=0; i<m_iTotal_object_counts; i++)

	{

		ModelMat.SetIdentity();

		float x_rotation = render_list->m_vRotation.x; 
		float y_rotation = render_list->m_vRotation.z; 
		float z_rotation = -render_list->m_vRotation.y; 

		rotX.SetRotX(x_rotation);
		rotY.SetRotY(y_rotation);
		rotZ.SetRotZ(z_rotation);

		ModelMat.CopyRot(rotY * rotZ * rotX);
		ModelMat.Scale(1.0);


		float x_transform = render_list->m_vTransform.x;
		float y_transform = render_list->m_vTransform.z;
		float z_transform = -render_list->m_vTransform.y;


		x_transform = x_transform + xoffset;
		y_transform = y_transform + yoffset;
		z_transform = z_transform + zoffset;
			

		ModelMat.SetTrans(CIwFVec3(x_transform,y_transform,z_transform));
		IwGxSetModelMatrix(&ModelMat);
	
		IwGxSetVertStreamModelSpace(render_list->m_pVertStream, render_list->m_inum_verts);
		IwGxSetUVStream(render_list->m_pUVStream1,0);
		IwGxSetUVStream(render_list->m_pUVStream2,1);
		IwGxSetNormStream(render_list->m_pNormStream, render_list->m_inum_normals);
		IwGxSetTangentStream(render_list->m_pTangentStream);
		IwGxSetBiTangentStream(render_list->m_pBiTangentStream);
		IwGxSetColStream(NULL);

		if (material_id != render_list->m_imaterial_id)
		{
			material_id = render_list->m_imaterial_id;
			IwGxSetMaterial(render_list->m_pMaterial);
		}

		IwGxDrawPrims(IW_GX_TRI_LIST, render_list->m_pIndexStream, render_list->m_inum_indices);

		render_list++;
	}


}

int CLevelManager::GetLevelTotalObjectCounts(int id_level)
{
	std::list<CLevel>::iterator level = LevelList.begin();

	while (id_level != level->m_ilevel_id) level++;
	return level->m_iTotal_object_counts;

}

/// return in bytes all allocated buffers size 
int CLevelManager::GetLevelMemoryUsed(int id_level)
{
	std::list<CLevel>::iterator level = LevelList.begin();

	while (id_level != level->m_ilevel_id) level++;
	return level->m_iLevel_size;
}

int CLevelManager::GetLevelCounts()
{
	return LevelList.size();
}

int CLevelManager::GetModelCounts()
{
	return ModelList.size();
}


int CLevelManager::GetMaterialCounts()
{
	return MaterialList.size();
}

void CLevelManager::Release()
{
	std::list<CLevel>::iterator level = LevelList.begin();

	if (!LevelList.empty())
	{

		while (level != LevelList.end())
		{
			s3eFreeBase((void*)level->m_txtptr);

			std::list<CRenderModel>::iterator model = level->RenderList.begin();

			while (model != level->RenderList.end())
			{
				if (model->m_pVertStream != NULL) s3eFreeBase((void*)model->m_pVertStream);
				if (model->m_pUVStream1 != NULL) s3eFreeBase((void*)model->m_pUVStream1);
				if (model->m_pUVStream2 != NULL) s3eFreeBase((void*)model->m_pUVStream2);
				if (model->m_pNormStream != NULL) s3eFreeBase((void*)model->m_pNormStream);
				if (model->m_pTangentStream != NULL) s3eFreeBase((void*)model->m_pTangentStream);
				if (model->m_pBiTangentStream != NULL) s3eFreeBase((void*)model->m_pBiTangentStream);
				if (model->m_pIndexStream != NULL) s3eFreeBase((void*)model->m_pIndexStream);

				model++;
			}

			level->RenderList.clear();
			level++;
		}

		LevelList.clear();

	}

	ModelList.clear();
	MaterialList.clear();
}

/*
void CLevel::Reliase()
{
//	s3eFreeBase((void*)m_txtptr);
	std::list<CRenderModel>::iterator model = RenderList.begin();

	while (model != RenderList.end())
	{
//		model->Release();
		model++;
	}

//	RenderList.clear();
}

void CRenderModel::Release()
{
	if (m_pVertStream != NULL) s3eFreeBase((void*)m_pVertStream);
	if (m_pUVStream1 != NULL) s3eFreeBase((void*)m_pUVStream1);
	if (m_pUVStream2 != NULL) s3eFreeBase((void*)m_pUVStream2);
	if (m_pNormStream != NULL) s3eFreeBase((void*)m_pNormStream);
	if (m_pTangentStream != NULL) s3eFreeBase((void*)m_pTangentStream);
	if (m_pBiTangentStream != NULL) s3eFreeBase((void*)m_pBiTangentStream);
	if (m_pIndexStream != NULL) s3eFreeBase((void*)m_pIndexStream);

}
*/
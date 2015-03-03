#include <list>
#include <math.h>
#include "IwModel.h"

#include "CParticleSystem.h"


void CParticleManager::AddParticleSystem(float xstart, float ystart, float zstart, float particle_size, int lenght, int id, int partice_count)
{

	ParticleSystemList.begin();

	sParticle tmpParticle;
	CParticleSystem tmpParticelSystem;

	float x_pos = xstart;
	float y_pos = ystart;
	float z_pos = zstart;

	tmpParticelSystem.m_id = id;
	tmpParticelSystem.m_iparticle_count = partice_count;
	tmpParticelSystem.m_lenght = lenght;
	tmpParticelSystem.PS_Yoffset = 0.0;
	tmpParticelSystem.PS_Yspeed = M_PI / 128.0;

	float delta_y = (M_PI/2) / float(partice_count);

//	float particle_size = (float)particle_size;

	for (int i=0; i<partice_count; i++)
	{
		tmpParticle.ParticleCurrPos.x = x_pos;
		tmpParticle.ParticleCurrPos.y = y_pos + i*delta_y;
		tmpParticle.ParticleCurrPos.z = z_pos;
		tmpParticle.ParticleEndPos.x = x_pos;
		tmpParticle.ParticleEndPos.y = 0.0;//y_pos + (float)lenght;
		tmpParticle.ParticleEndPos.z = z_pos;
		tmpParticle.ParticleSpeed = M_PI / 128.0;
		tmpParticle.ParticleOffset = (M_PI / 2) + float (delta_y * i);
		tmpParticle.ParticleRotSpeed =  M_PI / 256.0 + float(1 + rand()%64);
		tmpParticle.ParticelRotOffset = M_PI / float (1 + rand ()%360);
		

		tmpParticle.ParticleStartPos.x = x_pos;
		tmpParticle.ParticleStartPos.y = y_pos;
		tmpParticle.ParticleStartPos.z = z_pos;

		tmpParticle.ParticleSize = particle_size;

		tmpParticle.s_VertsParticle[0] = CIwFVec3(-particle_size / 2, -particle_size / 2 , 0.0);
		tmpParticle.s_VertsParticle[1] = CIwFVec3(particle_size / 2, -particle_size / 2 , 0.0);
		tmpParticle.s_VertsParticle[2] = CIwFVec3(particle_size / 2, particle_size / 2 , 0.0);
		tmpParticle.s_VertsParticle[3] = CIwFVec3(-particle_size / 2, particle_size / 2 , 0.0);
		
		tmpParticle.s_UVParticle[0] = CIwFVec2(0.0, 1.0);
		tmpParticle.s_UVParticle[1] = CIwFVec2(1.0, 1.0);
		tmpParticle.s_UVParticle[2] = CIwFVec2(1.0, 0.0);
		tmpParticle.s_UVParticle[3] = CIwFVec2(0.0, 0.0);

		tmpParticle.s_QuadListParticle[0] = 0;
		tmpParticle.s_QuadListParticle[1] = 1;
		tmpParticle.s_QuadListParticle[2] = 2;
		tmpParticle.s_QuadListParticle[3] = 3;

		tmpParticle.s_NormParticle[0] = CIwFVec3(1.0, 1.0, 1.0);
		tmpParticle.s_NormParticle[1] = CIwFVec3(1.0, 1.0, 1.0);
		tmpParticle.s_NormParticle[2] = CIwFVec3(1.0, 1.0, 1.0);
		tmpParticle.s_NormParticle[3] = CIwFVec3(1.0, 1.0, 1.0);

		tmpParticelSystem.ParticleList.push_back(tmpParticle);
	}

	ParticleSystemList.push_back(tmpParticelSystem);

}

void CParticleManager::SetParticleMaterial(CIwMaterial *pMaterial)
{
	m_pParticleMaterial = pMaterial;
}

void CParticleManager::Render(float xoffset, float yoffset, float zoffset, int id)
{
	
	std::list<CParticleSystem>::iterator particlesystem_list = ParticleSystemList.begin();

	while (id != particlesystem_list->m_id) particlesystem_list++;

	std::list<sParticle>::iterator particle_list = particlesystem_list->ParticleList.begin();


	CIwFMat rotX, rotY, rotZ;
	CIwFMat ModelMat;

	IwGxSetMaterial(m_pParticleMaterial);

	int count_particles = particlesystem_list->m_iparticle_count;

	for (int i=0; i<count_particles; i++)
	{

		ModelMat.SetIdentity();

		float x_rotation = 0.0;//render_list->m_vRotation.x; 
		float y_rotation = 0.0;//render_list->m_vRotation.z; 
		float z_rotation = particle_list->ParticelRotOffset; 

		rotX.SetRotX(x_rotation);
		rotY.SetRotY(y_rotation);
		rotZ.SetRotZ(z_rotation);

		ModelMat.CopyRot(rotY * rotZ * rotX);
		ModelMat.Scale(particle_list->ParticelZoom); 


		float x_transform = particle_list->ParticleCurrPos.x;
		float y_transform = particle_list->ParticleCurrPos.y;
		float z_transform = particle_list->ParticleCurrPos.z;


		x_transform = x_transform + xoffset;
		y_transform = y_transform + yoffset;
		z_transform = z_transform + zoffset;
			

		ModelMat.SetTrans(CIwFVec3(x_transform,y_transform,z_transform));
		IwGxSetModelMatrix(&ModelMat);
	
		IwGxSetVertStreamModelSpace(particle_list->s_VertsParticle, 4);
		IwGxSetUVStream(particle_list->s_UVParticle);
		IwGxSetNormStream(particle_list->s_NormParticle, 4);
		IwGxSetColStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, particle_list->s_QuadListParticle, 4);

		particle_list++;
	}

}

void CParticleManager::Update(int id)
{

	std::list<CParticleSystem>::iterator particlesystem_list = ParticleSystemList.begin();

	while (id != particlesystem_list->m_id) particlesystem_list++;

	std::list<sParticle>::iterator particle_list = particlesystem_list->ParticleList.begin();


	int count_particles = particlesystem_list->m_iparticle_count;

	for (int i=0; i<count_particles; i++)
	{

		particlesystem_list->PS_Yoffset = particlesystem_list->PS_Yoffset + particlesystem_list->PS_Yspeed;
		if (particlesystem_list->PS_Yoffset > 2 * M_PI / 2) particlesystem_list->PS_Yoffset = M_PI / 2.0 - particlesystem_list->PS_Yoffset;

		float lenght = particlesystem_list->m_lenght + 10.0f * sinf(particlesystem_list->PS_Yoffset);


		particle_list->ParticleOffset = particle_list->ParticleOffset + particle_list->ParticleSpeed;
		if (particle_list->ParticleOffset  >= M_PI)
			particle_list->ParticleOffset = M_PI / 2.0;

		particle_list->ParticelRotOffset = particle_list->ParticelRotOffset + particle_list->ParticleRotSpeed;

		if (particle_list->ParticleOffset > M_PI * 2)
		{
			particle_list->ParticleRotSpeed =  M_PI / 128.0 + float(1 + rand()%64);
			particle_list->ParticelRotOffset = M_PI / float (1 + rand ()%360);
		}

		particle_list->ParticleCurrPos.y = lenght * cosf (particle_list->ParticleOffset);
		particle_list->ParticelZoom = 0.1 + sinf (particle_list->ParticleOffset);

		particle_list++;

	}
}
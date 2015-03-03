#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <list>
#include "IwModel.h"

struct sParticle
{
	CIwFVec3 ParticleCurrPos;
	CIwFVec3 ParticleStartPos;
	CIwFVec3 ParticleEndPos;
	float ParticleSpeed;
	float ParticleRotSpeed;
	float ParticelRotOffset;
	float ParticleOffset;
	float ParticleSize;
	float ParticelZoom;

	CIwFVec3 s_VertsParticle[4];
	uint16 s_QuadListParticle[4];
	CIwFVec2 s_UVParticle[4];
	CIwFVec3 s_NormParticle[4];
};

class CParticleSystem
{
public:

	CParticleSystem(){};
	~CParticleSystem(){};

	int m_id;
	int m_iparticle_count;
	int m_lenght;
	float PS_Yoffset;
	float PS_Yspeed;
	std::list<sParticle> ParticleList;

};

class CParticleManager 
{
public:
	CParticleManager(){};
	~CParticleManager(){};


	void AddParticleSystem(float xstart, float ystart, float zstart, float particle_size, int lenght, int id, int partice_count);
	void SetParticleMaterial(CIwMaterial *pMaterial);
	void Render(float xoffset, float yoffset, float zoffset, int id);
	void Update(int id);

private:

	CIwMaterial *m_pParticleMaterial;

	std::list<CParticleSystem> ParticleSystemList;

};


#endif
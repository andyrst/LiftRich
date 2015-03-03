#ifndef SCORE_H
#define SCORE_H

#include "s3eIOSGameCenter.h"

class Score
{

public:

	Score();
	~Score();

	void Init();
	void DisplayLeaderBoard();
	void SetScoreGameCenter(int iscore);
	void LoadConfigFromDisc();
	void SaveConfigToDisc(int best_score, int best_level, int saved_level, int saved_score, int num_jack, int num_rocketengine, int num_doublecoins, int num_doublesave, bool music, bool sfx);

	int GetBestScore();
	int GetBestLevel();
	int GetSavedLevel();
	int GetSavedScore();
	int GetNumJack();
	int GetNumRocketEngine();
	int GetNumDoubleCoins();
	int GetNumDoubleSave();
	bool IsMusicOn();
	bool IsSfxOn();

private:


	static void AuthenticationCallbackIos(s3eIOSGameCenterError* error, void* userData);
	static void SetScoreCallbackIos(s3eIOSGameCenterError* error);

	int Parse_GetInt();
	void Parse_ToText(int val, const char *text);

	char *m_tmpptr;

//	const char mytetre[]="dedeeeer";

	int m_iBestScore;
	int m_iBestLevel;
	int m_iSavedLevel;
	int m_iSavedScore;
	int m_iNumJack;
	int m_iNumRocketEngine;
	int m_iNumDoubleCoins;
	int m_iNumDoublesave;
	bool m_bMusicOn;
	bool m_bSfxOn;

};


#endif
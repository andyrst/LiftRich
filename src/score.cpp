#include <string>

#include "s3e.h"
#include "s3eIOSGameCenter.h"
#include "score.h"

#include "s3eDialog.h"

//#include "s3eFlurry.h"

const char* GAMECENTER_LEADERBOARD_CATEGORY = "002";

bool g_Authenticated = false;
static char g_UserNameIos[S3E_IOSGAMECENTER_STRING_MAX] = "Unknown User";


static	s3eFile*    g_FileHandle;                   // Handle to file which is to be operated on
static	char*       g_DataReadFile = NULL;                 // Data which is being kRead from the file

bool g_isloadedconfig = false;

#define FILENAME "ram://config.txt"
#define CONFIG_MAX_SIZE 2048
char g_Ascii_Text[128] = {0};


Score::Score()
{

}

Score::~Score()
{
	s3eFreeBase((void*)g_DataReadFile);
//	delete g_DataReadFile;
}

// best score
// best level
// saved level
// save score
// num_jack
// num_rocketengine
// num_doublecoins
// num_doublesave
// music on/off
// sfx on/off

void Score::Init()
{
	/// check config.txt on the disc

	g_DataReadFile = (char*)s3eMallocBase(CONFIG_MAX_SIZE);

	if (s3eFileCheckExists(FILENAME) == S3E_FALSE)
	{
		/// create new one file

		SaveConfigToDisc(0,0,1,0,3,2,1,2,1,1);
//		SaveConfigToDisc(0,0,1,0,3,53,1,13,1,1);

	}

	g_isloadedconfig = false;

	m_tmpptr = g_DataReadFile;

	m_iBestScore = 0;
	m_iBestLevel = 0;
	m_iSavedLevel = 0;
	m_iNumJack = 0;
	m_iNumRocketEngine = 0;
	m_iNumDoubleCoins = 0;
	m_iNumDoublesave = 0;
	m_bMusicOn = true;
	m_bSfxOn = true;

	
	/// init Apple Game Center

		if (!g_Authenticated)
	        g_Authenticated = s3eIOSGameCenterGetInt(S3E_IOSGAMECENTER_LOCAL_PLAYER_IS_AUTHENTICATED) ? true : false;

	    if (g_Authenticated)
		    return;

	    s3eIOSGameCenterAuthenticate(AuthenticationCallbackIos, NULL);


	/// init Google Game Center
}


void Score::DisplayLeaderBoard()
{

	if (g_Authenticated)
	{
	    s3eIOSGameCenterLeaderboardShowGUI(GAMECENTER_LEADERBOARD_CATEGORY, S3E_IOSGAMECENTER_PLAYER_SCOPE_ALL_TIME);
	}
	else
	{
			s3eDialogAlertInfo info;
			info.m_Message = "There are no Game Center account\nconfigured. You can add or create \n a Game Center account in Settings.";
			info.m_Title = "No Game Center Account";
			info.m_Button[0] = "Ok";
			s3eDialogAlertBox(&info);
	}
}

void Score::SetScoreGameCenter(int iscore)
{

	if (g_Authenticated)
	{
		 s3eIOSGameCenterReportScore(iscore, GAMECENTER_LEADERBOARD_CATEGORY, SetScoreCallbackIos);
	}

 }


void Score::AuthenticationCallbackIos(s3eIOSGameCenterError* error, void* userData)
{

    if (*error != S3E_IOSGAMECENTER_ERR_NONE)
    {
        g_Authenticated = false;
	//	s3eFlurryLogError("error", "Game Center: Authentication Error!");
        return;
    }

    g_Authenticated = true;
    const char* alias = s3eIOSGameCenterGetString(S3E_IOSGAMECENTER_LOCAL_PLAYER_ALIAS);

    if (strlen(alias) > 0)
        strcpy(g_UserNameIos, alias);
    else
        strcpy(g_UserNameIos, "!user alias error!");

 //   s3eIOSGameCenterSetInviteHandler(InviteCallback);
}


void Score::SetScoreCallbackIos(s3eIOSGameCenterError* error)
{
    if (*error != S3E_IOSGAMECENTER_ERR_NONE)
    {
//        AppendMessageColour(RED, "Score upload failed with error: %d", *error);

        if (*error == S3E_IOSGAMECENTER_ERR_COMMUNICATIONS_FAILURE)
		{
        //    s3eFlurryLogError("error", "Game Center: Set score Failed!");
		}
    }


}


void Score::LoadConfigFromDisc()
{

	int Score = 0;
	int32 fileSize = 0;

	g_isloadedconfig = true;
	m_tmpptr = g_DataReadFile;

	g_FileHandle = s3eFileOpen(FILENAME, "rb");

	if (g_FileHandle != NULL)
    {
		fileSize = s3eFileGetSize(g_FileHandle);
		memset(g_DataReadFile, 0, CONFIG_MAX_SIZE);

		if (s3eFileRead(&g_DataReadFile[0], fileSize, 1, g_FileHandle) != 1)
		{
			g_isloadedconfig = false;
		}

		s3eFileClose(g_FileHandle);		
    }
	else
	{
		g_isloadedconfig = false;
	}

	s3eFileClose(g_FileHandle);	

	if (g_isloadedconfig)
	{
		/// parse config file
		m_iBestScore = Parse_GetInt();
		m_iBestLevel = Parse_GetInt();
		m_iSavedLevel = Parse_GetInt();
		m_iSavedScore = Parse_GetInt();
		m_iNumJack = Parse_GetInt();
		m_iNumRocketEngine = Parse_GetInt();
		m_iNumDoubleCoins = Parse_GetInt();
		m_iNumDoublesave = Parse_GetInt();
		
		int onoff = Parse_GetInt(); // music on off
		if (onoff > 0) m_bMusicOn = true;
		else
			m_bMusicOn = false;

		onoff = Parse_GetInt();
		if (onoff > 0) m_bSfxOn = true;
		else
			m_bSfxOn = false;
	}
	else
	{
		m_iBestScore = 0;
		m_iBestLevel = 0;
		m_iSavedLevel = 1;
		m_iSavedScore = 0;
		m_iNumJack = 0;
		m_iNumRocketEngine = 0;
		m_iNumDoubleCoins = 0;
		m_iNumDoublesave = 0;
		m_bMusicOn = true;
		m_bSfxOn = true;

		s3eDialogAlertInfo info;
		info.m_Message = "Loading config failed!";
		info.m_Title = "Error";
		info.m_Button[0] = "Ok";
		s3eDialogAlertBox(&info);
	}
}


void Score::SaveConfigToDisc(int best_score, int best_level, int saved_level, int saved_score, int num_jack, int num_rocketengine, int num_doublecoins, int num_doublesave, bool music, bool sfx)
{

	char asci_text[128] = {0};
	int32 fileSize = 0;


	g_FileHandle = s3eFileOpen(FILENAME, "wb");

	if (g_FileHandle != NULL)
	{
		memset(g_DataReadFile, 0, CONFIG_MAX_SIZE);


		Parse_ToText(best_score, "<best_score=");

		Parse_ToText(best_level, "<best_level=");

		Parse_ToText(saved_level, "<saved_level=");
		
		Parse_ToText(saved_score, "<saved_score=");

		Parse_ToText(num_jack, "<num_jack=");

		Parse_ToText(num_rocketengine, "<num_rocketengine=");

		Parse_ToText(num_doublecoins, "<num_doublecoins=");

		Parse_ToText(num_doublesave, "<num_doublesave=");

		if (music)
			Parse_ToText(1, "<on_music=");
		else
			Parse_ToText(0, "<on_music=");

		if (sfx)
			Parse_ToText(1, "<on_sfx=");
		else
			Parse_ToText(0, "<on_sfx=");

		fileSize = strlen(g_DataReadFile);

		s3eFileWrite (&g_DataReadFile[0], fileSize, 1, g_FileHandle);
		s3eFileClose(g_FileHandle);
	}
	else
	{
		s3eDialogAlertInfo info;
		info.m_Message = "Save config failed!";
		info.m_Title = "Error";
		info.m_Button[0] = "Ok";
		s3eDialogAlertBox(&info);
	}
}

void Score::Parse_ToText(int val, const char *text)
{
	char number[128];
	memset(g_Ascii_Text, 0, 128);
	strcpy(g_Ascii_Text, text);
	snprintf(number, sizeof(number), "%i", val);
	strncat(g_Ascii_Text, number, strlen(number));
	strncat(g_Ascii_Text, ">\n", 2);
	strcpy(g_DataReadFile + strlen(g_DataReadFile), g_Ascii_Text);

}

int Score::Parse_GetInt()
{
	int var;

	while (m_tmpptr[0] != 0x3d) m_tmpptr++;  // symbol =
	m_tmpptr++;

	sscanf(m_tmpptr, "%i", &var);

	while (m_tmpptr[0] != 0x0a) m_tmpptr++; // symbol next string
	m_tmpptr++;

	return var;
}

int Score::GetBestScore()
{
	LoadConfigFromDisc();

	return m_iBestScore;
}

int Score::GetBestLevel()
{
	LoadConfigFromDisc();

	return m_iBestLevel;
}

int Score::GetSavedLevel()
{
	LoadConfigFromDisc();

	return m_iSavedLevel;
}

int Score::GetSavedScore()
{
	LoadConfigFromDisc();

	return m_iSavedScore;
}


int Score::GetNumJack()
{
	LoadConfigFromDisc();

	return m_iNumJack;
}

int Score::GetNumRocketEngine()
{
	LoadConfigFromDisc();

	return m_iNumRocketEngine;
}

int Score::GetNumDoubleCoins()
{
	LoadConfigFromDisc();

	return m_iNumDoubleCoins;
}

int Score::GetNumDoubleSave()
{
	LoadConfigFromDisc();

	return m_iNumDoublesave;
}

bool Score::IsMusicOn()
{
	LoadConfigFromDisc();

	return m_bMusicOn;
}

bool Score::IsSfxOn()
{
	LoadConfigFromDisc();

	return m_bSfxOn;
}
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream> 
#include <list>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "IwGraphics.h"
#include "IwModel.h"
#include "IwModelBuilder.h"
#include "IwResManager.h"
#include "IwGxPrint.h"
//#include "IwGxFontContainer.h"
//#include "IwGxFont.h"
#include "IwModelBlockGL.h"
#include "s3eDialog.h"
//#include "IwBilling.h"
#include "s3eKeyboard.h"
#include "s3eSurface.h"

#include "Vectors.h"
#include "CInput.h"
#include "Audio.h"
#include "Level.h"

//#include "s3eFlurry.h"
#include "social.h"
#include "score.h"
#include "CGAdMob.h"
#include "s3eIOSAppStoreBilling.h"

#include "CParticleSystem.h"

const char AdmobTopBanner[]="";

const char AdmobInterstitalBanner[]="ca-app-pub-0846275235961833/5813173906";


//ca-app-pub-0846275235961833/1382974306 - small banner
//ca-app-pub-0846275235961833/5813173906 - fullscreen

static s3ePaymentRequest g_PaymentRequest;
static s3eTransactionReceipt g_TransactionReceipt;
static s3ePaymentTransaction* g_CompletedPayment = NULL;

static s3eProductInformation* g_ProductInformation = NULL;
static s3ePaymentTransaction* g_PaymentTransaction = NULL;

static const char* g_IAP_ID_Jack = "com.liftrich.game.iap.jack";
static const char* g_IAP_ID_RocketPower = "com.liftrich.game.iap.rocket";
static const char* g_IAP_ID_DoubleCoins = "com.liftrich.game.iap.doublecoins";
static const char* g_IAP_ID_Save = "com.liftrich.game.iap.save";

static const char* g_AndroidPublicKey = NULL;


///ru.rc61.testwork.mygame.liftjack
///ru.rc61.testwork.mygame.rocketpower
///ru.rc61.testwork.mygame.doublecoins 
///ru.rc61.testwork.mygame.save

//static int BGR_COLOR[3]={108, 0, 8}; // main backgroud color

//static int BGR_COLOR[3]={38, 59, 122};

//static int BGR_COLOR[3]={9, 128, 62};

//static int BGR_COLOR[3]={92, 73, 66};

static int BGR_COLOR[3]={136, 18, 66};

static CIwFVec3 BACKGRND_COLOR[5]={

	CIwFVec3(108, 0, 8),
//	CIwFVec3(38, 59, 122),
	CIwFVec3(104, 155, 197),
	CIwFVec3(9, 128, 62),
	CIwFVec3(92, 73, 66),
//	CIwFVec3(170, 164, 155),
	CIwFVec3(168, 148, 112),

//	232,217,176
};

CIwGxSurface *surface;

int bgr_offset = 0;
float wheel_uv_offset = 0.0f;

const int FRAMES_PER_SECOND = 50;

#define WAIT_TIME 3 * FRAMES_PER_SECOND // seconds * FPS

#define ZOOM_ASPECT_FACTOR 0.9

CInput g_Input;
Audio *g_pAudio;
Social *g_pSocial;
Score *g_pScore;
CLevelManager *g_LevelManager;
CParticleManager *g_ParticleManager;

//FlurryAppSpot *g_pFlurryAppSpot;

static int g_AudioSize;
static void* g_AudioData;

CIwSoundSpec* pGameOverSoundSpec;
CIwSoundSpec* pGameWinSoundSpec;
CIwSoundSpec* pGameMuteSoundSpec;
CIwSoundGroup* pSfxGroup;
CIwSoundSpec* pMainMusicSoundSpec;
CIwSoundInst* pSoundGameOverInst;
CIwSoundInst* pSoundGameWinInst;

float fDeltaTime;
int g_SurfaceDirection;

bool g_AudioStatus = true;

CIwResGroup* g_pGroup;

// std::vector<tinyobj::shape_t> shapes;

// Local matrix
CIwFMat     s_ModelMatrix;

CIwModel *objMainPlate;

CIwModel *objMenuWheel2;
CIwModel *objMenuBack;
CIwModel *objGoldCoin;
CIwModel *objDollar;

CIwModel *objMoneyBag;      /// model_id=1 mat id=1
CIwModel *objDollar_pack01; /// model id=2 mat id=1
CIwModel *objDollar_pack02; /// model id=3 mat id=1
CIwModel *objDollar_pack03; /// model id=4 mat id=1
CIwModel *objDollar_pack04; /// model id=5 mat id=1
CIwModel *objBaseball_glove; /// model id=6 mat id=1
CIwModel *objAntique_box;    /// model id=7 mat id=1
CIwModel *objGira;           /// model id=8 mat id=1
CIwModel *objBrick;          /// model id=9 mat id=1
CIwModel *objCoin_pack01;    /// model id=10 mat id=2
CIwModel *objCoin_pack02;    /// model id=11 mat id=2
CIwModel *objCoin_pack03;    /// model id=12 mat id=2
CIwModel *objGold_block;     /// model id=13 mat id=3
CIwModel *objEgg;            /// model id=14 mat id=4
CIwModel *objGoldphone;      /// model id=15 mat id=5
CIwModel *objTvSet;			/// model id=16 mat id=1
CIwModel *objMetall_plate;   /// model id=17 mat id=1
CIwModel *objGem;   /// model id=18 mat id=1


/// IAP models
CIwModel *objIAP_RocketEngine;
CIwModel *objIAP_LiftJack;
CIwModel *objIAP_DoubleCoins;
CIwModel *objIAP_DoubleSave;

CIwModel *objGameName;


//CIwGxFont *g_font;

CIwTexture *g_TextureFadeOut = NULL;
CIwTexture *g_TextureFadeIn = NULL;
CIwTexture *g_TextureGameOver = NULL;
CIwTexture *g_TextureGameUI = NULL;
CIwTexture *g_TextureHelpWin = NULL;
CIwTexture *g_TextureTimeBar = NULL;
CIwTexture *g_TextureMotivator = NULL;
CIwTexture *g_TextureDeMotivator = NULL;
CIwTexture *g_TextureFinalCut = NULL;
CIwMaterial *g_MaterialFadeOut;
CIwMaterial *g_MaterialFadeIn;
CIwMaterial *g_MaterialGameOver;
CIwMaterial *g_MaterialGameUI;
CIwMaterial *g_MaterialHelpWin;
CIwMaterial *g_MaterialTimeBar;
CIwMaterial *g_MaterialMotivator;
CIwMaterial *g_MaterialDeMotivator;
CIwMaterial *g_MaterialFinalCut;




CIwTexture *g_TextureParticle = NULL;
CIwMaterial *g_MaterialParticle;


CIwTexture *g_TextureShop = NULL;
CIwMaterial *g_MaterialShop;




// Handles to shaders and surfaces
CIwGxShaderTechnique* s_PostProcessShader;
CIwGxShaderTechnique* s_BloomBufferShader;
CIwGxShaderTechnique* s_BlurBufferShader;
CIwGxShaderTechnique *s_UVChange;
CIwGxShaderUniform* s_UVChangeParam;
/// surfaces

enum CSurfaceID
{
    OFFSCREEN_SCENE, //Main scene (full size buffer)
    BLOOM_BUFFER,    //Peak-extracted scene (small size buffer)
    BLUR_BUFFER_1,   //horizontally blurred bloom (small size buffer)
    BLUR_BUFFER_2,   //fully blurred bloom (small size buffer)

    SURFACE_COUNT,
    DISABLED,
};
CIwGxSurface* s_Surface[SURFACE_COUNT];
int s_DrawSurface = DISABLED;
#define BLOOM_DIM 8

int g_TexturesSize = 0;
float g_FontSizeX = 0;
float g_FontSizeY = 0;

float g_ScaleFactor = 0;
float g_AspectRatio = 0;
int g_ScreenWidth = 0;
int g_ScreenHeight = 0;

int g_CurrentLevel = 1;
int g_ShopReturnCallBack = -1;

float g_ymotivator_offset = 0.0f;
bool g_bselect_motivator = false;

int g_CurrentScore = 0;
int g_BestScore = 0;
int g_BestLevel = 0;
int g_NumJack = 0;
int g_NumRocketEngine = 0;
int g_NumDoubleCoins = 0;
int g_NumDoubleSave = 0;
bool g_bMusicOn = true;
bool g_bSfxOn = true;

int g_DropCounter = 0;
int g_DropDelay = 60;
float g_LevelPlayTime = 0;
int g_LevelElapsedTime = 0;
bool g_bLevelFirstTouch = false;
float g_TimeBarColorOffset = 0;

uint64 g_iAdStartTime = 0;
uint64 g_iAdWaitTime = 1;
bool init_timer_flag = false;

bool g_bshowAd = false;
bool g_SendFlurry = false;

int g_BannerSize = 0;

/////////////////////////////////////


#define VIRTUAL_SCREEN_HEIGTH 640 ///  virtual resolition. all models and logic are developed for this resolution. default res for scale_factor = 1
#define FADEIN_LENGHT 32
#define COINS_FLY_LENGHT 80

#define TEXTURES_SIZE_256 256
#define TEXTURES_SIZE_512 512
#define TEXTURES_SIZE_1024 1024

#define RED_TEXT 0
#define BLACK_TEXT 1

#define PRESSED_BUTTON_ZOOM 0.9

int amp_offset = 0;
int skip_counter = 0;
float y_wallpos = 0;
int drop_counter = 0;
int distort_counter = 0;
float rot_factorX = 0.0;
float rot_factorY = (float)M_PI / 4.0;

float t_dist_time = 0;
float cam_offset_x = 0;

float t_zoom_time = 0;
float t_zoom_time2 = 0;
float t_zoom_time3 = 0;

bool b_gameover_sound = false;

#define STATE_NORMAL_ACTION 0
#define STATE_DROP_DOWN 1
#define STATE_OUTOF_TIME 2
#define STATE_GAME_OVER 3
#define STATE_LEVEL_COMPLETE 4


#define GLOBAL_STATE_GAME_PLAY 0
#define GLOBAL_STATE_GAME_OVER 1
#define GLOBAL_STATE_MAIN_MENU 2
#define GLOBAL_STATE_NEXT_LEVEL 3
#define GLOBAL_STATE_SHOP 4
#define GLOBAL_STATE_FINALCUT 5


#define CAMERA_DISTORT_COUNT 10 // * FPS  number of cam distortions
#define CAMERA_DISTORT_RADIUS 120
#define CAMERA_DISTORT_SPEED 2

#define MENU_WHEEL_SPEED 1

#define DROP_SPEED 6.0
#define MAX_SPEED 16.0



#define MENU_BUTTON_PRESSED 1
#define MENU_BUTTON_NOTPRESSED 0

#define MENU_ACTION_NONE 0
#define MENU_ACTION_PLAY 1
#define MENU_ACTION_SETTINGS 2
#define MENU_ACTION_FACEBOOK 3
#define MENU_ACTION_TWITTER 4
#define MENU_ACTION_SCORE 5
#define MENU_ACTION_AUDIO 6
#define MENU_ACTION_SHOP 7
#define MENU_ACTION_RESET 8

float wheel_rot = 0;

bool prev_touch = false;
int press_id = 0;
int do_menu_action = MENU_ACTION_NONE;

int game_state = STATE_NORMAL_ACTION;
int global_state = GLOBAL_STATE_GAME_PLAY;

int g_LevelDifficulty = 0;
int g_LevelDifficulty2 = 0;

float g_MaxSpeed = 0;

int s_AlphaBlend = 0;

float s_CoinsTime = 0;
int g_finalcut_counter = 0;

//struct sDrawSymbol
//{
//	CIwFVec3 s_VertsScore[4];
//	CIwFVec2 s_UVScore[4];
//};

class CDrawSymbol
{
public:

	CDrawSymbol() 
	{
		m_Verts = new CIwFVec3[4];
		m_UVs = new CIwFVec2[4];
	}


	~CDrawSymbol()
	{
		delete m_Verts;
		delete m_UVs;
	}

	void SetVerts(int index, float xpos, float ypos, float zpos)
	{
		m_Verts[index].x = xpos;
		m_Verts[index].y = ypos;
		m_Verts[index].z = zpos;
	}

	void SetUVs(int index, float upos, float vpos)
	{
		m_UVs[index].x = upos;
		m_UVs[index].y = vpos;
	}

	CIwFVec3 *GetVertsStream()
	{
		return m_Verts;
	}


	CIwFVec2 *GetUVsStream()
	{
		return m_UVs;
	}

private:

	CIwFVec3 *m_Verts;
	CIwFVec2 *m_UVs;

//	CIwFVec3 s_VertsScore[4];
//	CIwFVec2 s_UVScore[4];
};



std::list<CDrawSymbol*> DrawTextList;


struct myIcon{
	int id;
	CIwFVec3 verts[4];
	CIwFVec2 uvs[4];
};

myIcon icon2xScore;
myIcon iconDoubleSave;
myIcon iconJack;
myIcon iconRocket;

struct myButton
{
	int id;
	int state;
	float xpos;
	float ypos;
	float zpos;

	float radius;

	bool do_action;

	CIwFVec3 s_Verts[4];
	CIwFVec2 s_UV[4];


};

#define MAX_BUTTONS 32

CIwFVec3 s_button_Verts[4 * MAX_BUTTONS]; 
CIwFVec2 s_button_UV[4 * MAX_BUTTONS];
uint16 s_button_QuadList[4] = {0, 1, 2, 3,};

myButton MenuPlayButton;
myButton MenuScoreButton;
myButton FaceBookButton;
myButton MenuAudioButton;
myButton MenuAudioButton_Disable;
myButton PauseButton;
myButton ResumeButton;
myButton HomeButton;
myButton NextLevelButton;
myButton ReplayButton;
myButton ShopButton;
myButton ShopExitButton;
myButton MenuResetGameButton;
myButton MenuYesButton;
myButton MenuNoButton;


/// in shop button
myButton ShopBuyJackButton;
myButton ShopBuyRocketPowerButton;
myButton ShopBuyDoubleCoinsButton;
myButton ShopBuyDoubleSaveButton;

/// sleect iap buttons
myButton IAPSelectJackButton;
myButton IAPSelectRocketPowerButton;
myButton IAPSelectDoubleCoinsButton;
myButton IAPSelectDoubleSaveButton;


/// reset dialog data
/// shop data

CIwFVec3 s_VertsReset[4] =
{	
	CIwFVec3(0, 0, 0),
	CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
};

CIwFVec2 s_UVReset[4] =
{
    CIwFVec2(0.0f, 0.0f),
    CIwFVec2(1.0f, 0.0f),
    CIwFVec2(1.0f, 0.9f),
    CIwFVec2(0.0f, 0.9f),
};



/// shop data

CIwFVec3 s_VertsShop[4] =
{	
	CIwFVec3(0, 0, 0),
	CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
};


CIwFVec2 s_UVShop[4] =
{
    CIwFVec2(0.0f, 0.0f),
    CIwFVec2(1.0f, 0.0f),
    CIwFVec2(1.0f, 0.9f),
    CIwFVec2(0.0f, 0.9f),
};



/// fadeOut data

CIwFVec3 s_VertsFadeOut[4] =
{	
	CIwFVec3(0.0f, 0.0f, 0.0f),
	CIwFVec3(0.0f, 0.0f, 0.0f),
    CIwFVec3(0.0f, 0.0f, 0.0f),
    CIwFVec3(0.0f, 0.0f, 0.0f),
};


CIwFVec2 s_UVFadeOut[4] =
{
    CIwFVec2(0, 0),
    CIwFVec2(1, 0),
    CIwFVec2(1, 1),
    CIwFVec2(0, 1),
};





/// fadeIn data

CIwFVec3 s_VertsFadeIn[4] =
{	
	CIwFVec3(0, 0, 0),
	CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
};


CIwFVec2 s_UVFadeIn[4] =
{
    CIwFVec2(0, 0),
    CIwFVec2(1, 0),
    CIwFVec2(1, 1),
    CIwFVec2(0, 1),
};


/// time bar data

CIwFVec3 s_VertsTimeBar[4] =
{	
	CIwFVec3(0, 0, 0),
	CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
};



CIwFVec2 s_UVTimeBar[4] =
{
    CIwFVec2(0, 0),
    CIwFVec2(1, 0),
    CIwFVec2(1, 1),
    CIwFVec2(0, 1),
};



/// motivator data
CIwFVec3 s_VertsMotivator[4] =
{	
	CIwFVec3(0, 0, 0),
	CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
};

CIwFVec2 s_UVMotivator[4] =
{
    CIwFVec2(0, 0),
    CIwFVec2(1, 0),
    CIwFVec2(1, 0.5),
    CIwFVec2(0, 0.5),
};

///



//// game over / level complete data

CIwFVec3 s_VertsGameOver[4] =
{	
	CIwFVec3(0, 0, 0),
	CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
    CIwFVec3(0, 0, 0),
};

CIwFVec2 s_UVGameOver[4] =
{
    CIwFVec2(0, 0),
    CIwFVec2(1, 0),
    CIwFVec2(1, 0.5),
    CIwFVec2(0, 0.5),
};




CIwSoundData *SoundData;

/*
void ReportFlurryNextLevel()
{
	char level_count[64];
	char level_time[64];

	sprintf(level_count, "level|%d", g_CurrentLevel);
	sprintf(level_time, "time|%d", g_LevelElapsedTime);

	s3eFlurryLogEventParams("Level_Complete", level_count);
	s3eFlurryLogEventParams("Level_Complete", level_time);
}


void ReportFlurryGameOver()
{
		char level_count[64];
		char level_time[64];

		printf(level_count, "level|%d", g_CurrentLevel);
		sprintf(level_time, "time|%d", g_LevelElapsedTime);

		s3eFlurryLogEventParams("Game_Over", level_count);
		s3eFlurryLogEventParams("Game_Over", level_time);
}
*/

    //---------------------------------------------------------------------------------------------------------------------
static void flipVertical(unsigned char *data, int w, int h)
{
    int x, y, i1, i2;
    unsigned char temp;

	h = h - g_BannerSize;

    for (x=0;x<w;x++){
    for (y=0;y<h/2;y++){
    i1 = (y*w + x)*4; // this pixel
    i2 = ((h - y - 1)*w + x)*4; // its opposite (across x-axis)
     
    // swap pixels
    temp = data[i1];
    data[i1] = data[i2];
    data[i2] = temp;
     
    i1++; i2++;
    temp = data[i1];
    data[i1] = data[i2];
    data[i2] = temp;
     
    i1++; i2++;
    temp = data[i1];
    data[i1] = data[i2];
    data[i2] = temp;

	i1++; i2++;
    temp = data[i1];
    data[i1] = data[i2];
    data[i2] = temp;
     
    }
    }
}
     
void MakeScreenshot()
{

    static char file[] = "ram://screenshot.png";
     
	int w = g_ScreenWidth;
	int h = g_ScreenHeight;
     
	int ypos = g_BannerSize;
	int xpos = 0;

	IwGxFlush();

    int dataSize = w * (h-ypos) * 4;
 //   unsigned char *framebuffer = (unsigned char *) malloc(dataSize * sizeof(unsigned char));

	unsigned char *framebuffer = (unsigned char *) s3eMallocBase(dataSize * sizeof(unsigned char));

	glReadPixels(0, 0, w, h-ypos, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);
    flipVertical(framebuffer, w, h);
     
     
    CIwImage image;
	image.SetFormat(CIwImage::ABGR_8888);
    image.SetWidth(w);
    image.SetHeight(h-ypos);
    image.SetBuffers(framebuffer, dataSize);
    image.SavePng(file);
    

	s3eFreeBase(framebuffer);
 //   free(framebuffer);
}


uint64 getCurrentTime()
{
	uint64 time = (s3eTimerGetUTC() % 86400000) / 1000;

	return time;
}

uint64 getCurrentTimeMs()
{
	uint64 time = (s3eTimerGetUTC() % 86400000);

	return time;
}


void UpdateConfig()
{

	int iSavedScore = g_pScore->GetSavedScore();
	int iSavedLevel = g_pScore->GetSavedLevel();

	if (g_CurrentLevel == 50 && global_state == GLOBAL_STATE_FINALCUT)
	{
		iSavedLevel = 1;
		iSavedScore = 0;
	}

	if (IAPSelectJackButton.do_action) g_NumJack--;
	if (IAPSelectRocketPowerButton.do_action) g_NumRocketEngine--;
	
	/// do Double Coins
	if (IAPSelectDoubleCoinsButton.do_action)
	{
		g_NumDoubleCoins--;

		/// no 2x coins if game over
		if (global_state != GLOBAL_STATE_GAME_OVER)
			g_CurrentScore = g_CurrentScore * 2;

	}
	
	// do Save
	if (IAPSelectDoubleSaveButton.do_action)
	{
		iSavedLevel = g_CurrentLevel;
		iSavedScore = g_CurrentScore;
		g_NumDoubleSave--;
	}

	if (g_CurrentScore > 999999999) g_CurrentScore = 999999999;

	if (g_CurrentScore > g_BestScore)
	{
		g_BestScore = g_CurrentScore;
	}

	if (g_CurrentLevel > g_BestLevel) g_BestLevel = g_CurrentLevel;

	g_pScore->SaveConfigToDisc(g_BestScore, g_BestLevel, iSavedLevel, iSavedScore, g_NumJack, g_NumRocketEngine, g_NumDoubleCoins, g_NumDoubleSave, g_AudioStatus, true);
	g_pScore->SetScoreGameCenter(g_BestScore);

}



void UpdateConfigPurchase()
{

	int iSavedScore = g_pScore->GetSavedScore();
	int iSavedLevel = g_pScore->GetSavedLevel();

	g_pScore->SaveConfigToDisc(g_BestScore, g_BestLevel, iSavedLevel, iSavedScore, g_NumJack, g_NumRocketEngine, g_NumDoubleCoins, g_NumDoubleSave, g_AudioStatus, true);

}


void loadConfig()
{

	g_BestScore = g_pScore->GetBestScore();
	g_BestLevel = g_pScore->GetBestLevel();	
	g_NumJack = g_pScore->GetNumJack();
	g_NumRocketEngine = g_pScore->GetNumRocketEngine();
	g_NumDoubleCoins = g_pScore->GetNumDoubleCoins();
	g_NumDoubleSave = g_pScore->GetNumDoubleSave();
	g_AudioStatus = g_pScore->IsMusicOn();


	if (g_pScore->GetSavedLevel() > 1)
	{
		g_CurrentScore = g_pScore->GetSavedScore();
		g_CurrentLevel = g_pScore->GetSavedLevel();
	
	}
	else
	{
		g_CurrentScore = 0;
		g_CurrentLevel = 1;
	}

}


void loadObjects()										// All Setup For OpenGL Goes Here
{	

		
//		CIwResGroup* pGroup;

		IwResManagerInit();

#ifdef IW_BUILD_RESOURCES
  //      IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif

		IwGetResManager()->LoadGroup("Common.group");


		g_pGroup = IwGetResManager()->GetGroupNamed("Models");
	
		objMainPlate = (CIwModel*)g_pGroup->GetResNamed("main_plate", IW_GRAPHICS_RESTYPE_MODEL);
		objMenuWheel2 = (CIwModel*)g_pGroup->GetResNamed("menu_wheel2", IW_GRAPHICS_RESTYPE_MODEL);
		objGoldCoin = (CIwModel*)g_pGroup->GetResNamed("gold_coin", IW_GRAPHICS_RESTYPE_MODEL);
		objDollar = (CIwModel*)g_pGroup->GetResNamed("dollar", IW_GRAPHICS_RESTYPE_MODEL);

		objIAP_RocketEngine = (CIwModel*)g_pGroup->GetResNamed("iap_rocketengine", IW_GRAPHICS_RESTYPE_MODEL);
		objIAP_LiftJack = (CIwModel*)g_pGroup->GetResNamed("iap_liftjack", IW_GRAPHICS_RESTYPE_MODEL);
		objIAP_DoubleCoins = (CIwModel*)g_pGroup->GetResNamed("iap_doublecoins", IW_GRAPHICS_RESTYPE_MODEL);

		objIAP_DoubleSave = (CIwModel*)g_pGroup->GetResNamed("iap_save", IW_GRAPHICS_RESTYPE_MODEL);

		objMoneyBag = (CIwModel*)g_pGroup->GetResNamed("money_bag", IW_GRAPHICS_RESTYPE_MODEL);
		objDollar_pack01 = (CIwModel*)g_pGroup->GetResNamed("dollar_pack01", IW_GRAPHICS_RESTYPE_MODEL);
		objDollar_pack02 = (CIwModel*)g_pGroup->GetResNamed("dollar_pack02", IW_GRAPHICS_RESTYPE_MODEL);
		objDollar_pack03 = (CIwModel*)g_pGroup->GetResNamed("dollar_pack03", IW_GRAPHICS_RESTYPE_MODEL);
		objDollar_pack04 = (CIwModel*)g_pGroup->GetResNamed("dollar_pack04", IW_GRAPHICS_RESTYPE_MODEL);
		objBaseball_glove = (CIwModel*)g_pGroup->GetResNamed("baseball_glove", IW_GRAPHICS_RESTYPE_MODEL);
		objAntique_box = (CIwModel*)g_pGroup->GetResNamed("antique_box", IW_GRAPHICS_RESTYPE_MODEL);
		objGira = (CIwModel*)g_pGroup->GetResNamed("gira", IW_GRAPHICS_RESTYPE_MODEL);
		objBrick = (CIwModel*)g_pGroup->GetResNamed("brick", IW_GRAPHICS_RESTYPE_MODEL);
		objCoin_pack01 = (CIwModel*)g_pGroup->GetResNamed("coin_pack01", IW_GRAPHICS_RESTYPE_MODEL);
		objCoin_pack02 = (CIwModel*)g_pGroup->GetResNamed("coin_pack02", IW_GRAPHICS_RESTYPE_MODEL);
		objCoin_pack03 = (CIwModel*)g_pGroup->GetResNamed("coin_pack03", IW_GRAPHICS_RESTYPE_MODEL);
		objGold_block = (CIwModel*)g_pGroup->GetResNamed("gold_block", IW_GRAPHICS_RESTYPE_MODEL);
		objEgg = (CIwModel*)g_pGroup->GetResNamed("egg", IW_GRAPHICS_RESTYPE_MODEL);
		objGoldphone = (CIwModel*)g_pGroup->GetResNamed("goldphone", IW_GRAPHICS_RESTYPE_MODEL);
		objTvSet = (CIwModel*)g_pGroup->GetResNamed("tvset", IW_GRAPHICS_RESTYPE_MODEL);
		objMetall_plate = (CIwModel*)g_pGroup->GetResNamed("metal_plate", IW_GRAPHICS_RESTYPE_MODEL);
		objGem = (CIwModel*)g_pGroup->GetResNamed("gem", IW_GRAPHICS_RESTYPE_MODEL);

		objGameName = (CIwModel*)g_pGroup->GetResNamed("game_name", IW_GRAPHICS_RESTYPE_MODEL);


		//Get default font
//		g_font = (CIwGxFont*)g_pGroup->GetResNamed("font", "CIwGxFont");
//		IwGxFontSetFont(g_font);

//		CIwResGroup* pSoundsGroup = IwGetResManager()->GetGroupNamed("Sounds");

		// Retrieve the "fire" sound specification
//		pGameOverSoundSpec = (CIwSoundSpec*) pSoundsGroup->GetResNamed("gameover", "CIwSoundSpec");
//		pGameWinSoundSpec = (CIwSoundSpec*) pSoundsGroup->GetResNamed("gamewin", "CIwSoundSpec");


//		pSoundGameWinInst = (CIwSoundInst*)pGameWinSoundSpec;
//		pSoundGameOverInst = (CIwSoundInst*)pGameOverSoundSpec;

		g_pAudio->PreloadSound("/sounds/game_over.wav");
		g_pAudio->PreloadSound("/sounds/win1.wav");


//		pSoundGameOverInst = pGameOverSoundSpec->Play();
//		pSoundGameOverInst->Stop();


//		pSoundGameWinInst = pGameWinSoundSpec->Play();-
//		pSoundGameWinInst->Stop();

//		pSoundGameWinInst->SetPan(1);
//		pSoundGameWinInst->SetPitch(1);


		/// load shaders
		CIwResGroup*  pShaderGroup = IwGetResManager()->GetGroupNamed("Shaders");
		s_BloomBufferShader = (CIwGxShaderTechnique*)pShaderGroup->GetResNamed("bloombuffer", "CIwGxShaderTechnique");
		s_BlurBufferShader = (CIwGxShaderTechnique*)pShaderGroup->GetResNamed("blurbuffer", "CIwGxShaderTechnique");
		s_PostProcessShader = (CIwGxShaderTechnique*)pShaderGroup->GetResNamed("postprocess", "CIwGxShaderTechnique");
		s_UVChange = (CIwGxShaderTechnique*)pShaderGroup->GetResNamed("uv_change", "CIwGxShaderTechnique");
		s_UVChangeParam = s_UVChange->GetParam("myparam");
		/// load shaders end

		/// fadeout texture and material setup
		uint8 texels[16 * 4] =
		{
	
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,
		0x00,0x00,0x00,0xde,

		};

		g_TextureFadeOut = new CIwTexture;
		g_TextureFadeOut->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureFadeOut->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureFadeOut->SetModifiable(false);
		g_TextureFadeOut->SetMipMapping(false);
		g_TextureFadeOut->CopyFromBuffer(0x4, 0x4, CIwImage::ABGR_8888, 4*4, texels, NULL);

		g_TextureFadeOut->Upload(); 

		g_MaterialFadeOut = new CIwMaterial;
		g_MaterialFadeOut->SetTexture(g_TextureFadeOut);
		g_MaterialFadeOut->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialFadeOut->SetColEmissive(0x808080);
		g_MaterialFadeOut->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);

		s_VertsFadeOut[0] =  CIwFVec3(float(-g_ScreenWidth / 2), float(-g_ScreenHeight / 2), -1.0f);
		s_VertsFadeOut[1] =  CIwFVec3(float(g_ScreenWidth / 2), float(-g_ScreenHeight / 2), -1.0f);
		s_VertsFadeOut[2] =  CIwFVec3(float(g_ScreenWidth / 2), float(g_ScreenHeight / 2), -1.0f);
		s_VertsFadeOut[3] =  CIwFVec3(float(-g_ScreenWidth / 2), float(g_ScreenHeight / 2), -1.0f);


		/// fadein texture setup
		uint8 texels2[16 * 4] =
		{
	
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,

		};


		g_TextureFadeIn = new CIwTexture;
		g_TextureFadeIn->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureFadeIn->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureFadeIn->SetModifiable(true);
		g_TextureFadeIn->SetMipMapping(false);
		g_TextureFadeIn->CopyFromBuffer(0x4, 0x4, CIwImage::ABGR_8888, 4*4, texels2, NULL);

		g_TextureFadeIn->Upload(); 

		g_MaterialFadeIn = new CIwMaterial;
		g_MaterialFadeIn->SetTexture(g_TextureFadeIn);
		g_MaterialFadeIn->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialFadeIn->SetColEmissive(0x808080);
		g_MaterialFadeIn->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);

		s_VertsFadeIn[0] =  CIwFVec3(float(-g_ScreenWidth / 2), float(-g_ScreenHeight / 2), -1.0f);
		s_VertsFadeIn[1] =  CIwFVec3(float(g_ScreenWidth / 2), float(-g_ScreenHeight / 2), -1.0f);
		s_VertsFadeIn[2] =  CIwFVec3(float(g_ScreenWidth / 2), float(g_ScreenHeight / 2), -1.0f);
		s_VertsFadeIn[3] =  CIwFVec3(float(-g_ScreenWidth / 2), float(g_ScreenHeight / 2), -1.0f);
		/// fadein end
		
		
		/// time bar texture setup
		uint8 timebar_texels[16 * 4] =
		{
	
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,

		};


		g_TextureTimeBar = new CIwTexture;
		g_TextureTimeBar->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureTimeBar->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureTimeBar->SetModifiable(true);
		g_TextureTimeBar->SetMipMapping(false);
		g_TextureTimeBar->CopyFromBuffer(0x4, 0x4, CIwImage::ABGR_8888, 4*4, timebar_texels, NULL);

		g_TextureTimeBar->Upload(); 

		g_MaterialTimeBar = new CIwMaterial;
		g_MaterialTimeBar->SetTexture(g_TextureTimeBar);
		g_MaterialTimeBar->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialTimeBar->SetColEmissive(0x808080);
		g_MaterialTimeBar->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);

		/// timebar end
		


		/// set particle material
		g_TextureParticle = new CIwTexture;
		g_TextureParticle->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureParticle->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureParticle->SetModifiable(false);
		g_TextureParticle->SetMipMapping(false);
		g_TextureParticle->LoadFromFile("/models/textures/fx/particle01.png");
		g_TextureParticle->Upload();

		g_MaterialParticle = new CIwMaterial;
		g_MaterialParticle->SetTexture(g_TextureParticle);
		g_MaterialParticle->SetAlphaMode(CIwMaterial::ALPHA_ADD);
		g_MaterialParticle->SetColEmissive(0x808080);
		g_MaterialParticle->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
		/// end particle


		/// set shop material
		g_TextureShop = new CIwTexture;
		g_TextureShop->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureShop->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureShop->SetModifiable(false);
		g_TextureShop->SetMipMapping(false);
		g_TextureShop->LoadFromFile("/ui/shop_1024.png");
		g_TextureShop->Upload();

		g_MaterialShop = new CIwMaterial;
		g_MaterialShop->SetTexture(g_TextureShop);
		g_MaterialShop->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialShop->SetColEmissive(0x808080);
		g_MaterialShop->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
		/// end shop







		/// setup font size
		g_FontSizeX = 52.0f;
		g_FontSizeY = 76.0f;



		/// prepare and load GAME OVER VICTORY texture
		g_TextureGameOver = new CIwTexture;
		g_TextureGameOver->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureGameOver->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureGameOver->SetModifiable(false);
		g_TextureGameOver->SetMipMapping(true);
		g_TextureGameOver->LoadFromFile("/ui/game_over_1024.png");
		g_TextureGameOver->Upload(); 
		g_MaterialGameOver = new CIwMaterial;
		g_MaterialGameOver->SetTexture(g_TextureGameOver);
		g_MaterialGameOver->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialGameOver->SetColEmissive(0x808080);
		g_MaterialGameOver->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);


		/// prepare and load UI texture
		g_TextureGameUI = new CIwTexture;
		g_TextureGameUI->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureGameUI->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureGameUI->SetModifiable(false);
		g_TextureGameUI->SetMipMapping(true);
		g_TextureGameUI->LoadFromFile("/ui/ui_1024.png");
		g_TextureGameUI->Upload(); 
		g_MaterialGameUI = new CIwMaterial;
		g_MaterialGameUI->SetTexture(g_TextureGameUI);
		g_MaterialGameUI->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialGameUI->SetColEmissive(0x808080);
		g_MaterialGameUI->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);



		/// prepare and load Help&Win texture
		g_TextureHelpWin = new CIwTexture;
		g_TextureHelpWin->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureHelpWin->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureHelpWin->SetModifiable(false);
		g_TextureHelpWin->SetMipMapping(false);
		g_TextureHelpWin->LoadFromFile("/ui/helpwin_1024.png");
		g_TextureHelpWin->Upload(); 
		g_MaterialHelpWin = new CIwMaterial;
		g_MaterialHelpWin->SetTexture(g_TextureHelpWin);
		g_MaterialHelpWin->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialHelpWin->SetColEmissive(0x808080);
		g_MaterialHelpWin->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);


		/// prepare and load Motivator texture
		g_TextureMotivator  = new CIwTexture;
		g_TextureMotivator->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureMotivator->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureMotivator->SetModifiable(false);
		g_TextureMotivator->SetMipMapping(false);
		g_TextureMotivator->LoadFromFile("/ui/motivator.png");
		g_TextureMotivator->Upload(); 
		g_MaterialMotivator = new CIwMaterial;
		g_MaterialMotivator->SetTexture(g_TextureMotivator);
		g_MaterialMotivator->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialMotivator->SetColEmissive(0x808080);
		g_MaterialMotivator->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);


		/// prepare and load Demotivator texture
		g_TextureDeMotivator  = new CIwTexture;
		g_TextureDeMotivator->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureDeMotivator->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureDeMotivator->SetModifiable(false);
		g_TextureDeMotivator->SetMipMapping(false);
		g_TextureDeMotivator->LoadFromFile("/ui/demotivator.png");
		g_TextureDeMotivator->Upload(); 
		g_MaterialDeMotivator = new CIwMaterial;
		g_MaterialDeMotivator->SetTexture(g_TextureDeMotivator);
		g_MaterialDeMotivator->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialDeMotivator->SetColEmissive(0x808080);
		g_MaterialDeMotivator->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);


		/// prepare and load Tits texture =)
		g_TextureFinalCut  = new CIwTexture;
		g_TextureFinalCut->SetFormatSW(CIwImage::ABGR_8888);
		g_TextureFinalCut->SetFormatHW(CIwImage::ABGR_8888);
		g_TextureFinalCut->SetModifiable(false);
		g_TextureFinalCut->SetMipMapping(false);
		g_TextureFinalCut->LoadFromFile("/ui/final_cut.png");
		g_TextureFinalCut->Upload(); 
		g_MaterialFinalCut = new CIwMaterial;
		g_MaterialFinalCut->SetTexture(g_TextureFinalCut);
		g_MaterialFinalCut->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		g_MaterialFinalCut->SetColEmissive(0x808080);
		g_MaterialFinalCut->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);



}




//-----------------------------------------------------------------------------
void ResizeSurfaces()
{
    int height = IwGxGetScreenHeight();
    int width = IwGxGetScreenWidth();

    for (uint32 i = 0; i < SURFACE_COUNT; i++)
    {
        if (i)
            s_Surface[i]->RecreateSurface(width/BLOOM_DIM, height/BLOOM_DIM);
        else
            s_Surface[i]->RecreateSurface(width, height);
    }
}


//-----------------------------------------------------------------------------
void RenderPass(CIwGxSurface* src, CIwGxSurface* dst, CIwGxShaderTechnique* shader)
{
    //Select the bloom buffer target as the current surface
    dst->MakeCurrent();

    //Render the frame through the shader
    IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

    CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
    pMat->SetTexture(src->GetTexture());
    pMat->SetShaderTechnique(shader);
    IwGxSetMaterial(pMat);
    CIwSVec2 wh((int16)IwGxGetDisplayWidth(), (int16)IwGxGetDisplayHeight());
    CIwSVec2 xy(0,0);
    CIwFVec2 uvxy(0, 0);
    CIwFVec2 uvwh(s_Surface[0]->GetClientUVExtentFloat());
    IwGxDrawRectScreenSpace(&xy, &wh, &uvxy, &uvwh);


    //no need to flush, will be flushed implicitly when next surface is made current
}



//CIwFMat myModel;

void drawModel(CIwModel *model, Vector3 pos, Vector3 rot, Vector3 scl)
{
	CIwFMat rotX, rotY, rotZ;
	CIwFMat myModel;

	rotX.SetIdentity();
	rotY.SetIdentity();
	rotZ.SetIdentity();

    rotX.SetRotX(rot.x);
    rotY.SetRotY(rot.y);
	rotZ.SetRotZ(rot.z);

	myModel.SetIdentity();


	myModel.CopyRot(rotY * rotX * rotZ);
	myModel.Scale(scl.z);

	myModel.SetTrans(CIwFVec3(pos.x,pos.y,pos.z));

	IwGxSetModelMatrix(&myModel);

	model->Render();


}


void DrawShadow(uint8 opacity)
{
    CIwMaterial* material=IW_GX_ALLOC_MATERIAL();
    material->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
    IwGxSetMaterial(material);
     
    static CIwFVec2 verts[4]={
    CIwFVec2(.0f,.0f),
    CIwFVec2(.0f,(float)s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)),
    CIwFVec2((float)s3eSurfaceGetInt(S3E_SURFACE_WIDTH),(float)s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)),
    CIwFVec2((float)s3eSurfaceGetInt(S3E_SURFACE_WIDTH),.0f)};
    IwGxSetVertStreamScreenSpace(verts,4);
    IwGxSetUVStream(NULL);
     
    uint32 clr=opacity<<24;
    static uint32 colors[4]={clr,clr,clr,clr};
    IwGxSetColStream((CIwColour*)colors);
     
    IwGxDrawPrims(IW_GX_QUAD_LIST,NULL,4);
}

void initButtons()
{

	/// play button
	MenuPlayButton.id = 1;
	MenuPlayButton.xpos = 0.0f;
	MenuPlayButton.ypos = - 180.0f;
	MenuPlayButton.zpos = 1.0f;
	MenuPlayButton.radius = 110.0f;
	MenuPlayButton.state = MENU_BUTTON_NOTPRESSED;
	MenuPlayButton.s_UV[0] = CIwFVec2 (0.56460f, 0.98474f);
	MenuPlayButton.s_UV[1] = CIwFVec2 (0.99437f, 0.98474f);
	MenuPlayButton.s_UV[2] = CIwFVec2 (0.56460f, 0.55496f);
	MenuPlayButton.s_UV[3] = CIwFVec2 (0.99437f, 0.55496f);

	


	/// facebook button
	FaceBookButton.id = 2;
	FaceBookButton.xpos = 0.0f;
	FaceBookButton.ypos = -234.0f;
	FaceBookButton.zpos = 1.0f;
	FaceBookButton.radius = 70.0f;
	FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
	FaceBookButton.s_UV[0] = CIwFVec2 (0.28470f, 0.43396f);
	FaceBookButton.s_UV[1] = CIwFVec2 (0.56310f, 0.43396f);
	FaceBookButton.s_UV[2] = CIwFVec2 (0.28470f, 0.15555f);
	FaceBookButton.s_UV[3] = CIwFVec2 (0.56310f, 0.15555f);



	// score button
	MenuScoreButton.id = 3;
	MenuScoreButton.xpos = 360.0f;
	MenuScoreButton.ypos = -180.0f;
	MenuScoreButton.zpos = 1.0f;
	MenuScoreButton.radius = 70.0f;
	MenuScoreButton.state = MENU_BUTTON_NOTPRESSED;
	MenuScoreButton.s_UV[0] = CIwFVec2 (0.00589f, 0.43512f);
	MenuScoreButton.s_UV[1] = CIwFVec2 (0.28429f, 0.43512f);
	MenuScoreButton.s_UV[2] = CIwFVec2 (0.00589f, 0.15671f);
	MenuScoreButton.s_UV[3] = CIwFVec2 (0.28429f, 0.15671f);




	/// audio button
	MenuAudioButton.id = 4;
	MenuAudioButton.xpos = -380.0f;
	MenuAudioButton.ypos = -20.0f;
	MenuAudioButton.zpos = 1.0f;
	MenuAudioButton.radius = 40.0f;
	MenuAudioButton.state = MENU_BUTTON_NOTPRESSED;
	MenuAudioButton.s_UV[0] = CIwFVec2 (0.00920f, 0.85770f);
	MenuAudioButton.s_UV[1] = CIwFVec2 (0.14867f, 0.85770f);
	MenuAudioButton.s_UV[2] = CIwFVec2 (0.00920f, 0.71822f);
	MenuAudioButton.s_UV[3] = CIwFVec2 (0.14867f, 0.71822f);


	/// audio button disabled
	MenuAudioButton_Disable.id = 0;
	MenuAudioButton_Disable.xpos = -380.0f;
	MenuAudioButton_Disable.ypos = -20.0f;
	MenuAudioButton_Disable.zpos = 1.0f;
	MenuAudioButton_Disable.radius = 40.0f;
	MenuAudioButton_Disable.state = MENU_BUTTON_NOTPRESSED;
	MenuAudioButton_Disable.s_UV[0] = CIwFVec2 (0.00920f, 0.99812f);
	MenuAudioButton_Disable.s_UV[1] = CIwFVec2 (0.14867f, 0.99812f);
	MenuAudioButton_Disable.s_UV[2] = CIwFVec2 (0.00920f, 0.85865f);
	MenuAudioButton_Disable.s_UV[3] = CIwFVec2 (0.14867f, 0.85865f);


	/// pause button
	PauseButton.id = 5;
	PauseButton.xpos = 428.0f;
	PauseButton.ypos = 270.0f;
	PauseButton.zpos = 1.0f;
	PauseButton.radius = 48.0f;
	PauseButton.state = MENU_BUTTON_NOTPRESSED;

	/// resume (play) button
	ResumeButton.id = 6;
	ResumeButton.xpos = 0.0f;
	ResumeButton.ypos = 0.0f;
	ResumeButton.zpos = 1.0f;
	ResumeButton.radius = 110.0f;
	ResumeButton.state = MENU_BUTTON_NOTPRESSED;
	ResumeButton.s_UV[0] = CIwFVec2 (0.56460f, 0.98474f);
	ResumeButton.s_UV[1] = CIwFVec2 (0.99437f, 0.98474f);
	ResumeButton.s_UV[2] = CIwFVec2 (0.56460f, 0.55496f);
	ResumeButton.s_UV[3] = CIwFVec2 (0.99437f, 0.55496f);





	/// home button
	HomeButton.id = 7;
	HomeButton.xpos = -360.0f;
	HomeButton.ypos = -234.0f;
	HomeButton.zpos = 1.0f;
	HomeButton.radius = 70.0f;
	HomeButton.state = MENU_BUTTON_NOTPRESSED;
	HomeButton.s_UV[0] = CIwFVec2 (0.28409f, 0.99331f);
	HomeButton.s_UV[1] = CIwFVec2 (0.56249f, 0.99331f);
	HomeButton.s_UV[2] = CIwFVec2 (0.28409f, 0.71490f);
	HomeButton.s_UV[3] = CIwFVec2 (0.56249f, 0.71490f);
	



	/// next (play) level button
	NextLevelButton.id = 8;
	NextLevelButton.xpos = 360.0f;
	NextLevelButton.ypos = -234.0f;
	NextLevelButton.zpos = 1.0f;
	NextLevelButton.radius = 70.0f;
	NextLevelButton.state = MENU_BUTTON_NOTPRESSED;
	NextLevelButton.s_UV[0] = CIwFVec2 (0.56460f, 0.98474f);
	NextLevelButton.s_UV[1] = CIwFVec2 (0.99437f, 0.98474f);
	NextLevelButton.s_UV[2] = CIwFVec2 (0.56460f, 0.55496f);
	NextLevelButton.s_UV[3] = CIwFVec2 (0.99437f, 0.55496f);

	

	/// replay  button
	ReplayButton.id = 9;
	ReplayButton.xpos = 360.0f;
	ReplayButton.ypos = -234.0f;
	ReplayButton.zpos = 1.0f;
	ReplayButton.radius = 70.0f;
	ReplayButton.state = MENU_BUTTON_NOTPRESSED;
	ReplayButton.s_UV[0] = CIwFVec2(0.56460f, 0.54931f);
	ReplayButton.s_UV[1] = CIwFVec2(0.99437f, 0.54931f);
	ReplayButton.s_UV[2] = CIwFVec2(0.56460f, 0.11954f);
	ReplayButton.s_UV[3] = CIwFVec2(0.99437f, 0.11954f);



		/// shop button
	ShopButton.id = 10;
	ShopButton.xpos = -320.0f;
	ShopButton.ypos = -220.0f;
	ShopButton.zpos = 1.0f;
	ShopButton.radius = 70.0f;
	ShopButton.state = MENU_BUTTON_NOTPRESSED;
	ShopButton.s_UV[0] = CIwFVec2 (0.00589f, 0.71599f);
	ShopButton.s_UV[1] = CIwFVec2 (0.28429f, 0.71599f);
	ShopButton.s_UV[2] = CIwFVec2 (0.00589f, 0.43758f);
	ShopButton.s_UV[3] = CIwFVec2 (0.28429f, 0.43758f);



	/////////// setup buttons for shop

	ShopBuyJackButton.id = 11;
	ShopBuyJackButton.xpos = 204.0f;
	ShopBuyJackButton.ypos = 189.0f;
	ShopBuyJackButton.zpos = 1.0f;
	ShopBuyJackButton.radius = 56.0f;
	ShopBuyJackButton.state = MENU_BUTTON_NOTPRESSED;
	ShopBuyJackButton.s_UV[0] = CIwFVec2 (0.07657f, 0.96589f);
	ShopBuyJackButton.s_UV[1] = CIwFVec2 (0.21777f, 0.96589f);
	ShopBuyJackButton.s_UV[2] = CIwFVec2 (0.07657f, 0.82470f);
	ShopBuyJackButton.s_UV[3] = CIwFVec2 (0.21777f, 0.82470f);


	ShopBuyRocketPowerButton.id = 12;
	ShopBuyRocketPowerButton.xpos = 204.0f;
	ShopBuyRocketPowerButton.ypos = 57.0f;
	ShopBuyRocketPowerButton.zpos = 1.0f;
	ShopBuyRocketPowerButton.radius = 56.0f;
	ShopBuyRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
	ShopBuyRocketPowerButton.s_UV[0] = CIwFVec2 (0.07657f, 0.96589f);
	ShopBuyRocketPowerButton.s_UV[1] = CIwFVec2 (0.21777f, 0.96589f);
	ShopBuyRocketPowerButton.s_UV[2] = CIwFVec2 (0.07657f, 0.82470f);
	ShopBuyRocketPowerButton.s_UV[3] = CIwFVec2 (0.21777f, 0.82470f);


	ShopBuyDoubleCoinsButton.id = 13;
	ShopBuyDoubleCoinsButton.xpos = 204.0f;
	ShopBuyDoubleCoinsButton.ypos = -75.0f;
	ShopBuyDoubleCoinsButton.zpos = 1.0f;
	ShopBuyDoubleCoinsButton.radius = 56.0f;
	ShopBuyDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
	ShopBuyDoubleCoinsButton.s_UV[0] = CIwFVec2 (0.07657f, 0.96589f);
	ShopBuyDoubleCoinsButton.s_UV[1] = CIwFVec2 (0.21777f, 0.96589f);
	ShopBuyDoubleCoinsButton.s_UV[2] = CIwFVec2 (0.07657f, 0.82470f);
	ShopBuyDoubleCoinsButton.s_UV[3] = CIwFVec2 (0.21777f, 0.82470f);


	
	ShopBuyDoubleSaveButton.id = 14;
	ShopBuyDoubleSaveButton.xpos = 204.0f;
	ShopBuyDoubleSaveButton.ypos = -207.0f;
	ShopBuyDoubleSaveButton.zpos = 1.0f;
	ShopBuyDoubleSaveButton.radius = 56.0f;
	ShopBuyDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
	ShopBuyDoubleSaveButton.s_UV[0] = CIwFVec2 (0.24635f, 0.96589f);
	ShopBuyDoubleSaveButton.s_UV[1] = CIwFVec2 (0.38754f, 0.96589f);
	ShopBuyDoubleSaveButton.s_UV[2] = CIwFVec2 (0.24635f, 0.82470f);
	ShopBuyDoubleSaveButton.s_UV[3] = CIwFVec2 (0.38754f, 0.82470f);


	ShopExitButton.id = 15;
	ShopExitButton.xpos = 327.0f;
	ShopExitButton.ypos = 279.0f;
	ShopExitButton.zpos = 1.0f;
	ShopExitButton.radius = 39.0f;
	ShopExitButton.state = MENU_BUTTON_NOTPRESSED;
	ShopExitButton.s_UV[0] = CIwFVec2 (0.42642f, 0.94066f);
	ShopExitButton.s_UV[1] = CIwFVec2 (0.51727f, 0.9406f);
	ShopExitButton.s_UV[2] = CIwFVec2 (0.42642f, 0.84981f);
	ShopExitButton.s_UV[3] = CIwFVec2 (0.51727f, 0.84981f);





	/////////// setup buttons for IAP select

	IAPSelectJackButton.id = 16;
	IAPSelectJackButton.xpos = -251.0f;
	IAPSelectJackButton.ypos = -46.0f;
	IAPSelectJackButton.zpos = 1.0f;
	IAPSelectJackButton.radius = 63.0f;
	IAPSelectJackButton.state = MENU_BUTTON_NOTPRESSED;
	IAPSelectJackButton.s_UV[0] = CIwFVec2 (0.55546f, 0.97478f);
	IAPSelectJackButton.s_UV[1] = CIwFVec2 (0.71414f, 0.97478f);
	IAPSelectJackButton.s_UV[2] = CIwFVec2 (0.55546f, 0.81610f);
	IAPSelectJackButton.s_UV[3] = CIwFVec2 (0.71414f, 0.81610f);


	IAPSelectRocketPowerButton.id = 17;
	IAPSelectRocketPowerButton.xpos = -84.0f;
	IAPSelectRocketPowerButton.ypos = -46.0f;
	IAPSelectRocketPowerButton.zpos = 1.0f;
	IAPSelectRocketPowerButton.radius = 63.0f;
	IAPSelectRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
	IAPSelectRocketPowerButton.s_UV[0] = CIwFVec2 (0.55546f, 0.97478f);
	IAPSelectRocketPowerButton.s_UV[1] = CIwFVec2 (0.71414f, 0.97478f);
	IAPSelectRocketPowerButton.s_UV[2] = CIwFVec2 (0.55546f, 0.81610f);
	IAPSelectRocketPowerButton.s_UV[3] = CIwFVec2 (0.71414f, 0.81610f);


	IAPSelectDoubleCoinsButton.id = 18;
	IAPSelectDoubleCoinsButton.xpos = 83.0f;
	IAPSelectDoubleCoinsButton.ypos = -46.0f;
	IAPSelectDoubleCoinsButton.zpos = 1.0f;
	IAPSelectDoubleCoinsButton.radius = 63.0f;
	IAPSelectDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
	IAPSelectDoubleCoinsButton.s_UV[0] = CIwFVec2 (0.55546f, 0.97478f);
	IAPSelectDoubleCoinsButton.s_UV[1] = CIwFVec2 (0.71414f, 0.97478f);
	IAPSelectDoubleCoinsButton.s_UV[2] = CIwFVec2 (0.55546f, 0.81610f);
	IAPSelectDoubleCoinsButton.s_UV[3] = CIwFVec2 (0.71414f, 0.81610f);


	
	IAPSelectDoubleSaveButton.id = 19;
	IAPSelectDoubleSaveButton.xpos = 250.0f;
	IAPSelectDoubleSaveButton.ypos = -46.0f;
	IAPSelectDoubleSaveButton.zpos = 1.0f;
	IAPSelectDoubleSaveButton.radius = 63.0f;
	IAPSelectDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
	IAPSelectDoubleSaveButton.s_UV[0] = CIwFVec2 (0.55546f, 0.97478f);
	IAPSelectDoubleSaveButton.s_UV[1] = CIwFVec2 (0.71414f, 0.97478f);
	IAPSelectDoubleSaveButton.s_UV[2] = CIwFVec2 (0.55546f, 0.81610f);
	IAPSelectDoubleSaveButton.s_UV[3] = CIwFVec2 (0.71414f, 0.81610f);


	/// reset game button
	MenuResetGameButton.id = 20;
	MenuResetGameButton.xpos = 380.0f;
	MenuResetGameButton.ypos = -20.0f;
	MenuResetGameButton.zpos = 1.0f;
	MenuResetGameButton.radius = 40.0f;
	MenuResetGameButton.state = MENU_BUTTON_NOTPRESSED;
	MenuResetGameButton.s_UV[0] = CIwFVec2 (0.55580f, 0.92789f);
	MenuResetGameButton.s_UV[1] = CIwFVec2 (0.68255f, 0.92789f);
	MenuResetGameButton.s_UV[2] = CIwFVec2 (0.55580f, 0.80114f);
	MenuResetGameButton.s_UV[3] = CIwFVec2 (0.68255f, 0.80114f);


	/// YES button
	MenuYesButton.id = 21;
	MenuYesButton.xpos = -360.0f;
	MenuYesButton.ypos = -180.0f;
	MenuYesButton.zpos = 1.0f;
	MenuYesButton.radius = 70.0f;
	MenuYesButton.state = MENU_BUTTON_NOTPRESSED;
	MenuYesButton.s_UV[0] = CIwFVec2 (0.02082f, 0.99175f);
	MenuYesButton.s_UV[1] = CIwFVec2 (0.27187f, 0.99175f);
	MenuYesButton.s_UV[2] = CIwFVec2 (0.02082f, 0.74070f);
	MenuYesButton.s_UV[3] = CIwFVec2 (0.27187f, 0.74070f);


	/// NO button
	MenuNoButton.id = 22;
	MenuNoButton.xpos = 360.0f;
	MenuNoButton.ypos = -180.0f;
	MenuNoButton.zpos = 1.0f;
	MenuNoButton.radius = 70.0f;
	MenuNoButton.state = MENU_BUTTON_NOTPRESSED;
	MenuNoButton.s_UV[0] = CIwFVec2 (0.27697f, 0.99175f);
	MenuNoButton.s_UV[1] = CIwFVec2 (0.52802f, 0.99175f);
	MenuNoButton.s_UV[2] = CIwFVec2 (0.27697f, 0.74070f);
	MenuNoButton.s_UV[3] = CIwFVec2 (0.52802f, 0.74070f);


}


void initIcons()
{

	iconJack.id = 0;
	iconJack.uvs[0] = CIwFVec2(0.92618f, 0.87292f);
	iconJack.uvs[1] = CIwFVec2(0.98214f, 0.87292f);
	iconJack.uvs[2] = CIwFVec2(0.92618f, 0.81696f);
	iconJack.uvs[3] = CIwFVec2(0.98214f, 0.81696f);

	iconRocket.id = 1;
	iconRocket.uvs[0] = CIwFVec2(0.86761f, 0.87292f);
	iconRocket.uvs[1] = CIwFVec2(0.92357f, 0.87292f);
	iconRocket.uvs[2] = CIwFVec2(0.86761f, 0.81696f);
	iconRocket.uvs[3] = CIwFVec2(0.92357f, 0.81696f);

	icon2xScore.id = 2;
	icon2xScore.uvs[0] = CIwFVec2(0.75049f, 0.87292f);
	icon2xScore.uvs[1] = CIwFVec2(0.80645f, 0.87292f);
	icon2xScore.uvs[2] = CIwFVec2(0.75049f, 0.81696f);
	icon2xScore.uvs[3] = CIwFVec2(0.80645f, 0.81696f);

	iconDoubleSave.id = 3;
	iconDoubleSave.uvs[0] = CIwFVec2(0.80910f, 0.87292f);
	iconDoubleSave.uvs[1] = CIwFVec2(0.86506f, 0.87292f);
	iconDoubleSave.uvs[2] = CIwFVec2(0.80910f, 0.81696f);
	iconDoubleSave.uvs[3] = CIwFVec2(0.86506f, 0.81696f);



}


void drawButton(myButton Button, float scale, CIwMaterial *p_material = NULL)
{

	CIwFMat model = CIwFMat::g_Identity;
	IwGxSetModelMatrix(&model);

	float xpos = Button.xpos * g_ScaleFactor;
	float ypos = Button.ypos * g_ScaleFactor;
	float radius = Button.radius * g_ScaleFactor * scale;

	s_button_Verts[4 * Button.id + 0] = CIwFVec3(xpos - radius, ypos - radius, Button.zpos);
	s_button_Verts[4 * Button.id + 1] = CIwFVec3(xpos + radius, ypos - radius, Button.zpos);
	s_button_Verts[4 * Button.id + 2] = CIwFVec3(xpos + radius, ypos + radius, Button.zpos);
	s_button_Verts[4 * Button.id + 3] = CIwFVec3(xpos - radius, ypos + radius, Button.zpos);

	s_button_UV[4 * Button.id + 0] = Button.s_UV[0];
	s_button_UV[4 * Button.id + 1] = Button.s_UV[1];
	s_button_UV[4 * Button.id + 2] = Button.s_UV[3];
	s_button_UV[4 * Button.id + 3] = Button.s_UV[2];

	if (!p_material)
	{
		IwGxSetMaterial(g_MaterialGameUI);
	}
	else
	{
		IwGxSetMaterial(p_material);
	}
	IwGxSetUVStream(&s_button_UV[4 * Button.id]);
	IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);
	IwGxSetVertStreamModelSpace(&s_button_Verts[4 * Button.id], 4);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

}

/*
bool TestButtonPressed(myButton Button, float x, float y)
{
	
	float x_pos_button = 0.0f;
	float y_pos_button = 0.0f;

	if (Button.ypos > 0) y_pos_button = float(g_ScreenHeight / 2 - (Button.ypos * g_ScaleFactor));
	if (Button.ypos < 0) y_pos_button = float(g_ScreenHeight / 2 + fabs(Button.ypos * g_ScaleFactor));
	if (Button.ypos == 0) y_pos_button = float (g_ScreenHeight / 2);

	if (Button.xpos < 0) x_pos_button = float(g_ScreenWidth / 2 - fabs(Button.xpos * g_ScaleFactor));
	if (Button.xpos > 0) x_pos_button = float(g_ScreenWidth / 2 + (Button.xpos * g_ScaleFactor));
	if (Button.xpos == 0) x_pos_button = float (g_ScreenWidth / 2);

	float x_size = float (fabs(x - x_pos_button));
	float y_size = float (fabs(y - y_pos_button));
	
	if (powf(x_size,2) + powf(y_size,2) <= powf(Button.radius * g_ScaleFactor,2))
		return true;
	
	return false;
}
*/

bool TestButtonPressed(myButton Button, float x, float y)
{
	
	float x_pos_button = 0.0f;
	float y_pos_button = 0.0f;

	if (Button.ypos > 0) y_pos_button = g_ScreenHeight / 2 - (Button.ypos * g_ScaleFactor);
	if (Button.ypos < 0) y_pos_button = g_ScreenHeight / 2 + fabs(Button.ypos * g_ScaleFactor);
	if (Button.ypos == 0) y_pos_button = g_ScreenHeight / 2;

	if (Button.xpos < 0) x_pos_button = g_ScreenWidth / 2 - fabs(Button.xpos * g_ScaleFactor);
	if (Button.xpos > 0) x_pos_button = g_ScreenWidth / 2 + (Button.xpos * g_ScaleFactor);
	if (Button.xpos == 0) x_pos_button = g_ScreenWidth / 2;

	float x_size = fabs(x - x_pos_button);
	float y_size = fabs(y - y_pos_button);
	
	if (powf(x_size,2) + powf(y_size,2) <= powf(Button.radius * g_ScaleFactor,2))
		return true;
	
	return false;
}



void initSystem()
{
//	s3eSurfaceSetup(S3E_SURFACE_PIXEL_TYPE_XRGB888, 0, NULL, S3E_SURFACE_BLIT_DIR_NORMAL);

//	surface = s3eSurfacePtr();

	IwGxInit();
    IwGraphicsInit();
//	IwGxFontInit();

	surface = (CIwGxSurface*)s3eSurfacePtr();

	//SetAssertOnUnrecognisedTokens(false);

	g_pAudio = new Audio();
	g_pSocial = new Social();
	g_pScore = new Score();
//	g_pFlurryAppSpot = new FlurryAppSpot();
	g_LevelManager = new CLevelManager();
	g_ParticleManager = new CParticleManager();

	g_Input.Init();


//	g_ScreenWidth = IwGxGetScreenWidth();
//	g_ScreenHeight = IwGxGetScreenHeight();

//	s3eSurfaceSetup(S3E_SURFACE_PIXEL_TYPE_XRGB888, 0, NULL, S3E_SURFACE_BLIT_DIR_NORMAL);

	g_ScreenWidth = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	g_ScreenHeight = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

	g_ScaleFactor = g_ScreenHeight / (float)VIRTUAL_SCREEN_HEIGTH;

	// fix for aspect ratio 1.333

	g_AspectRatio = (float)g_ScreenWidth / (float)g_ScreenHeight;
	
	if (g_AspectRatio < 1.5f) g_ScaleFactor =  g_ScaleFactor * float(ZOOM_ASPECT_FACTOR);

//	g_ScaleFactor = g_ScaleFactor * g_AspectRatio * 0.75;

//	g_MaxSpeed = g_ScaleFactor * float(MAX_SPEED);

	g_MaxSpeed = float(MAX_SPEED);


	g_TexturesSize = 1024;
}


void initRender()
{


    // Set field of view
//	IwGxSetPerspMul(658);

//	IwGxSetPerspMul(IwGxGetScreenWidth() / 2.0f);

/*	 float tanHalfHFOV = tanf ( 45.0 * 0.5f );
 
	 float aspect = (static_cast<float>(IwGxGetDisplayWidth()) / IwGxGetDisplayHeight());

	IwGxSetPerspMul ( static_cast<float> (IwGxGetDisplayWidth()) / ( tanHalfHFOV + tanHalfHFOV ) );

	IwGxSetPerspMul ( 2.0f * atanf ( tanHalfHFOV / aspect ));
*/

//	IwGxSetColClear(0x00, 0x00, 0x00, 0x00);
    IwGxSetFarZNearZ(10000, 30.0);

	float fov = g_ScreenHeight / (float)tan(0.5f * 45.0f);

	IwGxSetPerspMul(fov);



//  480: 320 = 1,5
//	960:640 = 1,5
//	1136:640 = 1,775
//  1024:768 = 1,3333


//	IwGxLightingOn();

//	IwGxSetLightType(0, IW_GX_LIGHT_AMBIENT);
//	IwGxSetLightCol(0, 0x60, 0x60, 0x60);



//	IwGxSetLightType(1, IW_GX_LIGHT_DIFFUSE);
//	IwGxSetLightCol(1, 0xff, 0xff, 0xff);
 
//	CIwFVec3 dd(0.5f, -1.0f, 0.0f);

//	IwGxSetLightDirn(1, &dd);


}



void SetCameraOrtho()
{
	CIwFMat view = CIwFMat::g_Identity;
	view.t.z = VIRTUAL_SCREEN_HEIGTH / 2;
	view.LookAt(view.GetTrans(), CIwFVec3(0, 0, 0), -CIwFVec3::g_AxisY);

	IwGxSetViewMatrix(&view);

	IwGxSetPerspMul(float(g_ScreenWidth / 2));
	IwGxSetOrtho(-(g_ScreenWidth / 2), g_ScreenWidth / 2 , -(g_ScreenHeight / 2), (g_ScreenHeight /2));

	CIwFMat model = CIwFMat::g_Identity;
	IwGxSetModelMatrix(&model);
}


void drawWall(float ypos)
{

	drawModel(objMainPlate, Vector3(0 , ypos , 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f));
	g_LevelManager->RenderLevel(g_CurrentLevel, 0, ypos,0);

}

float tempee = 0;


void drawIAP_RocketEngine(float ypos)
{

	tempee+=float(M_PI) / 128.0f;

	drawModel(objIAP_RocketEngine, Vector3(-300.0f, ypos - 400.0f, 0.0f), Vector3(0.0f, tempee, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	drawModel(objIAP_RocketEngine, Vector3(300.0f, ypos - 400.0f, 0.0f), Vector3(0.0f, tempee, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	
	g_ParticleManager->Update(0);
	g_ParticleManager->Render(-300.0f,ypos - 470.0f,0.0f,0);

	g_ParticleManager->Update(1);
	g_ParticleManager->Render(300.0f,ypos - 470.0f,0.0f,0);

}


void drawIAP_LiftJack(float ypos)
{
	drawModel(objIAP_LiftJack, Vector3(0.0f, ypos -661.537f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
}


float MaxDiff()
{
	float expon2 = expf(powf(2.3f, y_wallpos / 2400));

	float result = (expf((powf (2.3f, (600 + 3600) / ((3600+3600) / expon2)))));

	return result;
}

float GetExponDiff(float y_wallpos)
{

	float expon2 = expf(powf(2.3f, y_wallpos / g_LevelDifficulty));

	float result = (expf((powf (2.3f, (y_wallpos + g_LevelDifficulty2) / ((3600+g_LevelDifficulty2) / expon2)))));

	return result;
}



float s_AlphaBlendDelay = 0;



bool FadeIn(float fDelta, int col_r, int col_g, int col_b)
{

			
			if (s_AlphaBlend < FADEIN_LENGHT)
			{
				uint8 *texels = ((uint8*)g_TextureFadeIn->GetTexels());

				for (int i=0; i<16; i++)
				{
					int alpha_step = 255 / FADEIN_LENGHT;
					texels[i*4 + 0] = col_r;
					texels[i*4 + 1] = col_g;
					texels[i*4 + 2] = col_b;

					texels[i*4 + 3] = 255 - s_AlphaBlend * alpha_step;
		
				}

				g_TextureFadeIn->ChangeTexels(g_TextureFadeIn->GetTexels(), CIwImage::ABGR_8888);


				s_AlphaBlendDelay = s_AlphaBlendDelay + 1; // * fDelta

	//			if (s_AlphaBlendDelay > 2)
	//			{
					s_AlphaBlend = s_AlphaBlend + 1;
	//				s_AlphaBlendDelay = 0.0;
	//			}

				SetCameraOrtho();
				IwGxLightingOff();

				IwGxSetVertStreamModelSpace(s_VertsFadeIn, 4);
				IwGxSetMaterial(g_MaterialFadeIn);
				IwGxSetUVStream(s_UVFadeIn);
				IwGxSetColStream(NULL);
				IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

				return false;
			}

			else return true;
}

#define NUM_COINS 16

struct myCoin
{
	CIwFVec3 CoinPos;
	CIwFVec3 CoinInitPos;
	CIwFVec3 CoinSpeed;
	CIwFVec3 CoinRot;
	CIwFVec3 CoinRotSpeed;

};


myCoin s_CoinsArray[NUM_COINS];

void initCoins()
{
	for (int i=0; i<NUM_COINS; i++)
	{
		float xpos = float(rand() % 64);
		float ypos = -float(rand() % g_ScreenHeight);
		float yposinit = -float(g_ScreenHeight);
		float zpos = 0.0f;//-float(300 + rand() % 164);
		float xspeed = float(12 - rand() % 25);
		float yspeed = float(5 + rand() % 5);
		float zspeed = float(5 + rand() % 5);
		float xrot = float(rand() % 360);
		float yrot = float(rand() % 360);


		float xrotspeed = float(3 + rand() % 3);
		float yrotspeed = float(3 + rand() % 3);

		xrot =  xrot * float(M_PI) / 180.0f;
		yrot =  yrot * float(M_PI) / 180.0f;
				
		xrotspeed = xrotspeed * float(M_PI) / 180.0f;
		yrotspeed = yrotspeed * float(M_PI) / 180.0f;
		

		s_CoinsArray[i].CoinInitPos.x = xpos;
		s_CoinsArray[i].CoinInitPos.y = yposinit;
		s_CoinsArray[i].CoinInitPos.z = zpos;
		s_CoinsArray[i].CoinPos.x = xpos;
		s_CoinsArray[i].CoinPos.y = ypos;
		s_CoinsArray[i].CoinPos.z = zpos;
		s_CoinsArray[i].CoinSpeed.x = xspeed;
		s_CoinsArray[i].CoinSpeed.y = yspeed;
		s_CoinsArray[i].CoinSpeed.z = zspeed;
		s_CoinsArray[i].CoinRot.x = xrot;
		s_CoinsArray[i].CoinRot.y = yrot;
		s_CoinsArray[i].CoinRot.z = 0.0f;
		s_CoinsArray[i].CoinRotSpeed.x = xrotspeed;
		s_CoinsArray[i].CoinRotSpeed.y = yrotspeed;
		s_CoinsArray[i].CoinRotSpeed.z = 0.0f;
	}
}

int g_iShowAdCounter=0;

void showAdBanner()
{

	// if user have not complete 3rd level in the game, dont fuck your mind with ad =)

	if (g_CurrentLevel < 4) return;

	if (!g_bshowAd)
	{	
		g_bshowAd = true;
		if (g_iShowAdCounter==0)
		{
			ShowInterstitialAd();

			g_iShowAdCounter = rand()%3;
		}
		else
		{
			g_iShowAdCounter--;
		}
	}
}




void removeAdBanner()
{

//	admob_hide();

	g_bshowAd = false;

//	BannerAdHide();
//	 BannerAdHide();
}


void drawCoins(float fDelta)
{
	for (int i=0; i<NUM_COINS; i++)
	{
	//	if (s_CoinPosArray[i].

		drawModel(objGoldCoin, Vector3(s_CoinsArray[i].CoinPos.x, s_CoinsArray[i].CoinPos.y, s_CoinsArray[i].CoinPos.z), 
			Vector3(s_CoinsArray[i].CoinRot.x, s_CoinsArray[i].CoinRot.y, 0.0), 
//			Vector3(1.0 * g_ScaleFactor, 1.0 * g_ScaleFactor, 1.0 * g_ScaleFactor));
			Vector3(1.0 , 1.0 , 1.0 ));



		s_CoinsArray[i].CoinPos = s_CoinsArray[i].CoinPos + s_CoinsArray[i].CoinSpeed;

		if (s_CoinsArray[i].CoinPos.y > g_ScreenHeight / 2 + (164 * 1.0))
		{
			s_CoinsArray[i].CoinPos = s_CoinsArray[i].CoinInitPos;
		}


		s_CoinsArray[i].CoinRot = s_CoinsArray[i].CoinRot + s_CoinsArray[i].CoinRotSpeed;


	}
}


void initGameLevel()
{

	g_pAudio->StopSound("/sounds/win1.wav");
	g_pAudio->StopSound("/sounds/game_over.wav");

	game_state = STATE_NORMAL_ACTION; 

	g_DropCounter = 0;

	y_wallpos = 0;

	t_dist_time = 0;

	t_zoom_time = 0;
	t_zoom_time2 = 0;
	t_zoom_time3 = 0;

	cam_offset_x  = 0;
	
	g_SendFlurry = false;

	g_LevelDifficulty = 3600 - g_CurrentLevel * 48;

//	g_LevelPlayTime = 500;

	g_LevelElapsedTime = 0;

	g_bLevelFirstTouch = false;

	g_TimeBarColorOffset = float(M_PI) / 2.0f;

	/// max diffucluty = 6000

	if (g_CurrentLevel < 5)
	{
		g_LevelDifficulty2 = g_CurrentLevel * 150;
		g_DropDelay = 40;
		g_LevelPlayTime = 850;
	}
	

	/// 4 to 10
	if (g_CurrentLevel >= 5 && g_CurrentLevel < 10)
	{
		g_LevelDifficulty2 = 500 + g_CurrentLevel * 100;
		g_DropDelay = 40;
		g_LevelPlayTime = 1100;

		

	}


	/// 10 to 20
	if (g_CurrentLevel >= 10 && g_CurrentLevel < 20)
	{
		g_LevelDifficulty2 = 700 + g_CurrentLevel * 80;
		g_DropDelay = 40;
		g_LevelPlayTime = 1350;
	}


	/// 20 to 30
	if (g_CurrentLevel >= 20 && g_CurrentLevel < 30)
	{
		g_LevelDifficulty2 = 1000 + g_CurrentLevel * 100;
		g_DropDelay = 40;
		g_LevelPlayTime = 3200 + g_CurrentLevel * 50;
	}

	/// 30 to 40
	if (g_CurrentLevel >= 30 && g_CurrentLevel < 40)
	{
		g_LevelDifficulty2 = 1200 + g_CurrentLevel * 80;
		g_DropDelay = 30;
		g_LevelPlayTime = 5600 + g_CurrentLevel * 150;
	}


	/// 40 to 51
	if (g_CurrentLevel >= 40 && g_CurrentLevel < 52)
	{
		g_LevelDifficulty2 = 1500 + g_CurrentLevel * 40;
		g_DropDelay = 30;
		g_LevelPlayTime = 8000 + g_CurrentLevel * 100;
	}


	s_AlphaBlendDelay = 0;
	s_AlphaBlend = 0;
	s_CoinsTime = 0;

	initCoins();
	removeAdBanner();


}



void drawTimeBar(int elapsed_time)
{


	float _xpos = float(g_ScreenWidth) * 0.01f; // get 1% free space form the left side of screen
	float _ypos = float(g_ScreenHeight) * 0.01f; // get 1% free space form the top side of screen

	float x_bar_size = float(g_ScreenWidth) - (_xpos * 2.0f);
	float y_size = 12.0f * g_ScaleFactor;

	/// calculate decrement delta
	float delta = x_bar_size / g_LevelPlayTime;
	delta = delta * elapsed_time;
	float x_size = x_bar_size - delta;
	if (x_size < 1.0) x_size = 1.0;

	float xpos = float(-g_ScreenWidth / 2) + _xpos;// - x_offset;
	float ypos = float(g_ScreenHeight / 2) - (_ypos + y_size);


	s_VertsTimeBar[0] =  CIwFVec3(xpos, ypos - y_size, 1);
	s_VertsTimeBar[1] =  CIwFVec3(xpos + x_size, ypos - y_size, 1);
	s_VertsTimeBar[2] =  CIwFVec3(xpos + x_size, ypos, 1);
	s_VertsTimeBar[3] =  CIwFVec3(xpos, ypos, 1);

	s_UVTimeBar[0] = CIwFVec2(0.0f, 0.5f);
	s_UVTimeBar[1] = CIwFVec2(1.0f, 0.5f);
	s_UVTimeBar[2] = CIwFVec2(1.0f, 0.0f);
	s_UVTimeBar[3] = CIwFVec2(0.0f, 0.0f);



	/// make flashing time bar
	uint8 *texels = ((uint8*)g_TextureTimeBar->GetTexels());
			
	int color = 255;

	/// 70% show just with decrement
	if (x_size < x_bar_size * 0.4f)
	{
			color = (int)(255.0 * sinf (g_TimeBarColorOffset));
			g_TimeBarColorOffset = g_TimeBarColorOffset + float(M_PI) / 32.0f;

			if (g_TimeBarColorOffset > float(M_PI)) g_TimeBarColorOffset = 0;
	
	}


	for (int i=0; i<16; i++)
	{
			texels[i*4 + 0x01] = color;
			texels[i*4 + 0x02] = color;
	}


	g_TextureTimeBar->ChangeTexels(g_TextureTimeBar->GetTexels(), CIwImage::ABGR_8888);
	/// flash end



	IwGxSetVertStreamModelSpace(s_VertsTimeBar, 4);
	IwGxSetMaterial(g_MaterialTimeBar);
	IwGxSetUVStream(s_UVTimeBar);
	IwGxSetColStream(NULL);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

}

#define MAX_ICONS 12

static CIwFVec3 s_IconVerts[4*MAX_ICONS];
static CIwFVec2 s_IconUV[4*MAX_ICONS];


void drawIcon(myIcon icon, float _xpos, float _ypos, float size)
{

	float icon_size = size;

	float x_offset = (icon_size * g_ScaleFactor) / 2;
	float y_offset = (icon_size * g_ScaleFactor) / 2;

	icon_size = icon_size * g_ScaleFactor;

	float xpos = _xpos - x_offset;
	float ypos = _ypos - y_offset;

	
	s_IconVerts[icon.id * 4 + 0] = CIwFVec3(xpos, ypos, 1);
	s_IconVerts[icon.id * 4 + 1] = CIwFVec3(xpos + icon_size, ypos, 1);
	s_IconVerts[icon.id * 4 + 2] = CIwFVec3(xpos + icon_size, ypos + icon_size, 1);
	s_IconVerts[icon.id * 4 + 3] = CIwFVec3(xpos, ypos + icon_size, 1);

	s_IconUV[4 * icon.id + 0] = icon.uvs[0];
	s_IconUV[4 * icon.id + 1] = icon.uvs[1];
	s_IconUV[4 * icon.id + 2] = icon.uvs[3];
	s_IconUV[4 * icon.id + 3] = icon.uvs[2];

	IwGxSetVertStreamModelSpace(&s_IconVerts[icon.id * 4], 4);
	IwGxSetUVStream(&s_IconUV[icon.id * 4]);
	IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

}


void drawIAPIcons()
{
	IwGxSetMaterial(g_MaterialShop);

	float icon_size = 48.0f;

	float icon_xpos = float(-g_ScreenWidth / 2) + (float(g_ScreenWidth) * 0.01f + icon_size * float(g_ScaleFactor / 2)); // get 1% free space form the left side of screen
	float icon_ypos = float(g_ScreenHeight / 2) - (float(g_ScreenHeight) * 0.02f + icon_size * float(g_ScaleFactor)); // get 1,5% free space form the top side of screen


	// draw Jack icon
	if (IAPSelectJackButton.do_action)
	{
		drawIcon(iconJack, icon_xpos,icon_ypos,icon_size);
		icon_xpos = icon_xpos + icon_size * g_ScaleFactor;
	}


	// draw Rocket icon
	if (IAPSelectRocketPowerButton.do_action)
	{
		drawIcon(iconRocket, icon_xpos,icon_ypos,icon_size);
		icon_xpos = icon_xpos + icon_size * g_ScaleFactor;
	}

	/// draw X2 (double score) icon
	if (IAPSelectDoubleCoinsButton.do_action)
	{
		drawIcon(icon2xScore, icon_xpos,icon_ypos,icon_size);
		icon_xpos = icon_xpos + icon_size * g_ScaleFactor;
	}

	/// draw double SAVE icon
	if (IAPSelectDoubleSaveButton.do_action)
	{
		drawIcon(iconDoubleSave, icon_xpos,icon_ypos, icon_size);
		icon_xpos = icon_xpos + icon_size * g_ScaleFactor;
	}

}

void globalGamePlay(float fDelta)
{

		g_bshowAd = false;
		g_bselect_motivator = false;
//		s3eSurfaceSetDisplayID(num_surf);
	
		IwGxLightingOn();
       	IwGxLightingAmbient(true);
	    IwGxLightingEmissive(false);
	    IwGxLightingDiffuse(true);
	    IwGxLightingSpecular(true);

	    IwGxSetLightType(1, IW_GX_LIGHT_AMBIENT);
	    CIwColour c;
	    c.Set(0x81,0x81,0x81,0xff);
	    IwGxSetLightCol(1, &c);

	    IwGxSetLightType(0, IW_GX_LIGHT_DIFFUSE);
	    CIwFVec3 dir = CIwFVec3(0.1f, -0.5f, -1.0f);
	    dir.Normalise();
		c.Set(0xff,0xff,0xff,0xff);
		IwGxSetLightCol(0, &c);
	    IwGxSetLightDirn(0, &dir);


	 s_Surface[0]->MakeCurrent();
	 IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

	IwGxLightingOff();
	SetCameraOrtho();

	float YWheelPos = 0.0;
	float dist_offset = 0.0;

	if (g_AspectRatio >= 1.5f)
	{
		YWheelPos = -float(g_ScreenHeight / 2)  - 96.0f * g_ScaleFactor;
		dist_offset = cam_offset_x *g_ScaleFactor;
	}
	else 
	if (g_AspectRatio < 1.5f)
	{
		YWheelPos = -float(g_ScreenHeight / 2)  -96.0f * (1.0f / float(ZOOM_ASPECT_FACTOR));
		dist_offset = cam_offset_x * g_ScaleFactor * (1.0f / float(ZOOM_ASPECT_FACTOR));
	}



	drawModel(objMenuWheel2, Vector3(0 , YWheelPos - cam_offset_x  , 0.0), Vector3(0, 0, wheel_rot), Vector3(1.0 * g_ScaleFactor, 1.0 * g_ScaleFactor, 1.0 * g_ScaleFactor));


//	IwGxFlush();


	initRender();

		CIwFMat view = CIwFMat::g_Identity;
        view.t.y = 0 + cam_offset_x;
        view.t.x = 0;
		view.t.z =  1152+120;


		view.LookAt(view.GetTrans(), CIwFVec3(0, cam_offset_x, -100.0), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);


	//	y_wallpos++;


	//	SetCameraOrtho();

//	pp.Normalise();

		char vecstring[128];

//		IwGxFontSetCol(0xffffffff);
//		IwGxFontSetFlags(IW_GX_FONT_DEFAULT_F);


		float cur_delta = 0;
		float y_speed = 0;

		if (global_state == GLOBAL_STATE_GAME_PLAY)
		{

		g_Input.Update();

		int delta1 = 0;
		int delta2 = 0;


		bool b_touch1 = false;
		bool b_touch2 = false;

		CTouch* touch;

		if (g_Input.getTouchCount() != 0)
		{

			g_bLevelFirstTouch = true;

		  touch = g_Input.getTouch(0);
		  if (touch != NULL && touch->active)
			{
						if (touch->prevy == -1) touch->prevy = touch->y; // first touch
						delta1 = touch->prevy - touch->y;
						touch->prevy = touch->y;
						b_touch1 = true;
			}
		  	else
			{
				touch->prevy = -1;
				b_touch1 = false;
			}

		  touch = g_Input.getTouch(1);
		  if (touch != NULL && touch->active)
			{
				if (touch->prevy == -1) touch->prevy = touch->y; // first touch
					delta2 = touch->prevy - touch->y;
					touch->prevy = touch->y;
					b_touch2 = true;	
				  
			 }
		  	else 
			{
				touch->prevy = -1;
				b_touch2 = false;
			}

		}
		else {
			delta1 = 0;
			delta2 = 0;

			CTouch *touch = g_Input.getTouch(0);
			touch->prevy = -1;

			touch = g_Input.getTouch(1);
			touch->prevy = -1;
		}
/*
		/// check pause button
		if (g_Input.getTouchCount() != 0)
		{
			
			touch = g_Input.getTouch(0);
			if (touch != NULL && touch->active)
			{
				if (TestButtonPressed(PauseButton, touch->x, touch->y))
				{
					global_state = GLOBAL_STATE_GAME_PAUSE; 
					prev_touch = false;
				}
			}

			touch = g_Input.getTouch(1);
			if (touch != NULL && touch->active)
			{
				if (TestButtonPressed(PauseButton, touch->x, touch->y))
				{
					global_state = GLOBAL_STATE_GAME_PAUSE;
					prev_touch = false;
				}
			}
		}

*/

		////////// debug draw interface ///////////////////////////////////

#ifdef _DEBUG
/*
		IwGxFontSetRect(CIwRect(50, 50, 900, 600));
		sprintf(vecstring, "Delta: %i", delta1);
		IwGxFontDrawText(vecstring);

		IwGxFontSetRect(CIwRect(50, 110, 900, 600));
		if (b_touch1) IwGxFontDrawText("Touch1: ACTIVE");
		else IwGxFontDrawText("Touch1: NONE");

		touch = g_Input.getTouch(0);
		IwGxFontSetRect(CIwRect(50, 120, 900, 600));
		if (b_touch1)
		{
			sprintf(vecstring, "Id1: %i", touch->id);
			IwGxFontDrawText(vecstring);
		} else IwGxFontDrawText("Id1: NONE");

			
		IwGxFontSetRect(CIwRect(50, 130, 900, 600));
		if (b_touch2) IwGxFontDrawText("Touch2: ACTIVE");
		else IwGxFontDrawText("Touch2: NONE");


		touch = g_Input.getTouch(1);
		IwGxFontSetRect(CIwRect(50, 140, 900, 600));
		if (b_touch2)
		{
			sprintf(vecstring, "Id2: %i", touch->id);
			IwGxFontDrawText(vecstring);
		} else IwGxFontDrawText("Id2: NONE");


		if (g_Input.getTouchCount() != 0) 
		{
			IwGxFontSetRect(CIwRect(50, 150, 900, 600));

			CTouch* touch = g_Input.getTouch(0);
			sprintf(vecstring, "YPos(0): %i", touch->y);
			IwGxFontDrawText(vecstring);


			IwGxFontSetRect(CIwRect(50, 170, 900, 600));
			touch = g_Input.getTouch(1);
			sprintf(vecstring, "YPos(1): %i", touch->y);
			IwGxFontDrawText(vecstring);

		}

	*/
#endif
		// debug end ///////////////////////////////////////////////////////////////



		if (game_state != STATE_GAME_OVER && game_state != STATE_LEVEL_COMPLETE)
		{
			if (g_Input.getTouchCount() == 0)
			{
				if (y_wallpos > 0 && game_state != STATE_DROP_DOWN)	
				{
					if (!IAPSelectJackButton.do_action)
					{
						game_state = STATE_DROP_DOWN;
						g_DropCounter++;
					}
				}	
			} 
			else if (g_Input.getTouchCount() > 0)
			{
				if (delta1 != 0 || delta2 != 0)	
				{
					float drop_speed = sqrtf (float(VIRTUAL_SCREEN_HEIGTH) - y_wallpos) / 1.5f;
				
					if (delta1 > drop_speed || delta2 > drop_speed)
					{
						game_state = STATE_NORMAL_ACTION;
					}
				}
			}
		

		if (delta1 == 0 && delta2 == 0 && y_wallpos > 0)
		{
			if (drop_counter > g_DropDelay) // check time for wait drop
			{

//				game_state = STATE_LOSE_CONTROL;
				game_state = STATE_DROP_DOWN;
				drop_counter = 0;
				g_DropCounter++;
			//	exit (0);
		//		y_wallpos = 0 ;
			}

			if (!IAPSelectJackButton.do_action) drop_counter++;
		}
		}

		if (y_wallpos  <= 0 && game_state == STATE_DROP_DOWN)
		{
				y_wallpos = 0;
				game_state = STATE_GAME_OVER;
		}
			
		if (y_wallpos >= 640)
		{
				y_wallpos = 640;  // level is comlete
				game_state = STATE_LEVEL_COMPLETE;
		}


		float param1,param2;
	
///////////////////////////////////////////////////// test for Mike
		if (g_LevelElapsedTime > g_LevelPlayTime)
		{
			if (game_state != STATE_LEVEL_COMPLETE && game_state != STATE_GAME_OVER)
				game_state = STATE_DROP_DOWN;
		}

////////////////////////////////////////////////////////////////
		switch (game_state)
		{
		case STATE_NORMAL_ACTION:

			if (abs(delta1) > abs(delta2))
				cur_delta = (float)delta1;
			else cur_delta = (float)delta2;

			y_speed =float(fabs(cur_delta));

			if (y_speed > g_MaxSpeed) y_speed = g_MaxSpeed;

//			float expon2 = expf(powf(2.3, y_wallpos / 1240));

//			y_speed = (y_speed / (expf((powf (2.3, y_wallpos / (1640 / expon2))))));

			if (IAPSelectRocketPowerButton.do_action)
			{
				y_speed = (float(g_CurrentLevel) * 0.7f * y_speed) / GetExponDiff(320);
			}
			else
			{
				float dificulty_factor = GetExponDiff(y_wallpos);

				if (dificulty_factor > MaxDiff()) dificulty_factor = MaxDiff(); 

				y_speed = y_speed / dificulty_factor;
			}

			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////

//			if (IAPSelectRocketPowerButton.do_action) y_speed = y_speed + g_MaxSpeed * 1.5;

			
#ifdef _DEBUG
			y_speed+=10;
#endif

			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////


			if (cur_delta > 0) y_wallpos = y_wallpos + y_speed;
			else if (cur_delta < 0) y_wallpos = y_wallpos - y_speed;

#ifdef _DEBUG
/*
			char vecstring[128];
			IwGxFontSetRect(CIwRect(50, 290, 900, 600));
			sprintf(vecstring, "Speed: %f", y_speed);
			IwGxFontDrawText(vecstring);
		*/
#endif // DEBUG	
			
			break;

		case STATE_DROP_DOWN:
			y_speed = DROP_SPEED;
	//		y_speed = y_speed * GetExponDiff(sqrt(y_wallpos));
			
			y_speed = sqrtf(float(VIRTUAL_SCREEN_HEIGTH) - y_wallpos) * 2;
			
			y_wallpos = y_wallpos - y_speed;
			break;

		case STATE_GAME_OVER:
			if (t_dist_time >= CAMERA_DISTORT_COUNT)
			{
		//		distort_counter = 0;
				
				game_state = STATE_NORMAL_ACTION;
				global_state = GLOBAL_STATE_GAME_OVER;

				UpdateConfig();

				IAPSelectJackButton.do_action = false;
				IAPSelectRocketPowerButton.do_action = false;
				IAPSelectDoubleCoinsButton.do_action = false;
				IAPSelectDoubleSaveButton.do_action = false;


//				ReportFlurryGameOver();
			}

			g_pAudio->PlaySound("/sounds/game_over.wav");

			// f(t)=exp(-t/tau) * sin (t)

			param1 = cosf (t_dist_time);
			param2 = expf(-(CAMERA_DISTORT_COUNT) * (t_dist_time / 20));
		//	float param2 = -sinf(t_dist_time) / .1+ t_dist_time;

		//	x=x0+A*Math.exp(-g*time)*Math.cos(w*time);



		//	if (param1 == 0) param1 = 1;
		//	if (param2 == 0) param2 = 1;

			cam_offset_x = 120 * param1 * param2;

			t_dist_time+= 0.5; // fDelta

	//		distort_counter++;

	//		DistortCamera();
	//		GameOverBox();

#ifdef _DEBUG
//			IwGxFontSetRect(CIwRect(350, 250, 900, 600));
//			sprintf(vecstring, "Xoffset: %f", cam_offset_x);
//			IwGxFontDrawText(vecstring);
#endif

			break;


		case STATE_LEVEL_COMPLETE:

			g_pAudio->PlaySound("/sounds/win1.wav");


			////////////// END of GAME
			if (g_CurrentLevel == 50)
			{
				/// calculate coins
				int iCoins = g_LevelManager->GetLevelMemoryUsed(g_CurrentLevel) / 10;
				if (g_DropCounter > 7) g_DropCounter = 7; // max minus 70%
				int minus_score = iCoins * g_DropCounter / 10;

				g_CurrentScore = g_CurrentScore + (iCoins - minus_score);
				if (g_CurrentScore > g_BestScore) g_BestScore = g_CurrentScore;

				global_state = GLOBAL_STATE_FINALCUT;

				UpdateConfig();

				IAPSelectJackButton.do_action = false;
				IAPSelectRocketPowerButton.do_action = false;
				IAPSelectDoubleCoinsButton.do_action = false;
				IAPSelectDoubleSaveButton.do_action = false;

				g_CurrentLevel = 1;
				g_CurrentScore = 0;
				g_finalcut_counter = 0;
		
			}
			else
			{

				drawCoins(fDelta);

				s_CoinsTime = s_CoinsTime + 1.0f; //* fDelta;

				if (s_CoinsTime > COINS_FLY_LENGHT)
				{

				/// calculate coins
				int iCoins = g_LevelManager->GetLevelMemoryUsed(g_CurrentLevel) / 10;

				if (g_DropCounter > 7) g_DropCounter = 7; // max minus 70%

				int minus_score = iCoins * g_DropCounter / 10;

				g_CurrentScore = g_CurrentScore + (iCoins - minus_score);

				g_CurrentLevel++;

				UpdateConfig();

				IAPSelectJackButton.do_action = false;
				IAPSelectRocketPowerButton.do_action = false;
				IAPSelectDoubleCoinsButton.do_action = false;
				IAPSelectDoubleSaveButton.do_action = false;


				init_timer_flag = false;
				global_state = GLOBAL_STATE_NEXT_LEVEL;

//				ReportFlurryNextLevel();
				}
			}

			break;

		default:
			break;
		}

		}


		if (y_wallpos < 0) 
			y_wallpos = 0;

		if (y_wallpos > 640.0) 
			y_wallpos = 640.0;

		if (game_state != STATE_LEVEL_COMPLETE)
		{
			IwGxLightingOn();
			drawWall(y_wallpos);
	
			if (IAPSelectRocketPowerButton.do_action)
				drawIAP_RocketEngine(y_wallpos);

			if (IAPSelectJackButton.do_action)
				drawIAP_LiftJack(y_wallpos);
		

		}

/*		int screenWidth = IwGxGetScreenWidth();
		int screenHeight = IwGxGetScreenHeight();

		view = CIwFMat::g_Identity;
		view.t.z = 320;
		view.LookAt(view.GetTrans(), CIwFVec3(0, 0, 0), -CIwFVec3::g_AxisY);
		IwGxSetViewMatrix(&view);
		IwGxSetOrtho(-(screenWidth / 2), screenWidth / 2 , -(screenHeight / 2), (screenHeight /2));

		CIwFMat model = CIwFMat::g_Identity;
		IwGxSetModelMatrix(&model);
		float scale_button = (float)screenHeight / 640;
//		drawModel(objButtonPause, Vector3(PauseButton.xpos , PauseButton.ypos , 1), Vector3(0, 0, 0), Vector3(scale_button, scale_button, scale_button));
*/


//		Iw2DSetColour(0xff0000ff); // Set red

//		for (int i=0; i<640; i++)
//			Iw2DDrawRect(CIwFVec2(i, sin_buffer[i]), CIwFVec2(1,1)); // Draw red outline




//		Iw2DSetColour(0xffffffff); // Set red
//		Iw2DDrawRect(CIwFVec2(0, 124), CIwFVec2(640,1)); // Draw red outline

#ifdef _DEBUG
/*
		IwGxFontSetRect(CIwRect(50, 190, 900, 600));
		sprintf(vecstring, "Diff: %f", cur_delta);
		IwGxFontDrawText(vecstring);


		IwGxLightingOn();
		IwGxFontSetRect(CIwRect(310, 190, 900, 600));
		switch (game_state)
		{
		case STATE_NORMAL_ACTION:
		IwGxFontDrawText("State: NORMAL");
		break;

		case STATE_DROP_DOWN:
		IwGxFontDrawText("State: DROP_DOWN");
		break;

		case STATE_GAME_OVER:
		IwGxFontDrawText("State: GAME OVER");
		break;

		case STATE_LEVEL_COMPLETE:
		IwGxFontDrawText("State: LEVEL COMPLETE");
		break;

		}

		IwGxFontSetRect(CIwRect(310, 290, 900, 600));
		sprintf(vecstring, "Drop counter: %d", g_DropCounter);
		IwGxFontDrawText(vecstring);
*/
#endif




		IwGxFogOff();

		 IwGxFlush();

        //Render a smaller version of the frame buffer,
        //transforming into a fake HDR version using the bloombuffer shader
        RenderPass(s_Surface[OFFSCREEN_SCENE], s_Surface[BLOOM_BUFFER], s_BloomBufferShader);

        //Blur the bloom buffer to give spread
        const float BLUR_SPREAD_X = 0.007f;
        const float BLUR_SPREAD_Y = BLUR_SPREAD_X * s_Surface[0]->GetClientWidth() / s_Surface[0]->GetClientHeight();

        CIwGxShaderUniform* blurDir = s_BlurBufferShader->GetParam("inBlurStep");
        float dirs[3] = { BLUR_SPREAD_X, 0.0f, BLUR_SPREAD_Y };
        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs);

        RenderPass(s_Surface[BLOOM_BUFFER], s_Surface[BLUR_BUFFER_1], s_BlurBufferShader);
        IwGxFlush(); //need to submit before changing the shader parameter (it is not an instanceParam)

        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs+1);
        RenderPass(s_Surface[BLUR_BUFFER_1], s_Surface[BLUR_BUFFER_2], s_BlurBufferShader);

        //Now composite the frame buffer and the bloom buffer using the postprocess shader

        //Select the back buffer as the current surface
       CIwGxSurface::MakeDisplayCurrent();

        //Render the buffers through the shader
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

        CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();


            //Select the original surface and the bloom surface into the material
            pMat->SetTexture(s_Surface[OFFSCREEN_SCENE]->GetTexture());
            pMat->SetTexture(s_Surface[BLUR_BUFFER_2]->GetTexture(), 1);

            pMat->SetShaderTechnique(s_PostProcessShader);

        IwGxSetMaterial(pMat);
        CIwSVec2 xy(0,0);
        CIwSVec2 wh((int16)IwGxGetScreenWidth(), (int16)IwGxGetScreenHeight());
        CIwFVec2 uvxy(0, 0);


        IwGxDrawRectScreenSpace(&xy, &wh, &uvxy, &s_Surface[OFFSCREEN_SCENE]->GetClientUVExtentFloat());


		IwGxLightingOff();
		SetCameraOrtho();

		if (game_state != STATE_LEVEL_COMPLETE)
		{
			drawTimeBar(g_LevelElapsedTime);
			drawIAPIcons();
	//		drawIAP_RocketEngine(y_wallpos);
			if (g_bLevelFirstTouch)
				g_LevelElapsedTime++; // increase level play timer	
		}


		if (game_state == STATE_LEVEL_COMPLETE) FadeIn(fDelta, 0xff, 0xff, 0xff);
	//	if (game_state == STATE_GAME_OVER) FadeIn(fDelta, 0x1f, 0x00, 0x00);



	wheel_rot+=float(MENU_WHEEL_SPEED) * float(M_PI) / (4 * 720);// * ((int)scale_wheel));

	if (wheel_rot > 2 * M_PI)
		wheel_rot = wheel_rot - 2*float(M_PI);

	if (wheel_rot < 0) 
		wheel_rot = 2*float(M_PI) - wheel_rot;

}


#define NUM_DOLLARS 16

struct myDollar
{
	CIwFVec3 DollarPos;
	CIwFVec3 DollarSpeed;
	float DollarYlimit;
	float DollarOffset;
	CIwFVec3 DollarRot;
	CIwFVec3 DollarRotSpeed;


};


myDollar s_DollarsArray[NUM_DOLLARS];

void initDollars()
{
	for (int i=0; i<NUM_DOLLARS; i++)
	{
		float xpos = float(rand() % 64);
		float ypos = float(10 + rand() % g_ScreenHeight);
		float zpos = -float(300 + rand() % 164);
		float xspeed = float(6 - rand() % 12);
		if (xspeed == 0) xspeed = - 2.0f;
		float yspeed = float(2 + rand() % 3);
		float zspeed = float(1 + rand() % 3);
		float xrot = float(rand() % 360);
		float yrot = float(rand() % 360);
		float xrotspeed = float(1 + rand() % 2);
		float yrotspeed = float(1 + rand() % 2);


		float y_limit = float(rand () % g_ScreenHeight);
		float offset = float(float(M_PI) / (rand () % 180));

		xrot =  xrot * float(M_PI) / 180.0f;
		yrot =  yrot * float(M_PI) / 18.0f;
				
		xrotspeed = xrotspeed * float(M_PI) / 180.0f;
		yrotspeed = yrotspeed * float(M_PI) / 180.0f;
	
		s_DollarsArray[i].DollarPos.x = xpos;
		s_DollarsArray[i].DollarPos.y = ypos;
		s_DollarsArray[i].DollarPos.z = zpos;
		s_DollarsArray[i].DollarSpeed.x = xspeed;
		s_DollarsArray[i].DollarSpeed.y = yspeed;
		s_DollarsArray[i].DollarSpeed.z = zspeed;
		s_DollarsArray[i].DollarRot.x = xrot;
		s_DollarsArray[i].DollarRot.y = yrot;
		s_DollarsArray[i].DollarRot.z = 0.0;
		s_DollarsArray[i].DollarRotSpeed.x = xrotspeed;
		s_DollarsArray[i].DollarRotSpeed.y = yrotspeed;
		s_DollarsArray[i].DollarRotSpeed.z = 0.0;
		s_DollarsArray[i].DollarYlimit = y_limit;
		s_DollarsArray[i].DollarOffset = offset;

	}
}




void drawDollars(float fDelta)
{
	for (int i=0; i<NUM_DOLLARS; i++)
	{


//		drawModel(objGoldCoin, Vector3(s_DollarsArray[i].DollarPos.x, s_DollarsArray[i].DollarPos.y - (g_ScreenHeight / 2 + 128 * g_ScaleFactor) , s_DollarsArray[i].DollarPos.z), 
		drawModel(objDollar, Vector3(s_DollarsArray[i].DollarPos.x, s_DollarsArray[i].DollarPos.y  -  (g_ScreenHeight / 2 + 128 * g_ScaleFactor) , s_DollarsArray[i].DollarPos.z), 

		Vector3(s_DollarsArray[i].DollarRot.x,  s_DollarsArray[i].DollarRot.y, 0.0), 
			Vector3(1.0 , 1.0 , 1.0 ));

	

		s_DollarsArray[i].DollarPos.x = s_DollarsArray[i].DollarPos.x + s_DollarsArray[i].DollarSpeed.x;
		s_DollarsArray[i].DollarPos.z = s_DollarsArray[i].DollarPos.z + s_DollarsArray[i].DollarSpeed.z;

		s_DollarsArray[i].DollarPos.y = s_DollarsArray[i].DollarYlimit * sinf (s_DollarsArray[i].DollarOffset);


		if (s_DollarsArray[i].DollarOffset < M_PI / 2)
		{
			s_DollarsArray[i].DollarOffset = s_DollarsArray[i].DollarOffset +  (s_DollarsArray[i].DollarSpeed.y / 180);
		}
		else 
		{
			s_DollarsArray[i].DollarOffset = s_DollarsArray[i].DollarOffset + (s_DollarsArray[i].DollarSpeed.y / 360);
		}


		if (s_DollarsArray[i].DollarOffset > M_PI)
		{
			s_DollarsArray[i].DollarOffset = 0;

			float xpos = float(rand() % 64);
			float zpos = float(rand() % 164);
			float xspeed = float(6 - rand() % 12);
			if (xspeed == 0) xspeed = - 2.0f;
			float yspeed = float(2 + rand() % 3);
			float zspeed = float(1 + rand() % 3);
			float xrot = float(rand() % 360);
			float yrot = float(rand() % 360);


			float xrotspeed = 1.0f + float(rand() % 2);
			float yrotspeed = 1.0f + float(rand() % 2);


			float y_limit = float(g_ScreenHeight);

			xrot =  xrot * float(M_PI / 180.0);
			yrot =  yrot * float(M_PI / 180.0);
				
			xrotspeed = xrotspeed * float(M_PI / 180);
			yrotspeed = yrotspeed * float(M_PI / 180);
		
			s_DollarsArray[i].DollarPos.x = xpos;
			s_DollarsArray[i].DollarPos.y = 0.0f;
			s_DollarsArray[i].DollarPos.z = zpos;
			s_DollarsArray[i].DollarSpeed.x = xspeed;
			s_DollarsArray[i].DollarSpeed.y = yspeed;
			s_DollarsArray[i].DollarSpeed.z = zspeed;
			s_DollarsArray[i].DollarRot.x = xrot;
			s_DollarsArray[i].DollarRot.y = yrot;
			s_DollarsArray[i].DollarRot.z = 0.0;
			s_DollarsArray[i].DollarRotSpeed.x = xrotspeed;
			s_DollarsArray[i].DollarRotSpeed.y = yrotspeed;
			s_DollarsArray[i].DollarRotSpeed.z = 0.0;
			s_DollarsArray[i].DollarYlimit = y_limit;

		}

		s_DollarsArray[i].DollarRot = s_DollarsArray[i].DollarRot + s_DollarsArray[i].DollarRotSpeed;


	}
}




void drawDollars2(float fDelta)
{
	for (int i=0; i<NUM_DOLLARS; i++)
	{


//		drawModel(objGoldCoin, Vector3(s_DollarsArray[i].DollarPos.x, s_DollarsArray[i].DollarPos.y - (g_ScreenHeight / 2 + 128 * g_ScaleFactor) , s_DollarsArray[i].DollarPos.z), 
		drawModel(objDollar, Vector3(s_DollarsArray[i].DollarPos.x, s_DollarsArray[i].DollarPos.z, s_DollarsArray[i].DollarPos.y  -  (g_ScreenHeight / 2 + 128 * g_ScaleFactor) ), 

		Vector3(s_DollarsArray[i].DollarRot.x,  s_DollarsArray[i].DollarRot.y, 0.0), 
			Vector3(1.0 , 1.0 , 1.0 ));

	

//		s_DollarsArray[i].DollarPos.x = s_DollarsArray[i].DollarPos.x + s_DollarsArray[i].DollarSpeed.x;
//		s_DollarsArray[i].DollarPos.z = s_DollarsArray[i].DollarPos.z + s_DollarsArray[i].DollarSpeed.z;

		s_DollarsArray[i].DollarPos.y = s_DollarsArray[i].DollarYlimit * sinf (s_DollarsArray[i].DollarOffset);


		if (s_DollarsArray[i].DollarOffset < M_PI / 2)
		{
			s_DollarsArray[i].DollarOffset = s_DollarsArray[i].DollarOffset +  (s_DollarsArray[i].DollarSpeed.y / 180);
		}
		else 
		{
			s_DollarsArray[i].DollarOffset = s_DollarsArray[i].DollarOffset + (s_DollarsArray[i].DollarSpeed.y / 360);
		}


		if (s_DollarsArray[i].DollarOffset > M_PI)
		{
			s_DollarsArray[i].DollarOffset = 0;

			float xpos = float(rand() % 364);
			float zpos = float(rand() % 364);
			float xspeed = 6.0f - float(rand() % 12);
			if (xspeed == 0) xspeed = - 2.0f;
			float yspeed = 2.0f + float(rand() % 3);
			float zspeed = 1.0f + float(rand() % 3);
			float xrot = float(rand() % 360);
			float yrot = float(rand() % 360);
			float xrotspeed = 1.0f + float(rand() % 2);
			float yrotspeed = 1 + float(rand() % 2);


			float y_limit = float(g_ScreenHeight);

			xrot =  xrot * float(M_PI / 180);
			yrot =  yrot * float(M_PI / 180);
				
			xrotspeed = xrotspeed * float(M_PI / 180);
			yrotspeed = yrotspeed * float(M_PI / 180);
		
			s_DollarsArray[i].DollarPos.x = xpos;
			s_DollarsArray[i].DollarPos.y = 0.0f;
			s_DollarsArray[i].DollarPos.z = zpos;
			s_DollarsArray[i].DollarSpeed.x = xspeed;
			s_DollarsArray[i].DollarSpeed.y = yspeed;
			s_DollarsArray[i].DollarSpeed.z = zspeed;
			s_DollarsArray[i].DollarRot.x = xrot;
			s_DollarsArray[i].DollarRot.y = yrot;
			s_DollarsArray[i].DollarRot.z = 0.0f;
			s_DollarsArray[i].DollarRotSpeed.x = xrotspeed;
			s_DollarsArray[i].DollarRotSpeed.y = yrotspeed;
			s_DollarsArray[i].DollarRotSpeed.z = 0.0f;
			s_DollarsArray[i].DollarYlimit = y_limit;

		}

		s_DollarsArray[i].DollarRot = s_DollarsArray[i].DollarRot + s_DollarsArray[i].DollarRotSpeed;


	}
}

void SetColorTheme()
{
//	g_CurrentLevel = 43;

	if (g_CurrentLevel >= 1 && g_CurrentLevel < 10)
	{
		bgr_offset = 0;
		wheel_uv_offset = 0.0f; // gold
	}


	if (g_CurrentLevel >= 10 && g_CurrentLevel < 20)
	{
		bgr_offset = 1;
		wheel_uv_offset = 0.6f; /// blue
	}

	if (g_CurrentLevel >= 20 && g_CurrentLevel < 30)
	{
		bgr_offset = 2;
		wheel_uv_offset = 0.4f; // white
	}

	if (g_CurrentLevel >= 30 && g_CurrentLevel < 40)
	{
		bgr_offset = 3;
		wheel_uv_offset = 0.0f;
	}


	if (g_CurrentLevel >= 40 && g_CurrentLevel < 51)
	{
		bgr_offset = 4;
		wheel_uv_offset = 0.8f;
	}

	
	s_UVChangeParam->Set(wheel_uv_offset);

}

//			uv {0,0.73828}
//			uv {0.73828,0.73828}
//			uv {0,0.50007}
//			uv {0.73828,0.50007}

bool draw_reset = false;

void globalMainMenu(float fDelta)
{

	g_bshowAd = false;
	g_bselect_motivator = false;


	char vecstring[128];
//	IwGxLightingOff();
//	IwGxFontSetCol(0xffffffff);
//	IwGxFontSetFlags(IW_GX_FONT_DEFAULT_F);

	ShopButton.xpos = -360.0f;
	ShopButton.ypos = -180.0f;







	g_Input.Update();

	CTouch* touch;

		if (g_Input.getTouchCount() != 0)
		{

		  touch = g_Input.getTouch(0);

		  if (touch != NULL && touch->active)
			{

				if (!prev_touch)
				{
					if (do_menu_action != MENU_ACTION_RESET)
					{
						/// PLAY
						if (TestButtonPressed(MenuPlayButton, float(touch->x), float(touch->y)))
						{
							press_id = MenuPlayButton.id;
							MenuPlayButton.state = MENU_BUTTON_PRESSED;
						}

						/// SHOP
						if (TestButtonPressed(ShopButton, float(touch->x), float(touch->y)))
						{
							press_id = ShopButton.id;
							ShopButton.state = MENU_BUTTON_PRESSED;			
						}

						/// AUDIO
						if (TestButtonPressed(MenuAudioButton, float(touch->x), float(touch->y)))
						{
							press_id = MenuAudioButton.id;
							MenuAudioButton.state = MENU_BUTTON_PRESSED;				
						}

						/// SCORE
						if (TestButtonPressed(MenuScoreButton, float(touch->x), float(touch->y)))
						{
							press_id = MenuScoreButton.id;
							MenuScoreButton.state = MENU_BUTTON_PRESSED;		
						}
						/// RESET GAME
						if (TestButtonPressed(MenuResetGameButton, float(touch->x), float(touch->y)))
						{
							press_id = MenuResetGameButton.id;
							MenuResetGameButton.state = MENU_BUTTON_PRESSED;				
						}
					}
					else
					{
						/// RESET GAME CONTROL
						if (TestButtonPressed(MenuYesButton, float(touch->x), float(touch->y)))
						{
							press_id = MenuYesButton.id;
							MenuYesButton.state = MENU_BUTTON_PRESSED;				
						}

						if (TestButtonPressed(MenuNoButton, float(touch->x), float(touch->y)))
						{
							press_id = MenuNoButton.id;
							MenuNoButton.state = MENU_BUTTON_PRESSED;				
						}

					}


				} 
				else
				{
					if (do_menu_action != MENU_ACTION_RESET)
					{

						if (press_id ==  MenuPlayButton.id && !TestButtonPressed(MenuPlayButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							MenuPlayButton.state = MENU_BUTTON_NOTPRESSED;
						}

						if (press_id ==  MenuScoreButton.id && !TestButtonPressed(MenuScoreButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							MenuScoreButton.state = MENU_BUTTON_NOTPRESSED;
						}


						if (press_id ==  ShopButton.id && !TestButtonPressed(ShopButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							ShopButton.state = MENU_BUTTON_NOTPRESSED;
						}


						if (press_id ==  MenuAudioButton.id && !TestButtonPressed(MenuAudioButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							MenuAudioButton.state = MENU_BUTTON_NOTPRESSED;
						}

						if (press_id ==  MenuResetGameButton.id && !TestButtonPressed(MenuResetGameButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							MenuResetGameButton.state = MENU_BUTTON_NOTPRESSED;
						}
					}
					else
					{
						/// reset continue
						if (press_id == MenuYesButton.id && !TestButtonPressed(MenuYesButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							MenuYesButton.state = MENU_BUTTON_NOTPRESSED;
						}
						
						if (press_id == MenuNoButton.id && !TestButtonPressed(MenuNoButton, float(touch->x), float(touch->y)))
						{
							press_id = 0;
							MenuNoButton.state = MENU_BUTTON_NOTPRESSED;
						}



					}


				}

				prev_touch = true;
			}

		}
		  	else {

					touch = g_Input.getTouch(0);

					prev_touch = false;
					bool do_action = false;

					if (press_id > 0)
					{
						if (do_menu_action != MENU_ACTION_RESET)
						{
							if (press_id == MenuPlayButton.id && TestButtonPressed(MenuPlayButton, float(touch->x), float(touch->y)))
							{
								/// do action play
								do_menu_action = MENU_ACTION_PLAY;
								MenuPlayButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;					
							}

							if (press_id == MenuScoreButton.id && TestButtonPressed(MenuScoreButton, float(touch->x), float(touch->y)))
							{
								/// do action score
								do_menu_action = MENU_ACTION_SCORE;
								MenuScoreButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;
							}


							if (press_id == MenuAudioButton.id && TestButtonPressed(MenuAudioButton, float(touch->x), float(touch->y)))
							{
								/// do action audio
								do_menu_action = MENU_ACTION_AUDIO;
								MenuAudioButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;							
							}


							if (press_id == MenuResetGameButton.id && TestButtonPressed(MenuResetGameButton, float(touch->x), float(touch->y)))
							{
								/// do action audio
								do_menu_action = MENU_ACTION_RESET;
								MenuResetGameButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;
								draw_reset = true;			
							}


							if (press_id == ShopButton.id && TestButtonPressed(ShopButton, float(touch->x), float(touch->y)))
							{
								/// do action facebook
								do_menu_action = MENU_ACTION_SHOP;
								ShopButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;			
							}

						}
						else
						{
							if (press_id == MenuYesButton.id && TestButtonPressed(MenuYesButton, float(touch->x), float(touch->y)))
							{
								/// yes button
								do_menu_action = MENU_ACTION_NONE;
								MenuYesButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;

								g_pScore->SaveConfigToDisc(g_BestScore, g_BestLevel, 1, 0, g_NumJack, g_NumRocketEngine, g_NumDoubleCoins, g_NumDoubleSave, g_AudioStatus, true);

								g_CurrentLevel = 1;
								g_CurrentScore = 0;

							}

							if (press_id == MenuNoButton.id && TestButtonPressed(MenuNoButton, float(touch->x), float(touch->y)))
							{
								/// no button
								do_menu_action = MENU_ACTION_NONE;
								MenuNoButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;			
							}


						}

						if (!do_action)
						{
							if (press_id == MenuPlayButton.id) MenuPlayButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == MenuScoreButton.id) MenuScoreButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == MenuAudioButton.id) MenuAudioButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == ShopButton.id) ShopButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == MenuResetGameButton.id) ShopButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == MenuYesButton.id) MenuYesButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == MenuNoButton.id) MenuNoButton.state = MENU_BUTTON_NOTPRESSED;
						}

						press_id = 0;
					}

				}


			s_Surface[0]->MakeCurrent();
			IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

			IwGxLightingOff();
			SetCameraOrtho();



	float YWheelPos = 0.0f;
	float ZoomWheel = 1.0f * float(g_ScaleFactor);
	
	if (g_AspectRatio >=1.5f)
	{
		YWheelPos = -g_ScreenHeight / 2 + 128 * g_ScaleFactor;
	}
	else if (g_AspectRatio < 1.5f)
	{

		YWheelPos = -float(g_ScreenHeight / 2) + (128.0f * g_ScaleFactor * g_AspectRatio);

	}
	drawModel(objMenuWheel2, Vector3(0.0f , YWheelPos , 0.0f), Vector3(0.0f, 0.0f, wheel_rot), Vector3(0.0f, 0.0f, ZoomWheel));




//		s_Surface[0]->MakeCurrent();
	
//		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);


//		IwGxSetColClear(79, 16, 121, 0x00);
//		IwGxSetColClear(84, 0, 2, 0x00);
		initRender();

		CIwFMat view = CIwFMat::g_Identity;

        view.t.y = 0 + cam_offset_x;//IwGxGetScreenHeight() / 2.0f; //320;
        view.t.x = 0 ;//IwGxGetScreenWidth() / 2.0f; //480;
		view.t.z =  1152+120;
		view.LookAt(view.GetTrans(), CIwFVec3(0, cam_offset_x, -100.0), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);

		IwGxLightingAmbient(true);
	    IwGxLightingEmissive(false);
	    IwGxLightingDiffuse(true);
	    IwGxLightingSpecular(true);

	    IwGxSetLightType(1, IW_GX_LIGHT_AMBIENT);
	    CIwColour c;
	    c.Set(0x41,0x41,0x41,0xff);
	    IwGxSetLightCol(1, &c);

	    IwGxSetLightType(0, IW_GX_LIGHT_DIFFUSE);
	    CIwFVec3 dir = CIwFVec3(0.0f,-0.5f, -1.0f);
	    dir.Normalise();
		c.Set(0xff,0xff,0xff,0xff);
		IwGxSetLightCol(0, &c);
	    IwGxSetLightDirn(0, &dir);

		IwGxLightingOn();
		IwGxFogOff();
	//	IwGxSetFogFarZNearZ(800*10, 450*10);

//		drawDollars(fDelta);

		rot_factorY+=float(M_PI) / (64.0f);
		if (rot_factorY > 2*M_PI) rot_factorY = rot_factorY - float(M_PI) * 2;

		float rotY_rocketengine = 64.0f * sinf(rot_factorY);

///		rotY_rocketengine = rotY_rocketengine * float(M_PI) / 180.0f; 


		drawModel(objMainPlate, Vector3(0 , 360 + rotY_rocketengine , 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 0.80f));
		drawModel(objGameName, Vector3(0 , 174.0f + rotY_rocketengine , 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 0.80f));

		IwGxFlush();

        //Render a smaller version of the frame buffer,
        //transforming into a fake HDR version using the bloombuffer shader
        RenderPass(s_Surface[OFFSCREEN_SCENE], s_Surface[BLOOM_BUFFER], s_BloomBufferShader);

        //Blur the bloom buffer to give spread
        const float BLUR_SPREAD_X = 0.009f;
        const float BLUR_SPREAD_Y = BLUR_SPREAD_X * s_Surface[0]->GetClientWidth() / s_Surface[0]->GetClientHeight();

        CIwGxShaderUniform* blurDir = s_BlurBufferShader->GetParam("inBlurStep");
        float dirs[3] = { BLUR_SPREAD_X, 0.0f, BLUR_SPREAD_Y };
        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs);

        RenderPass(s_Surface[BLOOM_BUFFER], s_Surface[BLUR_BUFFER_1], s_BlurBufferShader);
        IwGxFlush(); //need to submit before changing the shader parameter (it is not an instanceParam)

        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs+1);
        RenderPass(s_Surface[BLUR_BUFFER_1], s_Surface[BLUR_BUFFER_2], s_BlurBufferShader);

        //Now composite the frame buffer and the bloom buffer using the postprocess shader

        //Select the back buffer as the current surface
        CIwGxSurface::MakeDisplayCurrent();

        //Render the buffers through the shader
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

        CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();


            //Select the original surface and the bloom surface into the material
            pMat->SetTexture(s_Surface[OFFSCREEN_SCENE]->GetTexture());
            pMat->SetTexture(s_Surface[BLUR_BUFFER_2]->GetTexture(), 1);

            pMat->SetShaderTechnique(s_PostProcessShader);

        IwGxSetMaterial(pMat);
        CIwSVec2 xy(0,0);
        CIwSVec2 wh((int16)IwGxGetScreenWidth(), (int16)IwGxGetScreenHeight());
        CIwFVec2 uvxy(0, 0);
        IwGxDrawRectScreenSpace(&xy, &wh, &uvxy, &s_Surface[OFFSCREEN_SCENE]->GetClientUVExtentFloat());




	
		IwGxFlush();


	IwGxLightingOff();
	SetCameraOrtho();



	if (MenuPlayButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(MenuPlayButton, 1.0f);
	else
		drawButton(MenuPlayButton, float(PRESSED_BUTTON_ZOOM));

	if (MenuScoreButton.state == float(MENU_BUTTON_NOTPRESSED)) drawButton(MenuScoreButton, 1.0f);
	else
		drawButton(MenuScoreButton, float(PRESSED_BUTTON_ZOOM));
		
	if (ShopButton.state == float(MENU_BUTTON_NOTPRESSED)) drawButton(ShopButton, 1.0f);
	else
		drawButton(ShopButton, float(PRESSED_BUTTON_ZOOM));

	if (MenuResetGameButton.state == float(MENU_BUTTON_NOTPRESSED)) drawButton(MenuResetGameButton, 1.0f, g_MaterialHelpWin);
	else
		drawButton(MenuResetGameButton, float(PRESSED_BUTTON_ZOOM), g_MaterialHelpWin);
/*
	if (ButtonFaceBook.state == MENU_BUTTON_NOTPRESSED)
	drawModel(objButtonFaceBook, Vector3(ButtonFaceBook.xpos , ButtonFaceBook.ypos , 1), Vector3(0, 0, 0), Vector3(scale_button, scale_button, scale_button));
	else
		drawModel(objButtonFaceBook, Vector3(ButtonFaceBook.xpos , ButtonFaceBook.ypos , 1), Vector3(0, 0, 0), Vector3(scale_button * 0.95, scale_button * 0.95, scale_button * 0.95));
*/

	


	if (g_AudioStatus) // audio enabled
	{
		if (MenuAudioButton.state == MENU_BUTTON_NOTPRESSED) drawButton(MenuAudioButton, 1.0f);
		else
			drawButton(MenuAudioButton, float(PRESSED_BUTTON_ZOOM));
	}
	else // audio disabled
	{
		if (MenuAudioButton.state == MENU_BUTTON_NOTPRESSED) drawButton(MenuAudioButton_Disable, 1.0f);
		else
			drawButton(MenuAudioButton_Disable, float(PRESSED_BUTTON_ZOOM));

	}


		float _xpos = 0.0f;
		float _ypos = 0.0f;

		float y_size = 186.0f;
		float x_size = y_size * 3.12f;


		float x_offset = (x_size * g_ScaleFactor) / 2;
		float y_offset = (y_size * g_ScaleFactor) / 2;

		x_size = x_size * g_ScaleFactor;
		y_size = y_size * g_ScaleFactor;

		float xpos = (_xpos * g_ScaleFactor) - x_offset;
		float ypos = (_ypos  * g_ScaleFactor)- y_offset;

		char score_text[128];

	switch (do_menu_action)
	{
/*	case MENU_ACTION_FACEBOOK:
		sprintf(vecstring, "I've just scored %i coins at %i level! #LiftRich", g_CurrentScore, g_CurrentLevel);
		g_pSocial->SetFBText(vecstring);
		g_pSocial->PostFBMessage();
		do_menu_action = MENU_ACTION_NONE;
		break;
*/
	case MENU_ACTION_SCORE:
		g_pScore->DisplayLeaderBoard();
		do_menu_action = MENU_ACTION_NONE;
		break;

	case MENU_ACTION_AUDIO:
		g_AudioStatus = !g_AudioStatus;
		if (g_AudioStatus) g_pAudio->PlayMusicFromBuffer(g_AudioData, g_AudioSize);
		else g_pAudio->PauseMusic();
		do_menu_action = MENU_ACTION_NONE;
		break;


	case MENU_ACTION_SHOP:
		global_state = GLOBAL_STATE_SHOP;
		do_menu_action = MENU_ACTION_NONE;
		g_ShopReturnCallBack = GLOBAL_STATE_MAIN_MENU;
		break;


	case MENU_ACTION_PLAY:
//		initGameLevel();
//		global_state = GLOBAL_STATE_GAME_PLAY;
		init_timer_flag = false;
		global_state = GLOBAL_STATE_NEXT_LEVEL;

		do_menu_action = MENU_ACTION_NONE;
		break;

	case MENU_ACTION_RESET:
		IwGxSetVertStreamModelSpace(s_VertsFadeOut, 4);
		IwGxSetMaterial(g_MaterialFadeOut);
		IwGxSetUVStream(s_UVFadeOut);
		IwGxSetColStream(NULL);
		IwGxSetNormStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

		s_VertsReset[0] =  CIwFVec3(xpos, ypos, 1.0f);
		s_VertsReset[1] =  CIwFVec3(xpos + x_size, ypos, 1.0f);
		s_VertsReset[2] =  CIwFVec3(xpos + x_size, ypos + y_size, 1.0f);
		s_VertsReset[3] =  CIwFVec3(xpos, ypos + y_size, 1.0f);

		s_UVReset[0] = CIwFVec2(0.0f, 0.73828f);
		s_UVReset[1] = CIwFVec2(0.73828f, 0.73828f);
		s_UVReset[2] = CIwFVec2(0.73828f, 0.50007f);
		s_UVReset[3] = CIwFVec2(0.0f, 0.50007f);
		//			uv {0,0.73828}
//			uv {0.73828,0.73828}
//			uv {0,0.50007}
//			uv {0.73828,0.50007}

		IwGxFlush();

		IwGxSetMaterial(g_MaterialHelpWin);

		IwGxSetVertStreamModelSpace(s_VertsReset, 4);
		IwGxSetUVStream(s_UVReset);
		IwGxSetColStream(NULL);
		IwGxSetNormStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);




		if (MenuYesButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(MenuYesButton, 1.0f, g_MaterialHelpWin);
		else
		drawButton(MenuYesButton, float(PRESSED_BUTTON_ZOOM), g_MaterialHelpWin);

		if (MenuNoButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(MenuNoButton, 1.0f, g_MaterialHelpWin);
		else
		drawButton(MenuNoButton, float(PRESSED_BUTTON_ZOOM), g_MaterialHelpWin);





		break;

	default:
		break;
	}



	wheel_rot+=float(MENU_WHEEL_SPEED) * float(M_PI) / 940.0f;

	if (wheel_rot > 2 * M_PI)
		wheel_rot = wheel_rot - 2 * float(M_PI);

	if (wheel_rot < 0) 
		wheel_rot = 2 * float(M_PI) - wheel_rot;


#ifdef _DEBUG

//			IwGxLightingOn();
//			IwGxFontSetRect(CIwRect(50, 190, 900, 600));
//			sprintf(vecstring, "SCORE: %d", g_CurrentScore);
//			IwGxFontDrawText(vecstring);
#endif

}


void globalGamePause(float fDelta)
{


	g_Input.Update();

	CTouch* touch;
	float finger_x = 0.0f;
	float finger_y = 0.0f;

		if (g_Input.getTouchCount() != 0)
		{
		  touch = g_Input.getTouch(0);

		  finger_x = float(touch->x);
		  finger_y = float(touch->y);

		  if (touch != NULL && touch->active)
			{

				if (!prev_touch)
				{
					if (TestButtonPressed(ResumeButton, finger_x, finger_y))
					{
						press_id = ResumeButton.id;
						ResumeButton.state = MENU_BUTTON_PRESSED;
					}

					if (TestButtonPressed(HomeButton, finger_x, finger_y))
					{
						press_id = HomeButton.id;
						HomeButton.state = MENU_BUTTON_PRESSED;
					
					}


				} else {

					if (press_id ==  ResumeButton.id && !TestButtonPressed(ResumeButton, finger_x, finger_y))
					{
						press_id = 0;
						ResumeButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id ==  HomeButton.id && !TestButtonPressed(HomeButton, finger_x, finger_y))
					{
						press_id = 0;
						HomeButton.state = MENU_BUTTON_NOTPRESSED;
					}

				}

				prev_touch = true;
			}

		}
		  	else {

					touch = g_Input.getTouch(0);

					finger_x = float(touch->x);
					finger_y = float(touch->y);

					bool do_action = false;
					prev_touch = false;

					if (press_id > 0)
					{
						if (press_id == ResumeButton.id && TestButtonPressed(ResumeButton, finger_x, finger_y))
						{
							/// do action play
							ResumeButton.state = MENU_BUTTON_NOTPRESSED;
							global_state = GLOBAL_STATE_GAME_PLAY;
							press_id = 0;
							prev_touch = false;
							do_action = true;
						}

						if (press_id == HomeButton.id && TestButtonPressed(HomeButton, finger_x, finger_y))
						{
							/// do action score
						//	do_menu_action = MENU_ACTION_SCORE;
							HomeButton.state = MENU_BUTTON_NOTPRESSED;
							global_state = GLOBAL_STATE_MAIN_MENU;
							press_id = 0;
							prev_touch = false;
							do_action = true;
						}


						
						if (!do_action)
						{
							if (press_id == ResumeButton.id) ResumeButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == HomeButton.id) HomeButton.state = MENU_BUTTON_NOTPRESSED;
						}

						press_id = 0;
					}

				}







	SetCameraOrtho();

	IwGxSetVertStreamModelSpace(s_VertsFadeOut, 4);
	IwGxSetMaterial(g_MaterialFadeOut);
	IwGxSetUVStream(s_UVFadeOut);
    IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	if (ResumeButton.state == MENU_BUTTON_NOTPRESSED) drawButton(ResumeButton, 1.0f);
	else
		drawButton(ResumeButton, float(PRESSED_BUTTON_ZOOM));

	if (HomeButton.state == MENU_BUTTON_NOTPRESSED) drawButton(HomeButton, 1.0f);
	else
	drawButton(HomeButton, float(PRESSED_BUTTON_ZOOM));



}

#define NODRAW_DOLLAR 0
#define DRAW_DOLLAR 1

/*
void prepareText(int score, float xpos, float ypos, int text_color, int hasdollar, float zoom_factor)
{

	char asci_text[32] = {0};

	snprintf(asci_text, sizeof(asci_text), "%i", score);

	int score_len = strlen(asci_text) + 1; //1 for additional space for '$' symbol

	float aspect_scale = float(g_ScreenHeight) / 1024.0f;

	if (g_AspectRatio >=1.5)
	{
		zoom_factor = zoom_factor * aspect_scale;
	}
	else if (g_AspectRatio < 1.5)
	{
		zoom_factor = zoom_factor * aspect_scale * float(ZOOM_ASPECT_FACTOR);

	}


	float xpos_score = xpos * g_ScaleFactor;
	float ypos_score = ypos * g_ScaleFactor;

	float symbol_size_x = g_FontSizeX * zoom_factor;
	float symbol_size_y = g_FontSizeY * zoom_factor;

	float offset_x = (float)((score_len + 1) * symbol_size_x) / 2 ;
	float offset_y = symbol_size_y / 2;

	xpos_score = xpos_score  - offset_x;
	ypos_score = ypos_score  - offset_y; 

	float uv_size_y = 1.0f / (float(g_TexturesSize) / g_FontSizeY);
	float uv_size_x = 1.0f / (float(g_TexturesSize) / g_FontSizeX);


	/// draw symbol '$'

	float uv_y = (float)text_color * uv_size_y;
	float uv_x;// = (float)10.0 * uv_size_x;

	if (hasdollar > 0)
	{
		uv_x = (float)10.0 * uv_size_x;
	}

	else 
	{
		uv_x = (float)11.0 * uv_size_x;
	}


	DrawTextList.begin();

	sDrawSymbol tmpDrawSymbol;

	tmpDrawSymbol.s_UVScore[0] = CIwFVec2(uv_x, uv_y + uv_size_y);
	tmpDrawSymbol.s_UVScore[1] = CIwFVec2(uv_x + uv_size_x, uv_y + uv_size_y);
	tmpDrawSymbol.s_UVScore[2] = CIwFVec2(uv_x + uv_size_x, uv_y);
	tmpDrawSymbol.s_UVScore[3] = CIwFVec2(uv_x, uv_y);

	tmpDrawSymbol.s_VertsScore[0] = CIwFVec3(xpos_score, ypos_score, 1);
	tmpDrawSymbol.s_VertsScore[1] = CIwFVec3(xpos_score + symbol_size_x, ypos_score, 1);
	tmpDrawSymbol.s_VertsScore[2] = CIwFVec3(xpos_score + symbol_size_x, ypos_score + symbol_size_y, 1);
	tmpDrawSymbol.s_VertsScore[3] = CIwFVec3(xpos_score, ypos_score + symbol_size_y, 1);

	DrawTextList.push_back(tmpDrawSymbol);
	//// end draw $


	for (int i=1; i<score_len; i++)
	{
		int symbol = asci_text[i-1] - 48;
		uv_x = (float)symbol * uv_size_x;
		float _size = (float)(i * g_FontSizeX * zoom_factor);

		tmpDrawSymbol.s_UVScore[0] = CIwFVec2(uv_x, uv_y + uv_size_y);
		tmpDrawSymbol.s_UVScore[1] = CIwFVec2(uv_x + uv_size_x, uv_y + uv_size_y);
		tmpDrawSymbol.s_UVScore[2] = CIwFVec2(uv_x + uv_size_x, uv_y);
		tmpDrawSymbol.s_UVScore[3] = CIwFVec2(uv_x, uv_y);

		tmpDrawSymbol.s_VertsScore[0] = CIwFVec3(_size + xpos_score, ypos_score, 1);
		tmpDrawSymbol.s_VertsScore[1] = CIwFVec3(_size + xpos_score + symbol_size_x, ypos_score, 1);
		tmpDrawSymbol.s_VertsScore[2] = CIwFVec3(_size + xpos_score + symbol_size_x, ypos_score + symbol_size_y, 1);
		tmpDrawSymbol.s_VertsScore[3] = CIwFVec3(_size + xpos_score, ypos_score + symbol_size_y, 1);

		DrawTextList.push_back(tmpDrawSymbol);
	}

}

void drawText()
{
	
		std::list<sDrawSymbol>::iterator drawsymbol_list = DrawTextList.begin();

		IwGxSetMaterial(g_MaterialGameUI);

		while (drawsymbol_list != DrawTextList.end())
		{
			IwGxSetVertStreamModelSpace(drawsymbol_list->s_VertsScore, 4);
			IwGxSetUVStream(drawsymbol_list->s_UVScore);
			IwGxSetColStream(NULL);
			IwGxSetNormStream(NULL);
			IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

			drawsymbol_list++;
		}

}
*/





void prepareText(int score, float xpos, float ypos, int text_color, int hasdollar, float zoom_factor)
{
	
	char asci_text[32] = {0};

	snprintf(asci_text, sizeof(asci_text), "%i", score);

	int score_len = strlen(asci_text) + 1; //1 for additional space for '$' symbol

	float aspect_scale = float(g_ScreenHeight) / 1024.0f;

	if (g_AspectRatio >=1.5)
	{
		zoom_factor = zoom_factor * aspect_scale;
	}
	else if (g_AspectRatio < 1.5)
	{
		zoom_factor = zoom_factor * aspect_scale * float(ZOOM_ASPECT_FACTOR);

	}


	float xpos_score = xpos * g_ScaleFactor;
	float ypos_score = ypos * g_ScaleFactor;

	float symbol_size_x = g_FontSizeX * zoom_factor;
	float symbol_size_y = g_FontSizeY * zoom_factor;

	float offset_x = (float)((score_len + 1) * symbol_size_x) / 2 ;
	float offset_y = symbol_size_y / 2;

	xpos_score = xpos_score  - offset_x;
	ypos_score = ypos_score  - offset_y; 

	float uv_size_y = 1.0f / (float(g_TexturesSize) / g_FontSizeY);
	float uv_size_x = 1.0f / (float(g_TexturesSize) / g_FontSizeX);


	/// draw symbol '$'

	float uv_y = (float)text_color * uv_size_y;
	float uv_x;// = (float)10.0 * uv_size_x;

	if (hasdollar > 0)
	{
		uv_x = (float)10.0 * uv_size_x;
	}

	else 
	{
		uv_x = (float)11.0 * uv_size_x;
	}


	DrawTextList.begin();

	CDrawSymbol *tmpDrawSymbol = new CDrawSymbol;

	tmpDrawSymbol->SetUVs(0, uv_x, uv_y + uv_size_y);
	tmpDrawSymbol->SetUVs(1, uv_x + uv_size_x, uv_y + uv_size_y);
	tmpDrawSymbol->SetUVs(2, uv_x + uv_size_x, uv_y);
	tmpDrawSymbol->SetUVs(3, uv_x, uv_y);

	tmpDrawSymbol->SetVerts(0, xpos_score, ypos_score, 1);
	tmpDrawSymbol->SetVerts(1, xpos_score + symbol_size_x, ypos_score, 1);
	tmpDrawSymbol->SetVerts(2, xpos_score + symbol_size_x, ypos_score + symbol_size_y, 1);
	tmpDrawSymbol->SetVerts(3, xpos_score, ypos_score + symbol_size_y, 1);

	DrawTextList.push_back(tmpDrawSymbol);
	//// end draw $


	for (int i=1; i<score_len; i++)
	{
		int symbol = asci_text[i-1] - 48;
		uv_x = (float)symbol * uv_size_x;
		float _size = (float)(i * g_FontSizeX * zoom_factor);

		tmpDrawSymbol = new CDrawSymbol;

		tmpDrawSymbol->SetUVs(0, uv_x, uv_y + uv_size_y);
		tmpDrawSymbol->SetUVs(1, uv_x + uv_size_x, uv_y + uv_size_y);
		tmpDrawSymbol->SetUVs(2, uv_x + uv_size_x, uv_y);
		tmpDrawSymbol->SetUVs(3, uv_x, uv_y);

		tmpDrawSymbol->SetVerts(0, _size + xpos_score, ypos_score, 1);
		tmpDrawSymbol->SetVerts(1, _size + xpos_score + symbol_size_x, ypos_score, 1);
		tmpDrawSymbol->SetVerts(2, _size + xpos_score + symbol_size_x, ypos_score + symbol_size_y, 1);
		tmpDrawSymbol->SetVerts(3, _size + xpos_score, ypos_score + symbol_size_y, 1);

		DrawTextList.push_back(tmpDrawSymbol);
	}

}

void drawText()
{
	
		std::list<CDrawSymbol*>::iterator drawsymbol_list = DrawTextList.begin();

//		 for (std::list<AudioSound*>::iterator it = m_Sounds.begin(); it !=  m_Sounds.end(); it++)
 //       if ((*it)->m_NameHash == name_hash)

		IwGxSetMaterial(g_MaterialGameUI);

		while (drawsymbol_list != DrawTextList.end())
		{
			IwGxSetVertStreamModelSpace((*drawsymbol_list)->GetVertsStream(), 4);
			IwGxSetUVStream((*drawsymbol_list)->GetUVsStream());
			IwGxSetColStream(NULL);
			IwGxSetNormStream(NULL);
			IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

			drawsymbol_list++;
		}

}



void drawMotivator(float xpos, float ypos, float zoom_factor, CIwMaterial *pMaterial)
{

	if (g_AspectRatio < 1.5)
		zoom_factor = zoom_factor * float(ZOOM_ASPECT_FACTOR);

	zoom_factor = zoom_factor * g_ScaleFactor;

	float xpos_score = xpos * g_ScaleFactor;
	float ypos_score = ypos * g_ScaleFactor;

	float symbol_size_x = 1024.0f * 0.8f * zoom_factor;
	float symbol_size_y = 42.0f * 0.8f * zoom_factor;

	float offset_x = symbol_size_x / 2 ;
	float offset_y = symbol_size_y / 2;

	xpos_score = xpos_score  - offset_x;
	ypos_score = ypos_score  - offset_y; 

		s_VertsMotivator[0] =  CIwFVec3(xpos_score, ypos_score, 1.0f);
		s_VertsMotivator[1] =  CIwFVec3(xpos_score + symbol_size_x, ypos_score, 1.0f);
		s_VertsMotivator[2] =  CIwFVec3(xpos_score + symbol_size_x, ypos_score + symbol_size_y, 1.0f);
		s_VertsMotivator[3] =  CIwFVec3(xpos_score, ypos_score + symbol_size_y, 1.0f);

		s_UVMotivator[0] = CIwFVec2(0.0f, g_ymotivator_offset + 0.04166);
		s_UVMotivator[1] = CIwFVec2(1.0f, g_ymotivator_offset + 0.04166);
		s_UVMotivator[2] = CIwFVec2(1.0f, g_ymotivator_offset);
		s_UVMotivator[3] = CIwFVec2(0.0f, g_ymotivator_offset);

		IwGxSetMaterial(pMaterial);
		IwGxSetVertStreamModelSpace(s_VertsMotivator, 4);
		IwGxSetUVStream(s_UVMotivator);
		IwGxSetColStream(NULL);
		IwGxSetNormStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
	
}



void globalGameOver(float fDelta)
{


		if (!g_bselect_motivator)
		{
			int num_motivator = rand() %24;
			g_ymotivator_offset = float(num_motivator * 0.041);
	
			g_bselect_motivator = true;
		}



			IwGxLightingOff();
			SetCameraOrtho();

			float zoom_factor = 1.0f;
			float zoom_factor2 = 1.0f;
			float zoom_factor3  = 1.0f;

			bool button_active = true;
			bool button_draw = true;


			if (t_zoom_time < 8)
			{

			float param1 = cosf (t_zoom_time);
			float param2 = expf(-(8) * (t_zoom_time / 20));

			zoom_factor  = 1.0f - float(fabs(1 * param1 * param2));
			t_zoom_time = t_zoom_time + float(M_PI) / 16.0f;

			button_draw = false;

			}


			if (button_draw && t_zoom_time2 < float(M_PI) / 2.0f)
			{
				zoom_factor2 = sinf(t_zoom_time2);

				t_zoom_time2 = t_zoom_time2 + float(M_PI) / 32.0f;

				button_active = false;
			}

			if (t_zoom_time3 < 8 && button_draw)
			{
					float param3 = cosf (t_zoom_time3);
					float param4 = expf(-(8) * (t_zoom_time3 / 20));

					zoom_factor3  = 1.0f - float(fabs(1 * param3 * param4));

					t_zoom_time3 = t_zoom_time3 + float(M_PI) / 16.0f;
			}

				float scale_factor = float(g_ScaleFactor);

	float _xpos = 0.0f;
	float _ypos = 30.0f;

	float y_size = 280.0f;
	float x_size = y_size * 2.11f;

	x_size = x_size * zoom_factor;
	y_size = y_size * zoom_factor;



	float x_offset = (x_size * scale_factor) / 2;
	float y_offset = (y_size * scale_factor) / 2;

	x_size = x_size * scale_factor;
	y_size = y_size * scale_factor;

	float xpos = (_xpos * scale_factor) - x_offset;
	float ypos = (_ypos * scale_factor)- y_offset;

	s_VertsGameOver[0] =  CIwFVec3(xpos, ypos, 1);
	s_VertsGameOver[1] =  CIwFVec3(xpos + x_size, ypos, 1);
	s_VertsGameOver[2] =  CIwFVec3(xpos + x_size, ypos + y_size, 1);
	s_VertsGameOver[3] =  CIwFVec3(xpos, ypos + y_size, 1);

	s_UVGameOver[0] = CIwFVec2(0.13170f, 0.88532f);
	s_UVGameOver[1] = CIwFVec2(0.86952f, 0.88532f);
	s_UVGameOver[3] = CIwFVec2(0.13170f, 0.53663f);
	s_UVGameOver[2] = CIwFVec2(0.86952f, 0.53663f);


	IwGxSetVertStreamModelSpace(s_VertsGameOver, 4);
	IwGxSetMaterial(g_MaterialGameOver);
	IwGxSetUVStream(s_UVGameOver);
	IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	prepareText(g_CurrentLevel, _xpos - 154 * zoom_factor, _ypos + 34 * zoom_factor, RED_TEXT, NODRAW_DOLLAR, zoom_factor);
	prepareText(g_CurrentScore, _xpos + 151 * zoom_factor, _ypos + 34 * zoom_factor, RED_TEXT, DRAW_DOLLAR, zoom_factor);


	prepareText(g_BestLevel, _xpos - 154 * zoom_factor, _ypos - 91 * zoom_factor, BLACK_TEXT, NODRAW_DOLLAR, zoom_factor);
	prepareText(g_BestScore, _xpos + 151 * zoom_factor, _ypos - 91 * zoom_factor, BLACK_TEXT, DRAW_DOLLAR, zoom_factor);
	drawText();


	if (button_active && button_draw)
	{
	if (ReplayButton.state == MENU_BUTTON_NOTPRESSED) drawButton(ReplayButton, 1.0f);
	else
		drawButton(ReplayButton, float(PRESSED_BUTTON_ZOOM));

	if (FaceBookButton.state == MENU_BUTTON_NOTPRESSED) drawButton(FaceBookButton, 1.0f);
	else
		drawButton(FaceBookButton, float(PRESSED_BUTTON_ZOOM));


	if (HomeButton.state == MENU_BUTTON_NOTPRESSED) drawButton(HomeButton, 1.0f);
	else
		drawButton(HomeButton, float(PRESSED_BUTTON_ZOOM));
	}

	if (!button_active && button_draw)
	{
		drawButton(ReplayButton, zoom_factor2);
		drawButton(FaceBookButton, zoom_factor2);
		drawButton(HomeButton, zoom_factor2);
	}



		/// draw motivator
		if (button_draw)
//		drawMotivator(_xpos * zoom_factor, _ypos + 123.0f * zoom_factor, zoom_factor, g_MaterialDeMotivator);
		drawMotivator(_xpos, _ypos + 123.0f, zoom_factor3, g_MaterialDeMotivator);

		/// end draw motivator

		

	g_Input.Update();

	CTouch* touch;

	float finger_x = 0.0f;
	float finger_y = 0.0f;

	if (button_active && button_draw)
	{

	if (g_Input.getTouchCount() != 0)
	{

	  touch = g_Input.getTouch(0);

	  finger_x = float(touch->x);
	  finger_y = float(touch->y);

		if (touch != NULL && touch->active)
			{

				if (!prev_touch)
				{
					if (TestButtonPressed(ReplayButton, finger_x, finger_y))
					{
						press_id = ReplayButton.id;
						ReplayButton.state = MENU_BUTTON_PRESSED;
					}

					if (TestButtonPressed(HomeButton, finger_x, finger_y))
					{
						press_id = HomeButton.id;
						HomeButton.state = MENU_BUTTON_PRESSED;
					
					}


					if (TestButtonPressed(FaceBookButton, finger_x, finger_y))
					{
						press_id = FaceBookButton.id;
						FaceBookButton.state = MENU_BUTTON_PRESSED;
					
					}



				} 
				else 
					{

					if (press_id == ReplayButton.id && !TestButtonPressed(ReplayButton, finger_x, finger_y))
					{
						press_id = 0;
						ReplayButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id ==  HomeButton.id && !TestButtonPressed(HomeButton, finger_x, finger_y))
					{
						press_id = 0;
						HomeButton.state = MENU_BUTTON_NOTPRESSED;
					}


					if (press_id ==  FaceBookButton.id && !TestButtonPressed(FaceBookButton, finger_x, finger_y))
					{
						press_id = 0;
						FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
					}

				}

				prev_touch = true;
			}

		}
		  	else {

					touch = g_Input.getTouch(0);

					finger_x = float(touch->x);
					finger_y = float(touch->y);

					bool do_action = false;
					prev_touch = false;

					if (press_id > 0)
					{
						if (press_id == ReplayButton.id && TestButtonPressed(ReplayButton, finger_x, finger_y))
						{
							/// do action replay
							ReplayButton.state = MENU_BUTTON_NOTPRESSED;
							global_state = GLOBAL_STATE_NEXT_LEVEL;
							init_timer_flag = false;
							press_id = 0;
							prev_touch = false;
							do_action = true;

							g_bshowAd = false;
							loadConfig();
							initGameLevel();

						

	
						}

						if (press_id == HomeButton.id && TestButtonPressed(HomeButton, finger_x, finger_y))
						{
							/// do action Home
							HomeButton.state = MENU_BUTTON_NOTPRESSED;
							global_state = GLOBAL_STATE_MAIN_MENU;
							press_id = 0;
							prev_touch = false;
							do_action = true;
							
							t_dist_time = 0;
							t_zoom_time = 0;
							t_zoom_time2 = 0;
							t_zoom_time3 = 0;
							loadConfig();
							removeAdBanner();
						}



						if (press_id == FaceBookButton.id && TestButtonPressed(FaceBookButton, finger_x, finger_y))
						{
							/// do action facebook

							FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
							press_id = 0;
							prev_touch = false;
							do_action = true;
							char fb_string[256];
							sprintf(fb_string, "I've just scored %i coins at %i level! #LiftRich", g_CurrentScore, g_CurrentLevel);
				//			MakeScreenshot();
							g_pSocial->SetFBText(fb_string);
							g_pSocial->PostFBMessage();

						}



						
						if (!do_action)
						{
							if (press_id == ReplayButton.id) ReplayButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == HomeButton.id) HomeButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == FaceBookButton.id) FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
						}

						press_id = 0;
					}

				}

	}

/*
		char vecstring[128];
		IwGxLightingOn();
		IwGxFontSetRect(CIwRect(0, 300, 900, 600));
		sprintf(vecstring, "PLAY TIME: %d", g_LevelElapsedTime);
		IwGxFontDrawText(vecstring);
*/

}



void globalNextLevel(float fDelta)
{

	if (!g_bselect_motivator)
	{
		int num_motivator = rand() %24;
		g_ymotivator_offset = float(num_motivator * 0.041);
		g_bselect_motivator = true;
	}





	IwGxLightingOff();
	SetCameraOrtho();

	g_Input.Update();

	ShopButton.xpos = - 360.0f;
	ShopButton.ypos = - 234.0f;

	CTouch* touch;
	float finger_x;
	float finger_y;


			float zoom_factor = 1.0f;
			float zoom_factor2 = 1.0f;
			float zoom_factor3 = 1.0f;

			bool button_active = true;
			bool button_draw = true;


//			if (g_CurrentLevel > 1)
			if (g_BestScore > 0)

			{
				if (t_zoom_time < 8)
				{

					float param1 = cosf (t_zoom_time);
					float param2 = expf(-(8) * (t_zoom_time / 20));

					zoom_factor  = 1.0f - float(fabs(1 * param1 * param2));
					t_zoom_time = t_zoom_time + float(M_PI) / 16.0f;
		
				button_draw = false;

				}


				if (button_draw && t_zoom_time2 < M_PI / 2.0)
				{
					zoom_factor2 = sinf(t_zoom_time2);

					t_zoom_time2 = t_zoom_time2 + float(M_PI) / 64.0f;

					button_active = false;

				}


				if (t_zoom_time3 < 8 && button_draw)
				{
					float param3 = cosf (t_zoom_time3);
					float param4 = expf(-(8) * (t_zoom_time3 / 20));

					zoom_factor3  = 1.0f - float(fabs(1 * param3 * param4));

					t_zoom_time3 = t_zoom_time3 + float(M_PI) / 16.0f;
				}

			}

///////////////////// banner integration//////////////////////////
	if (button_active && button_draw)
	{
		if (!init_timer_flag)
		{
			g_iAdStartTime = getCurrentTimeMs();
			g_iAdWaitTime = 250 + rand()%1000;
			init_timer_flag = true;
		}

		if (getCurrentTimeMs() >= g_iAdStartTime + g_iAdWaitTime)
		{
			showAdBanner();
		}
	}
/////////////////////////////////////////////////////////////


			if (button_active && button_draw)
			{

				if (g_Input.getTouchCount() != 0)
				{

				  touch = g_Input.getTouch(0);

				  finger_x = float(touch->x);
				  finger_y = float(touch->y);

			if (touch != NULL && touch->active)
			{


				if (!prev_touch)
				{
					if (TestButtonPressed(NextLevelButton, finger_x, finger_y))
					{
						press_id = NextLevelButton.id;
						NextLevelButton.state = MENU_BUTTON_PRESSED;
					}

//					if (g_CurrentLevel > 1)
					if (g_BestScore > 0)
					{
					if (TestButtonPressed(FaceBookButton, finger_x, finger_y))
					{
						press_id = FaceBookButton.id;
						FaceBookButton.state = MENU_BUTTON_PRESSED;					
					}


					if (TestButtonPressed(ShopButton, finger_x, finger_y))
					{
						press_id = ShopButton.id;
						ShopButton.state = MENU_BUTTON_PRESSED;					
					}


					///// check IAP select buttons

					if (TestButtonPressed(IAPSelectJackButton, finger_x, finger_y))
					{
						press_id = IAPSelectJackButton.id;
						IAPSelectJackButton.state = MENU_BUTTON_PRESSED;
					}

					if (TestButtonPressed(IAPSelectRocketPowerButton, finger_x, finger_y))
					{
						press_id = IAPSelectRocketPowerButton.id;
						IAPSelectRocketPowerButton.state = MENU_BUTTON_PRESSED;
					}

					if (TestButtonPressed(IAPSelectDoubleCoinsButton, finger_x, finger_y))
					{
						press_id = IAPSelectDoubleCoinsButton.id;
						IAPSelectDoubleCoinsButton.state = MENU_BUTTON_PRESSED;
					}

					if (TestButtonPressed(IAPSelectDoubleSaveButton, finger_x, finger_y))
					{
						press_id = IAPSelectDoubleSaveButton.id;
						IAPSelectDoubleSaveButton.state = MENU_BUTTON_PRESSED;
					}
					}
				} 
				else 
					{

					if (press_id == NextLevelButton.id && !TestButtonPressed(NextLevelButton, finger_x, finger_y))
					{
						press_id = 0;
						NextLevelButton.state = MENU_BUTTON_NOTPRESSED;
					}

//					if (g_CurrentLevel > 1)
					if (g_BestScore > 0)
					{
					if (press_id ==  FaceBookButton.id && !TestButtonPressed(FaceBookButton, finger_x, finger_y))
					{
						press_id = 0;
						FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id ==  ShopButton.id && !TestButtonPressed(ShopButton, finger_x, finger_y))
					{
						press_id = 0;
						ShopButton.state = MENU_BUTTON_NOTPRESSED;
					}


					/// IAP select
					if (press_id ==  IAPSelectJackButton.id && !TestButtonPressed(IAPSelectJackButton, finger_x, finger_y))
					{
						press_id = 0;
						IAPSelectJackButton.state = MENU_BUTTON_NOTPRESSED;
					}


					if (press_id ==  IAPSelectRocketPowerButton.id && !TestButtonPressed(IAPSelectRocketPowerButton, finger_x, finger_y))
					{
						press_id = 0;
						IAPSelectRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
					}


					if (press_id ==  IAPSelectDoubleCoinsButton.id && !TestButtonPressed(IAPSelectDoubleCoinsButton, finger_x, finger_y))
					{
						press_id = 0;
						IAPSelectDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
					}


					if (press_id ==  IAPSelectDoubleSaveButton.id && !TestButtonPressed(IAPSelectDoubleSaveButton, finger_x, finger_y))
					{
						press_id = 0;
						IAPSelectDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
					}
					}

				}

				prev_touch = true;
			}

		}
		  	else 
			{

					touch = g_Input.getTouch(0);

					finger_x = float(touch->x);
					finger_y = float(touch->y);

					bool do_action = false;
					prev_touch = false;

					if (press_id > 0)
					{
						if (press_id == NextLevelButton.id && TestButtonPressed(NextLevelButton, finger_x, finger_y))
						{
							/// do action play next level
							NextLevelButton.state = MENU_BUTTON_NOTPRESSED;
							global_state = GLOBAL_STATE_GAME_PLAY;
							press_id = 0;
							prev_touch = false;
							do_action = true;
							initGameLevel();

						

							
						}

//						if (g_CurrentLevel > 1)
						if (g_BestScore > 0)
						{
							if (press_id == FaceBookButton.id && TestButtonPressed(FaceBookButton, finger_x, finger_y))
							{
							/// do action score
								FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;
								char fb_string[256];
								sprintf(fb_string, "I've just scored %i coins at %i level! #LiftRich", g_CurrentScore, g_CurrentLevel);
								g_pSocial->SetFBText(fb_string);
								g_pSocial->PostFBMessage();
							}

							if (press_id == ShopButton.id && TestButtonPressed(ShopButton, finger_x, finger_y))
							{
								/// do action Shop
								ShopButton.state = MENU_BUTTON_NOTPRESSED;
								press_id = 0;
								prev_touch = false;
								do_action = true;

								global_state = GLOBAL_STATE_SHOP;
								g_ShopReturnCallBack = GLOBAL_STATE_NEXT_LEVEL;

							//	removeAdBanner();
							}



							/// IAP buttons actions
							if (g_NumJack > 0)
							{
								if (press_id == IAPSelectJackButton.id && TestButtonPressed(IAPSelectJackButton, finger_x, finger_y))
								{
									IAPSelectJackButton.state = MENU_BUTTON_NOTPRESSED;
									IAPSelectJackButton.do_action = !IAPSelectJackButton.do_action;
									press_id = 0;
									prev_touch = false;
									do_action = true;
								}
							}

							if (g_NumRocketEngine > 0)
							{
								if (press_id == IAPSelectRocketPowerButton.id && TestButtonPressed(IAPSelectRocketPowerButton, finger_x, finger_y))
								{
									IAPSelectRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
									IAPSelectRocketPowerButton.do_action = !IAPSelectRocketPowerButton.do_action;
									press_id = 0;
									prev_touch = false;
									do_action = true;
								}
							}



							if (g_NumDoubleCoins > 0)
							{
								if (press_id == IAPSelectDoubleCoinsButton.id && TestButtonPressed(IAPSelectDoubleCoinsButton, finger_x, finger_y))
								{
									IAPSelectDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
									IAPSelectDoubleCoinsButton.do_action = !IAPSelectDoubleCoinsButton.do_action;
									press_id = 0;
									prev_touch = false;
									do_action = true;
								}
							}


							
							if (g_NumDoubleSave > 0)
							{
								if (press_id == IAPSelectDoubleSaveButton.id && TestButtonPressed(IAPSelectDoubleSaveButton, finger_x, finger_y))
								{
									IAPSelectDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
									IAPSelectDoubleSaveButton.do_action = !IAPSelectDoubleSaveButton.do_action;
									press_id = 0;
									prev_touch = false;
									do_action = true;
								}
							}

						}

						
						if (!do_action)
						{
							if (press_id == NextLevelButton.id) NextLevelButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == FaceBookButton.id) FaceBookButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == ShopButton.id) ShopButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == IAPSelectJackButton.id) IAPSelectJackButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == IAPSelectRocketPowerButton .id) IAPSelectRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == IAPSelectDoubleCoinsButton.id) IAPSelectDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == IAPSelectDoubleSaveButton.id) IAPSelectDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;							
						}

						press_id = 0;
					}

				}

			}



	float _xpos = 0.0f;
	float _ypos = 10.0f;

	float y_size = 348.0f;
	float x_size = y_size * 2.29f;

//	if (g_CurrentLevel == 1)
	if (g_BestScore == 0)

	{
		y_size = 300.0f;
		x_size = y_size * 2.0f;
	}

	x_size = x_size * zoom_factor;
	y_size = y_size * zoom_factor;

	float x_offset = (x_size * g_ScaleFactor) / 2;
	float y_offset = (y_size * g_ScaleFactor) / 2;

	x_size = x_size * g_ScaleFactor;
	y_size = y_size * g_ScaleFactor;

	float xpos = (_xpos * g_ScaleFactor) - x_offset;
	float ypos = (_ypos  * g_ScaleFactor)- y_offset;

	s_VertsGameOver[0] =  CIwFVec3(xpos, ypos, 1.0f);
	s_VertsGameOver[1] =  CIwFVec3(xpos + x_size, ypos, 1.0f);
	s_VertsGameOver[2] =  CIwFVec3(xpos + x_size, ypos + y_size, 1.0f);
	s_VertsGameOver[3] =  CIwFVec3(xpos, ypos + y_size, 1.0f);

	s_UVGameOver[0] = CIwFVec2(0.0f, 0.433f);
	s_UVGameOver[1] = CIwFVec2(1.0f, 0.433f);
	s_UVGameOver[2] = CIwFVec2(1.0f, 0.0f);
	s_UVGameOver[3] = CIwFVec2(0.0f, 0.0f);



//	if (g_CurrentLevel > 1)
	if (g_BestScore > 0)
	{
		IwGxSetMaterial(g_MaterialGameOver);
		NextLevelButton.xpos = 360.0f;
		NextLevelButton.radius = 70.0f;
		NextLevelButton.ypos = - 234.0;
	}
	else
	{
		IwGxSetMaterial(g_MaterialHelpWin);
		NextLevelButton.xpos = 0.0f;
		NextLevelButton.radius = 90.0f;
		NextLevelButton.ypos = -210.0f;
		s_UVGameOver[0] = CIwFVec2(0.0, 0.5);
		s_UVGameOver[1] = CIwFVec2(1.0, 0.5);
		s_UVGameOver[2] = CIwFVec2(1.0, 0.0);
		s_UVGameOver[3] = CIwFVec2(0.0, 0.0);
	}

	IwGxSetVertStreamModelSpace(s_VertsGameOver, 4);
	IwGxSetUVStream(s_UVGameOver);
	IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);



//	if (g_CurrentLevel > 1)
	if (g_BestScore > 0)
	{
		prepareText(g_CurrentLevel, _xpos - 152 * zoom_factor, _ypos + 66 * zoom_factor , BLACK_TEXT, NODRAW_DOLLAR, zoom_factor);
		prepareText(g_CurrentScore, _xpos + 152 * zoom_factor, _ypos + 66 * zoom_factor, BLACK_TEXT, DRAW_DOLLAR, zoom_factor);
		prepareText(g_NumJack, _xpos - 328.0f * zoom_factor, _ypos - 55.0f * zoom_factor, BLACK_TEXT, NODRAW_DOLLAR, zoom_factor * 0.4f);
		prepareText(g_NumRocketEngine, _xpos - 160.0f * zoom_factor, _ypos - 55.0f * zoom_factor, BLACK_TEXT, NODRAW_DOLLAR, zoom_factor * 0.4f);
		prepareText(g_NumDoubleCoins, _xpos + 8.0f * zoom_factor, _ypos - 55.0f * zoom_factor, BLACK_TEXT, NODRAW_DOLLAR, zoom_factor * 0.4f);
		prepareText(g_NumDoubleSave, _xpos + 176.0f * zoom_factor, _ypos - 55.0f * zoom_factor, BLACK_TEXT, NODRAW_DOLLAR, zoom_factor * 0.4f);

		drawText();

		if (button_active && button_draw)
		{
			if (FaceBookButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(FaceBookButton, 1.0f);
			else
			drawButton(FaceBookButton, float(PRESSED_BUTTON_ZOOM));

			if (ShopButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(ShopButton, 1.0f);
			else
			drawButton(ShopButton, float(PRESSED_BUTTON_ZOOM));

			/// draw IAP selection
			if (IAPSelectJackButton.do_action) drawButton(IAPSelectJackButton, 1.0f, g_MaterialShop);
			if (IAPSelectRocketPowerButton.do_action) drawButton(IAPSelectRocketPowerButton, 1.0f, g_MaterialShop);
			if (IAPSelectDoubleCoinsButton.do_action) drawButton(IAPSelectDoubleCoinsButton, 1.0f, g_MaterialShop);
			if (IAPSelectDoubleSaveButton.do_action) drawButton(IAPSelectDoubleSaveButton, 1.0f, g_MaterialShop);

		}
	}

	if (button_active && button_draw)
	{
		if (NextLevelButton.state == MENU_BUTTON_NOTPRESSED) drawButton(NextLevelButton, 1.0f);
		else
		drawButton(NextLevelButton, float(PRESSED_BUTTON_ZOOM));


	}


	if (!button_active && button_draw)
	{
		drawButton(NextLevelButton, zoom_factor2);
		drawButton(FaceBookButton, zoom_factor2);
		drawButton(ShopButton, zoom_factor2);
	}




	/// draw Motivator
//	if (g_CurrentLevel > 1 && button_draw)
	if (g_BestScore > 0 && button_draw)
	drawMotivator(_xpos, _ypos + 154.f, zoom_factor3, g_MaterialMotivator);

		
		/// end draw motivator





	
		IwGxLightingAmbient(true);
	    IwGxLightingEmissive(false);
	    IwGxLightingDiffuse(true);
	    IwGxLightingSpecular(true);

	    IwGxSetLightType(1, IW_GX_LIGHT_AMBIENT);
	    CIwColour c;
	    c.Set(0x41,0x41,0x41,0xff);
	    IwGxSetLightCol(1, &c);

	    IwGxSetLightType(0, IW_GX_LIGHT_DIFFUSE);
	    CIwFVec3 dir = CIwFVec3(0.0f, -0.2f, -1.0f);
	    dir.Normalise();
		c.Set(0xff,0xff,0xff,0xff);
		IwGxSetLightCol(0, &c);
	    IwGxSetLightDirn(0, &dir);


		IwGxLightingOn();

		initRender();

		CIwFMat view = CIwFMat::g_Identity;
		

	    view.t.y = 0;//IwGxGetScreenHeight() / 2.0f; //320;
        view.t.x = 0 ;//IwGxGetScreenWidth() / 2.0f; //480;
		view.t.z =  1152+120;
		view.LookAt(view.GetTrans(), CIwFVec3(0, 0, -100.0), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);

		IwGxLightingOn();

		rot_factorX+=float(M_PI) / 32.0f;
		rot_factorY+=float(M_PI) / 160.0f;
		if (rot_factorX >= 2*M_PI) rot_factorX = rot_factorX - 2 * float(M_PI);
		if (rot_factorY >= 2*M_PI) rot_factorY = rot_factorY - 2 * float(M_PI);


		float rotX_rocketengine = rot_factorX;
		float rotY_rocketengine = rot_factorY;

//		rotX_rocketengine = rotX_rocketengine * M_PI / 180.0; 
//		rotY_rocketengine = rotY_rocketengine * M_PI / 180.0; 

		float yjack_pos = -56.0f;
		float xjack_pos = -282.0f;
		float jack_zoom = 0.13f * zoom_factor;

		float yrocket_pos = -58.0f;
		float xrocket_pos = -98.0f;
		float rocket_zoom = 0.4f * zoom_factor;

		float ydoublecoins_pos = -48.0f;
		float xdoublecoins_pos = 92.0f;
		float doublecoins_zoom = 0.4f * zoom_factor;

		float ydoublesave_pos = -50.0f;
		float xdoublesave_pos = 278.0f;		
		float doublesave_zoom = 0.55f * zoom_factor;

		if (g_AspectRatio < 1.5)
		{
			yjack_pos = yjack_pos * float(ZOOM_ASPECT_FACTOR);
			xjack_pos = xjack_pos * float(ZOOM_ASPECT_FACTOR);
			jack_zoom = jack_zoom * float(ZOOM_ASPECT_FACTOR);

			
			yrocket_pos = yrocket_pos * float(ZOOM_ASPECT_FACTOR);
			xrocket_pos = xrocket_pos * float(ZOOM_ASPECT_FACTOR);
			rocket_zoom = rocket_zoom * float(ZOOM_ASPECT_FACTOR);

			ydoublecoins_pos = ydoublecoins_pos * float(ZOOM_ASPECT_FACTOR);
			xdoublecoins_pos = xdoublecoins_pos * float(ZOOM_ASPECT_FACTOR);
			doublecoins_zoom = doublecoins_zoom * float(ZOOM_ASPECT_FACTOR);

			ydoublesave_pos = ydoublesave_pos * float(ZOOM_ASPECT_FACTOR);
			xdoublesave_pos = xdoublesave_pos * float(ZOOM_ASPECT_FACTOR);		
			doublesave_zoom = doublesave_zoom * float(ZOOM_ASPECT_FACTOR);

		}


		if (jack_zoom < 0.1f) jack_zoom = 0.1f;
		if (rocket_zoom < 0.1f) rocket_zoom = 0.1f;
		if (doublecoins_zoom < 0.1) doublecoins_zoom = 0.1f;
		if (doublesave_zoom < 0.1) doublesave_zoom = 0.1f;

//		if (g_CurrentLevel > 1)
		if (g_BestScore > 1)
		{
	//		if (IAPSelectJackButton.do_action) 
			drawModel(objIAP_LiftJack, Vector3(xjack_pos * zoom_factor, yjack_pos * zoom_factor, 0.0f), 
				Vector3(float(M_PI) / 8 , rotY_rocketengine, float(M_PI) / 8), 
				Vector3(0.0f, 0.0f, jack_zoom));
	
	//		if (IAPSelectRocketPowerButton.do_action)
			drawModel(objIAP_RocketEngine, Vector3(xrocket_pos * zoom_factor, yrocket_pos * zoom_factor, 0.0f), 
				Vector3(float(M_PI) / 8, rotY_rocketengine, float(M_PI) / 8), 
				Vector3(0.0f, 0.0f, rocket_zoom));
		
	//		if (IAPSelectDoubleCoinsButton.do_action)
			drawModel(objIAP_DoubleCoins, Vector3(xdoublecoins_pos * zoom_factor, ydoublecoins_pos * zoom_factor, 0.0f), 
				Vector3(0.0, float(M_PI) / 6 + rotY_rocketengine, 0.0f), 
				Vector3(0.0f, 0.0f, doublecoins_zoom));

	//		if (IAPSelectDoubleSaveButton.do_action)
			drawModel(objIAP_DoubleSave, Vector3(xdoublesave_pos * zoom_factor, ydoublesave_pos * zoom_factor, 0.0f), 
				Vector3(0, rotY_rocketengine, 0.0f), 
				Vector3(0.0f, 0.0f, doublesave_zoom));
		}


/*		char vecstring[128];
		IwGxFontSetRect(CIwRect(0, 300, 900, 600));
		sprintf(vecstring, "PLAY TIME: %d", g_LevelElapsedTime);
		IwGxFontDrawText(vecstring);
	*/



}


int g_internal_prod_id = 0;

void drawGift()
{

		float zoom_factor = 1.0f;

		if (t_zoom_time < 8)
		{

			float param1 = cosf (t_zoom_time);
			float param2 = expf(-(8) * (t_zoom_time / 20));

			zoom_factor  = 1.0f - float(fabs(1 * param1 * param2));
			t_zoom_time = t_zoom_time + float(M_PI) / 64.0f;
		
		}

		IwGxLightingAmbient(true);
	    IwGxLightingEmissive(false);
	    IwGxLightingDiffuse(true);
	    IwGxLightingSpecular(true);

	    IwGxSetLightType(1, IW_GX_LIGHT_AMBIENT);
	    CIwColour c;
	    c.Set(0x41,0x41,0x41,0xff);
	    IwGxSetLightCol(1, &c);

	    IwGxSetLightType(0, IW_GX_LIGHT_DIFFUSE);
	    CIwFVec3 dir = CIwFVec3(0.0f, -0.2f, -1.0f);
	    dir.Normalise();
		c.Set(0xff,0xff,0xff,0xff);
		IwGxSetLightCol(0, &c);
	    IwGxSetLightDirn(0, &dir);
		
		IwGxFogOff();
		IwGxLightingOff();
		
		s_Surface[0]->MakeCurrent();
		IwGxSetColClear(204, 214, 214, 0);
		s_UVChangeParam->Set(0.8f);
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		
		SetCameraOrtho();

		float ZoomWheel = 1.0f * g_ScaleFactor;
	
		drawModel(objMenuWheel2, Vector3(0 , 0 , 0), Vector3(0, 0, wheel_rot), Vector3(0, 0, ZoomWheel));


		initRender();

		CIwFMat view = CIwFMat::g_Identity;
		

	    view.t.y = 0;
        view.t.x = 0;
		view.t.z =  1152+120;
		view.LookAt(view.GetTrans(), CIwFVec3(0.0f, 0.0f, -100.0f), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);

		IwGxLightingOn();

		float scale_factor = 1.0f;


		rot_factorX+=M_PI / (128.0f * 4.5f);
		rot_factorY+=M_PI / (256.0f * 4.5f);
		if (rot_factorX > 2*M_PI) rot_factorX = M_PI * 2 - rot_factorX;
		if (rot_factorY > 2*M_PI) rot_factorY = M_PI * 2 - rot_factorY;


		float rotX_rocketengine = 36.0f * sinf(rot_factorX);
		float rotY_rocketengine = 180.0f * sinf(rot_factorY);

		rotX_rocketengine = rotX_rocketengine * M_PI / 180.0f; 
		rotY_rocketengine = rotY_rocketengine * M_PI / 180.0f; 

		float yjack_pos = 0.0f;
		float xjack_pos = 0.0f;
		float jack_zoom = 0.1 + 0.7f * zoom_factor;

		float yrocket_pos = 0.0f;
		float xrocket_pos = 0.0f;
		float rocket_zoom = 0.1 + 2.0f * zoom_factor;
		
		float ydoublecoins_pos = 0.0f;
		float xdoublecoins_pos = 0.0f;
		float doublecoins_zoom = 0.1 + 2.5f * zoom_factor;

		float ydoublesave_pos = 0.0f;
		float xdoublesave_pos = 0.0f;
		float doublesave_zoom = 0.1 + 4.0f * zoom_factor;
		

		switch (g_internal_prod_id)
		{

		case 1:
		drawModel(objIAP_LiftJack, Vector3(xjack_pos, yjack_pos, 0.0f), 
			Vector3(M_PI / 4 + rotY_rocketengine, rotY_rocketengine, M_PI / 4 - rotY_rocketengine), 
			Vector3(0.0f, 0.0f, jack_zoom));
		break;

		case 2:
		drawModel(objIAP_RocketEngine, Vector3(xrocket_pos, yrocket_pos, 0.0f), 
			Vector3(rotY_rocketengine, rotY_rocketengine, rotY_rocketengine), 
			Vector3(0.0f, 0.0f, rocket_zoom));
		break;

		case 3:
		drawModel(objIAP_DoubleCoins, Vector3(xdoublecoins_pos, ydoublecoins_pos, 0.0f), 
			Vector3(0.0, M_PI + rotY_rocketengine, 0.0f), 
			Vector3(0.0f, 0.0f, doublecoins_zoom));
		break;

		case 4:
		drawModel(objIAP_DoubleSave, Vector3(xdoublesave_pos, ydoublesave_pos, 0.0f), 
			Vector3(0, -rotY_rocketengine, 0.0f), 
			Vector3(0.0f, 0.0f, doublesave_zoom));
		break;

		default:
			break;
		}

		FadeIn(0.0f, 0xff,0xff,0xff);
		

		IwGxFlush();

        //Render a smaller version of the frame buffer,
        //transforming into a fake HDR version using the bloombuffer shader
        RenderPass(s_Surface[OFFSCREEN_SCENE], s_Surface[BLOOM_BUFFER], s_BloomBufferShader);

        //Blur the bloom buffer to give spread
        const float BLUR_SPREAD_X = 0.009f;
        const float BLUR_SPREAD_Y = BLUR_SPREAD_X * s_Surface[0]->GetClientWidth() / s_Surface[0]->GetClientHeight();

        CIwGxShaderUniform* blurDir = s_BlurBufferShader->GetParam("inBlurStep");
        float dirs[3] = { BLUR_SPREAD_X, 0.0f, BLUR_SPREAD_Y };
        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs);

        RenderPass(s_Surface[BLOOM_BUFFER], s_Surface[BLUR_BUFFER_1], s_BlurBufferShader);
        IwGxFlush(); //need to submit before changing the shader parameter (it is not an instanceParam)

        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs+1);
        RenderPass(s_Surface[BLUR_BUFFER_1], s_Surface[BLUR_BUFFER_2], s_BlurBufferShader);

        //Now composite the frame buffer and the bloom buffer using the postprocess shader

        //Select the back buffer as the current surface
        CIwGxSurface::MakeDisplayCurrent();

        //Render the buffers through the shader
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

        CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();


            //Select the original surface and the bloom surface into the material
            pMat->SetTexture(s_Surface[OFFSCREEN_SCENE]->GetTexture());
            pMat->SetTexture(s_Surface[BLUR_BUFFER_2]->GetTexture(), 1);

            pMat->SetShaderTechnique(s_PostProcessShader);

        IwGxSetMaterial(pMat);
        CIwSVec2 xy(0,0);
        CIwSVec2 wh((int16)IwGxGetScreenWidth(), (int16)IwGxGetScreenHeight());
        CIwFVec2 uvxy(0, 0);
        IwGxDrawRectScreenSpace(&xy, &wh, &uvxy, &s_Surface[OFFSCREEN_SCENE]->GetClientUVExtentFloat());

		IwGxFlush();


		wheel_rot+=MENU_WHEEL_SPEED * 1 * float(M_PI) / 720.0;

		if (wheel_rot > 2 * M_PI)
		wheel_rot = wheel_rot - 2 * float(M_PI);

		if (wheel_rot < 0) 
		wheel_rot = 2 * float(M_PI) - wheel_rot;


		IwGxFlush();

}


/// make a big $$$ =))

enum ShopActionID
{
    BUY_JACK, 
    BUY_ROCKETENGINE, 
    BUY_DOUBLECOINS, 
    BUY_SAVELEVEL,
};


#define SHOP_PURCHASE_NONE 0
#define SHOP_PURCHASE_SUCCESS 1
#define SHOP_PURCHASE_FAILED 2
#define SHOP_PURCHASE_PENDING 3

int g_shop_purchase_status = SHOP_PURCHASE_NONE;

/*
void ShopErrorCallback(void* userdata, IwBilling::CIwBillingErrorData* data)
{
	if (g_shop_purchase_status == SHOP_PURCHASE_SUCCESS) return;

	g_shop_purchase_status = SHOP_PURCHASE_FAILED;

	s3eDialogAlertInfo info;
	info.m_Message = "Can't purchase Booster!";
	info.m_Title = "=(";
	info.m_Button[0] = "Ok";
	s3eDialogAlertBox(&info);

}


void ShopBuyCallback(void* userdata, IwBilling::CIwBillingReceiptAvailableData* data)
{
   // AddStatus("Product purchase successfull");
  	g_shop_purchase_status = SHOP_PURCHASE_SUCCESS;
    
	IwBilling::FinishTransaction(data->FinaliseData);

	

//	s3eDialogAlertInfo info;
//	info.m_Message = "Thank you for purchase item!";
//	info.m_Title = "Good!";
//	info.m_Button[0] = "Ok";
//	s3eDialogAlertBox(&info);

}

*/

void TransactionUpdateCallback(s3ePaymentTransaction* transaction, void* userData)
{
 
    switch (transaction->m_TransactionStatus)
    {
        case S3E_PAYMENT_STATUS_PENDING:
			g_shop_purchase_status = SHOP_PURCHASE_PENDING;
            break;

        case S3E_PAYMENT_STATUS_PURCHASED:
        case S3E_PAYMENT_STATUS_RESTORED:
 
 
            // Check product ID of completed transaction
            if (!strcmp(transaction->m_Request->m_ProductID, g_PaymentRequest.m_ProductID))
            {
                if (g_TransactionReceipt.m_ReceiptData)
                {
                    s3eFree(g_TransactionReceipt.m_ReceiptData);
                    g_TransactionReceipt.m_ReceiptData = NULL;
                }

                if (g_PaymentTransaction)
                {
                    s3eFree(g_PaymentTransaction);
                    g_PaymentTransaction = NULL;
                }


                // Retain the transaction so it can be verified and finalised
                // later. In this simple example, we only track 1 at a time and
                // just complete it on the next update callback.
                if (!g_CompletedPayment)
                {
                    transaction->m_Retain = S3E_TRUE;
                    g_CompletedPayment = transaction;
                }

                // Copy transaction to local memory
                g_PaymentTransaction = (s3ePaymentTransaction*)s3eMalloc(sizeof(s3ePaymentTransaction));
                memcpy(g_PaymentTransaction, transaction, sizeof(s3ePaymentTransaction));

                // Point to local version of payment request identifier
                g_PaymentTransaction->m_Request = &g_PaymentRequest;

                // Copy receipt data to memory
                g_TransactionReceipt.m_ReceiptSize = transaction->m_TransactionReceipt->m_ReceiptSize;
                g_TransactionReceipt.m_ReceiptData = s3eMalloc(g_TransactionReceipt.m_ReceiptSize+1);
                memcpy(g_TransactionReceipt.m_ReceiptData, transaction->m_TransactionReceipt->m_ReceiptData, g_TransactionReceipt.m_ReceiptSize);
                g_PaymentTransaction->m_TransactionReceipt = &g_TransactionReceipt;

                // Make sure it's null terminated for convenient printing in this example
                char* endOfData = (char*)g_TransactionReceipt.m_ReceiptData + g_TransactionReceipt.m_ReceiptSize;
                *endOfData = '\0';


				g_shop_purchase_status = SHOP_PURCHASE_SUCCESS;
                break;
            }
            else
            {
				g_shop_purchase_status = SHOP_PURCHASE_SUCCESS;
            }
            break;

        case S3E_PAYMENT_STATUS_FAILED_CLIENT_INVALID:
			g_shop_purchase_status = SHOP_PURCHASE_FAILED;
            break;

        case S3E_PAYMENT_STATUS_FAILED_PAYMENT_CANCELLED:
			g_shop_purchase_status = SHOP_PURCHASE_FAILED;
            break;

        case S3E_PAYMENT_STATUS_FAILED_PAYMENT_INVALID:
			g_shop_purchase_status = SHOP_PURCHASE_FAILED;
            break;

        case S3E_PAYMENT_STATUS_FAILED_PAYMENT_NOT_ALLOWED:
			g_shop_purchase_status = SHOP_PURCHASE_FAILED;
            break;

        default:
			g_shop_purchase_status = SHOP_PURCHASE_FAILED;
            break;
    }
}

void ProductInfoCallback(s3eProductInformation* productInfo, void* userData)
{
 
}

/// internal ids:
/// 1 - Jack
/// 2 - rocket power
/// 3 - double coins
/// 4 - save


	
/*
bool shopPurchase(const char *product_id)
{
	g_shop_purchase_status = SHOP_PURCHASE_NONE;

	IwBilling::PurchaseProduct(product_id);

	bool ret = false;

	uint64 start_time = getCurrentTime();
	uint64 curr_time = 0;

	while (g_shop_purchase_status == SHOP_PURCHASE_NONE)
	{
//		sleep(1);
		s3eDeviceYield(100);

		curr_time = getCurrentTime();

		if (curr_time - start_time > 35) break;
		
	}

	if (g_shop_purchase_status == SHOP_PURCHASE_SUCCESS)
	{
		ret = true;
	}
	else
	{
//	if (g_shop_purchase_status == SHOP_PURCHASE_NONE || g_shop_purchase_status == SHOP_PURCHASE_FAILED) 
		ret = false;
	}

	return ret;
}
*/


bool shopPurchase(const char *product_id)
{
	g_shop_purchase_status = SHOP_PURCHASE_NONE;

	strcpy(g_PaymentRequest.m_ProductID, product_id);
    g_PaymentRequest.m_Quantity = 1;
//	g_CompletedPayment = NULL;

	s3eIOSAppStoreBillingRequestPayment(&g_PaymentRequest);

	bool ret = false;

	uint64 start_time = getCurrentTime();
	uint64 curr_time = start_time;

	while (curr_time - start_time < 28)
    {
		s3eDeviceYield(100);
		curr_time = getCurrentTime();

		if (g_shop_purchase_status == SHOP_PURCHASE_SUCCESS) break;
    }


//	if (g_CompletedPayment)
//	{
//		s3eIOSAppStoreBillingCompleteTransaction(g_CompletedPayment, S3E_TRUE);
//		g_CompletedPayment = NULL;
//	}


	if (g_shop_purchase_status == SHOP_PURCHASE_SUCCESS || g_shop_purchase_status == SHOP_PURCHASE_PENDING)
	{
		ret = true;
	}
	else
	{
		ret = false;

		s3eDialogAlertInfo info;
		info.m_Message = "Sorry, can't purchase Booster!";
		info.m_Title = "=(";
		info.m_Button[0] = "Ok";
		s3eDialogAlertBox(&info);
	
	}

	return ret;
}




void ShopReadyCallback(void* caller, void* data)
{
 //   s3eDebugTracePrintf("===================================================");
 //   s3eDebugTracePrintf("Billing is ready to use");
  //  s3eDebugTracePrintf("===================================================");
}





#define SHOP_STATE_NORMAL 0
#define SHOP_STATE_DRAWGIFT 1

int shop_state = SHOP_STATE_NORMAL;
int g_gift_counter = 0;

void globalShop()
{

	CTouch* touch;

	switch (shop_state)
	{

	case SHOP_STATE_DRAWGIFT:
		drawGift();
		g_gift_counter++;

		g_Input.Update();

		if (g_gift_counter > 120 && g_Input.getTouchCount() != 0)
		{
			s_AlphaBlend = 0;
			g_gift_counter = 0;
			shop_state = SHOP_STATE_NORMAL;
			prev_touch = true;
		}

		if (g_gift_counter > 1800)
		{
			s_AlphaBlend = 0;
			g_gift_counter = 0;
			shop_state = SHOP_STATE_NORMAL;
			prev_touch = true;
		}
	   
		break;

	case SHOP_STATE_NORMAL:
		g_bshowAd = false;

		g_Input.Update();


		float finger_x = 0.0f;
		float finger_y = 0.0f;

		if (g_Input.getTouchCount() != 0)
		{

			touch = g_Input.getTouch(0);

			finger_x = float(touch->x);
			finger_y = float(touch->y);

			if (touch != NULL && touch->active)
			{

				if (!prev_touch)
				{
					/// JACK
					if (TestButtonPressed(ShopBuyJackButton, finger_x, finger_y))
					{
						press_id = ShopBuyJackButton.id;
						ShopBuyJackButton.state = MENU_BUTTON_PRESSED;
					}

					if (TestButtonPressed(ShopBuyRocketPowerButton, finger_x, finger_y))
					{
						press_id = ShopBuyRocketPowerButton.id;
						ShopBuyRocketPowerButton.state = MENU_BUTTON_PRESSED;
					
					}

					if (TestButtonPressed(ShopBuyDoubleCoinsButton, finger_x, finger_y))
					{
						press_id = ShopBuyDoubleCoinsButton.id;
						ShopBuyDoubleCoinsButton.state = MENU_BUTTON_PRESSED;
					
					}

					if (TestButtonPressed(ShopBuyDoubleSaveButton, finger_x, finger_y))
					{
						press_id = ShopBuyDoubleSaveButton.id;
						ShopBuyDoubleSaveButton.state = MENU_BUTTON_PRESSED;
					
					}


					if (TestButtonPressed(ShopExitButton, finger_x, finger_y))
					{
						press_id = ShopExitButton.id;
						ShopExitButton.state = MENU_BUTTON_PRESSED;
					
					}
				} 
				else 
					{

					/// JACK
					if (press_id == ShopBuyJackButton.id && !TestButtonPressed(ShopBuyJackButton, finger_x, finger_y))
					{
						press_id = 0;
						ShopBuyJackButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id == ShopBuyRocketPowerButton.id && !TestButtonPressed(ShopBuyRocketPowerButton, finger_x, finger_y))
					{
						press_id = 0;
						ShopBuyRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id == ShopBuyDoubleCoinsButton.id && !TestButtonPressed(ShopBuyDoubleCoinsButton, finger_x, finger_y))
					{
						press_id = 0;
						ShopBuyDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id == ShopBuyDoubleSaveButton.id && !TestButtonPressed(ShopBuyDoubleSaveButton, finger_x, finger_y))
					{
						press_id = 0;
						ShopBuyDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
					}

					if (press_id == ShopExitButton.id && !TestButtonPressed(ShopExitButton, finger_x, finger_y))
					{
						press_id = 0;
						ShopExitButton.state = MENU_BUTTON_NOTPRESSED;
					}

				}

				prev_touch = true;
			}

		}
		  	else 
			{
					touch = g_Input.getTouch(0);

					finger_x = float(touch->x);
					finger_y = float(touch->y);

					bool do_action = false;
					prev_touch = false;

					if (press_id > 0)
					{
						if (press_id == ShopBuyJackButton.id && TestButtonPressed(ShopBuyJackButton, finger_x, finger_y))
						{
							/// JACK action
							ShopBuyJackButton.state = MENU_BUTTON_NOTPRESSED;
							press_id = 0;
							prev_touch = false;
							do_action = true;

							if (shopPurchase(g_IAP_ID_Jack))
							{
								g_NumJack++;
								g_NumJack++;
								UpdateConfigPurchase();
								g_pAudio->PlaySound("/sounds/win1.wav");
								s_AlphaBlend = 0;
								g_gift_counter = 0;
								t_zoom_time = 0;
								shop_state = SHOP_STATE_DRAWGIFT;
								g_internal_prod_id = 1;

							}
					//		g_NumJack++;
				
						}

					
						if (press_id == ShopBuyRocketPowerButton.id && TestButtonPressed(ShopBuyRocketPowerButton, finger_x, finger_y))
						{
							/// ROCKET ENGINE do action
							ShopBuyRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
							press_id = 0;
							prev_touch = false;
							do_action = true;

							if (shopPurchase(g_IAP_ID_RocketPower))
							{
								g_NumRocketEngine++;
								g_NumRocketEngine++;
								UpdateConfigPurchase();
								g_pAudio->PlaySound("/sounds/win1.wav");
								s_AlphaBlend = 0;
								g_gift_counter = 0;
								t_zoom_time = 0;
								shop_state = SHOP_STATE_DRAWGIFT;
								g_internal_prod_id = 2;

							}
						}
					
						if (press_id == ShopBuyDoubleCoinsButton.id && TestButtonPressed(ShopBuyDoubleCoinsButton, finger_x, finger_y))
						{
							/// DOUBLE COINS do action
							ShopBuyDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
							press_id = 0;
							prev_touch = false;
							do_action = true;

							if (shopPurchase(g_IAP_ID_DoubleCoins))
							{
								g_NumDoubleCoins++;
								UpdateConfigPurchase();
								g_pAudio->PlaySound("/sounds/win1.wav");
								s_AlphaBlend = 0;
								g_gift_counter = 0;
								t_zoom_time = 0;
								shop_state = SHOP_STATE_DRAWGIFT;
								g_internal_prod_id = 3;
							}
						}

						if (press_id == ShopBuyDoubleSaveButton.id && TestButtonPressed(ShopBuyDoubleSaveButton, finger_x, finger_y))
						{
							/// SAVE LEVEL do action
							ShopBuyDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
							press_id = 0;
							prev_touch = false;
							do_action = true;

							if (shopPurchase(g_IAP_ID_Save))
							{
								g_NumDoubleSave++;
								g_NumDoubleSave++;
								UpdateConfigPurchase();
								g_pAudio->PlaySound("/sounds/win1.wav");
								s_AlphaBlend = 0;
								g_gift_counter = 0;
								t_zoom_time = 0;
								shop_state = SHOP_STATE_DRAWGIFT;
								g_internal_prod_id = 4;
							}
						}


						if (press_id == ShopExitButton.id && TestButtonPressed(ShopExitButton, finger_x, finger_y))
						{
							/// SAVE LEVEL do action
							ShopExitButton.state = MENU_BUTTON_NOTPRESSED;
							press_id = 0;
							prev_touch = false;
							do_action = true;
							global_state = g_ShopReturnCallBack;
							g_ShopReturnCallBack = -1;
							t_dist_time = 0;
							t_zoom_time = 0;
							t_zoom_time2 = 0;
							t_zoom_time3 = 0;
						}


						if (!do_action)
						{
							if (press_id == ShopBuyJackButton.id) ShopBuyJackButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == ShopBuyRocketPowerButton.id) ShopBuyRocketPowerButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == ShopBuyDoubleCoinsButton.id) ShopBuyDoubleCoinsButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == ShopBuyDoubleSaveButton.id) ShopBuyDoubleSaveButton.state = MENU_BUTTON_NOTPRESSED;
							if (press_id == ShopExitButton.id) ShopExitButton.state = MENU_BUTTON_NOTPRESSED;

						}

						press_id = 0;
					}

				}





		IwGxLightingAmbient(true);
	    IwGxLightingEmissive(false);
	    IwGxLightingDiffuse(true);
	    IwGxLightingSpecular(true);

	    IwGxSetLightType(1, IW_GX_LIGHT_AMBIENT);
	    CIwColour c;
	    c.Set(0x41,0x41,0x41,0xff);
	    IwGxSetLightCol(1, &c);

	    IwGxSetLightType(0, IW_GX_LIGHT_DIFFUSE);
	    CIwFVec3 dir = CIwFVec3(0.0f, -0.2f, -1.0f);
	    dir.Normalise();
		c.Set(0xff,0xff,0xff,0xff);
		IwGxSetLightCol(0, &c);
	    IwGxSetLightDirn(0, &dir);
		
		IwGxFogOff();
		IwGxLightingOff();
		
		SetCameraOrtho();

	float ZoomWheel = 4.5f * g_ScaleFactor;
	
	drawModel(objMenuWheel2, Vector3(0 , 0 , 0), Vector3(0, 0, wheel_rot), Vector3(0, 0, ZoomWheel));

	CIwFMat model = CIwFMat::g_Identity;
	IwGxSetModelMatrix(&model);


	float _xpos = 0;
	float _ypos = 0;

	float y_size = 640.0f;
	float x_size = y_size * 1.242f;


	float x_offset = (x_size * g_ScaleFactor) / 2;
	float y_offset = (y_size * g_ScaleFactor) / 2;

	x_size = x_size * g_ScaleFactor;
	y_size = y_size * g_ScaleFactor;

	float xpos = (_xpos * g_ScaleFactor) - x_offset;
	float ypos = (_ypos  * g_ScaleFactor)- y_offset;

	s_VertsShop[0] =  CIwFVec3(xpos, ypos, 1.0f);
	s_VertsShop[1] =  CIwFVec3(xpos + x_size, ypos, 1.0f);
	s_VertsShop[2] =  CIwFVec3(xpos + x_size, ypos + y_size, 1.0f);
	s_VertsShop[3] =  CIwFVec3(xpos, ypos + y_size, 1.0f);

	s_UVShop[3] = CIwFVec2(0.0f, 0.0f);
	s_UVShop[2] = CIwFVec2(1.0f, 0.0f);
	s_UVShop[1] = CIwFVec2(1.0f, 0.80181f);
	s_UVShop[0] = CIwFVec2(0.0f, 0.80181f);



	IwGxSetMaterial(g_MaterialShop);
	IwGxSetVertStreamModelSpace(s_VertsShop, 4);
	IwGxSetUVStream(s_UVShop);
	IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);


	if (ShopBuyJackButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(ShopBuyJackButton, 1.0f, g_MaterialShop);
	else
	drawButton(ShopBuyJackButton, float(PRESSED_BUTTON_ZOOM), g_MaterialShop);

	if (ShopBuyRocketPowerButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(ShopBuyRocketPowerButton, 1.0f, g_MaterialShop);
	else
	drawButton(ShopBuyRocketPowerButton, float(PRESSED_BUTTON_ZOOM), g_MaterialShop);

	if (ShopBuyDoubleCoinsButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(ShopBuyDoubleCoinsButton, 1.0f, g_MaterialShop);
	else
	drawButton(ShopBuyDoubleCoinsButton, float(PRESSED_BUTTON_ZOOM), g_MaterialShop);

	if (ShopBuyDoubleSaveButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(ShopBuyDoubleSaveButton, 1.0f, g_MaterialShop);
	else
	drawButton(ShopBuyDoubleSaveButton, float(PRESSED_BUTTON_ZOOM), g_MaterialShop);


	if (ShopExitButton.state == MENU_BUTTON_NOTPRESSED)	drawButton(ShopExitButton, 1.0f, g_MaterialShop);
	else
	drawButton(ShopExitButton, float(PRESSED_BUTTON_ZOOM), g_MaterialShop);



	prepareText(g_NumJack, 300.0f, 202.f, BLACK_TEXT, 0, 0.8f);
	prepareText(g_NumRocketEngine, 300.0f, 70.0f, BLACK_TEXT, 0, 0.8f);
	prepareText(g_NumDoubleCoins, 300.0f, -62.0f, BLACK_TEXT, 0, 0.8f);
	prepareText(g_NumDoubleSave, 300.0f, -195.0f, BLACK_TEXT, 0, 0.8f);

	drawText();


	initRender();

		CIwFMat view = CIwFMat::g_Identity;
		

	    view.t.y = 0;
        view.t.x = 0;
		view.t.z =  1152+120;
		view.LookAt(view.GetTrans(), CIwFVec3(0.0f, 0.0f, -100.0f), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);

		IwGxLightingOn();

		float scale_factor = 1.0f;


		rot_factorX+=float(M_PI) / (128.0f * 2.5f);
		rot_factorY+=float(M_PI) / (256.0f * 2.5f);
		if (rot_factorX > 2*M_PI) rot_factorX = float(M_PI) * 2 - rot_factorX;
		if (rot_factorY > 2*M_PI) rot_factorY = float(M_PI) * 2 - rot_factorY;


		float rotX_rocketengine = 36 * sinf(rot_factorX);
		float rotY_rocketengine = 180 * sinf(rot_factorY);

		rotX_rocketengine = rotX_rocketengine * float(M_PI) / 180.0f; 
		rotY_rocketengine = rotY_rocketengine * float(M_PI) / 180.0f; 

		float yjack_pos = 210.0f;;
		float xjack_pos = -320.0f;
		float jack_zoom = 0.15f;

		float yrocket_pos = 56.0f;
		float xrocket_pos = -316.0f;
		float rocket_zoom = 0.43f;
		
		float ydoublecoins_pos = -80.0f;
		float xdoublecoins_pos = -316.0f;
		float doublecoins_zoom = 0.45f;

		float ydoublesave_pos = -230.0f;
		float xdoublesave_pos = -316.0f;
		float doublesave_zoom = 0.6f;



		if (g_AspectRatio < 1.5)
		{
			yjack_pos = yjack_pos * float(ZOOM_ASPECT_FACTOR);
			xjack_pos = xjack_pos * float(ZOOM_ASPECT_FACTOR);
			jack_zoom = jack_zoom * float(ZOOM_ASPECT_FACTOR);
			
			yrocket_pos = yrocket_pos * float(ZOOM_ASPECT_FACTOR);
			xrocket_pos = xrocket_pos * float(ZOOM_ASPECT_FACTOR);
			rocket_zoom = rocket_zoom * float(ZOOM_ASPECT_FACTOR);

			ydoublecoins_pos = ydoublecoins_pos * float(ZOOM_ASPECT_FACTOR);
			xdoublecoins_pos = xdoublecoins_pos * float(ZOOM_ASPECT_FACTOR);
			doublecoins_zoom = doublecoins_zoom * float(ZOOM_ASPECT_FACTOR);

			ydoublesave_pos = ydoublesave_pos * float(ZOOM_ASPECT_FACTOR);
			xdoublesave_pos = xdoublesave_pos * float(ZOOM_ASPECT_FACTOR);		
			doublesave_zoom = doublesave_zoom * float(ZOOM_ASPECT_FACTOR);

		}


		drawModel(objIAP_LiftJack, Vector3(xjack_pos, yjack_pos, 0.0f), 
			Vector3(float(M_PI) / 4 + rotY_rocketengine, rotY_rocketengine, float(M_PI) / 4 - rotY_rocketengine), 
			Vector3(0.0f, 0.0f, jack_zoom));

		drawModel(objIAP_RocketEngine, Vector3(xrocket_pos, yrocket_pos, 0.0f), 
			Vector3(rotY_rocketengine, rotY_rocketengine, rotY_rocketengine), 
			Vector3(0.0f, 0.0f, rocket_zoom));
		
		drawModel(objIAP_DoubleCoins, Vector3(xdoublecoins_pos, ydoublecoins_pos, 0.0f), 
			Vector3(0.0, float(M_PI) / 6 + rotY_rocketengine, 0.0f), 
			Vector3(0.0f, 0.0f, doublecoins_zoom));


		drawModel(objIAP_DoubleSave, Vector3(xdoublesave_pos, ydoublesave_pos, 0.0f), 
			Vector3(0, -rotY_rocketengine, 0.0f), 
			Vector3(0.0f, 0.0f, doublesave_zoom));


		wheel_rot+=MENU_WHEEL_SPEED * 0* float(M_PI) / 1080.0;

		if (wheel_rot > 2 * M_PI)
		wheel_rot = wheel_rot - 2 * float(M_PI);

		if (wheel_rot < 0) 
		wheel_rot = 2 * float(M_PI) - wheel_rot;

		break;

		}
}



void drawFinalCut(float fDelta)
{
		IwGxLightingAmbient(true);
	    IwGxLightingEmissive(false);
	    IwGxLightingDiffuse(true);
	    IwGxLightingSpecular(true);

	    IwGxSetLightType(1, IW_GX_LIGHT_AMBIENT);
	    CIwColour c;
	    c.Set(0x41,0x41,0x41,0xff);
	    IwGxSetLightCol(1, &c);

	    IwGxSetLightType(0, IW_GX_LIGHT_DIFFUSE);
	    CIwFVec3 dir = CIwFVec3(0.0f, -0.2f, -1.0f);
	    dir.Normalise();
		c.Set(0xff,0xff,0xff,0xff);
		IwGxSetLightCol(0, &c);
	    IwGxSetLightDirn(0, &dir);
		
		IwGxFogOff();
		IwGxLightingOff();
		
		s_Surface[0]->MakeCurrent();
		IwGxSetColClear(204, 214, 214, 0);
		s_UVChangeParam->Set(0.8f);
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		
		SetCameraOrtho();

	float ZoomWheel = 1.0f * g_ScaleFactor;
	
	drawModel(objMenuWheel2, Vector3(0 , 0 , 0), Vector3(0, 0, wheel_rot), Vector3(0, 0, ZoomWheel));


	initRender();

		CIwFMat view = CIwFMat::g_Identity;
		

	    view.t.y = 0;
        view.t.x = 0;
		view.t.z =  1152+120;
		view.LookAt(view.GetTrans(), CIwFVec3(0.0f, 0.0f, -100.0f), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);

		IwGxLightingOn();

		rot_factorY+=M_PI / 256.0f;
		if (rot_factorY > 2*M_PI) rot_factorY = M_PI * 2 - rot_factorY;

		float zoom_factor = 1.3 + 0.2 * sinf(rot_factorY);

		drawDollars(fDelta);


		IwGxLightingOff();
		SetCameraOrtho();


		float _xpos = 0.0f;
		float _ypos = 32.0f;

		float y_size = 512.0f;
		float x_size = 512.0f;


		x_size = x_size * zoom_factor;
		y_size = y_size * zoom_factor;

		float x_offset = (x_size * g_ScaleFactor) / 2;
		float y_offset = (y_size * g_ScaleFactor) / 2;

		x_size = x_size * g_ScaleFactor;
		y_size = y_size * g_ScaleFactor;

		float xpos = (_xpos * g_ScaleFactor) - x_offset;
		float ypos = (_ypos  * g_ScaleFactor)- y_offset;

		s_VertsGameOver[0] =  CIwFVec3(xpos, ypos, 1.0f);
		s_VertsGameOver[1] =  CIwFVec3(xpos + x_size, ypos, 1.0f);
		s_VertsGameOver[2] =  CIwFVec3(xpos + x_size, ypos + y_size, 1.0f);
		s_VertsGameOver[3] =  CIwFVec3(xpos, ypos + y_size, 1.0f);

		s_UVGameOver[0] = CIwFVec2(0.0f, 1.0f);
		s_UVGameOver[1] = CIwFVec2(1.0f, 1.0f);
		s_UVGameOver[2] = CIwFVec2(1.0f, 0.0f);
		s_UVGameOver[3] = CIwFVec2(0.0f, 0.0f);



		IwGxSetMaterial(g_MaterialFinalCut);
		IwGxSetVertStreamModelSpace(s_VertsGameOver, 4);
		IwGxSetUVStream(s_UVGameOver);
		IwGxSetColStream(NULL);
		IwGxSetNormStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);




/*		initRender();

		view = CIwFMat::g_Identity;
		

	    view.t.y = 0;
        view.t.x = 0;
		view.t.z =  1152+120;
		view.LookAt(view.GetTrans(), CIwFVec3(0.0f, 0.0f, -100.0f), -CIwFVec3::g_AxisY);

        IwGxSetViewMatrix(&view);

		IwGxLightingOn();

		rot_factorY+=M_PI / 256.0f;
		if (rot_factorY > 2*M_PI) rot_factorY = M_PI * 2 - rot_factorY;

	//	float zoom_factor = 1.3 + 0.2 * sinf(rot_factorY);
*/
		IwGxFlush();

		IwGxLightingOn();
		SetCameraOrtho();

		drawModel(objGameName, Vector3(0 , 204.0f , 10), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 0.75f));





		FadeIn(0.0f, 0xff,0xff,0xff);
		

		IwGxFlush();

        //Render a smaller version of the frame buffer,
        //transforming into a fake HDR version using the bloombuffer shader
        RenderPass(s_Surface[OFFSCREEN_SCENE], s_Surface[BLOOM_BUFFER], s_BloomBufferShader);

        //Blur the bloom buffer to give spread
        const float BLUR_SPREAD_X = 0.009f;
        const float BLUR_SPREAD_Y = BLUR_SPREAD_X * s_Surface[0]->GetClientWidth() / s_Surface[0]->GetClientHeight();

        CIwGxShaderUniform* blurDir = s_BlurBufferShader->GetParam("inBlurStep");
        float dirs[3] = { BLUR_SPREAD_X, 0.0f, BLUR_SPREAD_Y };
        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs);

        RenderPass(s_Surface[BLOOM_BUFFER], s_Surface[BLUR_BUFFER_1], s_BlurBufferShader);
        IwGxFlush(); //need to submit before changing the shader parameter (it is not an instanceParam)

        blurDir->Set(CIwGxShaderUniform::VEC2, 0, dirs+1);
        RenderPass(s_Surface[BLUR_BUFFER_1], s_Surface[BLUR_BUFFER_2], s_BlurBufferShader);

        //Now composite the frame buffer and the bloom buffer using the postprocess shader

        //Select the back buffer as the current surface
        CIwGxSurface::MakeDisplayCurrent();

        //Render the buffers through the shader
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

        CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();


            //Select the original surface and the bloom surface into the material
            pMat->SetTexture(s_Surface[OFFSCREEN_SCENE]->GetTexture());
            pMat->SetTexture(s_Surface[BLUR_BUFFER_2]->GetTexture(), 1);

            pMat->SetShaderTechnique(s_PostProcessShader);

        IwGxSetMaterial(pMat);
        CIwSVec2 xy(0,0);
        CIwSVec2 wh((int16)IwGxGetScreenWidth(), (int16)IwGxGetScreenHeight());
        CIwFVec2 uvxy(0, 0);
        IwGxDrawRectScreenSpace(&xy, &wh, &uvxy, &s_Surface[OFFSCREEN_SCENE]->GetClientUVExtentFloat());

		IwGxFlush();


		wheel_rot+=MENU_WHEEL_SPEED * 1 * float(M_PI) / 720.0;

		if (wheel_rot > 2 * M_PI)
		wheel_rot = wheel_rot - 2 * float(M_PI);

		if (wheel_rot < 0) 
		wheel_rot = 2 * float(M_PI) - wheel_rot;


		g_Input.Update();

		if (g_finalcut_counter > 180 && g_Input.getTouchCount() != 0)
		{
			global_state = GLOBAL_STATE_MAIN_MENU;
			prev_touch = true;
		}
		else
		{
			g_finalcut_counter++;
		}


}


void mainLoop2()									// Here's Where We Do All The Drawing
{

	double dwStartTime = 0;//timeGetTime();

	fDeltaTime = 0.9f;

	//	Iw2DFinishDrawing();
	
/*	int screenWidth = IwGxGetScreenWidth();
	int screenHeight = IwGxGetScreenHeight();

	CIwFMat view = CIwFMat::g_Identity;
	view.t.z = -500;
	view.LookAt(view.GetTrans(), CIwFVec3(0, 0, 0), -CIwFVec3::g_AxisY);

	IwGxSetViewMatrix(&view);

	IwGxSetOrtho(-(screenWidth >> 1), screenWidth >> 1, -(screenHeight >> 1), screenHeight >> 1);
*/
	

		switch (global_state)
		{
		case GLOBAL_STATE_GAME_PLAY:
			globalGamePlay(fDeltaTime);
		break;

		case GLOBAL_STATE_GAME_OVER:
			globalGameOver(fDeltaTime);
		break;

		case GLOBAL_STATE_MAIN_MENU:
			globalMainMenu(fDeltaTime);
		break;

		case GLOBAL_STATE_NEXT_LEVEL:
			globalNextLevel(fDeltaTime);
			break;

		case GLOBAL_STATE_SHOP:
			globalShop();
		break;

		case GLOBAL_STATE_FINALCUT:
			drawFinalCut(fDeltaTime);
		break;


		}

#ifdef _DEBUG

/*
		IwGxFontSetRect(CIwRect(310, 210, 900, 600));
		switch (global_state)
		{
		case GLOBAL_STATE_GAME_PLAY:
		IwGxFontDrawText("Global: GAME_PLAY");
		break;

		case GLOBAL_STATE_GAME_OVER:
		IwGxFontDrawText("Global: GAME_OVER");
		break;

		case GLOBAL_STATE_MAIN_MENU:
		IwGxFontDrawText("Global: MAIN_MENU");
		break;

		case GLOBAL_STATE_NEXT_LEVEL:
		IwGxFontDrawText("Global: NEXT_LEVEL");
		break;

		}
*/
#endif


		double dwCurrTime = 0;//timeGetTime();
			
		fDeltaTime =  float(dwCurrTime - dwStartTime);

		fDeltaTime = (1000.0f / FRAMES_PER_SECOND) / fDeltaTime;

		fDeltaTime = 1.0f / fDeltaTime;


//		g_pFlurryAppSpot->PreLoadFullScreenAd();

//		printGl(200, 240,"FPS: ", 1000.0f / (dwCurrTime - dwStartTime));


}
#include "s3e.h"

bool g_bShowBanner = false;

static int32 onAdMobDismiss(void* systemData, void* userData)
{
	g_bShowBanner = false;

    return 0;
}

static int32 onAdMobWillDismiss(void* systemData, void* userData)
{
    
    return 0;
}

static int32 onAdMobPresent(void* systemData, void* userData)
{
    g_bShowBanner = true;

    return 0;
}

static int32 onAdMobRecieved(void* systemData, void* userData)
{
    
    return 0;
}

static int32 onAdMobFailed(void* systemData, void* userData)
{
    
    return 0;
}

static int32 onAdLeaveApp(void* systemData, void* userData)
{
    
    return 0;
}  



void initInetStuff()
{
	/////////////////////////////////// setup social
	g_pSocial->Init();
	g_pSocial->SetGameName("Lift Rich");
	g_pSocial->SetURL("http://www.liftrich.com");
	g_pSocial->SetFBText("#LIftRich");


	//////////////////////////////// setup analytics
//	s3eFlurryStartSession(FLURRY_IOS_APP_KEY);
//	s3eFlurryLogEventTimed("Game started...");

	//// init GameCenter
	g_pScore->Init();


	int y_banner_size = int(g_ScreenHeight / 2 * 0.20f);

	g_BannerSize = y_banner_size * 2;


	SetGoogleAppKey(AdmobTopBanner, AdmobInterstitalBanner);
	IsLandscape(true);
	BannerAdPosition(CG_ADMOB_POSITION_TOP);

	CGAdMobRegister(CG_ADMOB_CALLBACK_INTERSTITIALWILLDISMISS, onAdMobWillDismiss, NULL);
    CGAdMobRegister(CG_ADMOB_CALLBACK_INTERSTITIALWILLPRESENT, onAdMobPresent, NULL);
    CGAdMobRegister(CG_ADMOB_CALLBACK_INTERSTITIALRECEIVED, onAdMobRecieved, NULL);
    CGAdMobRegister(CG_ADMOB_CALLBACK_INTERSTITIALFAILED, onAdMobFailed, NULL);
    CGAdMobRegister(CG_ADMOB_CALLBACK_INTERSTITIALDISMISS, onAdMobDismiss, NULL);
    CGAdMobRegister(CG_ADMOB_CALLBACK_INTERSTITIALLEAVEAPP, onAdLeaveApp, NULL);

	InitAdView();

	g_bShowBanner = false;


	 s3eIOSAppStoreBillingStart(ProductInfoCallback, TransactionUpdateCallback, NULL);

//	IwBilling::Init(ShopReadyCallback, (void*)g_AndroidPublicKey, IwBilling::BILLING_VENDOR_GOOGLE_PLAY);
//	IwBilling::setErrorCallback(ShopErrorCallback, 0);
//  IwBilling::setReceiptAvailableCallback(ShopBuyCallback, 0);


}
 

void ClearTextData()
{
	for (std::list<CDrawSymbol*>::iterator it = DrawTextList.begin(); it !=  DrawTextList.end(); it++)
		delete *it;
	
	DrawTextList.clear();
}




int main()
{



	initSystem();
	initInetStuff();
	initRender();

	initButtons();
	initIcons();
	loadObjects();

	loadConfig();

	///////////////////////////////// load config

	////////////////////////////////// load music track
	s3eFile * FileHandle = s3eFileOpen("sounds/main_music.mp3", "rb");
    g_AudioSize = s3eFileGetSize(FileHandle);
	g_AudioData = s3eMallocBase(g_AudioSize);
    s3eFileRead(g_AudioData, g_AudioSize, 1, FileHandle);
    s3eFileClose(FileHandle);
	if (g_AudioStatus)
	{
		g_pAudio->PlayMusicFromBuffer(g_AudioData, g_AudioSize);
	}

	////// setup Level Manager

	g_LevelManager->AddModel(objMoneyBag, 1);      /// model_id=1
	g_LevelManager->AddModel(objDollar_pack01, 2); /// model id=2
	g_LevelManager->AddModel(objDollar_pack02, 3); /// model id=3
	g_LevelManager->AddModel(objDollar_pack03, 4); /// model id=4
	g_LevelManager->AddModel(objDollar_pack04, 5); /// model id=5
	g_LevelManager->AddModel(objBaseball_glove, 6); /// model id=6
	g_LevelManager->AddModel(objAntique_box, 7);    /// model id=7
	g_LevelManager->AddModel(objGira, 8);           /// model id=8
	g_LevelManager->AddModel(objBrick, 9);          /// model id=9
	g_LevelManager->AddModel(objCoin_pack01, 10);    /// model id=10
	g_LevelManager->AddModel(objCoin_pack02, 11);    /// model id=11
	g_LevelManager->AddModel(objCoin_pack03, 12);    /// model id=12
	g_LevelManager->AddModel(objGold_block, 13);     /// model id=13
	g_LevelManager->AddModel(objEgg, 14);            /// model id=14
	g_LevelManager->AddModel(objGoldphone, 15);      /// model id=15
	g_LevelManager->AddModel(objTvSet, 16);      /// model id=15
	g_LevelManager->AddModel(objMetall_plate, 17);      /// model id=15
	g_LevelManager->AddModel(objGem, 18);      /// model id=15

	g_LevelManager->AddMaterial((CIwMaterial*)objMoneyBag->m_Materials[0], 1); // mat id=1
	g_LevelManager->AddMaterial((CIwMaterial*)objCoin_pack01->m_Materials[0], 2); // mat id=2
	g_LevelManager->AddMaterial((CIwMaterial*)objGold_block->m_Materials[0], 3); // mat id=3
	g_LevelManager->AddMaterial((CIwMaterial*)objEgg->m_Materials[0], 4); // mat id=4
	g_LevelManager->AddMaterial((CIwMaterial*)objGoldphone->m_Materials[0], 5); // mat id=5
	g_LevelManager->AddMaterial((CIwMaterial*)objGem->m_Materials[0], 6); // mat id=5

	///// Level Manager setup end



	////////////////////////////////////////////
	//// load levels

	for (int i=1; i<51; i++)
	{

		char level_name[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		char number[6] = {0,0,0,0,0,0};
	
		if (i > 9) strcpy(level_name, "levels/level");
		else strcpy(level_name, "levels/level0");

		snprintf(number , sizeof(number), "%d", i);
		strncat(level_name, number, sizeof(number));
		strncat(level_name, ".txt", 4);

		g_LevelManager->LoadLevel(level_name,i); 

	}

	////////////// load levels end


	    s_DrawSurface = SURFACE_COUNT; //start from full composited output

        for (uint32 i = 0; i < SURFACE_COUNT; i++)
        {
            s_Surface[i] = new CIwGxSurface();
            if (i)
				s_Surface[i]->CreateSurface(NULL, g_ScreenWidth/BLOOM_DIM, g_ScreenHeight/BLOOM_DIM, CIwGxSurface::EXACT_MATCH_F);
            else
				s_Surface[i]->CreateSurface(NULL, g_ScreenWidth, g_ScreenHeight, CIwGxSurface::EXACT_MATCH_F);
        }

        IwGxRegister(IW_GX_SCREENSIZE, ResizeSurfaces);



	global_state = GLOBAL_STATE_MAIN_MENU;

	//	g_CurrentLevel = 15;
	///		g_CurrentLevel = 23;
//	g_CurrentLevel = 31;
//	g_CurrentLevel = 40;

//	g_CurrentLevel = 1;

	//	global_state = GLOBAL_STATE_FINALCUT;

	initDollars();

	g_ParticleManager->SetParticleMaterial(g_MaterialParticle);
	g_ParticleManager->AddParticleSystem(0.0f, 0.0f, 0.0f, 160.0, 164, 0, 16); 
	g_ParticleManager->AddParticleSystem(0.0f, 0.0f, 0.0f, 160.0, 164, 1, 16); 

	srand ( time(NULL) ); 

	while (!s3eDeviceCheckQuitRequest())
    {

		s3eDeviceYield(0);

		SetColorTheme();

		IwGxSetColClear(BACKGRND_COLOR[bgr_offset].x, BACKGRND_COLOR[bgr_offset].y, BACKGRND_COLOR[bgr_offset].z, 0x00);
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

		g_pAudio->Update();
		ClearTextData();

		if (!g_bShowBanner)
		{
			mainLoop2();
			IwGxFlush();
			IwGxSwapBuffers();
		}

		if (g_CompletedPayment)
		{
			s3eIOSAppStoreBillingCompleteTransaction(g_CompletedPayment, S3E_TRUE);
			g_CompletedPayment = NULL;
		}

		//		IwGxPrintFrameRate(10, 100);

    }


	g_pAudio->StopMusic();
//	g_pAudio->StopAllSounds();
	g_LevelManager->Release();
	
	delete g_LevelManager;
	delete g_ParticleManager;
	delete g_pAudio;
	delete g_pScore;
	delete g_pSocial;


	delete g_MaterialFadeOut;
	delete g_MaterialFadeIn;
	delete g_MaterialGameOver;
	delete g_MaterialGameUI;
	delete g_MaterialHelpWin;
	delete g_MaterialTimeBar;
	delete g_MaterialParticle;
	delete g_MaterialShop;
	delete g_MaterialMotivator;
	delete g_MaterialDeMotivator;
	delete g_MaterialFinalCut;

	delete g_TextureFadeOut;
	delete g_TextureFadeIn;
	delete g_TextureGameOver;
	delete g_TextureGameUI;
	delete g_TextureHelpWin;
	delete g_TextureTimeBar;	
	delete g_TextureParticle;
	delete g_TextureShop;
	delete g_TextureMotivator; 
	delete g_TextureDeMotivator;
	delete g_TextureFinalCut; 

	for (uint32 i = 0; i < SURFACE_COUNT; i++)
    {
        delete s_Surface[i];
    }

	ClearTextData();

	IwGetResManager()->DestroyGroup("Common");

	

//	IwBilling::Terminate();
	
	if (g_ProductInformation)
        s3eFree(g_ProductInformation);

    if (g_TransactionReceipt.m_ReceiptData)
        s3eFree(g_TransactionReceipt.m_ReceiptData);

    if (g_PaymentTransaction)
        s3eFree(g_PaymentTransaction);

    s3eIOSAppStoreBillingTerminate();
	
	IwResManagerTerminate();
//	IwGxFontTerminate();
    IwGraphicsTerminate();
	IwGxTerminate();

	Release();

	return 0;
}

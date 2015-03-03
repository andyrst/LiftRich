#include "s3e.h"
//#include "s3eFlurry.h"
#include "s3eFlurryAppSpot.h"
#include <cstdlib>

#include "flurryappspot.h"


//const char FullScreenBanner[] = "BANNER_FULLSCREEN";
const char TopScreenBanner[] = "Marmalade_Test_AdSpace_BannerTop";
const char FullScreenBanner[] = "Marmalade_Test_AdSpace";
FlurryAppSpot::FlurryAppSpot()
{

}

FlurryAppSpot::~FlurryAppSpot()
{
}



void FlurryAppSpot::PreLoadFullScreenAd()
{

//	if (s3eFlurryAppSpotAdReadyForSpace(FullScreenBanner) ==  S3E_FALSE) 
//		s3eFlurryAppSpotFetchAdForSpace(FullScreenBanner, S3E_FLURRYAPPSPOT_FULLSCREEN);

}


void FlurryAppSpot::ShowFullscreenBanner()
{


	if (s3eFlurryAppSpotAdReadyForSpace(FullScreenBanner) ==  S3E_TRUE)
	{
//		s3eFlurryAppSpotFetchAndDisplayAdForSpace(FullScreenBanner, S3E_FLURRYAPPSPOT_FULLSCREEN);
		s3eFlurryAppSpotDisplayAdForSpace(FullScreenBanner);
	}
		s3eFlurryAppSpotFetchAdForSpace(FullScreenBanner, S3E_FLURRYAPPSPOT_FULLSCREEN);
}

void FlurryAppSpot::ShowBottomBanner()
{
	if (s3eFlurryAppSpotFetchAndDisplayAdForSpace("BANNER_BOTTOM", S3E_FLURRYAPPSPOT_BANNER_BOTTOM) != S3E_RESULT_SUCCESS)
	{
//		s3eFlurryLogError("error", "FlurryAppSpot: BANNER_BOTTOM Show Failed!");
	}

	s3eFlurryAppSpotFetchAdForSpace(TopScreenBanner, S3E_FLURRYAPPSPOT_FULLSCREEN);
}


void FlurryAppSpot::ShowTopBanner(int width)
{
	s3eFlurryAppSpotSetDisplayAdView(0, 1, 320, 50);
	    s3eFlurryAppSpotFetchAndDisplayAdForSpace(TopScreenBanner, S3E_FLURRYAPPSPOT_BANNER_TOP);
	//	if (s3eFlurryAppSpotAdReadyForSpace(TopScreenBanner) ==  S3E_TRUE)
//	{
//		s3eFlurryAppSpotSetDisplayAdView(0, 1, 320, 50);
//		s3eFlurryAppSpotDisplayAdForSpace(TopScreenBanner);
//	}

//	s3eFlurryAppSpotFetchAdForSpace(TopScreenBanner, S3E_FLURRYAPPSPOT_BANNER_TOP);
}

void FlurryAppSpot::RemoveTopBanner()
{
	s3eFlurryAppSpotRemoveAdFromSpace(TopScreenBanner);
}

void FlurryAppSpot::Init(const char *ApiKey)
{
	s3eFlurryAppSpotInitialize(ApiKey);

	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, (s3eCallback)adClosed_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, (s3eCallback)adSpaceReceived_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_APPLICATION_EXIT, (s3eCallback)applicationExit_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, (s3eCallback)adSpaceReceivedFail_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, (s3eCallback)adClicked_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADOPENED, (s3eCallback)adOpened_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, (s3eCallback)adVideoCompleted_Callback, NULL);
    s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, (s3eCallback)adRenderFail_Callback, NULL);

}


void FlurryAppSpot::Terminate()
{
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, (s3eCallback)adClosed_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, (s3eCallback)adSpaceReceived_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_APPLICATION_EXIT, (s3eCallback)applicationExit_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, (s3eCallback)adSpaceReceivedFail_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, (s3eCallback)adClicked_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADOPENED, (s3eCallback)adOpened_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, (s3eCallback)adVideoCompleted_Callback);
    s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, (s3eCallback)adRenderFail_Callback);
}

int32 FlurryAppSpot::adClosed_Callback(void* system, void* user)
{
//	s3eFlurryLogEvent("FlurryAppSpot: Ad Closed");
   return 0;
}

int32 FlurryAppSpot::adSpaceReceived_Callback(void* system, void* user)
{
    return 0;
}

int32 FlurryAppSpot::applicationExit_Callback(void* system, void* user)
{
    return 0;
}

int32 FlurryAppSpot::adSpaceReceivedFail_Callback(void* system, void* user)
{
//	s3eFlurryLogError("error", "FlurryAppSpot: Ad Recieve Failed!");
	return 0;
}

int32 FlurryAppSpot::adClicked_Callback(void* system, void* user)
{
//	s3eFlurryLogEvent("FlurryAppSpot: Ad Clicked");
    return 0;
}

int32 FlurryAppSpot::adOpened_Callback(void* system, void* user)
{
    return 0;
}

int32 FlurryAppSpot::adVideoCompleted_Callback(void* system, void* user)
{
    return 0;
}

int32 FlurryAppSpot::adRenderFail_Callback(void* system, void* user)
{
//	s3eFlurryLogError("error", "FlurryAppSpot: Ad Render Failed!");
    return 0;
}
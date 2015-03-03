#ifndef FLURRYAPPSPOT_H
#define FLURRYAPPSPOT_H

#include "s3e.h"

class FlurryAppSpot
{
public:
	FlurryAppSpot();
	~FlurryAppSpot();

	void Init(const char *ApiKey);
	void Terminate();
	void ShowTopBanner(int width);
	void ShowBottomBanner();
	void ShowFullscreenBanner();
	void Stop();
	void PreLoadFullScreenAd();
	void RemoveTopBanner();

private:

	static int32 adClosed_Callback(void* system, void* user);
	static int32 adSpaceReceived_Callback(void* system, void* user);
	static int32 applicationExit_Callback(void* system, void* user);
	static int32 adSpaceReceivedFail_Callback(void* system, void* user);	
	static int32 adClicked_Callback(void* system, void* user);
	static int32 adOpened_Callback(void* system, void* user);
	static int32 adVideoCompleted_Callback(void* system, void* user);
	static int32 adRenderFail_Callback(void* system, void* user);


};

#endif
#ifndef ADMOB_AD_H
#define ADMOB_AD_H

#include "s3e.h"

class AdmobAd
{
public:
	AdmobAd();
	~AdmobAd();

	void InitSmartBanner(const char *smart_id, int orientation, int x, int y, int width, int height);
	void HideBanner();
	void ShowBanner();


private:


};

#endif
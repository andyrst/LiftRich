#ifndef SOCIAL_H
#define SOCIAL_H


#define MAX_FACEBOOK_TEXT_LENGHT 1024
#define MAX_TWITTER_TEXT_LENGHT 140

#include "s3e.h"
#include "s3eFacebook.h"

class Social
{
public:
	Social();
	~Social();

	void Init();
	void IntitFBS3e();
	void SetURL(const char *text);
	void SetGameName(const char *text);

	void SetFBText(const char *text);
	void PostFBMessage();

	void SetTwitterText(const char *text);
	void PostTwitterMessage(); 


private:

	static void FBLoginCallback(struct s3eFBSession* session, s3eResult* loginResult, void* userData);
	static void FBStreamDialogCallback(struct s3eFBDialog* dialog, s3eResult* streamResult, void* userData);
	void WaitForFBCallback();

	char FBText[MAX_FACEBOOK_TEXT_LENGHT];
	char TWText[MAX_TWITTER_TEXT_LENGHT];
	char URL[256];
	char GameName[64];

};

#endif
/// include Facebook and Twitter intergration

#include "social.h"

#include "IwDebug.h"
#include <string>
#include <vector>

#include "s3eOSReadString.h"
#include "s3e.h"
#include "s3eFacebook.h"

#include "s3eDialog.h"
//#include "s3eFlurry.h"

const char* FACEBOOK_APP_ID_IOS = "15165043xxxxxx";
const char* FACEBOOK_APP_SECRET_IOS = "980xxxxxxxxxxxxxxxxxx";

bool g_NativeFBEnable = false;
s3eFBSession* g_CurrentFBSession = 0;
s3eFBSLComposeViewController* g_SLComposeViewController = 0;
bool g_WaitingForFBCallback = false;


Social::Social()
{

	memset(FBText, 0, MAX_FACEBOOK_TEXT_LENGHT);
	memset(TWText, 0, MAX_TWITTER_TEXT_LENGHT);
	memset(URL, 0, 256);
	memset(GameName, 0, 64);
}

Social::~Social()
{
	if (g_CurrentFBSession)
    {
		s3eFBTerminate(g_CurrentFBSession);
	}
}

void Social::Init()
{
	// init Facebook stuff
#ifdef BUILD_FOR_IOS
	{

	if (s3eFBSLComposeViewController_IsAvailable())
	{
		g_NativeFBEnable = true;
	}
	else
	{
//		s3eFlurryLogError("error", "FaceBook: No Account!");
	}

	}
#endif // IOS_BUILD


#ifdef BUILD_FOR_ANDROID
	{
		/// insert for android code there
	}
#endif


}

void Social::IntitFBS3e()
{
		if (!g_CurrentFBSession)
                g_CurrentFBSession = s3eFBInit(FACEBOOK_APP_ID_IOS);

		if (g_CurrentFBSession)
            {
  
                std::vector<const char*> read_permissions;
                read_permissions.push_back("read_stream");
                read_permissions.push_back("user_about_me");

                std::vector<const char*> read_write_permissions;
                read_write_permissions.push_back("read_stream");
                read_write_permissions.push_back("publish_stream");

                std::vector<const char*>& permissions = read_write_permissions;

                s3eFBSessionLoginBehaviour loginBehaviour = s3eFBSessionBehaviourWithFallbackToWebView;

                if (s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE || s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WP8)
                {
                    permissions = read_permissions;
                    int32 iOS_version_major = (s3eDeviceGetInt(S3E_DEVICE_OS_VERSION) >> 16) & 0xffff;

					if (iOS_version_major >= 6)
                    {
                        loginBehaviour = s3eFBSessionBehaviourUseSystemAccountIfPresent;
                    }
                }

				s3eFBSession_Login(g_CurrentFBSession, FBLoginCallback, NULL, &permissions[0], permissions.size(), loginBehaviour);
		}

	// init Twitter stuff

}


void Social::FBLoginCallback(struct s3eFBSession* session, s3eResult* loginResult, void* userData)
{
 //   IwTrace(FACEBOOK, ("FBLoginCallback %p %p %p", session, loginResult, userData));

    if (*loginResult == S3E_RESULT_SUCCESS)
    {
   //     m_SessionFBStatus = FB_LOGGED_IN;
    //    sprintf(g_StatusMessage, "`x66ee66Facebook Log-In succeeded");
    }
    else
    {
 //       g_SessionStatus = FB_LOGGED_OUT;
  //      sprintf(g_StatusMessage, "`xee6666Facebook Log-In failed");
	//	exit (0);
    }

 //   UpdateState();

//    g_WaitingForCallback = false;

	g_WaitingForFBCallback = false;
}


static void FBSLComposeViewControllerCallback(struct s3eFBSLComposeViewController* cvc, s3eResult* result, void*)
{

    s3eFBSLComposeViewController_Delete(g_SLComposeViewController);
    g_SLComposeViewController = NULL;

}

void Social::PostFBMessage()
{
#ifdef BUILD_FOR_IOS
	{	
		// post message with native iOS api
		if (g_NativeFBEnable && g_SLComposeViewController == NULL)
		{
			g_SLComposeViewController = s3eFBSLComposeViewController_Create();

		//	s3eFBSLComposeViewController_AddURL(g_SLComposeViewController, URL);
			s3eFBSLComposeViewController_SetInitialText(g_SLComposeViewController, FBText);
		//	s3eFBSLComposeViewController_AddImageWithFilePath(g_SLComposeViewController, "../data-ram/screenshot.jpg");

			s3eFBSLComposeViewController_Show(g_SLComposeViewController, FBSLComposeViewControllerCallback);
		}
		else 
		{
			s3eDialogAlertInfo info;
			info.m_Message = "There are no Facebook accounts\nconfigured. You can add or create \n a Facebook account in Settings.";
			info.m_Title = "No Facebook Account";
			info.m_Button[0] = "Ok";
			s3eDialogAlertBox(&info);

		}
	}
#endif


	// post message with s3e api
	if (!g_NativeFBEnable)
	{
			return; 
			IntitFBS3e();

		    s3eFBDialog* pDialog = s3eFBDialog_WithAction(g_CurrentFBSession, "stream.publish");
            s3eFBDialog_AddParamString(pDialog, "api_key", FACEBOOK_APP_SECRET_IOS);
			s3eFBDialog_AddParamString(pDialog, "access_token", s3eFBSession_AccessToken(g_CurrentFBSession));
			s3eFBDialog_AddParamString(pDialog, "name", GameName);
			s3eFBDialog_AddParamString(pDialog, "link", URL);
			s3eFBDialog_AddParamString(pDialog, "description", FBText);
			s3eFBDialog_AddParamString(pDialog, "text",  "some message");
//			s3eFBDialog_AddParamString(pDialog, "caption",  "CAPTION");


 
            // Show the dialog
            s3eFBDialog_Show(pDialog, FBStreamDialogCallback);

            // Wait until dialog has completed
            WaitForFBCallback();

            // Destroy the dialog
            s3eFBDialog_Delete(pDialog);
	}
 
}



void Social::FBStreamDialogCallback(struct s3eFBDialog* dialog, s3eResult* streamResult, void* userData)
{
	g_WaitingForFBCallback = false;
}

void Social::WaitForFBCallback()
{
    g_WaitingForFBCallback = true;

    while (g_WaitingForFBCallback && !s3eDeviceCheckQuitRequest())
    {
        s3eDeviceYield(100);
    }
}


void Social::SetFBText(const char *text)
{
	strcpy(FBText, text);
}

void Social::SetTwitterText(const char *text)
{

}

void Social::SetURL(const char *text)
{
	strcpy(URL, text);
}

void Social::SetGameName(const char *text)
{
	strcpy(GameName, text);
}

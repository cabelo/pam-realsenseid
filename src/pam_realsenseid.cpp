#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include "RealSenseID/FaceAuthenticator.h"
#include <iostream>
#include <stdio.h>

class MyAuthClbk : public RealSenseID::AuthenticationCallback
{
public:
    int result;
    void OnResult(const RealSenseID::AuthenticateStatus status, const char* user_id) override
    {
        this->result = 0;
        if (status == RealSenseID::AuthenticateStatus::Success)
        {
            std::cout << "Authenticated " << user_id << std::endl;
            this->result = 1;
        }
    }

    void OnHint(const RealSenseID::AuthenticateStatus hint) override
    {
        std::cout << "OnHint " << hint << std::endl;
    }

    void OnFaceDetected(const std::vector<RealSenseID::FaceRect>& faces, const unsigned int ts) override
    {
        for (auto& face : faces)
        {
            printf("** Detected face %u,%u %ux%u (timestamp %u)\n", face.x, face.y, face.w, face.h, ts);
        }
    }
};



PAM_EXTERN int pam_sm_setcred (pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt (pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate (pam_handle_t *pamh, int flags,int argc, const char **argv) {
    char *password = NULL;
    const char *username;
    const char *rhost = NULL;
    int r;

    r = pam_get_user(pamh, &username, NULL);
    if (r != PAM_SUCCESS)
        return PAM_AUTHINFO_UNAVAIL;

    RealSenseID::FaceAuthenticator authenticator;
    auto status = authenticator.Connect({"/dev/ttyACM0"});
    if (status != RealSenseID::Status::Ok)
    {
        std::cout << "Failed connecting with status " << status << std::endl;
        return PAM_SUCCESS;
	//return -1;
    }

    //system("cat /tmp/oiti ");
    MyAuthClbk auth_clbk;
    authenticator.Authenticate(auth_clbk);
    if(auth_clbk.result == 1)
    {
        //std::cout << "Authorized : " << auth_clbk.result << std::endl;
        std::cout << "Root Authorized by Intel RealsenseID !"  << std::endl;
	system("cat /tmp/cabelo ");
        //printf("Pilot BioPam: %s\n",username);
         return PAM_SUCCESS;
    }
    else
    {
    
    }

    return -1;
}

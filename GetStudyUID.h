//---------------------------------------------------------------------------

#ifndef GetStudyUIDH
#define GetStudyUIDH
//---------------------------------------------------------------------------

#define SITE_UID_ROOT                           "1.2.276.0.7230010.3"
#define SITE_STUDY_UID_ROOT                     SITE_UID_ROOT ".1.2"
#define SITE_SERIES_UID_ROOT                    SITE_UID_ROOT ".1.3"
#define SITE_INSTANCE_UID_ROOT                  SITE_UID_ROOT ".1.4"



char* dcmGenerateUniqueIdentifier(char* uid, const char* prefix);

#endif

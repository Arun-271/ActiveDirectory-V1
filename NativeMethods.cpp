#include <jni.h>
#include "NativeMethods.h"
#include <system_error>
#include <iostream>
#include "activeds.h"
#include "oaidl.h"
#include "iads.h"
#include "atlbase.h"
#include <atlstr.h>
#include <atlsafe.h>
// Method declaration.
int CreateUserFromADs(LPCWSTR pwszFirstName, LPCWSTR pwszLastName, LPCWSTR pwszSAMAccountName, LPCWSTR pwszInitialPassword,
    LPCWSTR pwszDescrpiton, LPCWSTR email, LPCWSTR street, LPCWSTR city, LPCWSTR state, LPCWSTR country, LPCWSTR postalCode,
    LPCWSTR telephone, LPCWSTR mobile, LPCWSTR jobTitle, LPCWSTR department, LPCWSTR company, LPCWSTR manager
);
int ModifyUserDetailsFromADs(LPCWSTR pwszAccountName, LPCWSTR pwszFirstName, LPCWSTR pwszLastName);
int DeleteUserFromADs(LPCWSTR pwszAccountName);
int ResetPassWord(LPCWSTR pwszAccountName, LPCWSTR pwszNewPassword);
wchar_t* getManagerName(LPCWSTR userpath);
//Convert jstring to LPWSTR string
LPWSTR ConvertToLPWSTR(const std::string& s)
{
    LPWSTR ws = new wchar_t[s.size() + 1]; // +1 for zero at the end
    copy(s.begin(), s.end(), ws);
    ws[s.size()] = 0; // zero at the end
    return ws;
}
LPCWSTR jstring2string(JNIEnv* env, jstring jStr) {
    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char*)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    LPCWSTR converted = ConvertToLPWSTR(ret);
    return converted;
}
LPWSTR convert(LPCWSTR s) {
    wchar_t* att2 = (wchar_t*)s;
    LPWSTR att3 = att2;
    return att3;
}
int ResetPassWord(LPCWSTR pwszAccountName, LPCWSTR pwszNewPassword) {
    HRESULT hr;
    CoInitialize(NULL);
    IADsContainer* pUsers = NULL;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += pwszAccountName;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsContainer,
        (void**)&pUsers);

    if (SUCCEEDED(hr))
    {
        IADsUser* pDisp = NULL;
        hr = pUsers->QueryInterface(IID_IADsUser, (void**)&pDisp);
        if (SUCCEEDED(hr)) {
            hr = pDisp->SetPassword((BSTR)pwszNewPassword);
            CoUninitialize();
            std::string message = std::system_category().message(hr);
            std::cout << message << std::endl;
            return hr;
        }
        std::string message = std::system_category().message(hr);
        std::cout << message << std::endl;
        return hr;
    }
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    return hr;
}
int DeleteUserFromADs(LPCWSTR pwszAccountName) {
    IADsContainer* pCont;
    CoInitialize(NULL);
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += pwszAccountName;
    path += L",CN=Users,DC=Test,DC=local";
    HRESULT hr = hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsContainer,
        (void**)&pCont);
    if (FAILED(hr)) exit(hr);

    IADsDeleteOps* pOps;
    hr = pCont->QueryInterface(IID_IADsDeleteOps, (void**)&pOps);
    pCont->Release();
    if (FAILED(hr)) { exit(hr); }
    pOps->DeleteObject(0);
    pOps->Release();
    CoUninitialize();
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    return hr;
}
int ModifyUserDetailsFromADs(LPCWSTR pwszAccountName, LPCWSTR pwszFirstName, LPCWSTR pwszLastName) {
    HRESULT hr;
    CoInitialize(NULL);
    IADs* pADs = NULL;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += pwszAccountName;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADs,
        (void**)&pADs);
    if (!SUCCEEDED(hr))
    {
        return hr;
    }

    VARIANT var;

    // Using Put with a single value for the first name
    VariantInit(&var);
    V_BSTR(&var) = SysAllocString((BSTR)pwszFirstName);
    V_VT(&var) = VT_BSTR;
    hr = pADs->Put((BSTR)L"givenName", var);

    // Using Put with a single value for the last name
    VariantClear(&var);
    V_BSTR(&var) = SysAllocString(pwszLastName);
    V_VT(&var) = VT_BSTR;
    hr = pADs->Put((BSTR)L"sn", var);
    VariantClear(&var);

    // Using Put with multiple values for other telephones
 /*   LPWSTR pszPhones[] = { L"425 844 1234", L"425 924 4321" };
    DWORD dwNumber = sizeof(pszPhones) / sizeof(LPWSTR);

    hr = ADsBuildVarArrayStr(pszPhones, dwNumber, &var);
    hr = pADs->Put(L"otherTelephone", var);
    VariantClear(&var);*/

    hr = pADs->SetInfo();
    pADs->Release();

    if (!SUCCEEDED(hr))
    {
        std::string message = std::system_category().message(hr);
        std::cout << message << std::endl;
        return hr;
    }

    CoUninitialize();
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    return hr;
}
int CreateUserFromADs(LPCWSTR pwszFirstName, LPCWSTR pwszLastName, LPCWSTR pwszSAMAccountName, LPCWSTR pwszInitialPassword,
    LPCWSTR pwszDescrpiton , LPCWSTR email , LPCWSTR street , LPCWSTR city ,LPCWSTR state, LPCWSTR country, LPCWSTR postalCode,
    LPCWSTR telephone, LPCWSTR mobile, LPCWSTR jobTitle , LPCWSTR department, LPCWSTR company , LPCWSTR manager
    )
{
    HRESULT hr;
    CoInitialize(NULL);
    IADsContainer* pUsers = NULL;
    // std::cout << "Intially pUsers : " << pUsers << std::endl;
    hr = ADsOpenObject(L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=Users,DC=Test,DC=local", L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsContainer,
        (void**)&pUsers);
    // Here pUser has some values now.
   // std::cout << "After :" << pUsers << std::endl;
    if (SUCCEEDED(hr))
    {
        IDispatch* __stdcall pDisp = NULL;
        //  std::cout << "Initially pdisp:" << pDisp << std::endl;
        CComBSTR sbstrName = "CN=";
        sbstrName += pwszFirstName;
        // Create the new object in the User folder.
        hr = pUsers->Create(CComBSTR("user"), sbstrName, &pDisp);
        // std::cout << "After pdisp:" << pDisp << std::endl;
        if (SUCCEEDED(hr))
        {
            IADsUser* padsUser = NULL;
            hr = pDisp->QueryInterface(IID_IADsUser, (void**)&padsUser);
            std::cout << "After padsUser:" << padsUser << std::endl;
            if (SUCCEEDED(hr))
            {
                CComBSTR sbstrProp;
                CComVariant svar;
                //Pre windows 2000 
                svar = pwszSAMAccountName;
                sbstrProp = "sAMAccountName";
                hr = padsUser->Put(sbstrProp, svar);
                hr = padsUser->SetInfo();
                // newer syntax
                sbstrProp = "userPrincipalName";
                hr = padsUser->Put(sbstrProp, svar);
                hr = padsUser->SetInfo();
                //This one sets the firstname.
                hr = padsUser->Put(CComBSTR("givenName"), (CComVariant)pwszFirstName);
                hr = padsUser->SetInfo();
                // Display name
                hr = padsUser->Put(CComBSTR("displayName"), (CComVariant)pwszFirstName);
                hr = padsUser->SetInfo();
                // This one sets the last name here.
                hr = padsUser->Put(CComBSTR("sn"), (CComVariant)pwszLastName);
                hr = padsUser->SetInfo();
                // Descrpition
                hr = padsUser->Put(CComBSTR("description"), (CComVariant)pwszDescrpiton);
                hr = padsUser->SetInfo();
                // Email
                hr = padsUser->Put(CComBSTR("mail"), (CComVariant)email);
                hr = padsUser->SetInfo();
                // Street
                hr = padsUser->Put(CComBSTR("streetAddress"), (CComVariant)street);
                hr = padsUser->SetInfo();
                // city
                hr = padsUser->Put(CComBSTR("l"), (CComVariant)city);
                hr = padsUser->SetInfo();
                // state 
                hr = padsUser->Put(CComBSTR("st"), (CComVariant)state);
                hr = padsUser->SetInfo();
                // country
                hr = padsUser->Put(CComBSTR("co"), (CComVariant)country);
                hr = padsUser->SetInfo();
                // postalcode
                hr = padsUser->Put(CComBSTR("postalCode"), (CComVariant)postalCode);
                hr = padsUser->SetInfo();
                // telephone
                hr = padsUser->Put(CComBSTR("homePhone"), (CComVariant)telephone);
                hr = padsUser->SetInfo();
                // mobile
                hr = padsUser->Put(CComBSTR("mobile"), (CComVariant)mobile);
                hr = padsUser->SetInfo();
                // jobtitle
                hr = padsUser->Put(CComBSTR("title"), (CComVariant)jobTitle);
                hr = padsUser->SetInfo();
                // department
                hr = padsUser->Put(CComBSTR("department"), (CComVariant)department);
                hr = padsUser->SetInfo();
                // company
                hr = padsUser->Put(CComBSTR("company"), (CComVariant)company);
                hr = padsUser->SetInfo();
                // manager
                CComBSTR managerPath = L"CN=";
                managerPath += manager;
                managerPath += L",CN=Users,DC=Test,DC=local";
                hr = padsUser->Put(CComBSTR("manager"), (CComVariant)managerPath);
                hr = padsUser->SetInfo();
                // Set Passoword here.
                hr = padsUser->SetPassword(CComBSTR(pwszInitialPassword));
                svar.Clear();
                sbstrProp = "userAccountControl";
                hr = padsUser->Get(sbstrProp, &svar);
                if (SUCCEEDED(hr))
                {
                    svar = svar.lVal & ~(ADS_UF_ACCOUNTDISABLE |
                        ADS_UF_PASSWD_NOTREQD |
                        ADS_UF_DONT_EXPIRE_PASSWD);
                    hr = padsUser->Put(sbstrProp, svar);
                    hr = padsUser->SetInfo();
                }
                hr = padsUser->put_AccountDisabled(VARIANT_FALSE);
                hr = padsUser->SetInfo();
                padsUser->Release();
            }
            pDisp->Release();
        }
        pUsers->Release();
    }
    CoUninitialize();
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    return hr;
}
// Method defnitionss
//JNI Method calls
JNIEXPORT jint JNICALL Java_NativeMethods_createUser
(JNIEnv* env, jobject obj, jstring pwszFirstName, jstring pwszLastName, jstring pwszSAMAccountName, jstring pwszInitialPassword,
    jstring pwszDescrpiton, jstring email, jstring street, jstring city, jstring state, jstring country, jstring postalCode,
    jstring telephone, jstring mobile, jstring jobTitle, jstring department, jstring company, jstring manager) {
    LPCWSTR conFirstName = jstring2string(env, pwszFirstName);
    LPCWSTR conLastName = jstring2string(env, pwszLastName);
    LPCWSTR conAccName = jstring2string(env, pwszSAMAccountName);
    LPCWSTR conIntialPassword = jstring2string(env, pwszInitialPassword);
    LPCWSTR pdescription = jstring2string(env, pwszDescrpiton);
    LPCWSTR pemail = jstring2string(env, email);
    LPCWSTR pstreet = jstring2string(env, street);
    LPCWSTR pcity = jstring2string(env, city);
    LPCWSTR pstate = jstring2string(env, state);
    LPCWSTR pcountry = jstring2string(env, country);
    LPCWSTR ppostalCode = jstring2string(env, postalCode);
    LPCWSTR ptelephone= jstring2string(env, telephone);
    LPCWSTR pmobile= jstring2string(env, mobile);
    LPCWSTR pJobTitle= jstring2string(env, jobTitle);
    LPCWSTR pDepartment= jstring2string(env, department);
    LPCWSTR pCompany = jstring2string(env, company);
    LPCWSTR pmanger = jstring2string(env, manager);
    int res = CreateUserFromADs(conFirstName, conLastName, conAccName, conIntialPassword,pdescription,pemail,
        pstreet,pcity,pstate,pcountry,
        ppostalCode,ptelephone,pmobile,pJobTitle,pDepartment,pCompany,pmanger);
    return res;
}
//Modify user method
JNIEXPORT jint JNICALL Java_NativeMethods_modifyUser
(JNIEnv* env, jobject obj, jstring accountName, jstring firstName, jstring lastName) {
    LPCWSTR conAccountName = jstring2string(env,accountName);
    LPCWSTR conFirstName = jstring2string(env, firstName);
    LPCWSTR conLastName = jstring2string(env, lastName);
    int result = ModifyUserDetailsFromADs(conAccountName, conFirstName, conLastName);
    return result;
}
// Delete User
JNIEXPORT jint JNICALL Java_NativeMethods_deleteUser
(JNIEnv* env, jobject obj, jstring accountName) {
    LPCWSTR conAccountName = jstring2string(env, accountName);
    int result = DeleteUserFromADs(conAccountName);
    return result;
}
// Reset password
JNIEXPORT jint JNICALL Java_NativeMethods_resetPassword
(JNIEnv* env, jobject obj , jstring accountName, jstring newPassword) {
    LPCWSTR conAccountName = jstring2string(env, accountName);
    LPCWSTR conNewPassword = jstring2string(env, newPassword);
    int result = ResetPassWord(conAccountName, conNewPassword);
    return result;
}
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_ListAllUserFromADs
(JNIEnv* env, jobject obj) {
    jobjectArray result;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    jstring* str;
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    // std::cout << "Intially pUsers : " << pUsers << std::endl;
    hr = ADsOpenObject(L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=Users,DC=Test,DC=local", L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPCWSTR att = L"givenName";
        wchar_t* att2 = (wchar_t*)att;
        LPWSTR att3 = att2;
        LPWSTR pszAttr[] = { att3 };
        ADS_SEARCH_HANDLE searchHa;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &searchHa);

        while (pdSearch->GetNextRow(searchHa) != S_ADS_NOMORE_ROWS)
        {
            arrSize++;
        }
        str = new jstring[arrSize];
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &searchHa);
        while (pdSearch->GetNextRow(searchHa) != S_ADS_NOMORE_ROWS)
        {
            hr = pdSearch->GetColumn(searchHa, att3, &col);
            if (SUCCEEDED(hr))
            {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
                pdSearch->FreeColumn(&col);
            }
            dwCount++;
        }

        pdSearch->CloseSearchHandle(searchHa);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        std::string message = std::system_category().message(hr);
        std::cout << message << std::endl;
        return result;
    }
}
// View User Details General tab
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_userGeneralDetails
(JNIEnv* env, jobject obj, jstring user) {
    jobjectArray result;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    jstring* str;
    LPCWSTR userPath = jstring2string(env, user);
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR givenName = convert(L"givenName");
        LPWSTR lastName = convert(L"sn");
        LPWSTR intial= convert(L"initials");
        LPWSTR displayName = convert(L"displayName");
        LPWSTR description = convert(L"description");
        LPWSTR office = convert(L"physicalDeliveryOfficeName");
        LPWSTR telephoneNumber = convert(L"telephoneNumber");
        LPWSTR webPage = convert(L"wWWHomePage");
        LPWSTR email = convert(L"mail");
        LPWSTR pszAttr[] = { givenName,lastName,intial,displayName,description ,office,telephoneNumber,webPage,email };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS)
        {
            arrSize++;
        }
        str = new jstring[9];
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            // FirstName
            hr = pdSearch->GetColumn(hSearch, givenName, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Last name
            hr = pdSearch->GetColumn(hSearch, lastName, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            //Intials
            hr = pdSearch->GetColumn(hSearch, intial, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // display name
            hr = pdSearch->GetColumn(hSearch, displayName, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Description
            hr = pdSearch->GetColumn(hSearch, description, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Office
            hr = pdSearch->GetColumn(hSearch, office, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // TelephoneNumber
            hr = pdSearch->GetColumn(hSearch, telephoneNumber, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            //Web Page
            hr = pdSearch->GetColumn(hSearch, webPage, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // email
            hr = pdSearch->GetColumn(hSearch, email, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            dwCount++;
        }
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        return result;
    }
}
// Address Tab
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_userAddresDetails
(JNIEnv* env, jobject obj, jstring user) {
    jobjectArray result;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    jstring* str;
    LPCWSTR userPath = jstring2string(env, user);
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR streetAddress = convert(L"streetAddress");
        LPWSTR city = convert(L"l");
        LPWSTR state = convert(L"st");
        LPWSTR poBox = convert(L"postOfficeBox");
        LPWSTR country = convert(L"co");
        LPWSTR postalCode = convert(L"postalCode");
        LPWSTR pszAttr[] = { streetAddress,city,state,poBox,country ,postalCode };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS)
        {
            arrSize++;
        }
        str = new jstring[6];
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            // Street Address
            hr = pdSearch->GetColumn(hSearch, streetAddress, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            } if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Post office Box
            hr = pdSearch->GetColumn(hSearch, poBox, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            } if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // city
            hr = pdSearch->GetColumn(hSearch, city, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // State
            hr = pdSearch->GetColumn(hSearch, state, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // country
            hr = pdSearch->GetColumn(hSearch, country, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // email
            hr = pdSearch->GetColumn(hSearch, postalCode, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            dwCount++;
        }
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        return result;
    }
}
// Account tab.
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_userAccountDetails
(JNIEnv* env, jobject obj, jstring user) {
    jobjectArray result;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    jstring* str;
    LPCWSTR userPath = jstring2string(env, user);
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR samAccountName = convert(L"sAMAccountName");
        LPWSTR universal = convert(L"userPrincipalName");
        LPWSTR pszAttr[] = { samAccountName,universal };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS)
        {
            arrSize++;
        }
        str = new jstring[2];
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            //  Sam Account name
            hr = pdSearch->GetColumn(hSearch, samAccountName, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            } if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Universal name
            hr = pdSearch->GetColumn(hSearch, universal, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            } if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            dwCount++;
        }
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        return result;
    }
}
// Get when password last set
JNIEXPORT jlong JNICALL Java_NativeMethods_passWordLastSet
(JNIEnv* env, jobject obj, jstring user) {
    HRESULT hr;
    jlong result;
    CComPtr<IADs> spads;
    CoInitialize(NULL);
    LPCWSTR userPath = jstring2string(env, user);
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADs,
        (void**)&spads);
    if (FAILED(hr))
    {
        return hr;
    }
    CComVariant svar;
    hr = spads->Get(CComBSTR("pwdLastSet"), &svar);
    if (FAILED(hr))
    {
        return hr;
    }

    // Get the IADsLargeInteger interface.
    CComPtr<IADsLargeInteger> spli;
    hr = svar.pdispVal->QueryInterface(IID_IADsLargeInteger,
        (LPVOID*)&spli);
    if (FAILED(hr))
    {
        return hr;
    }

    long lHigh;
    long lLow;
    hr = spli->get_HighPart(&lHigh);
    hr = spli->get_LowPart(&lLow);
    __int64 i64;
    i64 = (ULONG)lHigh;
    i64 = (i64 << 32);
    i64 = i64 + (ULONG)lLow;
    // Print all of the values.
    printf("Combined = %I64d\n", i64);
    return i64;
}
// Get account expires
JNIEXPORT jlong JNICALL Java_NativeMethods_accountExpires
(JNIEnv* env, jobject obj, jstring user) {
    HRESULT hr;
    jlong result;
    CComPtr<IADs> spads;
    CoInitialize(NULL);
    LPCWSTR userPath = jstring2string(env, user);
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADs,
        (void**)&spads);
    if (FAILED(hr))
    {
        return hr;
    }
    CComVariant svar;
    hr = spads->Get(CComBSTR("accountExpires"), &svar);
    if (FAILED(hr))
    {
        return hr;
    }

    // Get the IADsLargeInteger interface.
    CComPtr<IADsLargeInteger> spli;
    hr = svar.pdispVal->QueryInterface(IID_IADsLargeInteger,
        (LPVOID*)&spli);
    if (FAILED(hr))
    {
        return hr;
    }

    long lHigh;
    long lLow;
    hr = spli->get_HighPart(&lHigh);
    hr = spli->get_LowPart(&lLow);
    __int64 i64;
    i64 = (ULONG)lHigh;
    i64 = (i64 << 32);
    i64 = i64 + (ULONG)lLow;
    // Print all of the values.
    printf("Combined = %I64d\n", i64);
    return i64;
}
// TELEPHONE cate
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_telephone
(JNIEnv* env, jobject obj, jstring user) {
    jobjectArray result;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    jstring* str;
    LPCWSTR userPath = jstring2string(env, user);
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR homePhone = convert(L"homePhone");
        LPWSTR pager = convert(L"pager");
        LPWSTR mobile = convert(L"mobile");
        LPWSTR fax = convert(L"facsimileTelephoneNumber");
        LPWSTR ipPhone = convert(L"ipPhone");
        LPWSTR pszAttr[] = { homePhone,pager,mobile ,fax,ipPhone};
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS)
        {
            arrSize++;
        }
        str = new jstring[5];
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            //  Home  phone
            hr = pdSearch->GetColumn(hSearch, homePhone, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            } if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // pager
            hr = pdSearch->GetColumn(hSearch, pager, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Mobile
            hr = pdSearch->GetColumn(hSearch, mobile, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // fax
            hr = pdSearch->GetColumn(hSearch, fax, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            // Ipphone
            hr = pdSearch->GetColumn(hSearch, ipPhone, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            dwCount++;
        }
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        return result;
    }
}
// Organization 
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_organizationDetails
(JNIEnv* env, jobject obj, jstring user) {
    jobjectArray result;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    jstring* str;
    LPCWSTR userPath = jstring2string(env, user);
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR jobTitle = convert(L"title");
        LPWSTR department = convert(L"department");
        LPWSTR company = convert(L"company");
        LPWSTR manager = convert(L"manager");
        LPWSTR pszAttr[] = { jobTitle,department,company,manager };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS)
        {
            arrSize++;
        }
        str = new jstring[4];
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            //  Home  phone
            hr = pdSearch->GetColumn(hSearch, jobTitle, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            } 
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            hr = pdSearch->GetColumn(hSearch, department, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            hr = pdSearch->GetColumn(hSearch, company, &col);
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            hr = pdSearch->GetColumn(hSearch, manager, &col);
            if (SUCCEEDED(hr)) {
                    BSTR path = (BSTR)col.pADsValues->CaseIgnoreString;
                    wchar_t* managerName = getManagerName(path);
                    printf("%S\n", (BSTR)managerName);
                    buffer = managerName;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[i] = env->NewStringUTF(message);
                i++;
            }
            dwCount++;
        }
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        return result;
    }
}
wchar_t* getManagerName(LPCWSTR userpath) {
     wchar_t* name;
    HRESULT hr;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/";
    path += userpath;
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR name = convert(L"givenName");
        LPWSTR pszAttr[] = { name };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            hr = pdSearch->GetColumn(hSearch, name, &col);
            if (SUCCEEDED(hr)) {
                //printf("%S\n", (BSTR)col.pADsValues->CaseIgnoreString);
                name = (wchar_t*)col.pADsValues->CaseIgnoreString;
                return name;
            }
        }
    }
}
//Profile picuture
JNIEXPORT jbyteArray JNICALL Java_NativeMethods_profilePic
(JNIEnv* env, jobject obj, jstring user) {
    jbyteArray result;
    IADsUser* pUser;
    CoInitialize(NULL);
    LPCWSTR userPath = jstring2string(env, user);
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    HRESULT hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsUser,
        (void**)&pUser);
    if (FAILED(hr)) { return NULL; }
    VARIANT  var;
    hr = pUser->get_Picture(&var);
    if (SUCCEEDED(hr)) {
        SAFEARRAY* d = var.parray;
        CComSafeArray<BYTE> sa(d);
        const LONG lb = sa.GetLowerBound();
        const LONG ub = sa.GetUpperBound();
        jbyteArray result;
        result = env->NewByteArray(ub);
        jbyte r[5000];
        for (LONG i = lb; i <= ub; i++)
        {
            r[i] = sa.GetAt(i);
        }
        env->SetByteArrayRegion(result, 0, ub, r);
        return result;
    }
}
//Direct reports
JNIEXPORT jobjectArray JNICALL Java_NativeMethods_directReports
(JNIEnv* env, jobject obj, jstring user) {
    HRESULT hr;
    jobjectArray result;
    jstring* str;
    int i = 0;
    wchar_t* buffer;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    LPCWSTR userPath = jstring2string(env, user);
    ADS_SEARCH_COLUMN col;
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        LPWSTR directReports = convert(L"directReports");
        LPWSTR pszAttr[] = { directReports };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path, pszAttr, nameSize, &hSearch);
        str = new jstring[20];
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            hr = pdSearch->GetColumn(hSearch, directReports, &col);
            if (SUCCEEDED(hr)) {
                int number = col.dwNumValues;

                for (DWORD x = 0; x < col.dwNumValues; x++) {
                    LPCWSTR name = col.pADsValues[x].CaseIgnoreString;
                    wchar_t* managerName = getManagerName(name);
                    buffer = managerName;
                    size_t len = wcslen(buffer);
                    str[i] = env->NewString((const jchar*)buffer, (jsize)len);
                    i++;
                }
            }if (!SUCCEEDED(hr)) {
                const char* message = "No direct Entry found";
                str[i] = env->NewStringUTF(message);
                i++;
            }
        }
        dwCount++;
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();
        result = (jobjectArray)env->NewObjectArray(i, env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (int j = 0; j < i; j++)
        {
            env->SetObjectArrayElement(result, j, str[j]);
        }
        return result;
    }
}
// Get Logon hours
JNIEXPORT jbyteArray JNICALL Java_NativeMethods_LoginHours
(JNIEnv* env, jobject obj,jstring user) {
    jbyteArray result;
    IADsUser* pUser;
    LPCWSTR userPath = jstring2string(env, user);
    CoInitialize(NULL);
    CComBSTR  path = L"LDAP://WIN-F94H2MP3UJR.Test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    HRESULT hr = ADsOpenObject(path, L"Administrator", L"Arun@12",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsUser,
        (void**)&pUser);
    if (FAILED(hr)) { return NULL; }
    VARIANT  var;
    hr = pUser->get_LoginHours(&var);
    if (SUCCEEDED(hr)) {
        SAFEARRAY* d = var.parray;
        CComSafeArray<BOOLEAN> sa(d);
        std::cout << sa.GetCount() << std::endl;
        const LONG lb = sa.GetLowerBound();
        const LONG ub = sa.GetUpperBound();
        result = env->NewByteArray(21);
        const LONG totalByte = 21;
        jbyte r[totalByte];
        for (LONG i = 0; i <= ub; i++)
        {
            r[i] = sa.GetAt(i);
        }
        env->SetByteArrayRegion(result, lb, 21, r);
        return result;
    }
}
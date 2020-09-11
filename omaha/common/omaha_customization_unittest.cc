// Copyright 2010 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================
//
// Tests the constants that vary depending on the customization of Omaha.
// The test checks for the Google Update variations, but can be modified for
// your purposes.

#include <tchar.h>
#include "omaha/base/const_addresses.h"
#include "omaha/base/const_code_signing.h"
#include "omaha/base/const_config.h"
#include "omaha/base/const_debug.h"
#include "omaha/base/const_object_names.h"
#include "omaha/base/constants.h"
#include "omaha/base/logging.h"
#include "omaha/base/process.h"
#include "omaha/base/utils.h"
#include "omaha/common/config_manager.h"
#include "omaha/common/const_goopdate.h"
#include "omaha/common/const_group_policy.h"
#include "omaha/common/omaha_customization_proxy_clsid.h"
#include "omaha/testing/omaha_customization_test.h"

// TODO(omaha): Make use of EXPECT_GU_STREQ, etc.

namespace omaha {

TEST(OmahaCustomizationTest, Constants_BuildFiles) {
  // Primary main.scons values.

  // TODO(omaha): Most of these tests are of extremely questionable
  // value, as they're just checking that certain #defines exist and
  // haven't changed.  But, unanticipated changes to most of these
  // would cause build breaks anyways!  Consider deleting them.
#ifdef GOOGLE_UPDATE_BUILD
  EXPECT_STREQ("Google LLC", FULL_COMPANY_NAME_ANSI);
  EXPECT_STREQ("Google", SHORT_COMPANY_NAME_ANSI);
  EXPECT_STREQ("Update", PRODUCT_NAME_ANSI);

  EXPECT_STREQ("google", COMPANY_DOMAIN_BASE_ANSI);
  EXPECT_STREQ("google.com", COMPANY_DOMAIN_ANSI);

  EXPECT_STREQ("Google Update", OMAHA_APP_NAME_ANSI);
#endif  // GOOGLE_UPDATE_BUILD

  EXPECT_STREQ("goopdate", MAIN_DLL_BASE_NAME_ANSI);

  const GUID kActualProxyClsidIsMachineGuid = PROXY_CLSID_IS_MACHINE;
  EXPECT_TRUE(::IsEqualGUID(kProxyClsidIsMachineGuid,
                            kActualProxyClsidIsMachineGuid));

  const GUID kActualProxyClsidIsUserGuid = PROXY_CLSID_IS_USER;
  EXPECT_TRUE(::IsEqualGUID(kProxyClsidIsUserGuid,
                            kActualProxyClsidIsUserGuid));

  // Primary omaha_version_utils values.
  EXPECT_STREQ(_T("npBraveOneClick"), ONECLICK_PLUGIN_NAME);
  EXPECT_STREQ(_T("npBraveUpdate"), UPDATE_PLUGIN_NAME);
}

TEST(OmahaCustomizationTest, Constants_Names) {
  // Company and product names.

  // TODO(omaha): Most of these tests are of extremely questionable
  // value, as they're just checking that certain #defines exist and
  // haven't changed.  But, unanticipated changes to most of these
  // would cause build breaks anyways!  Consider deleting them.
#ifdef GOOGLE_UPDATE_BUILD
  EXPECT_STREQ(_T("Google LLC"), kFullCompanyName);
  EXPECT_STREQ(_T("Google"), SHORT_COMPANY_NAME);
  EXPECT_STREQ(_T("Google"), kShortCompanyName);
  EXPECT_STREQ(_T("Update"), PRODUCT_NAME);

  EXPECT_STREQ(_T("google.com"), COMPANY_DOMAIN);

  // Full app name.
  EXPECT_STREQ(_T("Google Update"), kAppName);

  // Identifiers.
  EXPECT_STREQ(_T("Google"), COMPANY_NAME_IDENTIFIER);
  EXPECT_STREQ(_T("Update"), PRODUCT_NAME_IDENTIFIER);
  EXPECT_STREQ(_T("GoogleUpdate"), APP_NAME_IDENTIFIER);

  // Other values based on the app name.
  EXPECT_STREQ(_T("_Google_Update_"), kLockPrefix);
#endif  // GOOGLE_UPDATE_BUILD

  // Filename bases
  EXPECT_STREQ(_T("BraveUpdate"), MAIN_EXE_BASE_NAME);
  EXPECT_STREQ(_T("goopdate"), MAIN_DLL_BASE_NAME);
}

TEST(OmahaCustomizationTest, Constants_Filenames) {
  EXPECT_STREQ(_T("BraveUpdate.exe"), kOmahaShellFileName);
  EXPECT_STREQ(_T("BraveCrashHandler.exe"), kCrashHandlerFileName);
  EXPECT_STREQ(_T("BraveCrashHandler64.exe"), kCrashHandler64FileName);
  EXPECT_STREQ(_T("goopdate.dll"), kOmahaDllName);
  EXPECT_STREQ(_T("goopdateres_%s.dll"), kOmahaResourceDllNameFormat);
  EXPECT_STREQ(_T("BraveUpdateBroker.exe"), kOmahaBrokerFileName);
  EXPECT_STREQ(_T("BraveUpdateCore.exe"), kOmahaCoreFileName);
  EXPECT_STREQ(_T("BraveUpdateOnDemand.exe"), kOmahaOnDemandFileName);
  EXPECT_STREQ(_T("BraveUpdateSetup.exe"), kOmahaMetainstallerFileName);
  EXPECT_STREQ(_T("BraveUpdateComRegisterShell64.exe"),
               kOmahaCOMRegisterShell64);
  EXPECT_STREQ(_T("psmachine.dll"), kPSFileNameMachine);
  EXPECT_STREQ(_T("psmachine_64.dll"), kPSFileNameMachine64);
  EXPECT_STREQ(_T("psuser.dll"), kPSFileNameUser);
  EXPECT_STREQ(_T("psuser_64.dll"), kPSFileNameUser64);
}

TEST(OmahaCustomizationTest, Constants_Certificate) {
  EXPECT_STREQ(_T("Google LLC"), kSha1CertificateSubjectName);
  EXPECT_STREQ(_T("Google LLC"), kSha256CertificateSubjectName);
}

TEST(OmahaCustomizationTest, Constants_OmahaAppId_String) {
  EXPECT_STREQ(_T("{B131C935-9BE6-41DA-9599-1F776BEB8019}"), GOOPDATE_APP_ID);
  EXPECT_STREQ(_T("{B131C935-9BE6-41DA-9599-1F776BEB8019}"),
               kGoogleUpdateAppId);
}

TEST(OmahaCustomizationTest, Constants_OmahaAppId_GUID) {
  const GUID kExpectedGoogleUpdateGuid =
      {0xB131C935, 0x9BE6, 0x41DA,
       {0x95, 0x99, 0x1F, 0x77, 0x6B, 0xEB, 0x80, 0x19}};
  EXPECT_TRUE(::IsEqualGUID(kExpectedGoogleUpdateGuid, kGoopdateGuid));
  EXPECT_STREQ(_T("{B131C935-9BE6-41DA-9599-1F776BEB8019}"),
               GuidToString(kGoopdateGuid));
}

TEST(OmahaCustomizationTest, Constants_OmahaAppId_GUIDAndStringMatch) {
  EXPECT_STREQ(kGoogleUpdateAppId, GuidToString(kGoopdateGuid));
}

TEST(OmahaCustomizationTest, Constants_Directories) {
  EXPECT_STREQ(_T("Offline"), OFFLINE_DIR_NAME);
  EXPECT_GU_STREQ(_T("BraveSoftware"), OMAHA_REL_COMPANY_DIR);
  EXPECT_GU_STREQ(_T("BraveSoftware\\CrashReports"), OMAHA_REL_CRASH_DIR);
  EXPECT_GU_STREQ(_T("BraveSoftware\\Update"), OMAHA_REL_GOOPDATE_INSTALL_DIR);
  EXPECT_GU_STREQ(_T("BraveSoftware\\Update\\Log"), OMAHA_REL_LOG_DIR);
  EXPECT_GU_STREQ(_T("BraveSoftware\\Update\\Offline"),
                  OMAHA_REL_OFFLINE_STORAGE_DIR);
  EXPECT_GU_STREQ(_T("BraveSoftware\\Update\\Download"),
                  OMAHA_REL_DOWNLOAD_STORAGE_DIR);
  EXPECT_GU_STREQ(_T("BraveSoftware\\Update\\Install"),
                  OMAHA_REL_INSTALL_WORKING_DIR);
}

TEST(OmahaCustomizationTest, Constants_RegistryKeys_NotCustomized) {
  EXPECT_STREQ(_T("HKLM"), MACHINE_KEY_NAME);
  EXPECT_STREQ(_T("HKLM\\"), MACHINE_KEY);
  EXPECT_STREQ(_T("HKCU"), USER_KEY_NAME);
  EXPECT_STREQ(_T("HKCU\\"), USER_KEY);
  EXPECT_STREQ(_T("HKU\\"), USERS_KEY);
}

TEST(OmahaCustomizationTest, Constants_RegistryKeys) {
  EXPECT_GU_STREQ(_T("Software\\BraveSoftware\\"), COMPANY_MAIN_KEY);
  EXPECT_GU_STREQ(_T("Software\\BraveSoftware\\Update\\"), GOOPDATE_MAIN_KEY);
  EXPECT_GU_STREQ(_T("Software\\BraveSoftware\\Update\\Clients\\"), GOOPDATE_REG_RELATIVE_CLIENTS);  // NOLINT
  EXPECT_GU_STREQ(_T("Software\\BraveSoftware\\Update\\ClientState\\"), GOOPDATE_REG_RELATIVE_CLIENT_STATE);  // NOLINT
  EXPECT_GU_STREQ(_T("Software\\BraveSoftware\\Update\\ClientStateMedium\\"), GOOPDATE_REG_RELATIVE_CLIENT_STATE_MEDIUM);  // NOLINT
  EXPECT_GU_STREQ(_T("Software\\Policies\\BraveSoftware\\"), COMPANY_POLICIES_MAIN_KEY);           // NOLINT
  EXPECT_GU_STREQ(_T("Software\\Policies\\BraveSoftware\\Update\\"), GOOPDATE_POLICIES_RELATIVE);  // NOLINT

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\"), USER_REG_GOOGLE);
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\"), USER_REG_UPDATE);
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\Clients\\"), USER_REG_CLIENTS);  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\Clients\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), USER_REG_CLIENTS_GOOPDATE);  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\ClientState\\"), USER_REG_CLIENT_STATE);  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\ClientState\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), USER_REG_CLIENT_STATE_GOOPDATE);  // NOLINT

  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\"), MACHINE_REG_GOOGLE);
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\"), MACHINE_REG_UPDATE);
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\Clients\\"), MACHINE_REG_CLIENTS);  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\Clients\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), MACHINE_REG_CLIENTS_GOOPDATE);  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientState\\"), MACHINE_REG_CLIENT_STATE);  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientState\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), MACHINE_REG_CLIENT_STATE_GOOPDATE);  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientStateMedium\\"), MACHINE_REG_CLIENT_STATE_MEDIUM);  // NOLINT

  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\UpdateDev\\"), MACHINE_REG_UPDATE_DEV);  // NOLINT
}

TEST(OmahaCustomizationTest, Constants_RegistryKeys_GroupPolicy) {
  EXPECT_GU_STREQ(_T("Software\\Policies\\BraveSoftware\\Update\\"), GOOPDATE_POLICIES_RELATIVE);  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\Policies\\BraveSoftware\\Update\\"), kRegKeyGoopdateGroupPolicy);  // NOLINT
}

TEST(OmahaCustomizationTest, Constants_RegistryValues) {
  EXPECT_GU_STREQ(_T("BraveSoftware Update"), kRunValueName);
}

TEST(OmahaCustomizationTest, Constants_MsiMsp) {
  EXPECT_STREQ(_T("BraveUpdateHelper.msi"), kHelperInstallerName);
  EXPECT_STREQ(_T("{A92DAB39-4E2C-4304-9AB6-BC44E68B55E2}"),
               kHelperInstallerProductGuid);
  EXPECT_STREQ(_T("BraveUpdateHelperPatch.msp"), kHelperPatchName);
  EXPECT_STREQ(_T("{E0D0D2C9-5836-4023-AB1D-54EC3B90AD03}"), kHelperPatchGuid);
}

TEST(OmahaCustomizationTest, Constants_CompatibleMinimumOlderShellVersion) {
  EXPECT_EQ(0x00010003001A0001, kCompatibleMinimumOlderShellVersion);
}

TEST(OmahaCustomizationTest, Constants_BrandCode) {
  EXPECT_STREQ(_T("GGLS"), kDefaultGoogleUpdateBrandCode);
}

TEST(OmahaCustomizationTest, Constants_Addresses) {
  EXPECT_STREQ(_T("www.brave.com"), kGoogleHttpServer);
  EXPECT_STREQ(_T("tools.brave.com"), kGoopdateServer);
  EXPECT_STREQ(_T("https://updates.bravesoftware.com/service/update2"),
               kUrlUpdateCheck);
  EXPECT_STREQ(_T("https://updates.bravesoftware.com/service/update2"), kUrlPing);
  EXPECT_STREQ(_T("https://updates.bravesoftware.com/cr/report"), kUrlCrashReport);
  EXPECT_STREQ(_T("https://updates.bravesoftware.com/support/installer/?"), kUrlMoreInfo);
  EXPECT_STREQ(_T("https://updates.bravesoftware.com/service/check2"),
               kUrlCodeRedCheck);
  EXPECT_STREQ(_T("https://updates.bravesoftware.com/tbproxy/usagestats"),
               kUrlUsageStatsReport);
}

TEST(OmahaCustomizationTest, Constants_Config) {
  EXPECT_GU_STREQ(_T("Software\\BraveSoftware\\Update\\Shared"), kCiRegKeyShared);
}

TEST(OmahaCustomizationTest, Constants_Debug) {
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate-debug"), kCiDebugDirectory);
}

TEST(OmahaCustomizationTest, Constants_Logging) {
  EXPECT_STREQ(_T("BraveUpdate.ini"), kLogConfigFileName);
  EXPECT_STREQ(_T("BraveUpdate.log"), kDefaultLogFileName);
}

// These should not change during customization.
TEST(OmahaCustomizationTest, Constants_ObjectNames_Prefixes) {
  EXPECT_GU_STREQ(_T("Global\\BraveSoftware"), kGlobalPrefix);
}

TEST(OmahaCustomizationTest, Constants_ObjectNames_Pipes) {
  EXPECT_STREQ(_T("\\\\.\\pipe\\BraveSoftwareCrashServices"), kCrashPipeNamePrefix);
}

TEST(OmahaCustomizationTest, Constants_ObjectNames_MutexesAndEvents) {
  EXPECT_STREQ(_T("{C50974A0-5616-4DC6-AC6D-D4EFF6F5FAC3}"), kSetupMutex);
  EXPECT_STREQ(_T("{4613C8D6-D26E-4F10-B494-72CFF6F0BF0B}"), kShutdownEvent);
  EXPECT_STREQ(_T("{EC98B00C-9557-4627-ADCF-5832C3B09AA3}"),
               kCoreSingleInstance);
  EXPECT_STREQ(_T("{08DA086F-9FC4-4B2E-954C-6D7D5ACD5167}"),
               kCrashHandlerSingleInstance);
  EXPECT_STREQ(_T("{0EADE80E-E9B8-4A5D-AF64-6D2A918F597C}"),
               kUpdateAppsSingleInstance);
  EXPECT_STREQ(_T("%s-{0684C3E2-4EFA-4D1C-AE8D-A61945B94687}"),
               kInstallAppSingleInstance);
  EXPECT_STREQ(_T("{FE0E7F6B-B8BD-4EEE-A8F1-8CE625AEF520}"),
               kInstallManagerSerializer);
  EXPECT_STREQ(_T("{D6025E95-A77B-4ADB-B46F-65CC31BB40E7}"),
               kMetricsSerializer);
  EXPECT_STREQ(_T("{4E15433F-5E08-47A1-AA4F-B1D1657EE725}"),
               kRegistryAccessMutex);
}

TEST(OmahaCustomizationTest, Constants_ObjectNames_SharedMemory) {
  EXPECT_GU_STREQ(_T("Global\\BraveSoftwareUpdate3"),
                  kGoogleUpdate3SharedMemoryName);
  EXPECT_GU_STREQ(_T("Global\\BraveSoftwareUpdateCore"),
                  kGoogleUpdateCoreSharedMemoryName);
}

TEST(OmahaCustomizationTest, Constants_Services) {
  EXPECT_GU_STREQ(_T("brave_service_name"), kRegValueServiceName);
  EXPECT_GU_STREQ(_T("bravem_service_name"), kRegValueMediumServiceName);
  EXPECT_GU_STREQ(_T("brave_task_name_c"), kRegValueTaskNameC);
  EXPECT_GU_STREQ(_T("brave_task_name_ua"), kRegValueTaskNameUA);

  EXPECT_GU_STREQ(_T("brave"), kServicePrefix);
  EXPECT_GU_STREQ(_T("bravem"), kMediumServicePrefix);

  EXPECT_STREQ(_T("BraveUpdate.exe"), kServiceFileName);
}

TEST(OmahaCustomizationTest, Constants_ScheduledTasks) {
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdateTaskUser"), kScheduledTaskNameUserPrefix);
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdateTaskMachine"), kScheduledTaskNameMachinePrefix);    // NOLINT
}

//
// ConfigManager keys.
//

TEST(OmahaCustomizationTest, ConfigManager_RegistryKeys) {
  const ConfigManager& cm = *ConfigManager::Instance();

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\Clients\\"), cm.user_registry_clients());  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\Clients\\"), cm.machine_registry_clients());  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\Clients\\"), cm.registry_clients(false));  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\Clients\\"), cm.registry_clients(true));  // NOLINT

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\Clients\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.user_registry_clients_goopdate());  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\Clients\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.machine_registry_clients_goopdate());  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\Clients\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.registry_clients_goopdate(false));  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\Clients\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.registry_clients_goopdate(true));  // NOLINT

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\ClientState\\"), cm.user_registry_client_state());  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientState\\"), cm.machine_registry_client_state());  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\ClientState\\"), cm.registry_client_state(false));  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientState\\"), cm.registry_client_state(true));  // NOLINT

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\ClientState\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.user_registry_client_state_goopdate());  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientState\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.machine_registry_client_state_goopdate());  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\ClientState\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.registry_client_state_goopdate(false));  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientState\\{B131C935-9BE6-41DA-9599-1F776BEB8019}"), cm.registry_client_state_goopdate(true));  // NOLINT

  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\ClientStateMedium\\"), cm.machine_registry_client_state_medium());  // NOLINT

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\"), cm.user_registry_update());  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\"), cm.machine_registry_update());  // NOLINT
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\Update\\"), cm.registry_update(false));  // NOLINT
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\Update\\"), cm.registry_update(true));  // NOLINT

  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\"), cm.user_registry_google());
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\"), cm.machine_registry_google());
  EXPECT_GU_STREQ(_T("HKCU\\Software\\BraveSoftware\\"), cm.registry_google(false));
  EXPECT_GU_STREQ(_T("HKLM\\Software\\BraveSoftware\\"), cm.registry_google(true));
}

// Internal means google internal. Disabled.
TEST(OmahaCustomizationTest, DISABLED_IsInternalUser) {
  if (IsBuildSystem()) {
  // The build system is not configured the same.
  // This may or may not be true in non-Google Update builds.
#ifdef GOOGLE_UPDATE_BUILD
    EXPECT_FALSE(ConfigManager::Instance()->IsInternalUser());
#else
  std::wcout << _T("Did not test IsInternalUser.") << std::endl;
#endif
  } else {
    EXPECT_TRUE(ConfigManager::Instance()->IsInternalUser());
  }
}

//
// Test helpers.
//

TEST(OmahaCustomizationTest, GetGoogleUserPath) {
  EXPECT_STREQ(GetLocalAppDataPath() + SHORT_COMPANY_NAME + _T("\\"),
               GetGoogleUserPath());
}

TEST(OmahaCustomizationTest, GetGoogleUpdateUserPath) {
  EXPECT_STREQ(GetLocalAppDataPath() + SHORT_COMPANY_NAME + _T("\\")
                                     + PRODUCT_NAME + _T("\\"),
               GetGoogleUpdateUserPath());
}

// Assumes Program Files is in the normal location.
TEST(OmahaCustomizationTest, GetGoogleUpdateMachinePath) {
  CString expected_machine_path;
  EXPECT_SUCCEEDED(GetFolderPath(CSIDL_PROGRAM_FILES | CSIDL_FLAG_DONT_VERIFY,
                                 &expected_machine_path));
  expected_machine_path.Append(_T("\\") SHORT_COMPANY_NAME
                               _T("\\") PRODUCT_NAME);
  EXPECT_STREQ(expected_machine_path, GetGoogleUpdateMachinePath());
}

}  // namespace omaha

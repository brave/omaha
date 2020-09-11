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

#include <windows.h>
#include <tchar.h>
#include <atlbase.h>
#include <oleauto.h>
#include "omaha/base/browser_utils.h"
#include "omaha/base/utils.h"
#include "omaha/common/const_goopdate.h"
#include "goopdate/omaha3_idl.h"
#include "omaha/testing/omaha_customization_test.h"

// Most of the tests are intentionally not using the omaha namespace. Most of
// the values being tested are not in this namespace, and being in the global
// namespace is required by TEST_GU_INT_F to catch conflicts with Google types
// when building non-Google versions.

class OmahaCustomizationGoopdateComInterfaceTest
    : public OmahaCustomizationTypeLibComInterfaceTest {
 protected:
  OmahaCustomizationGoopdateComInterfaceTest()
      : OmahaCustomizationTypeLibComInterfaceTest(omaha::kOmahaDllName) {
  }
};

// Fixture for testing interfaces that are not in a TypeLib.
// We can only verify the uuid of the interfaces and classes.
class OmahaCustomizationGoopdateComInterfaceNoTypeLibTest
    : public testing::Test {
};

//
// Omaha 3 COM Constants.
//

namespace omaha {

// TODO(omaha): We should probably move these to a separate
// const_com_customization.h in goopdate\.
TEST(OmahaCustomizationTest, Constants_ComProgIds) {
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.OnDemandCOMClassUser"), kProgIDOnDemandUser);
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.OnDemandCOMClassMachine"),
                  kProgIDOnDemandMachine);
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.OnDemandCOMClassSvc"), kProgIDOnDemandSvc);

  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.Update3WebUser"), kProgIDUpdate3WebUser);
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.Update3WebMachine"),
                  kProgIDUpdate3WebMachine);
  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.Update3WebSvc"), kProgIDUpdate3WebSvc);

  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.CoreClass"), kProgIDGoogleUpdateCoreService);

  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.ProcessLauncher"), kProgIDProcessLauncher);

  // TODO: mplesa see if this block is still needed
  EXPECT_GU_STREQ(_T("GoogleUpdate.PolicyStatusUser"), kProgIDPolicyStatusUser);
  EXPECT_GU_STREQ(_T("GoogleUpdate.PolicyStatusMachine"),
                  kProgIDPolicyStatusMachine);
  EXPECT_GU_STREQ(_T("GoogleUpdate.PolicyStatusMachineFallback"),
                  kProgIDPolicyStatusMachineFallback);
  EXPECT_GU_STREQ(_T("GoogleUpdate.PolicyStatusSvc"), kProgIDPolicyStatusSvc);

  EXPECT_GU_STREQ(_T("BraveSoftwareUpdate.PolicyStatus"), kProgIDPolicyStatus);
}

}  // namespace omaha

//
// Omaha 3 COM Interfaces Enums.
//

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest, BrowserType) {
  EXPECT_EQ(0, BROWSER_UNKNOWN);
  EXPECT_EQ(1, BROWSER_DEFAULT);
  EXPECT_EQ(2, BROWSER_INTERNET_EXPLORER);
  EXPECT_EQ(3, BROWSER_FIREFOX);
  EXPECT_EQ(4, BROWSER_CHROME);
}

// There are two different BrowserType definitions, one in the IDL and one
// in browser_utils. Verify they are identical.
TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       BrowserType_DefinitionsMatch) {
  EXPECT_EQ(::BROWSER_UNKNOWN,            omaha::BROWSER_UNKNOWN);
  EXPECT_EQ(::BROWSER_DEFAULT,            omaha::BROWSER_DEFAULT);
  EXPECT_EQ(::BROWSER_INTERNET_EXPLORER,  omaha::BROWSER_IE);
  EXPECT_EQ(::BROWSER_FIREFOX,            omaha::BROWSER_FIREFOX);
  EXPECT_EQ(::BROWSER_CHROME,             omaha::BROWSER_CHROME);

  EXPECT_EQ(::BROWSER_CHROME + 1, omaha::BROWSER_MAX)
      << _T("A browser has been added without updating test and/or the IDL");
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest, CurrentState) {
  EXPECT_EQ(1,  STATE_INIT);
  EXPECT_EQ(2,  STATE_WAITING_TO_CHECK_FOR_UPDATE);
  EXPECT_EQ(3,  STATE_CHECKING_FOR_UPDATE);
  EXPECT_EQ(4,  STATE_UPDATE_AVAILABLE);
  EXPECT_EQ(5,  STATE_WAITING_TO_DOWNLOAD);
  EXPECT_EQ(6,  STATE_RETRYING_DOWNLOAD);
  EXPECT_EQ(7,  STATE_DOWNLOADING);
  EXPECT_EQ(8,  STATE_DOWNLOAD_COMPLETE);
  EXPECT_EQ(9,  STATE_EXTRACTING);
  EXPECT_EQ(10, STATE_APPLYING_DIFFERENTIAL_PATCH);
  EXPECT_EQ(11, STATE_READY_TO_INSTALL);
  EXPECT_EQ(12, STATE_WAITING_TO_INSTALL);
  EXPECT_EQ(13, STATE_INSTALLING);
  EXPECT_EQ(14, STATE_INSTALL_COMPLETE);
  EXPECT_EQ(15, STATE_PAUSED);
  EXPECT_EQ(16, STATE_NO_UPDATE);
  EXPECT_EQ(17, STATE_ERROR);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest, InstallPriority) {
  EXPECT_EQ(0,  INSTALL_PRIORITY_LOW);
  EXPECT_EQ(10, INSTALL_PRIORITY_HIGH);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest, PostInstallAction) {
  EXPECT_EQ(0, POST_INSTALL_ACTION_DEFAULT);
  EXPECT_EQ(1, POST_INSTALL_ACTION_EXIT_SILENTLY);
  EXPECT_EQ(2, POST_INSTALL_ACTION_LAUNCH_COMMAND);
  EXPECT_EQ(3, POST_INSTALL_ACTION_EXIT_SILENTLY_ON_LAUNCH_COMMAND);
  EXPECT_EQ(4, POST_INSTALL_ACTION_RESTART_BROWSER);
  EXPECT_EQ(5, POST_INSTALL_ACTION_RESTART_ALL_BROWSERS);
  EXPECT_EQ(6, POST_INSTALL_ACTION_REBOOT);
}

//
// Omaha 3 COM Interfaces.
//

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, TypeLib) {
  EXPECT_GU_ID_EQ(_T("{6FF4F468-0B5A-4878-ACEB-86263DAAD877}"),
                  LIBID_GoogleUpdate3Lib);

  EXPECT_SUCCEEDED(GetTypeLibDocumentation());
  EXPECT_STREQ(_T("GoogleUpdate3Lib"), item_name_);
  EXPECT_GU_STREQ(_T("Omaha 3.0 Type Library"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest, IGoogleUpdate3) {
  // TODO(omaha): Test uuid constants after extracting from IDLs.
  EXPECT_GU_ID_EQ(_T("{7CB305B1-4D45-4668-AD91-677F87BED305}"),
                  __uuidof(IGoogleUpdate3));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IGoogleUpdate3")));
  EXPECT_STREQ(_T("IGoogleUpdate3 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// The IAppBundle interface name does not change for non-Google builds, but the
// ID must. The same is true for many of the interfaces.
TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppBundle) {
  EXPECT_GU_ID_EQ(_T("{1985533F-9B0F-490A-85C5-24F316E66FB2}"),
                  __uuidof(IAppBundle));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppBundle")));
  EXPECT_STREQ(_T("IAppBundle Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// This appears in the typelib for unknown reasons.
TEST_F(OmahaCustomizationGoopdateComInterfaceTest, ULONG_PTR) {
  EXPECT_SUCCEEDED(GetDocumentation(_T("ULONG_PTR")));
  EXPECT_TRUE(!item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IApp) {
  EXPECT_GU_ID_EQ(_T("{A4BCDF52-2179-4C77-8C5F-B8095712B563}"),
                  __uuidof(IApp));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IApp")));
  EXPECT_STREQ(_T("IApp Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IApp2) {
  EXPECT_GU_ID_EQ(_T("{A147722A-5568-4B84-B401-86D744470CBF}"),
                  __uuidof(IApp2));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IApp2")));
  EXPECT_STREQ(_T("IApp2 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppCommand) {
  EXPECT_GU_ID_EQ(_T("{4842EC21-0860-45B5-99F0-A1E6E7C11561}"),
                  __uuidof(IAppCommand));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppCommand")));
  EXPECT_STREQ(_T("IAppCommand Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppCommand2) {
  EXPECT_GU_ID_EQ(_T("{C11C073F-E6D0-4EF7-897B-AAF52498CD2F}"),
                  __uuidof(IAppCommand2));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppCommand2")));
  EXPECT_STREQ(_T("IAppCommand2 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppVersion) {
  EXPECT_GU_ID_EQ(_T("{F234546B-DACD-4374-97CF-7BADFAB76766}"),
                  __uuidof(IAppVersion));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppVersion")));
  EXPECT_STREQ(_T("IAppVersion Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IPackage) {
  EXPECT_GU_ID_EQ(_T("{C663DEBB-F082-4971-9F6E-35DE45C96F4E}"),
                  __uuidof(IPackage));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPackage")));
  EXPECT_STREQ(_T("IPackage Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, ICurrentState) {
  EXPECT_GU_ID_EQ(_T("{E6836CFF-5949-44BC-B6BE-9C8C48DD8D97}"),
                  __uuidof(ICurrentState));

  EXPECT_SUCCEEDED(GetDocumentation(_T("ICurrentState")));
  EXPECT_STREQ(_T("ICurrentState Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IRegistrationUpdateHook) {
  EXPECT_GU_ID_EQ(_T("{48B5E6B2-9383-4B1E-AAE7-720C4779ABA6}"),
                  __uuidof(IRegistrationUpdateHook));
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest, ICoCreateAsync) {
  EXPECT_GU_ID_EQ(_T("{DD84E356-3D21-44C8-83DD-6BEEC22FA427}"),
                  __uuidof(ICoCreateAsync));
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest, ICredentialDialog) {
  EXPECT_GU_ID_EQ(_T("{660130E8-74E4-4821-A6FD-4E9A86E06470}"),
                  __uuidof(ICredentialDialog));
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest, IPolicyStatus) {
  EXPECT_GU_ID_EQ(_T("{BFA9CB0F-987A-4E8A-A3BE-5988F315F35E}"),
                  __uuidof(IPolicyStatus));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatus")));
  EXPECT_STREQ(_T("IPolicyStatus Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest, IPolicyStatusValue) {
  EXPECT_GU_ID_EQ(_T("{27634814-8E41-4C35-8577-980134A96544}"),
                  __uuidof(IPolicyStatusValue));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatusValue")));
  EXPECT_STREQ(_T("IPolicyStatusValue Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest, IPolicyStatus2) {
  EXPECT_GU_ID_EQ(_T("{34527502-D3DB-4205-A69B-789B27EE0414}"),
                  __uuidof(IPolicyStatus2));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatus2")));
  EXPECT_STREQ(_T("IPolicyStatus2 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest, IPolicyStatus3) {
  EXPECT_GU_ID_EQ(_T("{05A30352-EB25-45B6-8449-BCA7B0542CE5}"),
                  __uuidof(IPolicyStatus3));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatus3")));
  EXPECT_STREQ(_T("IPolicyStatus3 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest, IGoogleUpdate3Web) {
  EXPECT_GU_ID_EQ(_T("{C9190589-ECEC-43F8-8AEC-62496BB87B26}"),
                  __uuidof(IGoogleUpdate3Web));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IGoogleUpdate3Web")));
  EXPECT_STREQ(_T("IGoogleUpdate3Web Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IGoogleUpdate3WebSecurity) {
  EXPECT_GU_ID_EQ(_T("{24D704AD-AC42-49F2-BB4F-68BA77C98E91}"),
                  __uuidof(IGoogleUpdate3WebSecurity));
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppBundleWeb) {
  EXPECT_GU_ID_EQ(_T("{852A0F87-D117-4B7C-ABA9-2F76D91BCB9D}"),
                  __uuidof(IAppBundleWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppBundleWeb")));
  EXPECT_STREQ(_T("IAppBundleWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppWeb) {
  EXPECT_GU_ID_EQ(_T("{FB43AAD0-DDBA-4D01-A3E0-FAB100E7926B}"),
                  __uuidof(IAppWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppWeb")));
  EXPECT_STREQ(_T("IAppWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppCommandWeb) {
  EXPECT_GU_ID_EQ(_T("{19F4616B-B7DD-4B3F-8084-C81C5C77AAA4}"),
                  __uuidof(IAppCommandWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppCommandWeb")));
  EXPECT_STREQ(_T("IAppCommandWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, IAppVersionWeb) {
  EXPECT_GU_ID_EQ(_T("{35A4470F-5EEC-4715-A2DC-6AA9F8E21183}"),
                  __uuidof(IAppVersionWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppVersionWeb")));
  EXPECT_STREQ(_T("IAppVersionWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaCustomizationGoopdateComInterfaceTest, ICoCreateAsyncStatus) {
  EXPECT_GU_ID_EQ(_T("{7A24060E-533F-4962-9E15-34BD82555FA7}"),
                  __uuidof(ICoCreateAsyncStatus));

  EXPECT_SUCCEEDED(GetDocumentation(_T("ICoCreateAsyncStatus")));
  EXPECT_STREQ(_T("ICoCreateAsyncStatus Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3UserClass) {
  EXPECT_GU_ID_EQ(_T("{2CF4A521-EC1E-4075-A20B-5DE3E5B89B1B}"),
                  __uuidof(GoogleUpdate3UserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3UserClass")));
  EXPECT_STREQ(_T("GoogleUpdate3 Class for per-user applications"),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3ServiceClass) {
  EXPECT_GU_ID_EQ(_T("{08F15E98-0442-45D3-82F1-F67495CC51EB}"),
                  __uuidof(GoogleUpdate3ServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3ServiceClass")));
  EXPECT_STREQ(_T("GoogleUpdate3 Service Class for machine applications"),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebUserClass) {
  EXPECT_GU_ID_EQ(_T("{2F78AECB-0A7F-4474-89D5-C325293DE960}"),
                  __uuidof(GoogleUpdate3WebUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3WebUserClass")));
  EXPECT_STREQ(_T("GoogleUpdate3Web for user applications"),
                  item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebMachineClass) {
  EXPECT_GU_ID_EQ(_T("{00B16F95-319A-4F01-AC81-CE69B8F4E387}"),
                  __uuidof(GoogleUpdate3WebMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3WebMachineClass")));
  EXPECT_STREQ(
      _T("Pass-through broker for the GoogleUpdate3WebServiceClass"),
      item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebServiceClass) {
  EXPECT_GU_ID_EQ(_T("{3A9D7221-2278-41DD-930B-C2356B7D3725}"),
                  __uuidof(GoogleUpdate3WebServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3WebServiceClass")));
  EXPECT_STREQ(_T("GoogleUpdate3Web"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebMachineFallbackClass) {
  EXPECT_GU_ID_EQ(_T("{66CE3D6C-0B35-4F78-AC77-39728A75CB75}"),
                  __uuidof(GoogleUpdate3WebMachineFallbackClass));

  EXPECT_SUCCEEDED(
      GetDocumentation(_T("GoogleUpdate3WebMachineFallbackClass")));
  EXPECT_STREQ(L"Fallback mechanism if GoogleUpdate3WebServiceClass fails",
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              CurrentStateUserClass) {
  EXPECT_GU_ID_EQ(_T("{06B74C04-E813-4DD4-A972-172836EFA8D6}"),
                  __uuidof(CurrentStateUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CurrentStateUserClass")));
  EXPECT_STREQ(_T("CurrentStateUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              CurrentStateMachineClass) {
  EXPECT_GU_ID_EQ(_T("{BC03C0E4-1528-4299-89B2-419644FA48AC}"),
                  __uuidof(CurrentStateMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CurrentStateMachineClass")));
  EXPECT_STREQ(_T("CurrentStateMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              CoCreateAsyncClass) {
  EXPECT_GU_ID_EQ(_T("{8F6D9FE5-6ED3-43A3-80D2-EA8766D65352}"),
                  __uuidof(CoCreateAsyncClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CoCreateAsyncClass")));
  EXPECT_STREQ(_T("CoCreateAsyncClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              CredentialDialogUserClass) {
  EXPECT_GU_ID_EQ(_T("{4C738ABE-2ED7-4BF6-B668-046DC1A72C00}"),
                  __uuidof(CredentialDialogUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CredentialDialogUserClass")));
  EXPECT_STREQ(_T("CredentialDialogUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              CredentialDialogMachineClass) {
  EXPECT_GU_ID_EQ(_T("{B5D1924F-CB80-47AA-8DEC-5E0854A42A73}"),
                  __uuidof(CredentialDialogMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CredentialDialogMachineClass")));
  EXPECT_STREQ(_T("CredentialDialogMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// TODO: mplesa this might need merge
TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              PolicyStatusValueUserClass) {
  EXPECT_GU_ID_EQ(_T("{85D8EE2F-794F-41F0-BB03-49D56A23BEF4}"),
                  __uuidof(PolicyStatusValueUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusValueUserClass")));
  EXPECT_STREQ(_T("PolicyStatusValueUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              PolicyStatusValueMachineClass) {
  EXPECT_GU_ID_EQ(_T("{C6271107-A214-4F11-98C0-3F16BC670D28}"),
                  __uuidof(PolicyStatusValueMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusValueMachineClass")));
  EXPECT_STREQ(_T("PolicyStatusValueMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              PolicyStatusUserClass) {
  EXPECT_GU_ID_EQ(_T("{6DDCE70D-A4AE-4E97-908C-BE7B2DB750AD}"),
                  __uuidof(PolicyStatusUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusUserClass")));
  EXPECT_STREQ(_T("Policy Status for per-user applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              PolicyStatusMachineClass) {
  EXPECT_GU_ID_EQ(_T("{521FDB42-7130-4806-822A-FC5163FAD983}"),
                  __uuidof(PolicyStatusMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusMachineClass")));
  EXPECT_STREQ(_T("Policy Status pass-through broker ")
               _T("for machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              PolicyStatusMachineServiceClass) {
  EXPECT_GU_ID_EQ(_T("{1C4CDEFF-756A-4804-9E77-3E8EB9361016}"),
                  __uuidof(PolicyStatusMachineServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusMachineServiceClass")));
  EXPECT_STREQ(_T("Policy Status for per-machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              PolicyStatusMachineFallbackClass) {
  EXPECT_GU_ID_EQ(_T("{ADDF22CF-3E9B-4CD7-9139-8169EA6636E4}"),
                  __uuidof(PolicyStatusMachineFallbackClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusMachineFallbackClass")));
  EXPECT_STREQ(_T("Fallback for if PolicyStatusMachineServiceClass fails."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleComProxyMachineClass) {
  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleComProxyMachineClass")));
  EXPECT_STREQ(_T("GoogleComProxyMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleComProxyUserClass) {
  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleComProxyUserClass")));
  EXPECT_STREQ(_T("GoogleComProxyUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              ProcessLauncherClass) {
  EXPECT_GU_ID_EQ(_T("{4C3BA8F3-1264-4BDB-BB2D-CA44734AD00D}"),
                  __uuidof(ProcessLauncherClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("ProcessLauncherClass")));
  EXPECT_STREQ(_T("ProcessLauncherClass Class"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              OnDemandUserAppsClass) {
  EXPECT_GU_ID_EQ(_T("{6DA8171A-7CEC-4E45-BDED-9F287AD7C854}"),
                  __uuidof(OnDemandUserAppsClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandUserAppsClass")));
  EXPECT_STREQ(_T("OnDemand updates for per-user applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              OnDemandMachineAppsClass) {
  EXPECT_GU_ID_EQ(_T("{28C83F57-E4C0-4B54-B187-585C51EE8F9C}"),
                  __uuidof(OnDemandMachineAppsClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandMachineAppsClass")));
  EXPECT_STREQ(_T("OnDemand pass-through broker for machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              OnDemandMachineAppsServiceClass) {
  EXPECT_GU_ID_EQ(_T("{D7D7525F-5DF4-4C9D-8781-C02F39F973E6}"),
                  __uuidof(OnDemandMachineAppsServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandMachineAppsServiceClass")));
  EXPECT_STREQ(_T("OnDemand updates for per-machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              OnDemandMachineAppsFallbackClass) {
  EXPECT_GU_ID_EQ(_T("{3282EB12-D954-4FD2-A2E1-C942C8745C65}"),
                  __uuidof(OnDemandMachineAppsFallbackClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandMachineAppsFallbackClass")));
  EXPECT_STREQ(_T("Fallback for if OnDemandMachineAppsServiceClass fails."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdateCoreClass) {
  EXPECT_GU_ID_EQ(_T("{3AD2D487-D166-4160-8E36-1AE505233A55}"),
                  __uuidof(GoogleUpdateCoreClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdateCoreClass")));
  EXPECT_STREQ(_T("GoogleUpdateCore Class"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaCustomizationGoopdateComInterfaceTest,
              GoogleUpdateCoreMachineClass) {
  EXPECT_GU_ID_EQ(_T("{F7FF255A-A593-41BD-A69B-E05D72B72756}"),
                  __uuidof(GoogleUpdateCoreMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdateCoreMachineClass")));
  EXPECT_STREQ(_T("GoogleUpdateCore Machine Class"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// Verifies there are no new interfaces in the TypeLib.
TEST_F(OmahaCustomizationGoopdateComInterfaceTest, VerifyNoNewInterfaces) {
  EXPECT_EQ(46, type_lib_->GetTypeInfoCount())
      << _T("A new interface may have been added. If so, add the interface to ")
      << _T("to kIIDsToRegister, and add test(s) for new interface(s).");
}

//
// Omaha 2 COM Interfaces.
//
// TODO(omaha): We should make it so open source versions do not need these
// legacy interfaces.

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IBrowserHttpRequest2) {
  EXPECT_GU_ID_EQ(_T("{AAE4AD28-500D-43BA-9F54-730CA146C190}"),
                  __uuidof(IBrowserHttpRequest2));
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IProcessLauncher) {
  EXPECT_GU_ID_EQ(_T("{70E5ECF5-2CA7-4019-9B23-916789A13C2C}"),
                  __uuidof(IProcessLauncher));
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IProgressWndEvents) {
  EXPECT_GU_ID_EQ(_T("{8504FB26-FC3E-4C1C-9C94-46EC93E6BA63}"),
                  __uuidof(IProgressWndEvents));
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IJobObserver) {
  EXPECT_GU_ID_EQ(_T("{7CFC4E00-1C9D-443D-B5BE-CEEEAC1443AF}"),
                  __uuidof(IJobObserver));
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IGoogleUpdate) {
  EXPECT_GU_ID_EQ(_T("{4C929BFE-4FA4-488D-B1E2-82ECD6F076C8}"),
                  __uuidof(IGoogleUpdate));
}

TEST_F(OmahaCustomizationGoopdateComInterfaceNoTypeLibTest,
       IGoogleUpdateCore) {
  EXPECT_GU_ID_EQ(_T("{10479D64-2C5F-46CD-9BC8-FD04FF4D02D8}"),
                  __uuidof(IGoogleUpdateCore));
}

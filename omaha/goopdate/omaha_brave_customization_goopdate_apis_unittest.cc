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

class OmahaBraveCustomizationGoopdateComInterfaceTest
    : public OmahaCustomizationTypeLibComInterfaceTest {
 protected:
  OmahaBraveCustomizationGoopdateComInterfaceTest()
      : OmahaCustomizationTypeLibComInterfaceTest(omaha::kOmahaDllName) {
  }
};

// Fixture for testing interfaces that are not in a TypeLib.
// We can only verify the uuid of the interfaces and classes.
class OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest
    : public testing::Test {
};

//
// Omaha 3 COM Constants.
//

namespace omaha {

// TODO(omaha): We should probably move these to a separate
// const_com_customization.h in goopdate\.
TEST(OmahaBraveCustomizationTest, Constants_ComProgIds) {
  EXPECT_STREQ(_T("BraveSoftwareUpdate.OnDemandCOMClassUser"), kProgIDOnDemandUser);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.OnDemandCOMClassMachine"),
                  kProgIDOnDemandMachine);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.OnDemandCOMClassSvc"), kProgIDOnDemandSvc);

  EXPECT_STREQ(_T("BraveSoftwareUpdate.Update3WebUser"), kProgIDUpdate3WebUser);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.Update3WebMachine"),
                  kProgIDUpdate3WebMachine);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.Update3WebSvc"), kProgIDUpdate3WebSvc);

  EXPECT_STREQ(_T("BraveSoftwareUpdate.CoreClass"), kProgIDGoogleUpdateCoreService);

  EXPECT_STREQ(_T("BraveSoftwareUpdate.ProcessLauncher"), kProgIDProcessLauncher);

  EXPECT_STREQ(_T("BraveSoftwareUpdate.PolicyStatusUser"), kProgIDPolicyStatusUser);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.PolicyStatusMachine"),
                  kProgIDPolicyStatusMachine);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.PolicyStatusMachineFallback"),
                  kProgIDPolicyStatusMachineFallback);
  EXPECT_STREQ(_T("BraveSoftwareUpdate.PolicyStatusSvc"), kProgIDPolicyStatusSvc);
}

}  // namespace omaha

//
// Omaha 3 COM Interfaces Enums.
//

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest, BrowserType) {
  EXPECT_EQ(0, BROWSER_UNKNOWN);
  EXPECT_EQ(1, BROWSER_DEFAULT);
  EXPECT_EQ(2, BROWSER_INTERNET_EXPLORER);
  EXPECT_EQ(3, BROWSER_FIREFOX);
  EXPECT_EQ(4, BROWSER_CHROME);
}

// There are two different BrowserType definitions, one in the IDL and one
// in browser_utils. Verify they are identical.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       BrowserType_DefinitionsMatch) {
  EXPECT_EQ(::BROWSER_UNKNOWN,            omaha::BROWSER_UNKNOWN);
  EXPECT_EQ(::BROWSER_DEFAULT,            omaha::BROWSER_DEFAULT);
  EXPECT_EQ(::BROWSER_INTERNET_EXPLORER,  omaha::BROWSER_IE);
  EXPECT_EQ(::BROWSER_FIREFOX,            omaha::BROWSER_FIREFOX);
  EXPECT_EQ(::BROWSER_CHROME,             omaha::BROWSER_CHROME);

  EXPECT_EQ(::BROWSER_CHROME + 1, omaha::BROWSER_MAX)
      << _T("A browser has been added without updating test and/or the IDL");
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest, CurrentState) {
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

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest, InstallPriority) {
  EXPECT_EQ(0,  INSTALL_PRIORITY_LOW);
  EXPECT_EQ(10, INSTALL_PRIORITY_HIGH);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest, PostInstallAction) {
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

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, TypeLib) {
  EXPECT_ID_EQ(_T("{6FF4F468-0B5A-4878-ACEB-86263DAAD877}"),
                  LIBID_GoogleUpdate3Lib);

  EXPECT_SUCCEEDED(GetTypeLibDocumentation());
  EXPECT_STREQ(_T("GoogleUpdate3Lib"), item_name_);
  EXPECT_STREQ(_T("Omaha 3.0 Type Library"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IGoogleUpdate3) {
  // TODO(omaha): Test uuid constants after extracting from IDLs.
  EXPECT_ID_EQ(_T("{7CB305B1-4D45-4668-AD91-677F87BED305}"),
                  __uuidof(IGoogleUpdate3));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IGoogleUpdate3")));
  EXPECT_STREQ(_T("IGoogleUpdate3 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// The IAppBundle interface name does not change for non-Google builds, but the
// ID must. The same is true for many of the interfaces.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppBundle) {
  EXPECT_ID_EQ(_T("{1985533F-9B0F-490A-85C5-24F316E66FB2}"),
                  __uuidof(IAppBundle));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppBundle")));
  EXPECT_STREQ(_T("IAppBundle Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// This appears in the typelib for unknown reasons.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, ULONG_PTR) {
  EXPECT_SUCCEEDED(GetDocumentation(_T("ULONG_PTR")));
  EXPECT_TRUE(!item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IApp) {
  EXPECT_ID_EQ(_T("{A4BCDF52-2179-4C77-8C5F-B8095712B563}"),
                  __uuidof(IApp));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IApp")));
  EXPECT_STREQ(_T("IApp Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IApp2) {
  EXPECT_ID_EQ(_T("{A147722A-5568-4B84-B401-86D744470CBF}"),
                  __uuidof(IApp2));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IApp2")));
  EXPECT_STREQ(_T("IApp2 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppCommand) {
  EXPECT_ID_EQ(_T("{4842EC21-0860-45B5-99F0-A1E6E7C11561}"),
                  __uuidof(IAppCommand));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppCommand")));
  EXPECT_STREQ(_T("IAppCommand Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppCommand2) {
  EXPECT_ID_EQ(_T("{C11C073F-E6D0-4EF7-897B-AAF52498CD2F}"),
                  __uuidof(IAppCommand2));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppCommand2")));
  EXPECT_STREQ(_T("IAppCommand2 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppVersion) {
  EXPECT_ID_EQ(_T("{F234546B-DACD-4374-97CF-7BADFAB76766}"),
                  __uuidof(IAppVersion));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppVersion")));
  EXPECT_STREQ(_T("IAppVersion Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IPackage) {
  EXPECT_ID_EQ(_T("{C663DEBB-F082-4971-9F6E-35DE45C96F4E}"),
                  __uuidof(IPackage));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPackage")));
  EXPECT_STREQ(_T("IPackage Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, ICurrentState) {
  EXPECT_ID_EQ(_T("{E6836CFF-5949-44BC-B6BE-9C8C48DD8D97}"),
                  __uuidof(ICurrentState));

  EXPECT_SUCCEEDED(GetDocumentation(_T("ICurrentState")));
  EXPECT_STREQ(_T("ICurrentState Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IRegistrationUpdateHook) {
  EXPECT_ID_EQ(_T("{48B5E6B2-9383-4B1E-AAE7-720C4779ABA6}"),
                  __uuidof(IRegistrationUpdateHook));
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest, ICoCreateAsync) {
  EXPECT_ID_EQ(_T("{DD84E356-3D21-44C8-83DD-6BEEC22FA427}"),
                  __uuidof(ICoCreateAsync));
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest, ICredentialDialog) {
  EXPECT_ID_EQ(_T("{660130E8-74E4-4821-A6FD-4E9A86E06470}"),
                  __uuidof(ICredentialDialog));
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IPolicyStatus) {
  EXPECT_ID_EQ(_T("{10DB7BD5-BD0B-4886-9705-174203FE0ADA}"),
                  __uuidof(IPolicyStatus));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatus")));
  EXPECT_STREQ(_T("IPolicyStatus Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IPolicyStatusValue) {
  EXPECT_ID_EQ(_T("{931E73FD-D487-4458-AA08-1FF41413377B}"),
                  __uuidof(IPolicyStatusValue));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatusValue")));
  EXPECT_STREQ(_T("IPolicyStatusValue Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IPolicyStatus2) {
  EXPECT_ID_EQ(_T("{EFF9CA12-4CD3-474B-B881-CDE1D92F1996}"),
                  __uuidof(IPolicyStatus2));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatus2")));
  EXPECT_STREQ(_T("IPolicyStatus2 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IPolicyStatus3) {
  EXPECT_ID_EQ(_T("{C974F2DD-CFB8-4466-8E6D-96ED901DAACA}"),
                  __uuidof(IPolicyStatus3));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IPolicyStatus3")));
  EXPECT_STREQ(_T("IPolicyStatus3 Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IGoogleUpdate3Web) {
  EXPECT_ID_EQ(_T("{C9190589-ECEC-43F8-8AEC-62496BB87B26}"),
                  __uuidof(IGoogleUpdate3Web));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IGoogleUpdate3Web")));
  EXPECT_STREQ(_T("IGoogleUpdate3Web Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// Not in the TypeLib because it derives from IUnknown.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IGoogleUpdate3WebSecurity) {
  EXPECT_ID_EQ(_T("{24D704AD-AC42-49F2-BB4F-68BA77C98E91}"),
                  __uuidof(IGoogleUpdate3WebSecurity));
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppBundleWeb) {
  EXPECT_ID_EQ(_T("{852A0F87-D117-4B7C-ABA9-2F76D91BCB9D}"),
                  __uuidof(IAppBundleWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppBundleWeb")));
  EXPECT_STREQ(_T("IAppBundleWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppWeb) {
  EXPECT_ID_EQ(_T("{FB43AAD0-DDBA-4D01-A3E0-FAB100E7926B}"),
                  __uuidof(IAppWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppWeb")));
  EXPECT_STREQ(_T("IAppWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppCommandWeb) {
  EXPECT_ID_EQ(_T("{19F4616B-B7DD-4B3F-8084-C81C5C77AAA4}"),
                  __uuidof(IAppCommandWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppCommandWeb")));
  EXPECT_STREQ(_T("IAppCommandWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, IAppVersionWeb) {
  EXPECT_ID_EQ(_T("{35A4470F-5EEC-4715-A2DC-6AA9F8E21183}"),
                  __uuidof(IAppVersionWeb));

  EXPECT_SUCCEEDED(GetDocumentation(_T("IAppVersionWeb")));
  EXPECT_STREQ(_T("IAppVersionWeb Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, ICoCreateAsyncStatus) {
  EXPECT_ID_EQ(_T("{7A24060E-533F-4962-9E15-34BD82555FA7}"),
                  __uuidof(ICoCreateAsyncStatus));

  EXPECT_SUCCEEDED(GetDocumentation(_T("ICoCreateAsyncStatus")));
  EXPECT_STREQ(_T("ICoCreateAsyncStatus Interface"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3UserClass) {
  EXPECT_ID_EQ(_T("{2CF4A521-EC1E-4075-A20B-5DE3E5B89B1B}"),
                  __uuidof(GoogleUpdate3UserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3UserClass")));
  EXPECT_STREQ(_T("GoogleUpdate3 Class for per-user applications"),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3ServiceClass) {
  EXPECT_ID_EQ(_T("{08F15E98-0442-45D3-82F1-F67495CC51EB}"),
                  __uuidof(GoogleUpdate3ServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3ServiceClass")));
  EXPECT_STREQ(_T("GoogleUpdate3 Service Class for machine applications"),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebUserClass) {
  EXPECT_ID_EQ(_T("{2F78AECB-0A7F-4474-89D5-C325293DE960}"),
                  __uuidof(GoogleUpdate3WebUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3WebUserClass")));
  EXPECT_STREQ(_T("GoogleUpdate3Web for user applications"),
                  item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebMachineClass) {
  EXPECT_ID_EQ(_T("{00B16F95-319A-4F01-AC81-CE69B8F4E387}"),
                  __uuidof(GoogleUpdate3WebMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3WebMachineClass")));
  EXPECT_STREQ(
      _T("Pass-through broker for the GoogleUpdate3WebServiceClass"),
      item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebServiceClass) {
  EXPECT_ID_EQ(_T("{3A9D7221-2278-41DD-930B-C2356B7D3725}"),
                  __uuidof(GoogleUpdate3WebServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdate3WebServiceClass")));
  EXPECT_STREQ(_T("GoogleUpdate3Web"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdate3WebMachineFallbackClass) {
  EXPECT_ID_EQ(_T("{66CE3D6C-0B35-4F78-AC77-39728A75CB75}"),
                  __uuidof(GoogleUpdate3WebMachineFallbackClass));

  EXPECT_SUCCEEDED(
      GetDocumentation(_T("GoogleUpdate3WebMachineFallbackClass")));
  EXPECT_STREQ(L"Fallback mechanism if GoogleUpdate3WebServiceClass fails",
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              CurrentStateUserClass) {
  EXPECT_ID_EQ(_T("{06B74C04-E813-4DD4-A972-172836EFA8D6}"),
                  __uuidof(CurrentStateUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CurrentStateUserClass")));
  EXPECT_STREQ(_T("CurrentStateUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              CurrentStateMachineClass) {
  EXPECT_ID_EQ(_T("{BC03C0E4-1528-4299-89B2-419644FA48AC}"),
                  __uuidof(CurrentStateMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CurrentStateMachineClass")));
  EXPECT_STREQ(_T("CurrentStateMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              CoCreateAsyncClass) {
  EXPECT_ID_EQ(_T("{8F6D9FE5-6ED3-43A3-80D2-EA8766D65352}"),
                  __uuidof(CoCreateAsyncClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CoCreateAsyncClass")));
  EXPECT_STREQ(_T("CoCreateAsyncClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              CredentialDialogUserClass) {
  EXPECT_ID_EQ(_T("{4C738ABE-2ED7-4BF6-B668-046DC1A72C00}"),
                  __uuidof(CredentialDialogUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CredentialDialogUserClass")));
  EXPECT_STREQ(_T("CredentialDialogUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              CredentialDialogMachineClass) {
  EXPECT_ID_EQ(_T("{B5D1924F-CB80-47AA-8DEC-5E0854A42A73}"),
                  __uuidof(CredentialDialogMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("CredentialDialogMachineClass")));
  EXPECT_STREQ(_T("CredentialDialogMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              PolicyStatusValueUserClass) {
  EXPECT_ID_EQ(_T("{F6E536F5-F795-49CE-A85D-2DA66503C6F1}"),
                  __uuidof(PolicyStatusValueUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusValueUserClass")));
  EXPECT_STREQ(_T("PolicyStatusValueUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              PolicyStatusValueMachineClass) {
  EXPECT_ID_EQ(_T("{EBDA5D88-AA7D-4A8C-A20C-C01FADB43EDA}"),
                  __uuidof(PolicyStatusValueMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusValueMachineClass")));
  EXPECT_STREQ(_T("PolicyStatusValueMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              PolicyStatusUserClass) {
  EXPECT_ID_EQ(_T("{02FA9A9C-3856-48A8-A62B-F898C64E45C5}"),
                  __uuidof(PolicyStatusUserClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusUserClass")));
  EXPECT_STREQ(_T("Policy Status for per-user applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              PolicyStatusMachineClass) {
  EXPECT_ID_EQ(_T("{598BBE98-5919-4392-B62A-50D7115F10A3}"),
                  __uuidof(PolicyStatusMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusMachineClass")));
  EXPECT_STREQ(_T("Policy Status pass-through broker ")
               _T("for machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              PolicyStatusMachineServiceClass) {
  EXPECT_ID_EQ(_T("{13B35483-DF37-4603-97F8-9504E48B49BF}"),
                  __uuidof(PolicyStatusMachineServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusMachineServiceClass")));
  EXPECT_STREQ(_T("Policy Status for per-machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              PolicyStatusMachineFallbackClass) {
  EXPECT_ID_EQ(_T("{652886FF-517B-4F23-A14F-F99563A04BCC}"),
                  __uuidof(PolicyStatusMachineFallbackClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("PolicyStatusMachineFallbackClass")));
  EXPECT_STREQ(_T("Fallback for if PolicyStatusMachineServiceClass fails."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleComProxyMachineClass) {
  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleComProxyMachineClass")));
  EXPECT_STREQ(_T("GoogleComProxyMachineClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleComProxyUserClass) {
  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleComProxyUserClass")));
  EXPECT_STREQ(_T("GoogleComProxyUserClass"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              ProcessLauncherClass) {
  EXPECT_ID_EQ(_T("{4C3BA8F3-1264-4BDB-BB2D-CA44734AD00D}"),
                  __uuidof(ProcessLauncherClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("ProcessLauncherClass")));
  EXPECT_STREQ(_T("ProcessLauncherClass Class"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              OnDemandUserAppsClass) {
  EXPECT_ID_EQ(_T("{6DA8171A-7CEC-4E45-BDED-9F287AD7C854}"),
                  __uuidof(OnDemandUserAppsClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandUserAppsClass")));
  EXPECT_STREQ(_T("OnDemand updates for per-user applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              OnDemandMachineAppsClass) {
  EXPECT_ID_EQ(_T("{28C83F57-E4C0-4B54-B187-585C51EE8F9C}"),
                  __uuidof(OnDemandMachineAppsClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandMachineAppsClass")));
  EXPECT_STREQ(_T("OnDemand pass-through broker for machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              OnDemandMachineAppsServiceClass) {
  EXPECT_ID_EQ(_T("{D7D7525F-5DF4-4C9D-8781-C02F39F973E6}"),
                  __uuidof(OnDemandMachineAppsServiceClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandMachineAppsServiceClass")));
  EXPECT_STREQ(_T("OnDemand updates for per-machine applications."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              OnDemandMachineAppsFallbackClass) {
  EXPECT_ID_EQ(_T("{3282EB12-D954-4FD2-A2E1-C942C8745C65}"),
                  __uuidof(OnDemandMachineAppsFallbackClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("OnDemandMachineAppsFallbackClass")));
  EXPECT_STREQ(_T("Fallback for if OnDemandMachineAppsServiceClass fails."),
               item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdateCoreClass) {
  EXPECT_ID_EQ(_T("{3AD2D487-D166-4160-8E36-1AE505233A55}"),
                  __uuidof(GoogleUpdateCoreClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdateCoreClass")));
  EXPECT_STREQ(_T("GoogleUpdateCore Class"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

TEST_GU_INT_F(OmahaBraveCustomizationGoopdateComInterfaceTest,
              GoogleUpdateCoreMachineClass) {
  EXPECT_ID_EQ(_T("{F7FF255A-A593-41BD-A69B-E05D72B72756}"),
                  __uuidof(GoogleUpdateCoreMachineClass));

  EXPECT_SUCCEEDED(GetDocumentation(_T("GoogleUpdateCoreMachineClass")));
  EXPECT_STREQ(_T("GoogleUpdateCore Machine Class"), item_doc_string_);
  EXPECT_EQ(0, help_context_);
  EXPECT_TRUE(!help_file_);
}

// Verifies there are no new interfaces in the TypeLib.
TEST_F(OmahaBraveCustomizationGoopdateComInterfaceTest, VerifyNoNewInterfaces) {
  EXPECT_EQ(46, type_lib_->GetTypeInfoCount())
      << _T("A new interface may have been added. If so, add the interface to ")
      << _T("to kIIDsToRegister, and add test(s) for new interface(s).");
}

//
// Omaha 2 COM Interfaces.
//
// TODO(omaha): We should make it so open source versions do not need these
// legacy interfaces.

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IBrowserHttpRequest2) {
  EXPECT_ID_EQ(_T("{AAE4AD28-500D-43BA-9F54-730CA146C190}"),
                  __uuidof(IBrowserHttpRequest2));
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IProcessLauncher) {
  EXPECT_ID_EQ(_T("{70E5ECF5-2CA7-4019-9B23-916789A13C2C}"),
                  __uuidof(IProcessLauncher));
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IProgressWndEvents) {
  EXPECT_ID_EQ(_T("{8504FB26-FC3E-4C1C-9C94-46EC93E6BA63}"),
                  __uuidof(IProgressWndEvents));
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IJobObserver) {
  EXPECT_ID_EQ(_T("{7CFC4E00-1C9D-443D-B5BE-CEEEAC1443AF}"),
                  __uuidof(IJobObserver));
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IGoogleUpdate) {
  EXPECT_ID_EQ(_T("{4C929BFE-4FA4-488D-B1E2-82ECD6F076C8}"),
                  __uuidof(IGoogleUpdate));
}

TEST_F(OmahaBraveCustomizationGoopdateComInterfaceNoTypeLibTest,
       IGoogleUpdateCore) {
  EXPECT_ID_EQ(_T("{10479D64-2C5F-46CD-9BC8-FD04FF4D02D8}"),
                  __uuidof(IGoogleUpdateCore));
}

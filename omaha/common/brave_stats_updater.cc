/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "omaha/common/brave_stats_updater.h"

#include "omaha/base/omaha_version.h"
#include "omaha/base/safe_format.h"
#include "omaha/base/utils.h"
#include "omaha/net/network_config.h"
#include "omaha/net/network_request.h"
#include "omaha/net/simple_request.h"

namespace omaha {

namespace {

bool ParseStandardReferralCode(const TCHAR* filename, CString& referral_code) {
  // Scan backwards for last dash in filename.
  const TCHAR* scan = filename + lstrlen(filename) - 1;
  while (scan != filename && *scan != _T('-'))
    --scan;

  if (*scan++ != _T('-'))
    return false;

  // Standard referral code is 6 characters.
  const TCHAR* ref_code = scan;
  if (lstrlen(ref_code) != 6)
    return false;

  // Ensure that first half of referral code is alphabetic.
  for (int i = 0; i < 3; ++i) {
    if ((ref_code[i] < _T('a') || ref_code[i] > _T('z')) &&
        (ref_code[i] < _T('A') || ref_code[i] > _T('Z')))
      return false;
  }

  // Ensure that second half of referral code is numeric.
  for (int i = 3; i < 6; ++i) {
    if (ref_code[i] < _T('0') || ref_code[i] > _T('9'))
      return false;
  }

  referral_code = ref_code;

  return true;
}

bool ParseExtendedReferralCode(const TCHAR* filename, CString& referral_code) {
  // Scan backwards for second-to-last dash in filename, since this
  // type of referral code has an embedded dash.
  const TCHAR* scan = filename + lstrlen(filename) - 1;
  while (scan != filename && *scan != _T('-'))
    --scan;

  if (*scan-- != _T('-'))
    return false;

  while (scan != filename && *scan != _T('-'))
    --scan;

  if (*scan++ != _T('-'))
    return false;

  // Ensure that referral code is alphabetic.
  const TCHAR* ref_code = scan;
  int dashes = 0;
  for (int i = 0; i < lstrlen(ref_code); ++i) {
    if ((ref_code[i] < _T('a') || ref_code[i] > _T('z')) &&
        (ref_code[i] < _T('A') || ref_code[i] > _T('Z')) &&
        (ref_code[i] != _T('-')))
      return NULL;
    if (ref_code[i] == _T('-'))
      ++dashes;
  }

  // Ensure that referral code contains exactly one dash.
  if (dashes != 1)
    return false;

  referral_code = ref_code;
  return true;
}

bool ParseReferralCode(const TCHAR* installer_filename,
                       CString& referral_code) {
  TCHAR filename[MAX_PATH];
  lstrcpyn(filename, installer_filename, MAX_PATH);

  // Strip path from filename.
  PathStripPath(filename);

  // Strip extension from filename.
  PathRemoveExtension(filename);

  // Strip any de-duplicating suffix from filename, e.g. "(1)".
  const TCHAR* scan = filename + lstrlen(filename) - 1;
  if (*scan == _T(')')) {
    --scan;
    while (scan != filename && *scan >= _T('0') && *scan <= _T('9'))
      --scan;
    if (*scan == _T('('))
      filename[scan - filename] = '\0';
  }

  // Strip trailing spaces from filename.
  scan = filename + lstrlen(filename) - 1;
  while (scan != filename && *scan == _T(' '))
    --scan;

  if (scan != filename && (scan != filename + lstrlen(filename)))
    filename[scan - filename + 1];

  // First check for 6-character standard referral code XXXDDD, where
  // X is an alphabetic character and D is a numeric character. If not
  // found, check for an alphabetic referral code of any length in the
  // form XXX-XXX.
  if (!ParseStandardReferralCode(filename, referral_code) &&
      !ParseExtendedReferralCode(filename, referral_code)) {
    return false;
  }

  return true;
}

CString ReadAppGuidFromReg() {
  HKEY key;
  int res =
      RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\BraveSoftware\\Installer"),
                   0, KEY_READ, &key);
  if (res != ERROR_SUCCESS) {
    return _T("");
  }

  DWORD bytes = 0;
  DWORD type = REG_SZ;
  res = ::RegQueryValueEx(key, _T("AppGuid"), 0, &type, NULL, &bytes);
  if (!(res == ERROR_SUCCESS && type == REG_SZ)) {
    return _T("");
  }

  scoped_array<byte> buf(new byte[bytes]);
  memset(buf.get(), 0, bytes);
  res = ::RegQueryValueEx(key, _T("AppGuid"), 0, NULL, buf.get(), &bytes);
  if (res != ERROR_SUCCESS) {
    return _T("");
  }

  RegCloseKey(key);

  return reinterpret_cast<const wchar_t*>(buf.get());
}

CString ReadStubInstallerPathFromReg() {
  HKEY key;
  int res =
      RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\BraveSoftware\\Promo"), 0,
                   KEY_READ, &key);
  if (res != ERROR_SUCCESS) {
    return _T("");
  }

  DWORD bytes = 0;
  DWORD type = REG_SZ;
  res = ::RegQueryValueEx(key, _T("StubInstallerPath"), 0, &type, NULL, &bytes);
  if (!(res == ERROR_SUCCESS && type == REG_SZ)) {
    return _T("");
  }

  scoped_array<byte> buf(new byte[bytes]);
  memset(buf.get(), 0, bytes);
  res = ::RegQueryValueEx(key, _T("StubInstallerPath"), 0, NULL, buf.get(),
                          &bytes);
  if (res != ERROR_SUCCESS) {
    return _T("");
  }

  RegCloseKey(key);

  return reinterpret_cast<const wchar_t*>(buf.get());
}

bool IsReferralCodeWhiteListed(const CString& referral_code) {
  static std::vector<CString> whitelist({
      CString(_T("ABC123")),
      CString(_T("XYZ123")),
  });
  return std::any_of(whitelist.begin(), whitelist.end(),
                     [&referral_code](CString candidate) {
                       return candidate == referral_code;
                     });
}

CString GetChannelName(const CString& app_guid) {
  CString app_guid_mutable = app_guid;
  if (app_guid_mutable.IsEmpty())
    app_guid_mutable = ReadAppGuidFromReg();

  if (app_guid_mutable == _T("{AFE6A462-C574-4B8A-AF43-4CC60DF4563B}"))
    return _T("release");
  else if (app_guid_mutable == _T("{103BD053-949B-43A8-9120-2E424887DE11}"))
    return _T("beta");
  else if (app_guid_mutable == _T("{CB2150F2-595F-4633-891A-E39720CE0531}"))
    return _T("dev");
  else if (app_guid_mutable == _T("{C6CB981E-DB30-4876-8639-109F8933582C}"))
    return _T("nightly");
  else
    return _T("");
}

CString GetBraveVersion(const CString& version) {
  // Strip first segment from version number.
  int pos = version.Find(_T('.'));
  if (pos != -1)
    return version.Right(version.GetLength() - pos - 1);
  return version;
}

CString GetReferralCode() {
  CString referral_code;
  CString stub_installer_path = ReadStubInstallerPathFromReg();
  if (!ParseReferralCode(stub_installer_path, referral_code))
    return _T("none");

  if (!IsReferralCodeWhiteListed(referral_code))
    return _T("none");

  return referral_code;
}

}  // namespace

HRESULT BraveSendStatsPing(const CString& event, const CString& app_guid,
                           const CString& version) {
  // Retrieve the update server name.
  CString update_server =
      omaha::GetEnvironmentVariableAsString(_T("BRAVE_UPDATE_SERVER"));
  if (update_server.IsEmpty())
    update_server = _T("laptop-updates.brave.com");

  _SYSTEM_INFO system_info = {};
  ::GetNativeSystemInfo(&system_info);

  // Ensure that channel is valid/supported.
  CString channel_name = GetChannelName(app_guid);
  if (channel_name.IsEmpty())
    return E_INVALIDARG;

  // Build the update url.
  CString url;
  omaha::SafeCStringAppendFormat(
      &url,
      _T("https://%s/api/1/")
      _T("installEvents?platform=%s&version=%s&channel=%s&ref=%s&event=%s"),
      update_server,
      system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL
          ? _T("winia32")
          : _T("winx64"),
      GetBraveVersion(version), channel_name, GetReferralCode(), event);

  omaha::NetworkConfig* network_config = NULL;
  omaha::NetworkConfigManager& network_manager =
      omaha::NetworkConfigManager::Instance();
  HRESULT hr = network_manager.GetUserNetworkConfig(&network_config);
  if (FAILED(hr))
    return hr;

  omaha::NetworkRequest network_request(network_config->session());

  network_request.set_num_retries(1);
  network_request.AddHttpRequest(new omaha::SimpleRequest);

  std::vector<uint8> response_buffer;
  hr = network_request.Get(url, &response_buffer);
  if (FAILED(hr))
    return hr;

  return S_OK;
}

}  // namespace omaha

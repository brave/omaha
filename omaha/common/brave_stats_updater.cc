/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "omaha/common/brave_stats_updater.h"

#include "omaha/base/safe_format.h"
#include "omaha/base/utils.h"
#include "omaha/net/network_config.h"
#include "omaha/net/network_request.h"
#include "omaha/net/simple_request.h"

namespace omaha {

namespace {

CString GetChannelName(const CString& app_guid) {
  if (app_guid == _T("{AFE6A462-C574-4B8A-AF43-4CC60DF4563B}"))
    return _T("release");
  else if (app_guid == _T("{103BD053-949B-43A8-9120-2E424887DE11}"))
    return _T("beta");
  else if (app_guid == _T("{CB2150F2-595F-4633-891A-E39720CE0531}"))
    return _T("dev");
  else if (app_guid == _T("{C6CB981E-DB30-4876-8639-109F8933582C}"))
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

}  // namespace

HRESULT BraveSendStatsPing(const CString& event, const CString& app_guid,
                           const CString& referral_code,
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
  if (channel_name.IsEmpty()) {
    CORE_LOG(LE, (_T("[Channel name is empty][%s]"), app_guid));
    return E_INVALIDARG;
  }

  // Build the update url.
  CString url;
  omaha::SafeCStringAppendFormat(
      &url,
      _T("https://%s/api/1/")
      _T("installEvents?platform=%s&version=%s&channel=%s&ref=%s&event=%s"),
      update_server,
      system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL
          ? _T("winia32-bc")
          : _T("winx64-bc"),
      GetBraveVersion(version), channel_name, referral_code, event);

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

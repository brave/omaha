/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef OMAHA_COMMON_BRAVE_STATS_UPDATER_H__
#define OMAHA_COMMON_BRAVE_STATS_UPDATER_H__

#include <atlstr.h>

namespace omaha {

HRESULT BraveSendStatsPing(const CString& event, const CString& app_guid,
                           const CString& referral_code,
                           const CString& version);

} // namespace omaha

#endif  // OMAHA_COMMON_BRAVE_STATS_UPDATER_H__

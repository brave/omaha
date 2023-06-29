/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "omaha/common/brave_referral_code_utils.h"

#include <algorithm>
#include <vector>

namespace omaha {

namespace {

bool ParseCustomReferralCode(const TCHAR* filename, CString& referral_code) {
  // Scan backwards for last dash in filename.
  const TCHAR* scan = filename + lstrlen(filename) - 1;
  while (scan != filename && *scan != _T('-'))
    --scan;

  if (*scan++ != _T('-'))
    return false;

  // Standard referral code is 6 characters.
  const TCHAR* ref_code = scan;

  if (lstrlen(ref_code) < 5 || lstrlen(ref_code) > 15)
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

  if (!ParseCustomReferralCode(filename, referral_code)) {
    return false;
  }

  return true;
}

}  // namespace

CString GetReferralCode(const CString& module_file_name) {
  CString referral_code;
  if (!ParseReferralCode(module_file_name, referral_code))
    return _T("none");

  referral_code.MakeUpper();

  return referral_code;
}

}  // namespace omaha

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "omaha/common/brave_referral_code_utils.h"

#include <algorithm>
#include <vector>

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

bool IsReferralCodeWhiteListed(const CString& referral_code) {
  static std::vector<CString> whitelist(
      {// ISOptOut2
       CString(_T("CTD238")), CString(_T("OJV851")), CString(_T("DPM796")),
       CString(_T("BNI512")), CString(_T("FEB604")), CString(_T("YDQ106")),
       CString(_T("NFF966")), CString(_T("FQW627")), CString(_T("AMV588")),
       CString(_T("YZR853")), CString(_T("IUX155")), CString(_T("DFQ107")),
       CString(_T("ZWV410")), CString(_T("ZTT758")), CString(_T("EPH628")),
       CString(_T("DKD200")), CString(_T("WIR635")), CString(_T("YHC941")),
       CString(_T("OPV062")), CString(_T("MTS962")), CString(_T("PSN487")),
       CString(_T("MZJ289")), CString(_T("EBC998")), CString(_T("KWZ833")),
       CString(_T("YQI537")), CString(_T("BUE553")), CString(_T("UGO473")),
       CString(_T("CFT923")), CString(_T("WKG906")), CString(_T("OXU789")),
       CString(_T("VGT997")), CString(_T("MOU235")), CString(_T("WLJ467")),
       CString(_T("YXG330")), CString(_T("RNH069")), CString(_T("LRT088")),
       CString(_T("BEM856")), CString(_T("DSL157")), CString(_T("ZGL739")),
       CString(_T("MZX864")), CString(_T("GTW616")), CString(_T("YBX790")),
       CString(_T("LJT611")), CString(_T("FST304")), CString(_T("TVJ301")),
       CString(_T("RMB962")), CString(_T("NKZ324")),

       // STTier1
       CString(_T("FBI093")), CString(_T("VNI569")), CString(_T("GMM900")),
       CString(_T("NOQ491")), CString(_T("IOB292")), CString(_T("JJE625")),
       CString(_T("FUX638")), CString(_T("MPO928")), CString(_T("LYD303")),
       CString(_T("TJF413")), CString(_T("MQP631")), CString(_T("ILY758")),
       CString(_T("VBQ225")), CString(_T("UQS362")),

       // GS
       CString(_T("PJJ948")), CString(_T("UIH408")), CString(_T("KXC376")),
       CString(_T("OZD582")), CString(_T("JDT909")), CString(_T("IJZ122")),
       CString(_T("OIE359")), CString(_T("DLQ981")), CString(_T("WIX525")),
       CString(_T("PZH825")), CString(_T("NGY511")), CString(_T("QXS120")),
       CString(_T("XTA152")), CString(_T("EZM037")),

       // JAc
       CString(_T("DHU083")), CString(_T("UGI415")), CString(_T("RQH046")),
       CString(_T("MEB961")),

       // Additional
       CString(_T("KJP000")), CString(_T("UNQ913")), CString(_T("KBJ557")),
       CString(_T("BOA207")), CString(_T("BYJ598")), CString(_T("ARO656")),
       CString(_T("EDE311")), CString(_T("RZD797")), CString(_T("QPE677")),
       CString(_T("RPW794")), CString(_T("TBD002")), CString(_T("BTZ601")),
       CString(_T("DRR783")), CString(_T("RMB905")), CString(_T("ZVI549")),
       CString(_T("DZF201")), CString(_T("RHI430")), CString(_T("XOX898")),
       CString(_T("WGN943")), CString(_T("OSB729")), CString(_T("JWA535")),
       CString(_T("TOT965")), CString(_T("NWN118")), CString(_T("YHS197")),
       CString(_T("IBU843")), CString(_T("UUD854")), CString(_T("ITO318")),
       CString(_T("TQD211")), CString(_T("SFM009")), CString(_T("ERQ913")),
       CString(_T("VZF120")), CString(_T("WIV076")), CString(_T("LWP706")),
       CString(_T("XMW172")), CString(_T("OOX752")), CString(_T("VWK786")),
       CString(_T("HQK918")), CString(_T("IXX292")), CString(_T("APG060")),
       CString(_T("KSU863")), CString(_T("ANM246")), CString(_T("OVK455")),
       CString(_T("TSM531")), CString(_T("ZMC689")), CString(_T("XPO114")),
       CString(_T("TJK021")), CString(_T("MTB027")), CString(_T("BWC385")),
       CString(_T("BSB375")), CString(_T("BXV384")), CString(_T("SLY677")),
       CString(_T("OIQ043")), CString(_T("DWC715")), CString(_T("CCU246")),
       CString(_T("UPQ934")), CString(_T("PRI875")), CString(_T("UQN934")),
       CString(_T("ASY224")), CString(_T("NUB585")), CString(_T("OLS686")),
       CString(_T("QLM391")), CString(_T("PNE044")), CString(_T("PPD370")),
       CString(_T("OOJ613")), CString(_T("EZM787")), CString(_T("NRX711")),
       CString(_T("APV192")), CString(_T("BHD893")), CString(_T("ERX748")),
       CString(_T("CBW249")), CString(_T("HYJ986")), CString(_T("CVB741")),
       CString(_T("YIZ978")), CString(_T("KYW570")), CString(_T("KVC600")),
       CString(_T("GMV203")), CString(_T("DVZ178")), CString(_T("IFN767")),
       CString(_T("YPH104")), CString(_T("UNK157")), CString(_T("NGU383")),
       CString(_T("BXN418")), CString(_T("RBW016")), CString(_T("ZQM087")),
       CString(_T("TRR894")), CString(_T("IXQ557")), CString(_T("TNW414")),
       CString(_T("QAA606")), CString(_T("OCA003")), CString(_T("IOJ320")),
       CString(_T("ZIQ953")), CString(_T("ZJN514")), CString(_T("XMZ986")),
       CString(_T("HLK526")), CString(_T("CCQ716")), CString(_T("LIR950")),
       CString(_T("JRN526")), CString(_T("TZS401")), CString(_T("DSE071")),
       CString(_T("CJO449")), CString(_T("ABO677")), CString(_T("GFQ506")),
       CString(_T("ZWR105")), CString(_T("VBC538")), CString(_T("IQC112")),
       CString(_T("FBW502")), CString(_T("XPM257")), CString(_T("MSG315")),
       CString(_T("ORX404")), CString(_T("UEO521")), CString(_T("EKB849")),
       CString(_T("ERW274")), CString(_T("ZYB215")), CString(_T("OOB354")),
       CString(_T("UTD029")), CString(_T("OZC828")), CString(_T("HXM441")),
       CString(_T("LSS305")), CString(_T("KJD945")), CString(_T("SOB703")),
       CString(_T("CCJ078")), CString(_T("QFE427")), CString(_T("CZQ141")),
       CString(_T("DFG223")), CString(_T("SIV168")), CString(_T("XER314")),
       CString(_T("EHH775")), CString(_T("MMT745")), CString(_T("SOB084")),
       CString(_T("HFS553")), CString(_T("YZV909")), CString(_T("QJQ727")),
       CString(_T("HQL833")), CString(_T("BSC752")), CString(_T("APN295")),
       CString(_T("AAV730")), CString(_T("YXT225")), CString(_T("CHY006")),
       CString(_T("CEL792")), CString(_T("VIW485")), CString(_T("SBW951")),
       CString(_T("MJM666")), CString(_T("DPX255")), CString(_T("JQP631")),
       CString(_T("NHX686")), CString(_T("YAB346")), CString(_T("RDS304")),
       CString(_T("FFL997")), CString(_T("UPF883")), CString(_T("JYK534")),
       CString(_T("OIL401")), CString(_T("CSY475")), CString(_T("MGD911")),
       CString(_T("ESP177")), CString(_T("FEX878")), CString(_T("LDD929")),
       CString(_T("XOB016"))});
  return std::any_of(whitelist.begin(), whitelist.end(),
                     [&referral_code](CString candidate) {
                       return candidate == referral_code;
                     });
}

}  // namespace

CString GetReferralCode(const CString& module_file_name) {
  CString referral_code;
  if (!ParseReferralCode(module_file_name, referral_code))
    return _T("none");

  referral_code.MakeUpper();

  if (!IsReferralCodeWhiteListed(referral_code))
    return _T("none");

  return referral_code;
}

}  // namespace omaha

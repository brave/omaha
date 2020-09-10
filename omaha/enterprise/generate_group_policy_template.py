# Copyright 2009 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ========================================================================

"""Generates a Group Policy template file for Google Update policies.

The resulting strings and files use CRLF as required by gpedit.msc.

To unit test this module, just run the file from the command line.
"""

from __future__ import print_function

import codecs
import filecmp
import os
import string
import sys


HORIZONTAL_RULE = ';%s\n' % ('-' * 78)
MAIN_POLICY_KEY = r'Software\Policies\BraveSoftware\Update'

# pylint: disable-msg=C6004
HEADER = """\
CLASS MACHINE
  CATEGORY !!Cat_Google
    CATEGORY !!Cat_GoogleUpdate
      KEYNAME \"""" + MAIN_POLICY_KEY + """\"
      EXPLAIN !!Explain_GoogleUpdate
"""

PREFERENCES = """
      CATEGORY !!Cat_Preferences
        KEYNAME \"""" + MAIN_POLICY_KEY + """\"
        EXPLAIN !!Explain_Preferences

        POLICY !!Pol_AutoUpdateCheckPeriod
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_2_145_5
          #endif
          EXPLAIN !!Explain_AutoUpdateCheckPeriod
          PART !!Part_AutoUpdateCheckPeriod NUMERIC
            VALUENAME AutoUpdateCheckPeriodMinutes
            DEFAULT 1400  ; 23 hours 20 minutes.
            MIN 0
            MAX 43200     ; 30 days.
            SPIN 60       ; Increment in hour chunks.
          END PART
          PART !!Part_DisableAllAutoUpdateChecks CHECKBOX
            VALUENAME DisableAutoUpdateChecksCheckboxValue  ; Required, unused.
            ACTIONLISTON
              ; Writes over Part_AutoUpdateCheckPeriod. Assumes this runs last.
              VALUENAME AutoUpdateCheckPeriodMinutes VALUE NUMERIC 0
            END ACTIONLISTON
            ACTIONLISTOFF
              ; Do nothing. Let Part_AutoUpdateCheckPeriod take effect.
            END ACTIONLISTOFF
            VALUEOFF  NUMERIC 0
            VALUEON   NUMERIC 1
          END PART
        END POLICY

        POLICY !!Pol_UpdateCheckSuppressedPeriod
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_3_33_5
          #endif
          EXPLAIN !!Explain_UpdateCheckSuppressedPeriod
          PART !!Part_UpdateCheckSuppressedStartHour NUMERIC
            VALUENAME UpdatesSuppressedStartHour
            DEFAULT 0
            MIN 0
            MAX 23  ; 0-23 hour
            SPIN 1
          END PART
          PART !!Part_UpdateCheckSuppressedStartMin NUMERIC
            VALUENAME UpdatesSuppressedStartMin
            DEFAULT 0
            MIN 0
            MAX 59  ; 0-59 minute in an hour
            SPIN 1
          END PART
          PART !!Part_UpdateCheckSuppressedDurationMin NUMERIC
            VALUENAME UpdatesSuppressedDurationMin
            DEFAULT 60
            MIN 1
            MAX 960  ; Maximum duration is 960 minutes = 16 hours
            SPIN 30
          END PART
        END POLICY

        POLICY !!Pol_DownloadPreference
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_3_26_0
          #endif
          EXPLAIN !!Explain_DownloadPreference
          PART !!Part_DownloadPreference DROPDOWNLIST
            VALUENAME "DownloadPreference"
            ITEMLIST
              NAME !!DownloadPreference_Cacheable VALUE "cacheable"
            END ITEMLIST
          END PART
        END POLICY

      END CATEGORY  ; Preferences

      CATEGORY !!Cat_ProxyServer
        KEYNAME \"""" + MAIN_POLICY_KEY + """\"

        POLICY !!Pol_ProxyMode
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_3_21_81
          #endif
          EXPLAIN !!Explain_ProxyMode

          PART !!Part_ProxyMode  DROPDOWNLIST
            VALUENAME "ProxyMode"
            ITEMLIST
              NAME !!ProxyDisabled_DropDown VALUE "direct"
              NAME !!ProxyAutoDetect_DropDown VALUE "auto_detect"
              NAME !!ProxyPacScript_DropDown VALUE "pac_script"
              NAME !!ProxyFixedServers_DropDown VALUE "fixed_servers"
              NAME !!ProxyUseSystem_DropDown VALUE "system"
            END ITEMLIST
          END PART
        END POLICY

        POLICY !!Pol_ProxyServer
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_3_21_81
          #endif
          EXPLAIN !!Explain_ProxyServer

          PART !!Part_ProxyServer  EDITTEXT
            VALUENAME "ProxyServer"
          END PART
        END POLICY

        POLICY !!Pol_ProxyPacUrl
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_3_21_81
          #endif
          EXPLAIN !!Explain_ProxyPacUrl

          PART !!Part_ProxyPacUrl  EDITTEXT
            VALUENAME "ProxyPacUrl"
          END PART
        END POLICY

      END CATEGORY
"""

APPLICATIONS_HEADER = """
      CATEGORY !!Cat_Applications
        KEYNAME \"""" + MAIN_POLICY_KEY + """\"
        EXPLAIN !!Explain_Applications
"""

INSTALL_POLICY_ITEMLIST = """\
            ITEMLIST
              NAME  !!Name_InstallsEnabled
              VALUE NUMERIC 1
              NAME  !!Name_InstallsEnabledMachineOnly
              VALUE NUMERIC 4
              NAME  !!Name_InstallsDisabled
              VALUE NUMERIC 0
            END ITEMLIST
            REQUIRED"""

UPDATE_POLICY_ITEMLIST = """\
            ITEMLIST
              NAME  !!Name_UpdatesEnabled
              VALUE NUMERIC 1
              NAME  !!Name_ManualUpdatesOnly
              VALUE NUMERIC 2
              NAME  !!Name_AutomaticUpdatesOnly
              VALUE NUMERIC 3
              NAME  !!Name_UpdatesDisabled
              VALUE NUMERIC 0
            END ITEMLIST
            REQUIRED"""

APPLICATION_DEFAULTS = ("""
        POLICY !!Pol_DefaultAllowInstallation
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_2_145_5
          #endif
          EXPLAIN !!Explain_DefaultAllowInstallation
          PART !!Part_InstallPolicy DROPDOWNLIST
            VALUENAME InstallDefault
""" +
INSTALL_POLICY_ITEMLIST + """
          END PART
        END POLICY

        POLICY !!Pol_DefaultUpdatePolicy
          #if version >= 4
            SUPPORTED !!Sup_GoogleUpdate1_2_145_5
          #endif
          EXPLAIN !!Explain_DefaultUpdatePolicy
          PART !!Part_UpdatePolicy DROPDOWNLIST
            VALUENAME UpdateDefault
""" +
UPDATE_POLICY_ITEMLIST + """
          END PART
        END POLICY
""")

APP_POLICIES_TEMPLATE = ("""
        CATEGORY !!Cat_$AppLegalId$
          KEYNAME \"""" + MAIN_POLICY_KEY + """\"

          POLICY !!Pol_AllowInstallation
            #if version >= 4
              SUPPORTED !!Sup_GoogleUpdate1_2_145_5
            #endif
            EXPLAIN !!Explain_Install$AppLegalId$
            PART !!Part_InstallPolicy DROPDOWNLIST
              VALUENAME Install$AppGuid$
""" +
INSTALL_POLICY_ITEMLIST.replace('            ', '              ') + """
            END PART
          END POLICY

          POLICY !!Pol_UpdatePolicy
            #if version >= 4
              SUPPORTED !!Sup_GoogleUpdate1_2_145_5
            #endif
            EXPLAIN !!Explain_AutoUpdate$AppLegalId$
            PART !!Part_UpdatePolicy DROPDOWNLIST
              VALUENAME Update$AppGuid$
""" +
UPDATE_POLICY_ITEMLIST.replace('            ', '              ') + """
            END PART
          END POLICY

          POLICY !!Pol_TargetVersionPrefix
            #if version >= 4
              SUPPORTED !!Sup_GoogleUpdate1_3_33_5
            #endif
            EXPLAIN !!Explain_TargetVersionPrefix$AppLegalId$

            PART !!Part_TargetVersionPrefix EDITTEXT
              VALUENAME "TargetVersionPrefix$AppGuid$"
            END PART
          END POLICY

          POLICY !!Pol_RollbackToTargetVersion
            #if version >= 4
              SUPPORTED !!Sup_GoogleUpdate1_3_34_3
            #endif
            EXPLAIN !!Explain_RollbackToTargetVersion$AppLegalId$
            VALUENAME RollbackToTargetVersion$AppGuid$
            VALUEOFF  NUMERIC 0
            VALUEON   NUMERIC 1
          END POLICY

        END CATEGORY  ; $AppName$
""")

APPLICATIONS_FOOTER = """
      END CATEGORY  ; Applications

    END CATEGORY  ; GoogleUpdate

  END CATEGORY  ; Google
"""

# Policy names that are used in multiple locations.
ALLOW_INSTALLATION_POLICY = 'Allow installation'
DEFAULT_ALLOW_INSTALLATION_POLICY = ALLOW_INSTALLATION_POLICY + ' default'
UPDATE_POLICY = 'Update policy override'
TARGET_VERSION_POLICY = 'Target version prefix override'
ROLLBACK_VERSION_POLICY = 'Rollback to Target version'
DEFAULT_UPDATE_POLICY = UPDATE_POLICY + ' default'

# Update policy options that are used in multiple locations.
UPDATES_ENABLED = 'Always allow updates'
AUTOMATIC_UPDATES_ONLY = 'Automatic silent updates only'
MANUAL_UPDATES_ONLY = 'Manual updates only'
UPDATES_DISABLED = 'Updates disabled'

# Category names that are used in multiple locations.
PREFERENCES_CATEGORY = 'Preferences'
PROXYSERVER_CATEGORY = 'Proxy Server'
APPLICATIONS_CATEGORY = 'Applications'

# The captions for update policy were selected such that they appear in order of
# decreasing preference when organized alphabetically in gpedit.
STRINGS_HEADER_AND_COMMON = ('\n' +
HORIZONTAL_RULE +
"""
[strings]
Sup_GoogleUpdate1_2_145_5=At least Google Update 1.2.145.5
Sup_GoogleUpdate1_3_21_81=At least Google Update 1.3.21.81
Sup_GoogleUpdate1_3_26_0=At least Google Update 1.3.26.0
Sup_GoogleUpdate1_3_33_5=At least Google Update 1.3.33.5
Sup_GoogleUpdate1_3_34_3=At least Google Update 1.3.34.3

Cat_Google=Google
Cat_GoogleUpdate=Google Update
Cat_Preferences=""" + PREFERENCES_CATEGORY + """
Cat_ProxyServer=""" + PROXYSERVER_CATEGORY + """
Cat_Applications=""" + APPLICATIONS_CATEGORY + """

Pol_AutoUpdateCheckPeriod=Auto-update check period override
Pol_DownloadPreference=Download URL class override
Pol_UpdateCheckSuppressedPeriod=Time period in each day to suppress auto update check
Pol_ProxyMode=Choose how to specify proxy server settings
Pol_ProxyServer=Address or URL of proxy server
Pol_ProxyPacUrl=URL to a proxy .pac file
Pol_DefaultAllowInstallation=""" + DEFAULT_ALLOW_INSTALLATION_POLICY + """
Pol_AllowInstallation=""" + ALLOW_INSTALLATION_POLICY + """
Pol_DefaultUpdatePolicy=""" + DEFAULT_UPDATE_POLICY + """
Pol_UpdatePolicy=""" + UPDATE_POLICY + """
Pol_TargetVersionPrefix=""" + TARGET_VERSION_POLICY + """
Pol_RollbackToTargetVersion=""" + ROLLBACK_VERSION_POLICY + """

Part_AutoUpdateCheckPeriod=Minutes between update checks
Part_DownloadPreference=Type of download URL to request
Part_UpdateCheckSuppressedStartHour=Hour in a day that start to suppress update check
Part_UpdateCheckSuppressedStartMin=Minute in hour that starts to suppress update check
Part_UpdateCheckSuppressedDurationMin=Number of minutes to suppress update check each day
Part_DisableAllAutoUpdateChecks=Disable all auto-update checks (not recommended)
Part_ProxyMode=Choose how to specify proxy server settings
Part_ProxyServer=Address or URL of proxy server
Part_ProxyPacUrl=URL to a proxy .pac file
Part_InstallPolicy=Policy
Part_UpdatePolicy=Policy
Part_TargetVersionPrefix=Target version prefix

Name_InstallsEnabled=Always allow Installs (recommended)
Name_InstallsEnabledMachineOnly=Always allow Machine-Wide Installs, but not Per-User Installs
Name_InstallsDisabled=Installs disabled

Name_UpdatesEnabled=""" + UPDATES_ENABLED + """ (recommended)
Name_ManualUpdatesOnly=""" + MANUAL_UPDATES_ONLY + """
Name_AutomaticUpdatesOnly=""" + AUTOMATIC_UPDATES_ONLY + """
Name_UpdatesDisabled=""" + UPDATES_DISABLED + """

ProxyDisabled_DropDown=Never use a proxy
ProxyAutoDetect_DropDown=Auto detect proxy settings
ProxyPacScript_DropDown=Use a .pac proxy script
ProxyFixedServers_DropDown=Use fixed proxy servers
ProxyUseSystem_DropDown=Use system proxy settings

DownloadPreference_Cacheable=Cacheable download URLs

""")

STRINGS_APP_NAME_TEMPLATE = """\
Cat_$AppLegalId$=$AppName$
"""

# pylint: disable-msg=C6310
# pylint: disable-msg=C6013

ADM_DOMAIN_REQUIREMENT_EN = """\
This policy is available only on Windows instances that are joined to a Microsoft Active Directory domain."""

# "application's" should be preceded by a different word in different contexts.
# The word is specified by replacing the $PreApplicationWord$ token.
STRINGS_UPDATE_POLICY_OPTIONS = """\
    \\n\\nOptions:\\
    \\n - """ + UPDATES_ENABLED + """: Updates are always applied when found, either by periodic update check or by a manual update check.\\
    \\n - """ + MANUAL_UPDATES_ONLY + """: Updates are only applied when the user does a manual update check. (Not all apps provide an interface for this.)\\
    \\n - """ + AUTOMATIC_UPDATES_ONLY + """: Updates are only applied when they are found via the periodic update check.\\
    \\n - """ + UPDATES_DISABLED + """: Never apply updates.\\
    \\n\\nIf you select manual updates, you should periodically check for updates using $PreApplicationWord$ application's manual update mechanism if available. If you disable updates, you should periodically check for updates and distribute them to users."""

STRINGS_COMMON_EXPLANATIONS = ("""
Explain_GoogleUpdate=Policies to control the installation and updating of Google applications that use Google Update/Google Installer.

""" +
HORIZONTAL_RULE +
'; ' + PREFERENCES_CATEGORY + '\n' +
HORIZONTAL_RULE + """
Explain_Preferences=General policies for Google Update.

Explain_AutoUpdateCheckPeriod=Minimum number of minutes between automatic update checks.\\n\\n%(domain_requirement)s

Explain_UpdateCheckSuppressedPeriod=If this setting is enabled, update checks will be suppressed during each day starting from Hour:Minute for a period of Duration (in minutes). Duration does not account for daylight savings time. So for instance, if the start time is 22:00, and with a duration of 480 minutes, the updates will be suppressed for 8 hours regardless of whether daylight savings time changes happen in between.\\n\\n%(domain_requirement)s

Explain_DownloadPreference=If enabled, the Google Update server will attempt to provide cache-friendly URLs for update payloads in its responses.\\n\\n%(domain_requirement)s

Explain_ProxyMode=Allows you to specify the proxy server used by Google Update.\\n\\nIf you choose to never use a proxy server and always connect directly, all other options are ignored.\\n\\nIf you choose to use system proxy settings or auto detect the proxy server, all other options are ignored.\\n\\nIf you choose fixed server proxy mode, you can specify further options in 'Address or URL of proxy server'.\\n\\nIf you choose to use a .pac proxy script, you must specify the URL to the script in 'URL to a proxy .pac file.'\\n\\n%(domain_requirement)s
Explain_ProxyServer=You can specify the URL of the proxy server here.\\n\\nThis policy only takes effect if you have selected manual proxy settings at 'Choose how to specify proxy server settings'.\\n\\n%(domain_requirement)s
Explain_ProxyPacUrl=You can specify a URL to a proxy .pac file here.\\n\\nThis policy only takes effect if you have selected manual proxy settings at 'Choose how to specify proxy server settings'.\\n\\n%(domain_requirement)s

""" % {"domain_requirement": ADM_DOMAIN_REQUIREMENT_EN} +
HORIZONTAL_RULE +
'; ' + APPLICATIONS_CATEGORY + '\n' +
HORIZONTAL_RULE + """
Explain_Applications=Policies for individual applications.\\
    \\n\\nAn updated ADM template will be required to support Google applications released in the future.

Explain_DefaultAllowInstallation=Specifies the default behavior for whether Google software can be installed using Google Update/Google Installer.\\
    \\n\\nCan be overridden by the \"""" + ALLOW_INSTALLATION_POLICY + """\" for individual applications.\\
    \\n\\nOnly affects installation of Google software using Google Update/Google Installer. Cannot prevent running the application installer directly or installation of Google software that does not use Google Update/Google Installer for installation.\\
    \\n\\n%(domain_requirement)s

Explain_DefaultUpdatePolicy=Specifies the default policy for software updates from Google.\\
    \\n\\nCan be overridden by the \"""" \
    % {"domain_requirement": ADM_DOMAIN_REQUIREMENT_EN} +
UPDATE_POLICY + """\" for individual applications.\\
""" +
STRINGS_UPDATE_POLICY_OPTIONS.replace('$PreApplicationWord$', 'each') + """\\
    \\n\\nOnly affects updates for Google software that uses Google Update for updates. Does not prevent auto-updates of Google software that does not use Google Update for updates.\\
    \\n\\nUpdates for Google Update are not affected by this setting; Google Update will continue to update itself while it is installed.\\
    \\n\\nWARNING: Disabing updates will also prevent updates of any new Google applications released in the future, possibly including dependencies for future versions of installed applications.\\
    \\n\\n%(domain_requirement)s

""" % {"domain_requirement": ADM_DOMAIN_REQUIREMENT_EN} +
HORIZONTAL_RULE +
'; Individual Applications\n' +
HORIZONTAL_RULE)

DEFAULT_ROLLBACK_DISCLAIMER = """This policy is meant to serve as temporary measure when Enterprise Administrators need to downgrade for business reasons. To ensure users are protected by the latest security updates, the most recent version should be used. When versions are downgraded to older versions, there could be incompatibilities."""

STRINGS_APP_POLICY_EXPLANATIONS_TEMPLATE = ("""
; $AppName$
Explain_Install$AppLegalId$=Specifies whether $AppName$ can be installed using Google Update/Google Installer.\\
    \\n\\nIf this policy is not configured, $AppName$ can be installed as specified by \"""" + DEFAULT_ALLOW_INSTALLATION_POLICY + """\".\\
    \\n\\n%(domain_requirement)s

Explain_AutoUpdate$AppLegalId$=Specifies how Google Update handles available $AppName$ updates from Google.\\
    \\n\\nIf this policy is not configured, Google Update handles available updates as specified by \"""" % {"domain_requirement": ADM_DOMAIN_REQUIREMENT_EN} + DEFAULT_UPDATE_POLICY + """\".\\
""" +
STRINGS_UPDATE_POLICY_OPTIONS.replace('$PreApplicationWord$', 'the') + '$AppUpdateExplainExtra$') + """\\
    \\n\\n%(domain_requirement)s

Explain_TargetVersionPrefix$AppLegalId$=Specifies which version $AppName$ should be updated to.\\
    \\n\\nWhen this policy is enabled, the app will be updated to the version prefixed with this policy value.\\
    \\n\\nSome examples:\\n\\
    1) Not configured: app will be updated to the latest version available.\\n\\
    2) Policy value is set to "55.": the app will be updated to any minor version of 55 (e.g., 55.24.34 or 55.60.2).\\n\\
    3) Policy value is "55.2.": the app will be updated to any minor version of 55.2 (e.g., 55.2.34 or 55.2.2).\\n\\
    4) Policy value is "55.24.34": the app will be updated to this specific version only.\\
    \\n\\n%(domain_requirement)s

Explain_RollbackToTargetVersion$AppLegalId$=Specifies that Google Update should roll installations of $AppName$ back to the version indicated by \"""" % {"domain_requirement": ADM_DOMAIN_REQUIREMENT_EN} + TARGET_VERSION_POLICY + """\".\\
    \\n\\nThis policy setting has no effect unless \"""" + TARGET_VERSION_POLICY + """\" is set.\\
    \\n\\nIf this policy is not configured or is disabled, installs that have a version higher than that specified by \"""" + TARGET_VERSION_POLICY + """\" will be left as-is.\\
    \\n\\nIf this policy is enabled, installs that have a version higher than that specified by \"""" + TARGET_VERSION_POLICY + """\" will be downgraded to the highest available version that matches the target version.\\
    \\n\\n$AppRollbackDisclaimer$\\
    \\n\\n%(domain_requirement)s
""" % {"domain_requirement": ADM_DOMAIN_REQUIREMENT_EN}

# pylint: enable-msg=C6013
# pylint: enable-msg=C6310
# pylint: enable-msg=C6004


def GenerateGroupPolicyTemplate(apps):
  # pylint: disable-msg=C6114
  """Generates a Group Policy template (ADM format)for the specified apps.

  Replaces LF in strings above with CRLF as required by gpedit.msc.
  When writing the resulting contents to a file, use binary mode to ensure the
  CRLFs are preserved.

  Args:
    apps: A list of tuples containing information about each app.
        Each element of the list is a tuple of:
          * app name
          * app ID
          * optional string to append to the auto-update explanation
            - Should start with a space or double new line (\n\n).

  Returns:
    String containing the contents of the .ADM file.
  """
  # pylint: enable-msg=C6114

  def _CreateLegalIdentifier(input_string):
    """Converts input_string to a legal identifier for ADM files.

    Changes some characters that do not necessarily cause problems and may not
    handle all cases.

    Args:
      input_string: Text to convert to a legal identifier.

    Returns:
      String containing a legal identifier based on input_string.
    """

    # pylint: disable-msg=C6004
    return (input_string.replace(' ', '')
                        .replace('&', '')
                        .replace('=', '')
                        .replace(';', '')
                        .replace(',', '')
                        .replace('.', '')
                        .replace('?', '')
                        .replace('=', '')
                        .replace(';', '')
                        .replace("'", '')
                        .replace('"', '')
                        .replace('\\', '')
                        .replace('/', '')
                        .replace('(', '')
                        .replace(')', '')
                        .replace('[', '')
                        .replace(']', '')
                        .replace('{', '')
                        .replace('}', '')
                        .replace('-', '')
                        .replace('!', '')
                        .replace('@', '')
                        .replace('#', '')
                        .replace('$', '')
                        .replace('%', '')
                        .replace('^', '')
                        .replace('*', '')
                        .replace('+', '')
                        .replace(u'\u00a9', '')   # Copyright (C).
                        .replace(u'\u00ae', '')   # Registered Trademark (R).
                        .replace(u'\u2122', ''))  # Trademark (TM).

    # pylint: enable-msg=C6004

  def _WriteTemplateForApp(template, app):
    """Writes the text for the specified app based on the template.

    Replaces $AppName$, $AppLegalId$, $AppGuid$, and $AppUpdateExplainExtra$.

    Args:
      template: text to process and write.
      app: tuple containing information about the app.

    Returns:
      String containing a copy of the template populated with app-specific
      strings.
    """

    (app_name, app_guid, update_explain_extra, rollback_disclaimer) = app
    if not rollback_disclaimer:
      rollback_disclaimer = DEFAULT_ROLLBACK_DISCLAIMER
    rollback_disclaimer = string.replace(rollback_disclaimer, '\n', '\\n')
    # pylint: disable-msg=C6004
    return (template.replace('$AppName$', app_name)
                    .replace('$AppLegalId$', _CreateLegalIdentifier(app_name))
                    .replace('$AppGuid$', app_guid)
                    .replace('$AppUpdateExplainExtra$', update_explain_extra)
                    .replace('$AppRollbackDisclaimer$', rollback_disclaimer)
           )
    # pylint: enable-msg=C6004

  def _WriteTemplateForAllApps(template, apps):
    """Writes a copy of the template for each of the specified apps.

    Args:
      template: text to process and write.
      apps: list of tuples containing information about the apps.

    Returns:
      String containing concatenated copies of the template for each app in
      apps, each populated with the appropriate app-specific strings.
    """

    content = [_WriteTemplateForApp(template, app) for app in apps]
    return ''.join(content)

  target_contents = [
      HEADER,
      PREFERENCES,
      APPLICATIONS_HEADER,
      APPLICATION_DEFAULTS,
      _WriteTemplateForAllApps(APP_POLICIES_TEMPLATE, apps),
      APPLICATIONS_FOOTER,
      STRINGS_HEADER_AND_COMMON,
      _WriteTemplateForAllApps(STRINGS_APP_NAME_TEMPLATE, apps),
      STRINGS_COMMON_EXPLANATIONS,
      _WriteTemplateForAllApps(STRINGS_APP_POLICY_EXPLANATIONS_TEMPLATE, apps),
      ]

  # Join the sections of content then replace LF with CRLF.
  return ''.join(target_contents).replace('\n', '\r\n')


def WriteGroupPolicyTemplate(target_path, apps):
  """Writes a Group Policy template (ADM format)for the specified apps.

  The file is UTF-16 and contains CRLF on all platforms.

  Args:
    target_path: Output path of the .ADM template file.
    apps: A list of tuples containing information about each app.
        Each element of the list is a tuple of:
          * app name
          * app ID
          * optional string to append to the auto-update explanation
            - Should start with a space or double new line (\n\n).
  """  # pylint: disable-msg=C6114

  contents = GenerateGroupPolicyTemplate(apps)
  f = codecs.open(target_path, 'wb', 'utf-16')
  f.write(contents)
  f.close()

# Run a unit test when the module is run directly.
if __name__ == '__main__':
  TEST_APPS = [
      ('Google Test Foo',
       '{D6B08267-B440-4c85-9F79-E195E80D9937}',
       ' Check http://www.google.com/test_foo/.',
       'Disclaimer'),
      (u'Google User Test Foo\u00a9\u00ae\u2122',
       '{104844D6-7DDA-460b-89F0-FBF8AFDD0A67}',
       ' Check http://www.google.com/user_test_foo/.',
       ''),
      ]
  TEST_GOLD_FILENAME = 'test_gold.adm'
  TEST_OUTPUT_FILENAME = 'test_out.adm'

  module_dir = os.path.abspath(os.path.dirname(__file__))
  gold_path = os.path.join(module_dir, TEST_GOLD_FILENAME)
  output_path = os.path.join(module_dir, TEST_OUTPUT_FILENAME)

  WriteGroupPolicyTemplate(output_path, TEST_APPS)

  if filecmp.cmp(gold_path, output_path, shallow=False):
    print('PASS: Contents equal.')
  else:
    print('FAIL: Contents not equal.')
    sys.exit(-1)

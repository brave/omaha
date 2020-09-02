# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import argparse
import os
import os.path
import shutil
import subprocess as sp
import sys


def FetchThirdParties(args, omaha_dir):
  os.chdir(omaha_dir)

  command = ['git', 'submodule', 'update', '--init']
  sp.check_call(command, stderr=sp.STDOUT)

def Build(args, omaha_dir, build_all):
  # move to omaha/omaha and start build.
  os.chdir(os.path.join(omaha_dir, 'omaha'))

  # set signing environment variables
  key_pfx_path = os.environ.get('KEY_PFX_PATH', '')
  key_cer_path = os.environ.get('KEY_CER_PATH', '')
  authenticode_password = os.environ.get('AUTHENTICODE_PASSWORD', '')

  mode = 'opt-win'
  if build_all:
    mode = 'all'
  command = ['hammer.bat', 'MODE=' + mode, '--all', '--sha2_authenticode_file=' + key_pfx_path,
    '--sha2_authenticode_password=' + authenticode_password, '--sha1_authenticode_file=' + key_pfx_path,
    '--sha1_authenticode_password=' + authenticode_password, '--patching_certificate=' + key_cer_path,
    '--authenticode_file=' + key_pfx_path, '--authenticode_password=' + authenticode_password]

  sp.check_call(command, stderr=sp.STDOUT)

def Copy_Untagged_Installers(args, omaha_dir, debug):
  last_win_dir = 'opt-win'
  if debug:
    last_win_dir = 'dbg-win'
  omaha_out_dir = os.path.join(omaha_dir, 'omaha', 'scons-out', last_win_dir)

  source_untagged_installer = os.path.join(omaha_out_dir, 'Test_Installers', 'UNOFFICIAL_' + args.untagged_installer_exe[0])
  target_untagged_installer_file = args.untagged_installer_exe[0]
  if debug:
    target_untagged_installer_file = 'Debug' + target_untagged_installer_file
  target_untagged_installer = os.path.join(args.root_out_dir[0], target_untagged_installer_file)

  shutil.copyfile(source_untagged_installer, target_untagged_installer)

  source_untagged_stub_installer = os.path.join(omaha_out_dir, 'staging', 'BraveUpdateSetup.exe')
  target_untagged_stub_installer_file = args.stub_untagged_exe[0]
  if debug:
    target_untagged_stub_installer_file = 'Debug' + target_untagged_stub_installer_file
  target_untagged_stub_installer = os.path.join(args.root_out_dir[0], target_untagged_stub_installer_file)

  shutil.copyfile(source_untagged_stub_installer, target_untagged_stub_installer)

def PrepareStandalone(args, omaha_dir):
  # copy brave installer to create standalone installer.
  installer_file = os.path.join(args.root_out_dir[0], args.brave_installer_exe[0])
  shutil.copyfile(installer_file, os.path.join(omaha_dir, 'omaha', 'standalone', args.brave_installer_exe[0]))
  shutil.copyfile(installer_file, os.path.join(omaha_dir, 'omaha', 'standalone', args.untagged_installer_exe[0]))
  shutil.copyfile(installer_file, os.path.join(omaha_dir, 'omaha', 'standalone', args.silent_installer_exe[0]))

  # prepare manifset file.
  f = open(os.path.join(omaha_dir, 'manifest_template.gup'),'r')
  filedata = f.read()
  f.close()

  newdata = filedata.replace("APP_GUID", args.guid[0])
  newdata = newdata.replace("BRAVE_INSTALLER_EXE", args.brave_installer_exe[0])
  newdata = newdata.replace("INSTALL_SWITCH", args.install_switch[0])

  target_manifest_file = args.guid[0] + '.gup'
  target_manifest_path = os.path.join(omaha_dir, 'omaha', 'standalone', 'manifests', target_manifest_file)
  f = open(target_manifest_path,'w')
  f.write(newdata)
  f.close()

  # update standalone_installers.txt.
  installer_text = "('STANDALONE_FILE_NAME', 'STANDALONE_FILE_NAME', [('BRAVE_VERSION', '$MAIN_DIR/standalone/BRAVE_INSTALLER_EXE', 'APP_GUID')], None, None, None, False, '', '')"
  installer_text = installer_text.replace("APP_GUID", args.guid[0])
  installer_text = installer_text.replace("STANDALONE_FILE_NAME", os.path.splitext(args.standalone_installer_exe[0])[0])
  installer_text = installer_text.replace("BRAVE_INSTALLER_EXE", args.brave_installer_exe[0])
  installer_text = installer_text.replace("BRAVE_VERSION", args.brave_full_version[0])

  # add untagged installer info to standalone_installers.txt.
  untagged_installer_text = "('UNTAGGED_FILE_NAME', 'UNTAGGED_FILE_NAME', [('BRAVE_VERSION', '$MAIN_DIR/standalone/UNTAGGED_INSTALLER_EXE', 'APP_GUID')], None, None, None, False, '', '')"
  untagged_installer_text = untagged_installer_text.replace("APP_GUID", args.guid[0])
  untagged_installer_text = untagged_installer_text.replace("UNTAGGED_FILE_NAME", os.path.splitext(args.untagged_installer_exe[0])[0])
  untagged_installer_text = untagged_installer_text.replace("UNTAGGED_INSTALLER_EXE", args.untagged_installer_exe[0])
  untagged_installer_text = untagged_installer_text.replace("BRAVE_VERSION", args.brave_full_version[0])

  # add silent installer info to standalone_installers.txt.
  silent_installer_text = "('SILENT_FILE_NAME', 'SILENT_FILE_NAME', [('BRAVE_VERSION', '$MAIN_DIR/standalone/SILENT_INSTALLER_EXE', 'APP_GUID')], None, None, None, False, '', '')"
  silent_installer_text = silent_installer_text.replace("APP_GUID", args.guid[0])
  silent_installer_text = silent_installer_text.replace("SILENT_FILE_NAME", os.path.splitext(args.silent_installer_exe[0])[0])
  silent_installer_text = silent_installer_text.replace("SILENT_INSTALLER_EXE", args.silent_installer_exe[0])
  silent_installer_text = silent_installer_text.replace("BRAVE_VERSION", args.brave_full_version[0])

  target_installer_text_path = os.path.join(omaha_dir, 'omaha', 'standalone', 'standalone_installers.txt')
  f = open(target_installer_text_path,'w')
  f.write(installer_text + '\n')
  f.write(untagged_installer_text + '\n')
  f.write(silent_installer_text)
  f.close()

def Tagging(args, omaha_dir, debug):
  last_win_dir = 'opt-win'
  if debug:
    last_win_dir = 'dbg-win'
  omaha_out_dir = os.path.join(omaha_dir, 'omaha', 'scons-out', last_win_dir)
  apply_tag_exe = os.path.join(omaha_out_dir, 'obj', 'tools', 'ApplyTag', 'ApplyTag.exe')

  tag_admin = os.environ.get('TAG_ADMIN', 'prefers')
  tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=TAG_ADMIN&ap=TAG_AP'
  tag = tag.replace("TAG_ADMIN", tag_admin)
  tag = tag.replace("APP_GUID", args.guid[0])
  tag = tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  tag = tag.replace("TAG_AP", args.tag_ap[0])

  silent_tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=TAG_ADMIN&ap=TAG_AP&silent'
  silent_tag = silent_tag.replace("TAG_ADMIN", 'False')
  silent_tag = silent_tag.replace("APP_GUID", args.guid[0])
  silent_tag = silent_tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  silent_tag = silent_tag.replace("TAG_AP", args.tag_ap[0])

  source_standalone_installer = os.path.join(omaha_out_dir, 'Test_Installers', 'UNOFFICIAL_' + args.standalone_installer_exe[0])
  target_standalone_installer_file = args.standalone_installer_exe[0]
  if debug:
    target_standalone_installer_file = 'Debug' + target_standalone_installer_file
  target_standalone_installer = os.path.join(args.root_out_dir[0], target_standalone_installer_file)
  command = [apply_tag_exe, source_standalone_installer, target_standalone_installer, tag]
  sp.check_call(command, stderr=sp.STDOUT)

  source_silent_installer = os.path.join(omaha_out_dir, 'Test_Installers', 'UNOFFICIAL_' + args.silent_installer_exe[0])
  target_silent_installer_file = args.silent_installer_exe[0]
  if debug:
    target_silent_installer_file = 'Debug' + target_silent_installer_file
  target_silent_installer = os.path.join(args.root_out_dir[0], target_silent_installer_file)
  command = [apply_tag_exe, source_silent_installer, target_silent_installer, silent_tag]
  sp.check_call(command, stderr=sp.STDOUT)

  source_stub_installer = os.path.join(omaha_out_dir, 'staging', 'BraveUpdateSetup.exe')
  target_stub_installer_file = args.stub_installer_exe[0]
  if debug:
    target_stub_installer_file = 'Debug' + target_stub_installer_file
  target_stub_installer = os.path.join(args.root_out_dir[0], target_stub_installer_file)
  command = [apply_tag_exe, source_stub_installer, target_stub_installer, tag]
  sp.check_call(command, stderr=sp.STDOUT)

  source_silent_stub_installer = os.path.join(omaha_out_dir, 'staging', 'BraveUpdateSetup.exe')
  target_silent_stub_installer_file = args.stub_silent_exe[0]
  if debug:
    target_silent_stub_installer_file = 'Debug' + target_silent_stub_installer_file
  target_silent_stub_installer = os.path.join(args.root_out_dir[0], target_silent_stub_installer_file)
  command = [apply_tag_exe, source_silent_stub_installer, target_silent_stub_installer, silent_tag]
  sp.check_call(command, stderr=sp.STDOUT)
  return

def ParseArgs():
  parser = argparse.ArgumentParser(description='build omaha installer')
  parser.add_argument('--root_out_dir',
                      nargs=1)
  parser.add_argument('--brave_installer_exe',
                      nargs=1)
  parser.add_argument('--stub_installer_exe',
                      nargs=1)
  parser.add_argument('--stub_untagged_exe',
                      nargs=1)
  parser.add_argument('--stub_silent_exe',
                      nargs=1)
  parser.add_argument('--standalone_installer_exe',
                      nargs=1)
  parser.add_argument('--silent_installer_exe',
                      nargs=1)
  parser.add_argument('--untagged_installer_exe',
                      nargs=1)
  parser.add_argument('--guid',
                      nargs=1)
  parser.add_argument('--install_switch',
                      nargs=1)
  parser.add_argument('--tag_ap',
                      nargs=1)
  parser.add_argument('--tag_app_name',
                      nargs=1)
  parser.add_argument('--brave_full_version',
                      nargs=1)
  return parser.parse_args()

def Main(args):
  args = ParseArgs()
  omaha_dir = os.path.join(args.root_out_dir[0], '..', '..', 'brave', 'vendor', 'omaha')

  FetchThirdParties(args, omaha_dir)
  PrepareStandalone(args, omaha_dir)
  Build(args, omaha_dir, False)
  Tagging(args, omaha_dir, False)
  Copy_Untagged_Installers(args, omaha_dir, False)

  return 0


if __name__ == '__main__':
  sys.exit(Main(sys.argv))

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import argparse
import os
import os.path
import shutil
import subprocess as sp
import sys


def fetch_submodules(omaha_dir):
  sp.check_call(['git', 'submodule', 'update', '--init'], stderr=sp.STDOUT, cwd=omaha_dir)

def build(omaha_dir, build_all):
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
    '--sha1_authenticode_password=' + authenticode_password, '--authenticode_file=' + key_pfx_path,
    '--authenticode_password=' + authenticode_password]

  sp.check_call(command, stderr=sp.STDOUT)

def copy_untagged_installers(args, omaha_dir, debug):
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

def prepare_untagged_standalone(args, omaha_dir):
  prepare_untagged(omaha_dir, args.root_out_dir[0], args.brave_installer_exe[0], args.guid[0], args.install_switch[0], args.brave_full_version[0],
    [(args.standalone_installer_exe[0], args.brave_installer_exe[0]),
     (args.untagged_installer_exe[0], args.untagged_installer_exe[0])]
  )

def prepare_untagged_silent(args, omaha_dir):
  prepare_untagged(
    omaha_dir, args.root_out_dir[0], args.brave_installer_exe[0], args.guid[0], '--do-not-launch-chrome ' + args.install_switch[0], args.brave_full_version[0],
    [(args.silent_installer_exe[0], args.silent_installer_exe[0])]
  )

def prepare_untagged(omaha_dir, root_out_dir, brave_installer_exe, app_guid, install_switch, brave_full_version, details):
  # copy brave installer to create standalone installer.
  installer_file = os.path.join(root_out_dir, brave_installer_exe)

  # prepare manifset file.
  f = open(os.path.join(omaha_dir, 'manifest_template.gup'),'r')
  filedata = f.read()
  f.close()

  newdata = filedata.replace("APP_GUID", app_guid)
  newdata = newdata.replace("BRAVE_INSTALLER_EXE", brave_installer_exe)
  newdata = newdata.replace("INSTALL_SWITCH", install_switch)

  target_manifest_file = app_guid + '.gup'
  target_manifest_path = os.path.join(omaha_dir, 'omaha', 'standalone', 'manifests', target_manifest_file)
  f = open(target_manifest_path,'w')
  f.write(newdata)
  f.close()

  target_installer_text_path = os.path.join(omaha_dir, 'omaha', 'standalone', 'standalone_installers.txt')

  # Clear the file:
  open(target_installer_text_path,'w').close()

  for file_name, installer_exe in details:
    add_to_standalone_installers_txt(target_installer_text_path, file_name, app_guid, installer_exe, brave_full_version)
    shutil.copyfile(installer_file, os.path.join(omaha_dir, 'omaha', 'standalone', installer_exe))

def add_to_standalone_installers_txt(target_installer_text_path, file_name, app_guid, brave_installer_exe, brave_version):
  installer_text = "('FILE_NAME', 'FILE_NAME', [('BRAVE_VERSION', '$MAIN_DIR/standalone/BRAVE_INSTALLER_EXE', 'APP_GUID')], None, None, None, False, '', '')"
  installer_text = installer_text.replace("FILE_NAME", os.path.splitext(file_name)[0])
  installer_text = installer_text.replace("APP_GUID", app_guid)
  installer_text = installer_text.replace("BRAVE_INSTALLER_EXE", brave_installer_exe)
  installer_text = installer_text.replace("BRAVE_VERSION", brave_version)
  f = open(target_installer_text_path,'a+')
  f.write(installer_text + '\n')
  f.close()

def tag_standalone(args, omaha_dir, debug):
  tag_admin = os.environ.get('TAG_ADMIN', 'prefers')
  tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=TAG_ADMIN&ap=TAG_AP'
  tag = tag.replace("TAG_ADMIN", tag_admin)
  tag = tag.replace("APP_GUID", args.guid[0])
  tag = tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  tag = tag.replace("TAG_AP", args.tag_ap[0])

  apply_tag(omaha_dir, debug, 'Test_Installers/UNOFFICIAL_' + args.standalone_installer_exe[0], args.standalone_installer_exe[0], tag, args.root_out_dir[0])
  apply_tag(omaha_dir, debug, 'staging/BraveUpdateSetup.exe', args.stub_installer_exe[0], tag, args.root_out_dir[0])

def tag_silent(args, omaha_dir, debug):
  silent_tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=TAG_ADMIN&ap=TAG_AP&silent'
  silent_tag = silent_tag.replace("TAG_ADMIN", 'False')
  silent_tag = silent_tag.replace("APP_GUID", args.guid[0])
  silent_tag = silent_tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  silent_tag = silent_tag.replace("TAG_AP", args.tag_ap[0])

  apply_tag(omaha_dir, debug, 'Test_Installers/UNOFFICIAL_' + args.silent_installer_exe[0], args.silent_installer_exe[0], silent_tag, args.root_out_dir[0])

def apply_tag(omaha_dir, debug, source_installer, target_installer_file, tag, root_out_dir):
  last_win_dir = 'opt-win'
  if debug:
    last_win_dir = 'dbg-win'
  omaha_out_dir = os.path.join(omaha_dir, 'omaha', 'scons-out', last_win_dir)
  apply_tag_exe = os.path.join(omaha_out_dir, 'obj', 'tools', 'ApplyTag', 'ApplyTag.exe')

  source_installer_path = os.path.join(omaha_out_dir, *source_installer.split('/'))
  if debug:
    target_installer_file = 'Debug' + target_installer_file
  target_installer_path = os.path.join(root_out_dir, target_installer_file)
  command = [apply_tag_exe, source_installer_path, target_installer_path, tag]
  sp.check_call(command, stderr=sp.STDOUT)

def parse_args():
  parser = argparse.ArgumentParser(description='build omaha installer')
  parser.add_argument('--root_out_dir',
                      nargs=1)
  parser.add_argument('--brave_installer_exe',
                      nargs=1)
  parser.add_argument('--stub_installer_exe',
                      nargs=1)
  parser.add_argument('--stub_untagged_exe',
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

def main():
  args = parse_args()
  omaha_dir = os.path.join(args.root_out_dir[0], '..', '..', 'brave', 'vendor', 'omaha')

  fetch_submodules(omaha_dir)
  prepare_untagged_standalone(args, omaha_dir)
  build(omaha_dir, False)
  tag_standalone(args, omaha_dir, False)

  prepare_untagged_silent(args, omaha_dir)
  build(omaha_dir, False)
  tag_silent(args, omaha_dir, False)

  copy_untagged_installers(args, omaha_dir, False)

  return 0


if __name__ == '__main__':
  sys.exit(main())

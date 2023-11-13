# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import argparse
import os
import os.path
import shutil
import subprocess as sp
import sys


def build(omaha_dir, standalone_installers_dir, debug):
  # move to omaha/omaha and start build.
  os.chdir(os.path.join(omaha_dir, 'omaha'))

  # set signing environment variables
  key_pfx_path = os.environ.get('KEY_PFX_PATH', '')
  key_cer_path = os.environ.get('KEY_CER_PATH', '')
  authenticode_password = os.environ.get('AUTHENTICODE_PASSWORD', '')
  authenticode_hash = os.environ.get('AUTHENTICODE_HASH', '')

  mode = 'opt-win'
  if debug:
    mode = 'dbg-win'
  command = ['hammer.bat', 'MODE=' + mode, '--all', '--standalone_installers_dir=' + standalone_installers_dir]
  if key_pfx_path:
    command.append('--authenticode_file=' + key_pfx_path)
    command.append('--sha1_authenticode_file=' + key_pfx_path)
    command.append('--sha2_authenticode_file=' + key_pfx_path)
  if authenticode_password:
    command.append('--authenticode_password=' + authenticode_password)
    command.append('--sha1_authenticode_password=' + authenticode_password)
    command.append('--sha2_authenticode_password=' + authenticode_password)
  if authenticode_hash:
    command.append('--authenticode_hash=' + authenticode_hash)
    command.append('--sha1_authenticode_hash=' + authenticode_hash)
    command.append('--sha2_authenticode_hash=' + authenticode_hash)
    # Our certs identified by hash are always in the machine store:
    command.append('--use_authenticode_machine_store')

  # Pick signtool.exe from PATH. This in particular ensures that we use the same
  # signtool as Chromium, which is 64 bit and thus has access to the same certs.
  env = dict(os.environ)
  signtool_path = shutil.which('signtool.exe')
  assert signtool_path, 'signtool.exe is expected to be on PATH'
  env['OMAHA_SIGNTOOL_SDK_DIR'] = os.path.dirname(signtool_path)

  sp.check_call(command, stderr=sp.STDOUT, env=env)

def copy_untagged_installers(args, omaha_dir):
  omaha_out_dir = get_omaha_out_dir(omaha_dir, args.debug)

  source_untagged_installer = os.path.join(omaha_out_dir, 'Test_Installers', 'UNOFFICIAL_' + args.untagged_installer_exe[0])
  target_untagged_installer_file = args.untagged_installer_exe[0]
  if args.debug:
    target_untagged_installer_file = 'Debug' + target_untagged_installer_file
  target_untagged_installer = os.path.join(args.root_out_dir[0], target_untagged_installer_file)

  shutil.copyfile(source_untagged_installer, target_untagged_installer)

  source_untagged_stub_installer = os.path.join(omaha_out_dir, 'staging', 'BraveUpdateSetup.exe')
  target_untagged_stub_installer_file = args.stub_untagged_exe[0]
  if args.debug:
    target_untagged_stub_installer_file = 'Debug' + target_untagged_stub_installer_file
  target_untagged_stub_installer = os.path.join(args.root_out_dir[0], target_untagged_stub_installer_file)

  shutil.copyfile(source_untagged_stub_installer, target_untagged_stub_installer)

def get_omaha_out_dir(omaha_dir, debug):
  last_win_dir = 'opt-win'
  if debug:
    last_win_dir = 'dbg-win'
  return os.path.join(omaha_dir, 'omaha', 'scons-out', last_win_dir)

def prepare_untagged_standalone(args, omaha_dir):
  return prepare_untagged(
    omaha_dir, 'standalone', args.root_out_dir[0], args.brave_installer_exe[0], args.guid[0],
    '--do-not-launch-chrome ' + args.install_switch[0], args.brave_full_version[0],
    [(args.standalone_installer_exe[0], args.brave_installer_exe[0]),
     (args.untagged_installer_exe[0], args.untagged_installer_exe[0])], args.debug
  )

def prepare_untagged_silent(args, omaha_dir):
  return prepare_untagged(
    omaha_dir, 'silent', args.root_out_dir[0], args.brave_installer_exe[0], args.guid[0],
    '--do-not-launch-chrome ' + args.install_switch[0], args.brave_full_version[0],
    [(args.silent_installer_exe[0], args.silent_installer_exe[0])], args.debug
  )

def prepare_untagged(omaha_dir, name, root_out_dir, brave_installer_exe, app_guid, install_switch, brave_full_version, details, debug):
  omaha_out_dir = get_omaha_out_dir(omaha_dir, debug)
  out_dir = os.path.join(omaha_out_dir, 'Brave_Installers', name)

  if not os.path.exists(out_dir):
    os.makedirs(out_dir)

  # prepare manifest file.
  f = open(os.path.join(omaha_dir, 'manifest_template.gup'),'r')
  filedata = f.read()
  f.close()

  newdata = filedata.replace("APP_GUID", app_guid)
  newdata = newdata.replace("BRAVE_INSTALLER_EXE", brave_installer_exe)
  newdata = newdata.replace("INSTALL_SWITCH", install_switch)

  target_manifest_dir = os.path.join(out_dir, 'manifests')

  if not os.path.exists(target_manifest_dir):
    os.mkdir(target_manifest_dir)

  target_manifest_file = app_guid + '.gup'
  target_manifest_path = os.path.join(target_manifest_dir, target_manifest_file)
  f = open(target_manifest_path, 'w')
  f.write(newdata)
  f.close()

  target_installer_text_path = os.path.join(out_dir, 'standalone_installers.txt')

  # Clear the file:
  open(target_installer_text_path, 'w').close()

  for file_name, installer_exe in details:
    brave_installer_path = os.path.join(root_out_dir, brave_installer_exe)
    brave_installer_path_fixed_name = os.path.join(out_dir, installer_exe)
    shutil.copyfile(brave_installer_path, brave_installer_path_fixed_name)
    add_to_standalone_installers_txt(target_installer_text_path, file_name, app_guid, brave_installer_path_fixed_name, brave_full_version)

  return out_dir

def add_to_standalone_installers_txt(target_installer_text_path, file_name, app_guid, installer_exe_path, brave_version):
  installer_text = "('FILE_NAME', 'FILE_NAME', [('BRAVE_VERSION', 'BRAVE_INSTALLER_EXE', 'APP_GUID')], None, None, None, False, '', '')"
  installer_text = installer_text.replace("FILE_NAME", os.path.splitext(file_name)[0])
  installer_text = installer_text.replace("APP_GUID", app_guid)
  installer_text = installer_text.replace("BRAVE_INSTALLER_EXE", installer_exe_path.replace('\\', '/'))
  installer_text = installer_text.replace("BRAVE_VERSION", brave_version)
  f = open(target_installer_text_path,'a+')
  f.write(installer_text + '\n')
  f.close()

def tag_standalone(args, omaha_dir):
  tag_admin = os.environ.get('TAG_ADMIN', 'prefers')
  tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=TAG_ADMIN&ap=TAG_AP'
  tag = tag.replace("TAG_ADMIN", tag_admin)
  tag = tag.replace("APP_GUID", args.guid[0])
  tag = tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  tag = tag.replace("TAG_AP", args.tag_ap[0])

  apply_tag(omaha_dir, args.debug, 'Test_Installers/UNOFFICIAL_' + args.standalone_installer_exe[0], args.standalone_installer_exe[0], tag, args.root_out_dir[0])

  if args.tag_installdataindex:
    # This current implementation doesn't have the install data value. But it
    # exists on the update server. So we only apply the index to the online
    # ("stub") installer.
    tag += '&installdataindex=' + args.tag_installdataindex

  apply_tag(omaha_dir, args.debug, 'staging/BraveUpdateSetup.exe', args.stub_installer_exe[0], tag, args.root_out_dir[0])

def tag_silent(args, omaha_dir):
  silent_tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=TAG_ADMIN&ap=TAG_AP&silent'
  silent_tag = silent_tag.replace("TAG_ADMIN", 'False')
  silent_tag = silent_tag.replace("APP_GUID", args.guid[0])
  silent_tag = silent_tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  silent_tag = silent_tag.replace("TAG_AP", args.tag_ap[0])

  apply_tag(omaha_dir, args.debug, 'Test_Installers/UNOFFICIAL_' + args.silent_installer_exe[0], args.silent_installer_exe[0], silent_tag, args.root_out_dir[0])

def apply_tag(omaha_dir, debug, source_installer, target_installer_file, tag, root_out_dir):
  omaha_out_dir = get_omaha_out_dir(omaha_dir, debug)
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
  parser.add_argument('--tag_installdataindex')
  parser.add_argument('--debug', action='store_true')
  return parser.parse_args()

def main():
  args = parse_args()
  omaha_dir = os.path.join(args.root_out_dir[0], '..', '..', 'brave', 'vendor', 'omaha')

  installer_metadata_dir = prepare_untagged_standalone(args, omaha_dir)
  build(omaha_dir, installer_metadata_dir, args.debug)
  tag_standalone(args, omaha_dir)

  installer_metadata_dir = prepare_untagged_silent(args, omaha_dir)
  build(omaha_dir, installer_metadata_dir, args.debug)
  tag_silent(args, omaha_dir)

  copy_untagged_installers(args, omaha_dir)

  return 0


if __name__ == '__main__':
  sys.exit(main())
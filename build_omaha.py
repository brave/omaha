# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import argparse
import os
import os.path
import shutil
import subprocess as sp
import sys

# https://stackoverflow.com/a/33856172
import ctypes

def run_as_admin(argv=None, debug=False):
    shell32 = ctypes.windll.shell32
    if argv is None and shell32.IsUserAnAdmin():
        return True

    if argv is None:
        argv = sys.argv
    if hasattr(sys, '_MEIPASS'):
        # Support pyinstaller wrapped program.
        arguments = map(unicode, argv[1:])
    else:
        arguments = map(unicode, argv)
    argument_line = u' '.join(arguments)
    executable = unicode(sys.executable)
    if debug:
        print 'Command line: ', executable, argument_line
    ret = shell32.ShellExecuteW(None, u"runas", executable, argument_line, None, 1)
    if int(ret) <= 32:
        return False
    return None

def Build(args, omaha_dir):
  os.chdir(omaha_dir)

  command = ['git', 'submodule', 'update', '--init']
  sp.check_call(command, stderr=sp.STDOUT)

  # move to omaha/omaha and start build.
  os.chdir(os.path.join(omaha_dir, 'omaha'))
  command = ['hammer-brave.bat', 'MODE=all', '--all']
  sp.check_call(command, stderr=sp.STDOUT)

def PrepareStandalone(args, omaha_dir):
  # copy brave installer to staing folder to create standalond installer.
  installer_file = os.path.join(args.root_out_dir[0], args.brave_installer_exe[0])
  shutil.copyfile(installer_file, os.path.join(omaha_dir, 'omaha', 'scons-out', 'opt-win', 'staging', args.brave_installer_exe[0]))
  shutil.copyfile(installer_file, os.path.join(omaha_dir, 'omaha', 'scons-out', 'dbg-win', 'staging', args.brave_installer_exe[0]))

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
  installer_text = "('STANDALONE_FILE_NAME', 'STANDALONE_FILE_NAME', [('BRAVE_VERSION', '$STAGING_DIR/BRAVE_INSTALLER_EXE', 'APP_GUID')], None, None, None, False, '', '')"
  installer_text = installer_text.replace("APP_GUID", args.guid[0])
  installer_text = installer_text.replace("STANDALONE_FILE_NAME", os.path.splitext(args.standalone_installer_exe[0])[0])
  installer_text = installer_text.replace("BRAVE_INSTALLER_EXE", args.brave_installer_exe[0])
  installer_text = installer_text.replace("BRAVE_VERSION", args.brave_full_version[0])

  target_installer_text_path = os.path.join(omaha_dir, 'omaha', 'standalone', 'standalone_installers.txt')
  f = open(target_installer_text_path,'w')
  f.write(installer_text)
  f.close()

def Tagging(args, omaha_dir, debug):
  last_win_dir = 'opt-win'
  if debug:
    last_win_dir = 'dbg-win'
  omaha_out_dir = os.path.join(omaha_dir, 'omaha', 'scons-out', last_win_dir)
  apply_tag_exe = os.path.join(omaha_out_dir, 'obj', 'tools', 'ApplyTag', 'ApplyTag.exe')

  tag = 'appguid=APP_GUID&appname=TAG_APP_NAME&needsadmin=prefers&lang=en&ap=TAG_AP'
  tag = tag.replace("APP_GUID", args.guid[0])
  tag = tag.replace("TAG_APP_NAME", args.tag_app_name[0])
  tag = tag.replace("TAG_AP", args.tag_ap[0])

  source_standalone_installer = os.path.join(omaha_out_dir, 'Test_Installers', 'UNOFFICIAL_' + args.standalone_installer_exe[0])
  target_standalone_installer_file = args.standalone_installer_exe[0]
  if debug:
    target_standalone_installer_file = 'Debug' + target_standalone_installer_file
  target_standalone_installer = os.path.join(args.root_out_dir[0], target_standalone_installer_file)
  command = [apply_tag_exe, source_standalone_installer, target_standalone_installer, tag]
  sp.check_call(command, stderr=sp.STDOUT)

  source_stub_installer = os.path.join(omaha_out_dir, 'staging', 'BraveUpdateSetup.exe')
  target_stub_installer_file = args.stub_installer_exe[0]
  if debug:
    target_stub_installer_file = 'Debug' + target_stub_installer_file
  target_stub_installer = os.path.join(args.root_out_dir[0], target_stub_installer_file)
  command = [apply_tag_exe, source_stub_installer, target_stub_installer, tag]
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
  parser.add_argument('--standalone_installer_exe',
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

# Wait to see ths compile logs. If not, elevated python program will quit w/o
# any notice.
def WaitFromUser():
  raw_input('\nPress ENTER to exit.')

def Main(args):
  ret = run_as_admin()
  if ret is True:
      print 'I have admin privilege.'
      args = ParseArgs()
      omaha_dir = os.path.join(args.root_out_dir[0], '..', '..', 'brave', 'vendor', 'omaha')

      # To build standalone installer, brave_installer should be copied into
      # scons-out/. However, this folder is created during the build.
      # Because of this scons-out folder isn't existed at first.
      # So, Build all except brave stub/standalone installer first, then copy
      # them. And build again finally to make standalone installer.
      # Second build only makes standalone. So, this isn't same build with first
      # build.
      Build(args, omaha_dir)
      PrepareStandalone(args, omaha_dir)
      Build(args, omaha_dir)
      # Create both(debug/release) executables
      Tagging(args, omaha_dir, True)
      Tagging(args, omaha_dir, False)

      WaitFromUser()

  return 0


if __name__ == '__main__':
  sys.exit(Main(sys.argv))

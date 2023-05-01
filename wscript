#! /usr/bin/env python
# encoding: utf-8
# mittorn, 2018

from waflib import Logs
import os

top = '.'

VGUI_SUPPORTED_OS = ['win32', 'darwin', 'linux']

def options(opt):
	grp = opt.add_option_group('VGUI options')

	vgui2_dev_path = os.path.join(opt.path.path_from(opt.root), 'vgui2-dev')

	grp.add_option('--disable-vgui', action = 'store_true', dest = 'NO_VGUI', default = False,
		help = 'disable vgui_support [default: %default]')

	opt.load('vgui')

def configure(conf):
	conf.env.NO_VGUI = conf.options.NO_VGUI

	if conf.options.NO_VGUI:
		return

	conf.load('vgui')
	conf.env.NO_VGUI = not conf.check_vgui()

def build(bld):
	if bld.env.NO_VGUI:
		return

	libs = [ 'public', 'filesystem_includes' ]

	# basic build: dedicated only, no dependencies
	if bld.env.DEST_OS != 'win32':
		libs += ['DL','M']

	libs.append('VGUI')

	source = bld.path.ant_glob(['*.cpp', 'vgui2-dev/src/*.cpp'])
	includes = [ '.', 'vgui2-dev/include' ]

	bld.shlib(
		source   = source,
		target   = 'vgui_support',
		features = 'cxx',
		includes = includes,
		use      = libs,
		rpath    = '$ORIGIN',
		install_path = bld.env.LIBDIR,
		subsystem = bld.env.MSVC_SUBSYSTEM
	)

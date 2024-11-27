#! /usr/bin/env python
# encoding: utf-8
# mittorn, 2018

from waflib import Logs
import os

top = '.'

VGUI_SUPPORTED_OS = ['win32', 'darwin', 'linux']

def options(opt):
	grp = opt.add_option_group('VGUI options')

	grp.add_option('--disable-vgui', action = 'store_true', dest = 'NO_VGUI', default = False,
		help = 'disable vgui_support [default: %(default)s]')

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

	libs = [ 'sdk_includes', 'werror' ]

	# basic build: dedicated only, no dependencies
	if bld.env.DEST_OS != 'win32':
		libs += ['DL','M']

	libs.append('VGUI')

	source = bld.path.ant_glob(['*.cpp'])

	includes = [ '.' ]

	bld.shlib(
		source   = source,
		target   = 'vgui_support',
		includes = includes,
		use      = libs,
		rpath    = bld.env.DEFAULT_RPATH,
		install_path = bld.env.LIBDIR
	)

#! /usr/bin/env python
# encoding: utf-8
# mittorn, 2018

def options(opt):
	opt.load('vgui')

def configure(conf):
	conf.load('vgui')
	conf.env.NO_VGUI = not conf.check_vgui()

def build(bld):
	if bld.env.NO_VGUI:
		return

	bld.shlib(
		source   = bld.path.ant_glob(['*.cpp']),
		target   = 'vgui_support',
		includes = '.',
		use      = 'sdk_includes werror DL M VGUI',
		rpath    = bld.env.DEFAULT_RPATH,
		install_path = bld.env.LIBDIR
	)

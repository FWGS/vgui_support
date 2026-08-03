#! /usr/bin/env python
# encoding: utf-8
# mittorn, 2018

def options(opt):
	pass

def configure(conf):
	pass

def build(bld):
	bld.shlib(
		source   = bld.path.ant_glob(['*.cpp']),
		target   = 'vgui_support',
		features = 'fix_freevgui_link',
		includes = '.',
		use      = 'sdk_includes werror DL M vgui yy_thunks',
		rpath    = bld.env.DEFAULT_RPATH,
		install_path = bld.env.LIBDIR
	)

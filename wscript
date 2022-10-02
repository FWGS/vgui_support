#! /usr/bin/env python
# encoding: utf-8
# mittorn, 2018

from waflib.TaskGen import feature, after_method

def options(opt):
	return

def configure(conf):
	return

@feature('fixup_link')
@after_method('process_use')
def fixup_link(self):
	if self.env.cxxshlib_PATTERN.endswith('.so'):
		fixedlibs = []
		for lib in self.env.LIB:
			fixedlibs += [':%s.so' % lib]
		self.env.LIB = fixedlibs

def build(bld):
	# in case of broken compatibility, vgui could be provided
	# by proprietary version, rather than through uselib hack
	# we had before
	libs = ['sdk_includes', 'vgui']

	if bld.env.DEST_OS != 'win32':
		libs += ['DL', 'M']

	tsk = bld.shlib(
		source   = bld.path.ant_glob(['*.cpp']),
		target   = 'vgui_support',
		features = 'cxx fixup_link',
		includes = '.',
		use      = libs,
		rpath    = '$ORIGIN',
		install_path = bld.env.LIBDIR,
		subsystem = bld.env.MSVC_SUBSYSTEM
	)

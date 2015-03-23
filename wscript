#!/usr/bin/python
top = '.'
out = 'build_waf'


def options(opt):
        opt.load('compiler_cxx qt4')
def configure(cnf):
        cnf.load('compiler_cxx qt4')
        cnf.check(features='cxx', lib=['sndfile','samplerate', 'xerces-c'], cxxflags=['-g', '-Wall'], uselib_store='He')
	cnf.check_cfg (package='sndfile, samplerate, xerces-c', uselib_store='He', args='--cflags --libs')


def build(bld):


	bld(features='cxx cxxshlib',
                source       = 'src/lv2_he.cpp src/he.cpp src/xml_parser.cpp src/hydro_dk_scan.cpp src/sample.cpp', 
                target       = 'helium', 
		use	     = ['He']
        )

        bld(features='cxx cxxshlib qt4',
                source       = 'src/lv2_he_gui.cpp src/hegui.cpp src/xml_parser.cpp src/hydro_dk_scan.cpp src/sample.cpp',
                target       = 'helium_gui',
                includes     = "qt4/Qt/ qt4/QtGui/ src/",
                uselib       = "QTCORE QTGUI",
                lib          = ['qwt'],
                use          = ['He']
        )


#Copy engine plugin
	bld(rule='cp ${SRC} ${TGT}', source='libhelium.so', target='plugin/helium.lv2/helium.so')

	bld(rule='cp ${SRC} ${TGT}', source='libhelium_gui.so', target='plugin/helium.lv2/helium_gui.so')

#Coopy ttl file
	bld(rule='cp ${SRC} ${TGT}', source='ttl/manifest.ttl', target='plugin/helium.lv2/')

	bld(rule='cp ${SRC} ${TGT}', source='ttl/helium.ttl', target='plugin/helium.lv2/')


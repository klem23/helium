#!/usr/local/bin/python

############################
#Environment & Common Source
############################
#env = Environment();
env = Environment(CCFLAGS = " -Wall ");

#debug mode
debug = ARGUMENTS.get('debug', 0)
if int(debug):
 env.Append(CCFLAGS = " -g ")

VariantDir('build_scons/', 'src/', duplicate=0)

common_src = Split("build_scons/hydro_dk_scan.cpp build_scons/xml_parser.cpp build_scons/sample.cpp")
common_obj = env.SharedObject(common_src);

#############
#Helium
#############
#env_he = Environment(CPPFLAGS = "-g -Wall");
env_he = env.Clone();
he_src = Split("build_scons/lv2_he.cpp build_scons/he.cpp")
env_he.Append(LIBS=['sndfile'])
env_he.Append(LIBS=['samplerate'])
env_he.Append(LIBS=['xerces-c'])
env_he.SharedLibrary('build_scons/helium', he_src + common_obj)


##############
#Helium_gui
##############
#env_heg = Environment(CPPFLAGS = "-g -Wall -DLV2_GUI")
env_heg = env.Clone();
env_heg.Append(CCFLAGS = " -DLV2_GUI ")
#######
#Qt ENV
env_heg.Tool('qt');
#env_heg['QTDIR'] = '/usr/lib/qt/'
env_heg['QT_CPPPATH'] = ['/usr/include/qt4/', 
			'/usr/include/qt4/Qt/',
			'/usr/include/qt4/QtCore/',
			'/usr/include/qt4/QtGui/']
#env_heg['QT_LIBPATH'] = '/usr/lib/i386-linux-gnu/'
#env_heg['QT_BINPATH'] = '/usr/bin/'
env_heg['QT_MOC'] = '/usr/bin/moc-qt4'
#env_heg.EnableQtModules(['QtGui', 'QtCore'])
env_heg.Append(LIBS=['QtCore'])
env_heg.Append(LIBS=['QtGui'])
#Qt ENV end
#########
heg_src = Split("build_scons/lv2_he_gui.cpp build_scons/hegui.cpp")
heg_src_sa = Split("build_scons/gui_main.cpp build_scons/hegui.cpp")
env_heg.Append(LIBS=['sndfile'])
env_heg.Append(LIBS=['samplerate'])
env_heg.Append(LIBS=['xerces-c'])
env_heg.Append(LIBS=['qwt'])
env_heg.SharedLibrary('build_scons/helium_gui', heg_src + common_obj)
env_hegsa = env_heg.Clone();
env_heg.Program('build_scons/heliumgui', heg_src_sa + common_obj)


##############
#Create bundle
##############
Command("build_scons/helium.lv2/manifest.ttl","ttl/manifest.ttl",          
                         Copy("$TARGET","$SOURCE"))
Command("build_scons/helium.lv2/helium.ttl","ttl/helium.ttl",          
                         Copy("$TARGET","$SOURCE"))
Command("build_scons/helium.lv2/helium.so","build_scons/libhelium.so",
                         Copy("$TARGET","$SOURCE"))
Command("build_scons/helium.lv2/helium_gui.so","build_scons/libhelium_gui.so",
                         Copy("$TARGET","$SOURCE"))


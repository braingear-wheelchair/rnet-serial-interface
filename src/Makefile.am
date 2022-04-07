AM_CXXFLAGS = -std=c++11 $(CPPFLAGS) -I$(top_srcdir)/src

lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = rnet_keyboard_control

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetDefinitions.hpp \
								RNetChecksum.hpp \
								RNetUtility.hpp \
								RNetPacket.hpp \
								RNetBuffer.hpp \
								RNetSerial.hpp \
								RNetReader.hpp \
								RNetWriter.hpp \
								RNetService.hpp \
								RNetServiceRx.hpp \
								RNetServiceXY.hpp \
								RNetServiceKeyboard.hpp \
								RNetCounter.hpp \
								RNetTimer.hpp

librnetserial_la_SOURCES =  RNetChecksum.cpp \
							RNetChecksum.hpp \
							RNetUtility.cpp \
							RNetUtility.hpp \
							RNetPacket.cpp \
							RNetPacket.hpp \
							RNetBuffer.cpp \
							RNetBuffer.hpp \
							RNetSerial.cpp \
							RNetSerial.hpp \
							RNetReader.cpp \
							RNetReader.hpp \
							RNetWriter.cpp \
							RNetWriter.hpp \
							RNetService.cpp \
							RNetService.hpp \
							RNetServiceRx.cpp \
							RNetServiceRx.hpp \
							RNetServiceXY.cpp \
							RNetServiceXY.hpp \
							RNetServiceKeyboard.cpp \
							RNetServiceKeyboard.hpp \
							RNetCounter.cpp \
							RNetCounter.hpp \
							RNetTimer.hpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)

LDADD = $(top_builddir)/src/librnetserial.la

rnet_keyboard_control_SOURCES = rnet_keyboard_control.cpp 
rnet_keyboard_control_LDADD   = librnetserial.la \
								$(top_builddir)/src/librnetserial.la \
								$(LDADD)
rnet_keyboard_control_CFLAGS  = $(AM_CFLAGS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = rnetserial.pc

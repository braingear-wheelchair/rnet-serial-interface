lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = rnet_keyboard_control

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetDefinitions.hpp \
								RNetChecksum.hpp \
								RNetUtility.hpp \
								RNetPacket.hpp \
								RNetBuffer.hpp \
								RNetSerial.hpp \
								RNetReceiver.hpp \
								RNetTrasmitter.hpp \
								RNetService.hpp \
								RNetServiceRx.hpp \
								RNetServiceXY.hpp \
								RNetServiceKeyboard.hpp \
								RNetCounter.hpp \
								RNetTimer.hpp

librnetserial_la_SOURCES =  RNetChecksum.cpp \
							RNetUtility.cpp \
							RNetPacket.cpp \
							RNetBuffer.cpp \
							RNetSerial.cpp \
							RNetReceiver.cpp \
							RNetTrasmitter.cpp \
							RNetService.cpp \
							RNetServiceRx.cpp \
							RNetServiceXY.cpp \
							RNetServiceKeyboard.cpp \
							RNetCounter.cpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)


rnet_keyboard_control_SOURCES = rnet_keyboard_control.cpp
rnet_keyboard_control_LDADD   = $(top_builddir)/src/librnetserial.la

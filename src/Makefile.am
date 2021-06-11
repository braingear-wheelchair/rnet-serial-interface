lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = test_rnetpacket \
			   test_rnetbuffer \
			   test_rnetservicerx \
			   test_rnetserial_servicerx \
			   test_rnetserial_servicexy \
			   test_rnetkeyboard

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetDefinitions.hpp \
								RNetChecksum.hpp \
								RNetUtility.hpp \
								RNetPacket.hpp \
								RNetBuffer.hpp \
								RNetSerial.hpp \
								RNetSerialRx.hpp \
								RNetSerialTx.hpp \
								RNetService.hpp \
								RNetServiceRx.hpp \
								RNetServiceXY.hpp \
								RNetServiceKeyboard.hpp

librnetserial_la_SOURCES =  RNetChecksum.cpp \
							RNetUtility.cpp \
							RNetPacket.cpp \
							RNetBuffer.cpp \
							RNetSerial.cpp \
							RNetSerialRx.cpp \
							RNetSerialTx.cpp \
							RNetService.cpp \
							RNetServiceRx.cpp \
							RNetServiceXY.cpp \
							RNetServiceKeyboard.cpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)


test_rnetpacket_SOURCES = test_rnetpacket.cpp
test_rnetpacket_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetbuffer_SOURCES = test_rnetbuffer.cpp
test_rnetbuffer_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetservicerx_SOURCES = test_rnetservicerx.cpp
test_rnetservicerx_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetserial_servicerx_SOURCES = test_rnetserial_servicerx.cpp
test_rnetserial_servicerx_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetserial_servicexy_SOURCES = test_rnetserial_servicexy.cpp
test_rnetserial_servicexy_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetkeyboard_SOURCES = test_rnetkeyboard.cpp
test_rnetkeyboard_LDADD   = $(top_builddir)/src/librnetserial.la

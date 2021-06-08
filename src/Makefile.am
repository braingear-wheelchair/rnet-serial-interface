lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = test_rnetserial

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetDefinitions.hpp \
								RNetSerial.hpp \
								RNetPacket.hpp \
								RNetUtility.hpp \
								RNetService.hpp \
								RNetServiceRx.hpp \
								RNetServiceXY.hpp \
								RNetServiceKeyboard.hpp

librnetserial_la_SOURCES =  RNetSerial.cpp \
							RNetPacket.cpp \
							RNetUtility.cpp \
							RNetService.cpp \
							RNetServiceRx.cpp \
							RNetServiceXY.cpp \
							RNetServiceKeyboard.cpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)


test_rnetserial_SOURCES = test_rnetserial.cpp
test_rnetserial_LDADD   = $(top_builddir)/src/librnetserial.la

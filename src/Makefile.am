lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = test_rnetpacket

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetDefinitions.hpp \
								RNetChecksum.hpp \
								RNetPacket.hpp

librnetserial_la_SOURCES =  RNetChecksum.cpp \
							RNetPacket.cpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)


test_rnetpacket_SOURCES = test_rnetpacket.cpp
test_rnetpacket_LDADD   = $(top_builddir)/src/librnetserial.la

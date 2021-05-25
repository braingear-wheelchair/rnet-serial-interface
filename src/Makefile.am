lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = serial_read_write \
			   test_rnetserial

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetChecksum.hpp \
								RNetPacket.hpp \
								RNetSerial.hpp

librnetserial_la_SOURCES =  RNetChecksum.cpp \
							RNetPacket.cpp \
							RNetSerial.cpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)


serial_read_write_SOURCES = serial_read_write.cpp
serial_read_write_LDADD = -lserial

test_rnetserial_SOURCES = test_rnetserial.cpp
test_rnetserial_LDADD   = $(top_builddir)/src/librnetserial.la

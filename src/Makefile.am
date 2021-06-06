lib_LTLIBRARIES = librnetserial.la

bin_PROGRAMS = serial_read_write \
			   test_rnetserial \
			   test_rnetserial_thread \
			   test_rnet_receive \
			   test_rnet_keyboard \
			   test_connection_disconnection

librnetserial_includedir = $(includedir)/rnetserial

librnetserial_include_HEADERS = RNetDefinitions.hpp \
								RNetUtility.hpp \
								RNetPacket.hpp \
								RNetSerial.hpp \
								RNetServiceRx.hpp \
								RNetTimer.hpp \
								RNetServiceKeyboard.hpp

librnetserial_la_SOURCES =  RNetUtility.cpp \
							RNetPacket.cpp \
							RNetSerial.cpp \
							RNetServiceRx.cpp \
							RNetTimer.hpp \
							RNetServiceKeyboard.cpp

librnetserial_la_LIBADD = -lserial

librnetserial_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE)


serial_read_write_SOURCES = serial_read_write.cpp
serial_read_write_LDADD = -lserial

test_rnetserial_SOURCES = test_rnetserial.cpp
test_rnetserial_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetserial_thread_SOURCES = test_rnetserial_thread.cpp
test_rnetserial_thread_LDADD   = $(top_builddir)/src/librnetserial.la 

test_rnet_receive_SOURCES = test_rnet_receive.cpp
test_rnet_receive_LDADD   = $(top_builddir)/src/librnetserial.la 

test_rnet_keyboard_SOURCES = test_rnet_keyboard.cpp
test_rnet_keyboard_LDADD = $(top_builddir)/src/librnetserial.la

test_connection_disconnection_SOURCES = test_connection_disconnection.cpp
test_connection_disconnection_LDADD = $(top_builddir)/src/librnetserial.la

AM_CPPFLAGS = -I$(top_builddir)/src/

noinst_PROGRAMS = test_rnetpacket \
			   	  test_rnetbuffer \
			   	  test_rnetservicerx \
			   	  test_rnetserial_servicerx \
			   	  test_rnetserial_servicexy \
			   	  test_rnetkeyboard \
			   	  test_rnetcounter \
			   	  test_thread \
			   	  test_rnetservice_keyboard

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

test_rnetcounter_SOURCES = test_rnetcounter.cpp
test_rnetcounter_LDADD   = $(top_builddir)/src/librnetserial.la

test_thread_SOURCES = test_thread.cpp
test_thread_LDADD   = $(top_builddir)/src/librnetserial.la

test_rnetservice_keyboard_SOURCES = test_rnetservice_keyboard.cpp
test_rnetservice_keyboard_LDADD   = $(top_builddir)/src/librnetserial.la

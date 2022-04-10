ACLOCAL_AMFLAGS=-I m4
SUBDIRS = src
 
doc_DATA = AUTHORS NEWS README COPYING

install-usb-serial-rnetchipset:
	@echo "Installing 99-usb-serial-rnetchipset.rules into /etc/udev/rules.d/ ..." \
	&& $(INSTALL) $(top_srcdir)/auxconfig/99-usb-serial-rnetchipset.rules /etc/udev/rules.d/ \
	&& echo "done."

uninstall-usb-serial-rnetchipset:
	@echo "Removing 99-usb-serial-rnetchipset.rules from /etc/udev/rules.d/ ..." \
	&& rm -f /etc/udev/rules.d/99-usb-serial-rnetchipset.rules && echo "done."

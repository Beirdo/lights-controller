SUBDIRS = remote base
.PHONY:	${SUBDIRS}

all:	${SUBDIRS}

${SUBDIRS}:
	make -C $@

clean:
	for i in ${SUBDIRS} ; do make -C $$i clean ; done

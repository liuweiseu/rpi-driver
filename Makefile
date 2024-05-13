SUBDIRS=${shell ls -d */ |grep -}

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

echo:
	@echo $(SUBDIRS)
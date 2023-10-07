CFLAGS ?= -std=c11 -O3 -DNDEBUG

SOEXT = so
ifeq ($(shell uname -s),Darwin)
	SOEXT = dylib
endif

SHARED_LIB = libmd5.$(SOEXT)
STATIC_LIB = libmd5.a
LIB_SRCS = src/md5.c src/padding.c src/rounds.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

EXE = md5
EXE_SRCS = src/main.c
EXE_OBJS = $(EXE_SRCS:.c=.o)
EXE_SYMLINKS = md5sum

LINK_SHARED ?=
ifneq ($(LINK_SHARED),)
	EXE_LINK_LIB = $(SHARED_LIB)
	override EXE_LDFLAGS += -L. -lmd5
else
	EXE_LINK_LIB = $(STATIC_LIB)
	override EXE_LDFLAGS += $(STATIC_LIB)
endif

.PHONY: all
all: $(SHARED_LIB) $(STATIC_LIB) $(EXE) $(EXE_SYMLINKS)

$(SHARED_LIB): LDFLAGS += $(LIB_LDFLAGS)
$(SHARED_LIB): $(LIB_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $^

$(STATIC_LIB): $(LIB_OBJS)
	$(AR) rcs $@ $^

$(LIB_OBJS): CPPFLAGS += $(LIB_CPPFLAGS)
$(LIB_OBJS): CFLAGS += $(LIB_CFLAGS)
$(LIB_OBJS): %.o: %.c

$(EXE): LDFLAGS += $(EXE_LDFLAGS)
$(EXE): $(EXE_OBJS) $(EXE_LINK_LIB)
	$(CC) $(LDFLAGS) $(EXE_OBJS) -o $@

$(EXE_OBJS): CPPFLAGS += $(EXE_CPPFLAGS)
$(EXE_OBJS): CFLAGS += $(EXE_CFLAGS)
$(EXE_OBJS): %.o: %.c

$(EXE_SYMLINKS): %: $(EXE)
	ln -sf $^ $@

.PHONY: clean
clean:
	rm -f $(SHARED_LIB) $(STATIC_LIB) $(LIB_OBJS) $(EXE) $(EXE_OBJS) $(EXE_SYMLINKS)
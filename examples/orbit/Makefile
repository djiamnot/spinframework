NAME=orbit

all: $(NAME)

###################################################################

CPPFLAGS += -I../../include -g -I/usr/include/boost $(shell python-config --cflags)
LDFLAGS +=  -llo -lSPIN $(shell python-config --libs)
LDFLAGS +=  -lpthread -lOpenThreads -losg -losgGA -losgSim -losgDB -losgFX \
		-losgIntrospection -losgManipulator -losgText -losgUtil \
		-losgParticle -losgViewer

ifeq ($(shell uname -s),Darwin)
  CPPFLAGS += -I/opt/local/include
  LDFLAGS += -lboost_python -L/opt/local/lib
else #linux
  LDFLAGS += -lc -lm -ldl -lGL -lX11 -lboost_python
endif


# TARGETS:

%.o:%.cpp
	@echo "--------------- compiling $@ ---------------" 
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(NAME): $(NAME).o
	@echo "\n--------------- linking $@: ---------------" 
	$(CXX) -o $@ $^ $(LDFLAGS)

install:
	-cp $(NAME) /usr/local/bin

clean:
	-rm -f $(NAME) *.o

PN_PATH= src/pn/
SERVER_PATH= src/server/
SERVERBIN=server2012
CLIENTBIN=pn2012
ARCHNAME=xsebek02.tar.gz
QMAKE=qmake-qt4
ifeq ($(shell hostname),merlin.fit.vutbr.cz)
	QMAKE=/usr/local/share/Qt/bin/qmake
endif

all:
	$(QMAKE) -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)
	$(QMAKE) -o $(SERVER_PATH)Makefile $(SERVER_PATH)pnserver.pro
	make -C $(SERVER_PATH)
	mkdir -p $(SERVER_PATH)nets

client:
	$(QMAKE) -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)

server:
	$(QMAKE) -o $(SERVER_PATH)Makefile $(SERVER_PATH)pnserver.pro
	make -C $(SERVER_PATH)
	mkdir -p $(SERVER_PATH)nets


run:
	./$(PN_PATH)$(CLIENTBIN)

runserver:
	./$(SERVER_PATH)$(SERVERBIN)


doxygen: Doxyfile
	doxygen Doxyfile

clean:
	make -C $(PN_PATH) clean
	make -C $(SERVER_PATH) clean
	rm -f $(PN_PATH)$(CLIENTBIN) $(SERVER_PATH)$(SERVERBIN) $(SERVER_PATH)Makefile $(PN_PATH)Makefile
	rm -rf $(SERVER_PATH)nets

pack:
	rm -f $(ARCHNAME)
	tar -czf $(ARCHNAME) src/*/*.{cpp,h,pro,ui} Doxyfile examples/* README.txt Makefile $(PN_PATH)settings.xml doc/help.html $(SERVER_PATH)passwd $(SERVER_PATH)nets/*
	
unpack:
	tar -xzf $(ARCHNAME)

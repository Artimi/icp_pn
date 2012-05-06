PN_PATH= src/pn/
SERVER_PATH= src/server/
SERVERBIN=server2012
CLIENTBIN=pn2012
ARCHNAME=xsebek02.tar.gz
QMAKE=qmake-qt4
ROOT_DIR=../xsebek02/

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
	tar -czf $(ARCHNAME) $(ROOT_DIR)src/*/*.{cpp,h,pro,ui} $(ROOT_DIR)Doxyfile $(ROOT_DIR)examples/* $(ROOT_DIR)README.txt $(ROOT_DIR)Makefile $(ROOT_DIR)$(PN_PATH)settings.xml $(ROOT_DIR)doc/help.html $(ROOT_DIR)$(SERVER_PATH)passwd $(ROOT_DIR)$(SERVER_PATH)nets/*

unpack:
	tar -xzf $(ARCHNAME)

PN_PATH= src/pn/
SERVER_PATH= src/server/
SERVERBIN=server2012
CLIENTBIN=pn2012
TESTDIR=testarchivefolder
ARCHNAME=xsebek02.tar.gz
QMAKE=/usr/local/share/Qt/bin/qmake

all:
#	qmake-qt4 -project -o $(PN_PATH)pn2012.pro
	$(QMAKE) -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)
#	qmake-qt4 -project -o $(SERVER_PATH)pnserver.pro
	$(QMAKE) -o $(SERVER_PATH)Makefile $(SERVER_PATH)pnserver.pro
	make -C $(SERVER_PATH)
	mkdir -p $(SERVER_PATH)nets

client:
#	qmake-qt4 -project -o $(PN_PATH)pn2012.pro
	$(QMAKE) -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)

server:
#	qmake-qt4 -project -o $(SERVER_PATH)pnserver.pro
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
	tar -czf $(ARCHNAME) src/*/[^moc_]*.{cpp,h,pro,ui} Doxyfile examples/* README.txt Makefile settings.xml doc/help.html
	
testarchive:
	rm -rf $(TESTDIR)
	mkdir -p $(TESTDIR)
	tar -xzf $(ARCHNAME) -C $(TESTDIR)
	
	qmake-qt4 -o $(TESTDIR)/$(PN_PATH)Makefile $(TESTDIR)/$(PN_PATH)pn2012.pro
	if [ $? -ne 0]; then \
		echo "---------- Chyba pri prekladu!! ---------"; \
	fi
	make -C $(TESTDIR)/$(PN_PATH)
	if [ $? -ne 0]; then \
		echo "---------- Chyba pri prekladu!! ---------"; \
	fi
	qmake-qt4 -o $(TESTDIR)/$(SERVER_PATH)Makefile $(TESTDIR)/$(SERVER_PATH)pnserver.pro
	if [ $? -ne 0]; then \
		echo "---------- Chyba pri prekladu!! ---------"; \
	fi
	make -C $(TESTDIR)/$(SERVER_PATH)
	if [ $? -ne 0]; then \
		echo "---------- Chyba pri prekladu!! ---------"; \
	fi

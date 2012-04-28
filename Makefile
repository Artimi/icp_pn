PN_PATH= src/pn/
SERVER_PATH= src/server/


all:
#	qmake-qt4 -project -o $(PN_PATH)pn2012.pro
	qmake-qt4 -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)
#	qmake-qt4 -project -o $(SERVER_PATH)pnserver.pro
	qmake-qt4 -o $(SERVER_PATH)Makefile $(SERVER_PATH)pnserver.pro
	make -C $(SERVER_PATH)

client:
#	qmake-qt4 -project -o $(PN_PATH)pn2012.pro
	qmake-qt4 -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)

server:
#	qmake-qt4 -project -o $(SERVER_PATH)pnserver.pro
	qmake-qt4 -o $(SERVER_PATH)Makefile $(SERVER_PATH)pnserver.pro
	make -C $(SERVER_PATH)


run:
	./$(PN_PATH)pn2012

runserver:
	./$(SERVER_PATH)pnserver


doxygen: Doxyfile
	doxygen Doxyfile

clean:
	make -C $(PN_PATH) clean
	make -C $(SERVER_PATH) clean

pack:

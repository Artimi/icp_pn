PN_PATH= src/pn/


all:
	qmake-qt4 -project -o $(PN_PATH)pn2012.pro
	qmake-qt4 -o $(PN_PATH)Makefile $(PN_PATH)pn2012.pro
	make -C $(PN_PATH)

run:
	./$(PN_PATH)pn2012

runserver:
	./$(PN_PATH)server2012


doxygen: Doxyfile
	doxygen Doxyfile

clean:
	make -C $(PN_PATH) clean

pack:

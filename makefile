all : asd asd_fast

asd : asd.cc
	g++ -D _SUNOS asd.cc /home/common/dyd/aisdi/tree/main.cc /home/common/dyd/aisdi/tree/timer.cc /home/common/dyd/aisdi/testtree/ltest_tree.so -o asd 
asd_fast : asd.cc
	g++ -O2 -D NDEBUG -D _SUNOS asd.cc /home/common/dyd/aisdi/tree/main.cc /home/common/dyd/aisdi/tree/timer.cc /home/common/dyd/aisdi/testtree/ltest_tree.so -o asd_fast
del :
	rm asd

debug : 
	g++ -g -D _SUNOS asd.cc /home/common/dyd/aisdi/tree/main.cc /home/common/dyd/aisdi/tree/timer.cc /home/common/dyd/aisdi/testtree/ltest_tree.so -o asd_debug 
	gdb asd_debug

view:
	lynx /home/common/dyd/aisdi/tree/info/index.html

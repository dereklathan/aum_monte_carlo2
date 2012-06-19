work : modtester.o cube.o Atom.o infile_reader.o vtf_file_writer.o dat_file_writer.o
	g++ modtester.o cube.o Atom.o infile_reader.o vtf_file_writer.o dat_file_writer.o -o work

modtester.o : modtester.cpp
	g++ -c modtester.cpp

cube.o : cube.cpp cube.h
	g++ -c cube.cpp

Atom.o : Atom.cpp Atom.h
	g++ -c Atom.cpp

infile_reader.o : infile_reader.cpp infile_reader.h
	g++ -c infile_reader.cpp

vtf_file_writer.o : vtf_file_writer.cpp vtf_file_writer.h
	g++ -c vtf_file_writer.cpp

dat_file_writer.o : dat_file_writer.cpp dat_file_writer.h
	g++ -c dat_file_writer.cpp

clean : 
	rm *.o

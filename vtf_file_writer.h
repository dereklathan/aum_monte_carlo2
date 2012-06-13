#ifndef VTF_FILE_WRITER_H
#define VTF_FILE_WRITER_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "Atom.h"
#include "cube.h"
using namespace std;

class vtf_file_writer{
	private:
		ofstream outfile;
	public:
		vtf_file_writer(){};
		//default constructor
		vtf_file_writer(string);
		//constructor. string is vtf filename for output.
		void open_file(string);
		//opens output file for writing
		void close_file();
		//closes output file
		void define_atom(Atom);
		//defines all atoms in list<Atom>
		void write_timestep(Cube);
		//sets positions of atoms and unitcell params
};
#endif

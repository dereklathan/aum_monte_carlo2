#include <iostream>
#include "infile_reader.h"
#include "vtf_file_writer.h"
#include "Atom.h"
#include "cube.h"
using namespace std;

int main(){
	Infile_reader reader("infile");
	//collect all data from infile
	reader.setData();
	vtf_file_writer writer(reader.getoutfilename());
	Cube cube;
	//set size of cube
	cube.set_domain(reader.getunitcellsize()[0], reader.getunitcellsize()[1], reader.getunitcellsize()[2]);
	Atom atom;
	//set up particles and put them in cube
	for(int c=0;c<reader.get_particle_types();c++){
		atom.set_name(reader.get_particle_name()[c]);
		atom.set_type_num(c);
		atom.set_strength(reader.get_strength()[c]);
		atom.set_mass(reader.get_mass()[c]);
		atom.set_fixed(reader.is_fixed()[c]);
		for(int d=0;d<cube.get_volume()*(reader.getpercentdomainfill()[c]/(float)100);d++)
		{
			writer.define_atom(cube.insert_atom(atom));
		}		
	}
	//make interaction factor matrix;
	cube.set_interaction_factor(reader.get_particle_types());
	//write initial positions to vtf file
	writer.write_timestep(cube);
	for(int c=0;c<reader.gettimesteps();c++){
		cube.advance_timestep_pbc();
		writer.write_timestep(cube);
	}
//	writer.close_file();
	cout << "here\n";
	for(int c=0;c<cube.get_domain_x();c++){
		for(int d=0;d<cube.get_domain_y();d++){
			for(int e=0;e<cube.get_domain_z();e++){
				if(cube.get_occupy_space(c,d,e)){
					Atom a=cube.get_atom(c,d,e);
					cout << a.get_name() << endl;
					cout << a.get_type_num() << endl;
					cout << a.get_mass() << endl;
					cout << a.is_fixed() << endl;
					cout << a.get_x_pos() << " " << a.get_y_pos() << " " << a.get_z_pos() << endl;
					cout << a.get_index() << endl;
					a.set_grav_pot_energy();
					cout << a.get_grav_pot_energy() << endl ;
					cout << a.get_strength() << endl;
					cout << a.get_attempted() << endl << endl;
				}
			}
		}
	}
	return 0;		
}

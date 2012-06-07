#include <iostream>
#include "infile_reader.h"
#include "vtf_file_writer.h"
#include "Atom.h"
#include "cube.h"
using namespace std;

int main(){
	Infile_reader reader("infile");
	reader.setData();
	vtf_file_writer writer(reader.getoutfilename());
	Cube cube;
	cout << reader.getunitcellsize()[2] << endl;
	cube.set_domain(reader.getunitcellsize()[0], reader.getunitcellsize()[1], reader.getunitcellsize()[2]);
	Atom atom;
	for(int c=0;c<reader.get_particle_types();c++){
		atom.set_name(reader.get_particle_name()[c]);
		atom.set_charge(reader.get_charge()[c]);
		atom.set_mass(reader.get_mass()[c]);
		atom.set_fixed(false);
		for(int d=0;d<cube.get_volume()*(reader.getpercentdomainfill()[c]/(float)100.0);d++)
		{
			writer.define_atom(cube.insert_atom(atom));
		}		
	}
	writer.write_timestep(cube);
	cout  << cube.get_domain_x() << " " << cube.get_domain_y() << " " << cube.get_domain_z() << endl;
	for(int c=0;c<cube.get_domain_x();c++){
		for(int d=0;d<cube.get_domain_y();d++){
			for(int e=0;e<cube.get_domain_z();e++){
				if(cube.get_occupy_space(c,d,e)){
					Atom a=cube.get_atom(c,d,e);
					cout << a.get_name() << endl;
					cout << a.get_mass() << endl;
					cout << a.is_fixed() << endl;
					cout << a.get_x_pos() << " " << a.get_y_pos() << " " << a.get_z_pos() << endl;
					cout << a.get_index() << endl;
					a.set_grav_pot_energy();
					cout << a.get_grav_pot_energy() << endl ;
					cout << a.get_charge() << endl;
					cout << a.get_attempted() << endl << endl;
				}
			}
		}
	}		
}

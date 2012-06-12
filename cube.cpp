#include "cube.h"
using namespace std;

Cube::Cube(){
	srand(time(NULL));
	population=0;
}

void Cube::set_domain(int x, int y, int z){
	domain_x=x;
	domain_y=y;
	domain_z=z;
	atomlocation = new Atom**[x];
	for(int c=0;c<x;c++){
		atomlocation[c] = new Atom*[y];
	}
	for(int c=0;c<x;c++){
		for(int d=0;d<=y;d++)
			atomlocation[c][d] = new Atom[z];
	}	
}

int Cube::get_domain_x(){
	return domain_x;
}

int Cube::get_domain_y(){
	return domain_y;
}

int Cube::get_domain_z(){
	return domain_z;
}

int Cube::get_volume(){
	return (domain_x*domain_y*domain_z);
}

Atom Cube::insert_atom(Atom &atom){
	int xval, yval, zval;
	xval=rand()%get_domain_x();
	yval=rand()%get_domain_y();
	zval=rand()%get_domain_z();
	if(atomlocation[xval][yval][zval].get_exists()){
		xval=rand()%get_domain_x();
		yval=rand()%get_domain_y();
		zval=rand()%get_domain_z();
	}
	atom.set_exists(true);
	atom.set_x_pos(xval);
	atom.set_y_pos(yval);
	atom.set_z_pos(zval);
	if(atom.is_fixed())
		atom.set_attempted(true);
	population++;
	atom.set_index(population);
	atomlocation[xval][yval][zval]=atom;
	return atom;
}

bool Cube::insert_atom(Atom atom, int x, int y, int z){
	if(atomlocation[x][y][z].get_exists())
		return false;
	else{
		atom.set_exists(true);
		atomlocation[x][y][z]=atom;
		return true;
	}
}

Atom Cube::get_atom(int x, int y, int z){
		return atomlocation[x][y][z];
}

void Cube::set_occupy_space(int x, int y, int z, bool occupied){
	atomlocation[x][y][z].set_exists(occupied);
}

bool Cube::get_occupy_space(int x, int y, int z){
	if(atomlocation[x][y][z].get_exists())
		return true;
	else
		return false;
}

int Cube::get_population(){return population;}

bool Cube::all_attempted(){
	for(int c=0;c<domain_x;c++){
		for(int d=0;d<domain_y;d++){
			for(int e=0;e<domain_z;e++){
				if(atomlocation[c][d][e].get_exists() && !atomlocation[c][d][e].get_attempted())
					return false;
				}
			}
	}
	return true;
}

void Cube::advance_timestep(){
	int x, y, z;
	int rand_axis, rand_dir;
	while(!all_attempted()){
		x=rand()%domain_x;
		y=rand()%domain_y;
		z=rand()%domain_z;
		if(atomlocation[x][y][z].get_exists() && !atomlocation[x][y][z].get_attempted()){
			
		}
	}
}

void Cube::set_interaction_factor(int types){
	interaction_factor = new double*[types];
	for(int c=0;c<types;c++)
		interaction_factor[c] = new double[types];
	for(int c=0;c<types;c++){
		for(int d=0;d<types;d++)
			cin >> interaction_factor[c][d];
	}
}

double Cube::calculate_pot_energy_pbc(){
	double grav_energy=0;
	double internal_energy=0;

	for(int c=0;c<domain_x;c++){
		for(int d=0;d<domain_y;d++){
			for(int e=0;e<domain_z;e++){
				if(atomlocation[c][d][e].get_exists()){
					grav_energy+=(e*atomlocation[c][d][e].get_mass());
					if(c==0){
						if(atomlocation[domain_x-1][d][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[domain_x-1][d][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[domain_x-1][d][e].get_type_num()]);
						if(atomlocation[1][d][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[1][d][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[1][d][e].get_type_num()]);					
					}
					else if(c==domain_x-1){
						if(atomlocation[0][d][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[0][d][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[0][d][e].get_type_num()]);
						if(atomlocation[domain_x-2][d][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[domain_x-2][d][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[domain_x-2][d][e].get_type_num()]);
					}
					else{
						if(atomlocation[c+1][d][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c+1][d][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c+1][d][e].get_type_num()]);
						if(atomlocation[c-1][d][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c-1][d][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c-1][d][e].get_type_num()]);
					}
					if(d==0){
						if(atomlocation[c][domain_y-1][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][domain_y-1][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][domain_y-1][e].get_type_num()]);
						if(atomlocation[c][1][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][1][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][1][e].get_type_num()]);
					}
					else if(d==domain_y-1){
						if(atomlocation[c][0][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][0][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][0][e].get_type_num()]);
						if(atomlocation[c][domain_y-2][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][domain_y-2][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][domain_y-2][e].get_type_num()]);
					}
					else{
						if(atomlocation[c][d+1][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d+1][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d+1][e].get_type_num()]);
						if(atomlocation[c][d-1][e].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d-1][e].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d-1][e].get_type_num()]);
					}
					if(e==0){
						if(atomlocation[c][d][domain_z-1].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d][domain_z-1].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d][domain_z-1].get_type_num()]);
						if(atomlocation[c][d][1].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d][1].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d][1].get_type_num()]);
					}
					else if(e==domain_z-1){
						if(atomlocation[c][d][0].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d][0].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d][0].get_type_num()]);
						if(atomlocation[c][d][domain_z-2].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d][domain_z-2].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d][domain_z-2].get_type_num()]);
					}
					else{
						if(atomlocation[c][d][e+1].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d][e+1].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d][e+1].get_type_num()]);
						if(atomlocation[c][d][e-1].get_exists())
							internal_energy+=(atomlocation[c][d][e].get_strength()*atomlocation[c][d][e-1].get_strength()*interaction_factor[atomlocation[c][d][e].get_type_num()][atomlocation[c][d][e-1].get_type_num()]);
					}
				}
			}
		}
	}
	return grav_energy + (internal_energy/2);
}

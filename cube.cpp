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

void Cube::advance_timestep_pbc(){
	int x1, y1, z1, x2, y2, z2;
	int rand_axis, rand_dir, dir;
	while(!all_attempted()){
		x1=rand()%domain_x;
		y1=rand()%domain_y;
		z1=rand()%domain_z;
		rand_axis=rand()%3;
		rand_dir=rand()%2;
		x2=x1;
		y2=y1;
		z2=z1;
		if(rand_dir==0)	dir=1;
		else dir=-1;
		if(rand_axis==0) x2+=dir;
		else if(rand_axis==1) y2+=dir;
		else z2+=dir;
		if(x2<0) x2=domain_x-1;
		else if(x2>=domain_x) x2=0;
		else if(y2<0) y2=domain_y-1;
		else if(y2>=domain_y) y2=0;
		else if(z2<0) z2=domain_z-1;
		else if(z2>=domain_z) z2=0;
		if(atomlocation[x1][y1][z1].get_exists() && !atomlocation[x1][y1][z1].get_attempted() && !atomlocation[x2][y2][z2].get_exists() && !atomlocation[x1][y1][z1].is_fixed()){
			if(calculate_pot_energy_pbc(x1,y1,z1,x2,y2,z2)<=calculate_pot_energy_pbc()){
				atomlocation[x2][y2][z2]=atomlocation[x1][y2][z2];
				atomlocation[x1][y1][z1].set_exists(false);
				atomlocation[x2][y2][z2].set_exists(true);
				atomlocation[x2][y2][z2].set_x_pos(x2);
				atomlocation[x2][y2][z2].set_y_pos(y2);
				atomlocation[x2][y2][z2].set_z_pos(z2);
			}			
		}
		atomlocation[x1][y1][z1].set_attempted(true);
	}
	for(int c=0;c<domain_x;c++){
		for(int d=0;d<domain_y;d++){
			for(int e=0;e<domain_z;e++)
				atomlocation[c][d][e].set_attempted(false);
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

double Cube::calculate_pot_energy_pbc(int x1, int y1, int z1, int x2, int y2, int z2){
	double grav_energy=0;
	double internal_energy=0;
	Atom *** temp=atomlocation;
	if(x1>=0 && x1<domain_x && x2>=0 && x2<domain_x && y1>=0 && y1<domain_y && y2>=0 && y2<domain_y && z1>=0 && z1<domain_z && z2>=0 && z2<domain_z && temp[x1][y1][z1].get_exists() && !temp[x2][y2][z2].get_exists() && !temp[x2][y2][z2].is_fixed()){
		temp[x2][y2][z2]=temp[x1][y1][z1];
		temp[x2][y2][z2].set_x_pos(x2);
		temp[x2][y2][z2].set_y_pos(y2);
		temp[x2][y2][z2].set_z_pos(z2);
		for(int c=0;c<domain_x;c++){
			for(int d=0;d<domain_y;d++){
				for(int e=0;e<domain_z;e++){
					if(temp[c][d][e].get_exists()){
						grav_energy+=(e*temp[c][d][e].get_mass());
						if(c==0){
							if(temp[domain_x-1][d][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[domain_x-1][d][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[domain_x-1][d][e].get_type_num()]);
							if(temp[1][d][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[1][d][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[1][d][e].get_type_num()]);					
						}
						else if(c==domain_x-1){
							if(temp[0][d][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[0][d][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[0][d][e].get_type_num()]);
							if(temp[domain_x-2][d][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[domain_x-2][d][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[domain_x-2][d][e].get_type_num()]);
						}
						else{
							if(temp[c+1][d][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c+1][d][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c+1][d][e].get_type_num()]);
							if(temp[c-1][d][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c-1][d][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c-1][d][e].get_type_num()]);
						}
						if(d==0){
							if(temp[c][domain_y-1][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][domain_y-1][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][domain_y-1][e].get_type_num()]);
							if(temp[c][1][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][1][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][1][e].get_type_num()]);
						}
						else if(d==domain_y-1){
							if(temp[c][0][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][0][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][0][e].get_type_num()]);
							if(temp[c][domain_y-2][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][domain_y-2][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][domain_y-2][e].get_type_num()]);
						}
						else{
							if(temp[c][d+1][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d+1][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d+1][e].get_type_num()]);
							if(temp[c][d-1][e].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d-1][e].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d-1][e].get_type_num()]);
						}
						if(e==0){
							if(temp[c][d][domain_z-1].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d][domain_z-1].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d][domain_z-1].get_type_num()]);
							if(temp[c][d][1].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d][1].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d][1].get_type_num()]);
						}
						else if(e==domain_z-1){
							if(temp[c][d][0].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d][0].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d][0].get_type_num()]);
							if(temp[c][d][domain_z-2].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d][domain_z-2].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d][domain_z-2].get_type_num()]);
						}
						else{
							if(temp[c][d][e+1].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d][e+1].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d][e+1].get_type_num()]);
							if(temp[c][d][e-1].get_exists())
								internal_energy+=(temp[c][d][e].get_strength()*temp[c][d][e-1].get_strength()*interaction_factor[temp[c][d][e].get_type_num()][temp[c][d][e-1].get_type_num()]);
						}
					}
				}
			}
		}
		return grav_energy + (internal_energy/2);
	}
	else
		return 1.79769e+308;	
}

aum_monte_carlo2
================
this is a monte carlo simulation in a cubic domain with periodic boundary conditions for all axes. the particles are
inserted into the domain at random x,y,z coordinates. this is timestep zero. each particle is selected at random and
will make an attempt to move. the particles can only move along a particular axis in a positive or negative direction.
the program will randomly select and axis and direction and if there is not a particle in the position it is trying to
move to, it will check to see if the total potential energy of the system is less than or equal to, after the possible move
,than the current potential energy. if this is true the particle will move. otherwise the program generates a random number
between 0 and 1 and compares it to e^-(1/T) where T is the temperature of the cube. if the random number is less than or equal to e^-(1/T) then the particle will still move. fixed particles will never move. once every non-fixed particle has
made an attempt then the program will continue on to the next timestep. 

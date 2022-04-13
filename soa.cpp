// Structure of arrays (soa) implementation
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <iomanip> /* Library that provides manipulators as changing floating precision as function setprecision*/
#include <vector>
#include <cstdlib> /* Library for using atoi function*/
struct object{ //structure of arrays
    // Definition of the parameters mass and velocity and position decomposed in the 3D axis as vectors
    std::vector<double> position_x;
    std::vector<double> position_y;
    std::vector<double> position_z;
    std::vector<double> velocity_x;
    std::vector<double> velocity_y;
    std::vector<double> velocity_z;
    std::vector<double> mass;
};


int main(int argc, char *argv[]) {
    using namespace std; // We predefine the namespace in this scope instead of repeatdly use std::
    const double G = 6.674e-11;  // Universal gravitational constant


/* ********** Check type of parameters ***********      */
    // check number of arguments
    // If the number of arguments is less than 6 the program cannot be excuted and we exit it
    cout << argv[0]<<"  Invoked with " << argc -1<< " parameters."<<endl;

    if (argc < 6){
        cout << "Invalid number of arguments." << endl;
    }
    // convert arguments to integers
    double num_objects=0.0, num_iterations=0.0, random_seed=0.0;
    double time_step=0.0;
    double size_enclosure=0.0;
    // atoi allows to convert from string to int, here we want to make sure it is the type we want

    cout << "Arguments: " << endl; // Parameters for the simulation

    for(int i=0; i < argc; i++){
        if (i==1){
            num_objects = stod(argv[1]);
            cout << "num_objects:"<< argv[1]<< endl; // Number of objects
        }
        if (i==2){
            num_iterations = stod(argv[2]);
            cout <<"num_iterations:"<< argv[2]<< endl; // Time steps
        }
        if (i==3){
            random_seed = stod(argv[3]);
            cout << "random_seed:" << argv[3] << endl; // For the random distribution generator function
        }
        if (i==4){
            size_enclosure = stod(argv[4]);
            cout << "size_enclosure: "<< argv[4]<< endl;  // Dimension of the cubic region
        }
        if (i==5){
            time_step = stod(argv[5]);
            cout << "time_step: "<< argv[5]<< endl; // Time increment for each iteration

        }
    }

    for(int i=argc; i < 6; i++){
        if (i==1){
            cout << "num_objects:  ?" << endl; // Number of objects

        }
        if (i==2){
            cout <<"num_iterations:  ?" << endl; // Time steps
        }
        if (i==3){
            cout << "random_seed: ?"  << endl; // For the random distribution generator function
        }
        if (i==4){
            cout << "size_enclosure: ?"<< endl;  // Dimension of the cubic region
        }
        if (i==5){
            cout << "time_step: ?"<< endl; // Time increment for each iteration

        }
    }

    if (argc < 6){
        exit(-1);
    }


    if (num_objects<0||(int)num_objects!=num_objects){
        cout<<"Error: invalid num_objects"<<endl;
    }
    if (num_iterations<0||(int)num_iterations!=num_iterations){
        cout<<"Error: invalid num_iterations"<<endl;
    }
    if (random_seed<0||(int)random_seed!=random_seed){
        cout<<"Error: invalid random_seed"<<endl;
    }
    if (size_enclosure<0){
        cout<<"Error: invalid size_enclosure"<<endl;
    }
    if (time_step<0){
        cout<<"Error: invalid time_step"<<endl;
    }


    if (num_objects<0 ||num_iterations<0 ||(int)num_objects!=num_objects|| random_seed<0 ||(int)num_iterations!=num_iterations||size_enclosure<0 ||(int)random_seed!=random_seed|| time_step<0){
        exit(-2);
    }
    /* ******* a file is created with this header **************** */
    ofstream file, final_file;
    file.open("init_conf.txt");
    file <<fixed<< setprecision(3)<< size_enclosure << " " << time_step << " " << (int)num_objects<< "\n";

    // If at least of the arguments is not a positive integer the program cannot continue


    object objects; // create a structure objects of arrays

/* ************** UNIFORM RANDOM DISTRIBUTION *********************/

    mt19937_64 gen((int)random_seed); // Standard mersenne_twister_engine seeded with random_value()

    uniform_real_distribution<double> dis(0, size_enclosure);
    normal_distribution<double> d{pow(10,21),pow(10,15)};

    for(int i=0; i<(int)num_objects; ++i){
        //   ***** Store initial values in the array of objects *******
        objects.position_x.push_back(dis(gen));
        objects.position_y.push_back(dis(gen));
        objects.position_z.push_back(dis(gen));
        // intial velocity set to 0
        objects.velocity_x.push_back(0);
        objects.velocity_y.push_back(0);
        objects.velocity_z.push_back(0);
        objects.mass.push_back(d(gen));
        // ***** Write initial values in the file ******
        file << fixed << setprecision(3) <<objects.position_x[i] << " " << objects.position_y[i] << " " << objects.position_z[i]<< " "<< objects.velocity_x[i] << " "<< objects.velocity_y[i] << " " << objects.velocity_z[i] << " "<<objects.mass[i]<<"\n";
    }

    // Intialization to 0 of the forces decomposed (3D), as many as objects where created
    vector<double>forceX((int)num_objects,0);
    vector<double>forceY((int)num_objects,0);
    vector<double>forceZ((int)num_objects,0);

    double norm,result,x;
    double dist_x, dist_y,dist_z;
    double Fx, Fy, Fz;

    // Calculation of the relative position between objects
    for ( int n=0; n<(int)num_iterations; ++n) {
        for (int i = 0; i < (int)num_objects; ++i) {
            for (int u = i+1; u < (int)num_objects; ++u) {
                // Calculation of the norm to check their distance
                dist_x=objects.position_x[u] - objects.position_x[i];
                dist_y=objects.position_y[u] - objects.position_y[i];
                dist_z=objects.position_z[u] - objects.position_z[i];
                x=std::sqrt((dist_x*dist_x) + (dist_y*dist_y) + (dist_z*dist_z));
                norm=x*x*x;

                result = G * objects.mass[i] * objects.mass[u] ; // Gravitational constant time the masses of the two objects

                // Computation of the forces in the three coordinates for each object n
                Fx = result* dist_x/ norm;
                Fy = result*dist_y/ norm;
                Fz = result	*dist_z/ norm;

                // Update the vectors of forces
                forceX[i] += Fx;
                forceX[u] += (-1) * Fx;
                forceY[i] += Fy;
                forceY[u] += (-1) * Fy ;
                forceZ[i] += Fz;
                forceZ[u] += (-1) * Fz;
            }
            /* ****ACCELERATION*** */

            // Velocities corresponding to the time step and forces
            objects.velocity_x[i] += (forceX[i]/objects.mass[i])*time_step;
            objects.velocity_y[i] += (forceY[i]/objects.mass[i])*time_step;
            objects.velocity_z[i] += (forceZ[i]/objects.mass[i])*time_step;

            // Calculation of the position updated
            objects.position_x[i] += objects.velocity_x[i] * time_step;
            objects.position_y[i] += objects.velocity_y[i] * time_step;
            objects.position_z[i] += objects.velocity_z[i] * time_step;

            // redefine them to 0
            forceX[i] = 0;
            forceY[i] = 0;
            forceZ[i] = 0;

            /* *************** REBOUND EFFECT *************** */
            // If the position is greater (in any direction) than the dimensions of the cubic enclosure we must establish the position as the cubes'
            // dimensions and we set the velocities in the opposite direction (as a rebound effect)
            if (objects.position_x[i] >= size_enclosure) {
                objects.position_x[i] = size_enclosure;
                objects.velocity_x[i] = (-1) * objects.velocity_x[i];
            }
            if (objects.position_y[i] >= size_enclosure) {
                objects.position_y[i] = size_enclosure;
                objects.velocity_y[i] = (-1) * objects.velocity_y[i];
            }
            if (objects.position_z[i] >= size_enclosure) {
                objects.position_z[i] = size_enclosure;
                objects.velocity_z[i] = (-1) * objects.velocity_z[i];
            }
            // If any of the position get to 0 in any axis
            if (objects.position_x[i] <= 0) {
                objects.position_x[i] = 0;
                objects.velocity_x[i] = (-1) * objects.velocity_x[i];
            }
            if (objects.position_y[i] <= 0) {
                objects.position_y[i] = 0;
                objects.velocity_y[i] = (-1) * objects.velocity_y[i];
            }
            if (objects.position_z[i] <= 0) {
                objects.position_z[i] = 0;
                objects.velocity_z[i] = (-1) * objects.velocity_z[i];
            }
        }

        for (int i = 0; i < (int)num_objects; ++i) {
            for (int u = i+1; u < (int)num_objects; ++u) {
                dist_x=objects.position_x[u] - objects.position_x[i];
                dist_y=objects.position_y[u] - objects.position_y[i];
                dist_z=objects.position_z[u] - objects.position_z[i];
                norm=sqrt((dist_x*dist_x) + (dist_y*dist_y) + (dist_z*dist_z));

                // COLLISIONS
                if (norm < 1) { // Therefore they are considered to collide
                    // We update the data, their velocities and masses are sumed
                    objects.velocity_x[i] += objects.velocity_x[u];
                    objects.velocity_y[i] += objects.velocity_y[u];
                    objects.velocity_z[i] += objects.velocity_z[u];
                    objects.mass[i] += objects.mass[u];
                    // We procede to delete object #u and its corresponding data
                    objects.position_x.erase(objects.position_x.begin() + u);
                    objects.position_y.erase(objects.position_y.begin() + u);
                    objects.position_z.erase(objects.position_z.begin() + u);
                    objects.velocity_x.erase(objects.velocity_x.begin() + u);
                    objects.velocity_y.erase(objects.velocity_y.begin() + u);
                    objects.velocity_z.erase(objects.velocity_z.begin() + u);
                    objects.mass.erase(objects.mass.begin() + u);
                    --num_objects;
                    --u;
                }
            }
        }


    }
    final_file.open("final_conf.txt");
    final_file<<fixed << setprecision(3)  << size_enclosure << " " << time_step << " " << (int)num_objects<< "\n"; /* then put in each line position velocity y mass*/
    for(int i=0; i<(int)num_objects; i++){
        // ***** Write final values in the file ******
        final_file << fixed << setprecision(3) <<objects.position_x[i] << " " << objects.position_y[i] << " " << objects.position_z[i]<< " "<< objects.velocity_x[i] << " "<< objects.velocity_y[i] << " " << objects.velocity_z[i] << " "<<objects.mass[i]<<"\n";
    }

    return 0;
}

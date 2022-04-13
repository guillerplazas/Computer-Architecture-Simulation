#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <iomanip> /* Poner 3 decimales*/

#include <vector>

struct object{      //structure objects
    double position_x;
    double position_y;
    double position_z;
    double velocity_x;
    double velocity_y;
    double velocity_z;
    double mass;
};



int main(int argc, char *argv[]) {
    using namespace std;
    const double  G = 6.674e-11;


/* ********** Check type of parameters ***********      */
    // check number of arguments
    // If the number of arguments is less than 6 the program cannot be excuted and we exit it
    cout << argv[0]<<"  Invoked with " << argc -1<< " parameters."<<endl;

    if (argc < 6){
        cout << "Invalid number of arguments." << endl;

    }
    // convert arguments to integers
    double num_objects=0, num_iterations=0, random_seed=0;
    double time_step=0.0;
    double size_enclosure=0.0;

    cout << "Arguments: " << endl; // Parameters for the simulation

    for(int i=0; i < argc; i++){
        if (i==1){
            // atoi allows to convert from string to int, here we want to make sure it is the type we want
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

    /* ******* crear file con este header **************** */
    ofstream file, final_file;
    file.open("init_conf.txt");
    file <<fixed<< setprecision(3)<< size_enclosure << " " << time_step << " " << (int)num_objects<< "\n"; /* luego poner en cada linea position velocity y mass*/


    vector<object> objects; // create a vector of structure of objects
    /* ****************************************** */


    /* ************** UNIFORM RANDOM DISTRIBUTION *********************/
    mt19937_64 gen((int)random_seed); // Standard mersenne_twister_engine seeded with random_value()
    uniform_real_distribution<> dis(0, size_enclosure);
    normal_distribution<> d{pow(10,21),pow(10,15)};

    object obj{};
    for(int i=0; i<num_objects; i++){
        //   ***** Store initial values in the array of objects *******
        obj.position_x= dis(gen);
        obj.position_y = dis(gen);
        obj.position_z = dis(gen);
        obj.velocity_x = 0.0;//initial velocity is 0
        obj.velocity_y = 0.0;
        obj.velocity_z = 0.0;
        obj.mass = d(gen);
        // ***** Write initial values in the file ******
        file <<fixed<< setprecision(3) <<obj.position_x << " " << obj.position_y << " " << obj.position_z<< " "<< obj.velocity_x << " "<< obj.velocity_y << " " << obj.velocity_z << " "<<obj.mass<<"\n";
        objects.push_back(obj);
    }
    /* ************************************************************+++++++* */


    vector<double>forceX((int)num_objects,0);
    vector<double>forceY((int)num_objects,0);
    vector<double>forceZ((int)num_objects,0);




    double norm,result;
    double dist_x, dist_y,dist_z;
    double Fx, Fy, Fz;



    for(int i=0;i<(int)num_objects;i++){
        for(int u=i+1;u<(int)num_objects;u++) {
            dist_x = objects[u].position_x - objects[i].position_x;
            dist_y = objects[u].position_y - objects[i].position_y;
            dist_z = objects[u].position_z - objects[i].position_z;
            norm = std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);

            if (norm < 1) {
                obj.velocity_x = objects[i].velocity_x + objects[u].velocity_x;
                obj.velocity_y = objects[i].velocity_y + objects[u].velocity_y;
                obj.velocity_z = objects[i].velocity_z + objects[u].velocity_z;
                obj.mass = objects[i].mass + objects[u].mass;
                obj.position_x= objects[i].position_x;
                obj.position_y= objects[i].position_y;
                obj.position_z= objects[i].position_z;

                objects[u].mass=0;
                objects[i]= obj;
            }
        }
    }

    for(int i=0;i<(int)num_objects;i++){
        if(objects[i].mass==0){
            objects.erase(objects.begin()+i);
            num_objects--;
            i--;
        }

    }


    for ( int n=0; n<(int)num_iterations; n++) {
        //cout<<"ITERATION  "<<n<<endl;
        for (int i = 0; i < (int)num_objects; i++) {

            for (int u = i + 1; u < (int)num_objects; u++) {

                //calculation of the norm
                dist_x = objects[u].position_x - objects[i].position_x;
                dist_y = objects[u].position_y - objects[i].position_y;
                dist_z = objects[u].position_z - objects[i].position_z;

                norm = dist_x * dist_x + dist_y * dist_y + dist_z * dist_z;
                norm = std:: sqrt(norm);
                norm = norm*norm*norm;

                //Computation of the forces in the three coordinates for each object n
                result = G * objects[i].mass * objects[u].mass;
                Fx = result*dist_x/norm;
                Fy = result*dist_y/norm;
                Fz = result*dist_z/norm;





                forceX[i] = forceX[i] + Fx;
                forceX[u] = forceX[u] + (-1.0) * Fx ;
                forceY[i] = forceY[i] + Fy;
                forceY[u] = forceY[u] + (-1.0) * Fy ;
                forceZ[i] = forceZ[i] + Fz;
                forceZ[u] = forceZ[u] + (-1.0) * Fz ;


            }
            /* ****VELOCITY*** */
            // Velocities corresponding to the time step and forces
            objects[i].velocity_x += (forceX[i]/objects[i].mass)*time_step ;
            objects[i].velocity_y += (forceY[i]/objects[i].mass)*time_step ;
            objects[i].velocity_z += (forceZ[i]/objects[i].mass)*time_step ;

            // Calculation of the position updated
            objects[i].position_x += objects[i].velocity_x * time_step;
            objects[i].position_y += objects[i].velocity_y * time_step;
            objects[i].position_z += objects[i].velocity_z * time_step;



            forceX[i] = 0.0;
            forceY[i] = 0.0;
            forceZ[i] = 0.0;



            /* *************** REBOUND EFFECT *************** */
            // If the position is greater (in any direction) than the dimensions of the cubic enclosure we must establish the position as the cubes'
            // dimensions and we set the velocities in the opposite direction (as a rebound effect)
            if (objects[i].position_x >= size_enclosure) {
                objects[i].position_x = size_enclosure;
                objects[i].velocity_x = (-1.0) * objects[i].velocity_x;
            }
            if (objects[i].position_y >= size_enclosure) {
                objects[i].position_y = size_enclosure;
                objects[i].velocity_y = (-1.0) * objects[i].velocity_y;
            }
            if (objects[i].position_z >= size_enclosure) {
                objects[i].position_z = size_enclosure;
                objects[i].velocity_z = (-1.0) * objects[i].velocity_z;
            }
            if (objects[i].position_x <= 0.0) {
                objects[i].position_x = 0.0;
                objects[i].velocity_x = (-1.0) * objects[i].velocity_x;
            }
            if (objects[i].position_y <= 0.0) {
                objects[i].position_y = 0.0;
                objects[i].velocity_y = (-1.0) * objects[i].velocity_y;
            }
            if (objects[i].position_z <= 0.0) {
                objects[i].position_z = 0.0;
                objects[i].velocity_z = (-1.0) * objects[i].velocity_z;
            }

        }

        for(int i=0;i<(int)num_objects;i++){
            for(int u=i+1;u<(int)num_objects;u++) {
                dist_x = objects[u].position_x - objects[i].position_x;
                dist_y = objects[u].position_y - objects[i].position_y;
                dist_z = objects[u].position_z - objects[i].position_z;
                norm = std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);

                if (norm < 1) {// Therefore they are considered to collide
                    // We update the data, their velocities and masses are sumed
                    obj.velocity_x = objects[i].velocity_x + objects[u].velocity_x;
                    obj.velocity_y = objects[i].velocity_y + objects[u].velocity_y;
                    obj.velocity_z = objects[i].velocity_z + objects[u].velocity_z;
                    obj.mass = objects[i].mass + objects[u].mass;
                    obj.position_x= objects[i].position_x;
                    obj.position_y= objects[i].position_y;
                    obj.position_z= objects[i].position_z;

                    objects[u].mass=0;
                    objects[i]= obj;


                }

            }
        }
        for(int i=0;i<(int)num_objects;i++){
            if(objects[i].mass==0){
                objects.erase(objects.begin()+i);
                num_objects--;
                i--;
            }

        }
    }

    final_file.open("final_conf.txt");
    final_file<<fixed << setprecision(3)  << size_enclosure << " " << time_step << " " << (int)num_objects<< "\n";

    for(int i=0; i<(int)num_objects; i++){
        // ***** Write final values in the file ******
        final_file <<fixed << setprecision(3) <<objects[i].position_x << " " << objects[i].position_y << " " << objects[i].position_z<< " "<< objects[i].velocity_x << " "<< objects[i].velocity_y << " " << objects[i].velocity_z << " "<<objects[i].mass<<"\n";

    }

    return 0;
}
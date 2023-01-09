#include <iostream>
#include <string>
#include <dirent.h>

#include <arguments.hpp>
#include <polyline.hpp>
#include <optimization.hpp>

#include <filesystem>

#define L "10"
#define MIN_OR_MAX "max"
#define THRESHOLD "0.1"

int main(int argc, char *argv[]) {

    if (argc < 5) throw std::invalid_argument("Wrong arguments!");
    if (strcmp(argv[1], "-i") || strcmp(argv[3], "-o")) throw std::invalid_argument("Wrong arguments!");
    std::string path = std::string(argv[2]), out = std::string(argv[4]);

    if (argc > 5) {
        if (argc != 7) throw std::invalid_argument("Wrong arguments!");
        if (strcmp(argv[5], "-preprocess")) throw std::invalid_argument("Wrong arguments!");
        std::string preprocess = std::string(argv[6]);
    }
    const std::string poly_algos[2] = {"incremental", "convex_hull"};
    const std::string edge_sel[3] = {"1", "2", "3"};
    const std::string init[4] = {"1a", "1b", "2a", "2b"}; //incremental only

    // part 2
    const std::string opt_algos[2] = {"local_search", "simulated_annealing"};
    const std::string annealing[3] = {"local", "global", "subdivision"}; //simulated annealing only no subdiv



    try {
        // open dir
        DIR *d;
        struct dirent *dir;
        d = opendir(path.c_str());
        if (d)
        {
            // write initial lines to file
            std::ofstream file(out);
            file << "       ||                  local_search                 ||              simulated_annealing              ||" << std::endl;
            file << "Size   || min score | max score | min bound | max bound || min score | max score | min bound | max bound ||" << std::endl;

            while ((dir = readdir(d)) != NULL)
            {
                arguments arg(dir->d_name);
                // wtire size of points of file
                
                // loop throught algorithms
                for (int l = 0; l < 2; l++){
                    double min_score = 1;
                    double max_score = 0;
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 3; j++) {
                            for (int k = 0; k < 4; k++) {
                                // create initial polygon
                                polyline S(arg.get_points(),poly_algos[i], edge_sel[j], init[k], out);
                
                                if ( l = 0) {
                                    // optimize polygon area, write results in given out_file
                                    optimization O(S.get_pl_points(),S.get_poly_line(), opt_algos[l], L, "min", THRESHOLD, out, S.get_area(), S.get_ch_area());
                                    optimization O2(S.get_pl_points(),S.get_poly_line(), opt_algos[l], L, "max", THRESHOLD, out, S.get_area(), S.get_ch_area());
                                    // ti einai to bound
                                    if (O.get_end_area() < min_score) min_score = O.get_end_area();
                                    if (O2.get_end_area() > max_score) max_score = O2.get_end_area();
                                }
                                else
                                    for ( int m = 0; m < 3; m++) {
                                        // optimize polygon area, write results in given out_file
                                        optimization O(S.get_pl_points(),S.get_poly_line(), opt_algos[l], L, "min", annealing[m], out, S.get_area(), S.get_ch_area());
                                        optimization O2(S.get_pl_points(),S.get_poly_line(), opt_algos[l], L, "max", annealing[m], out, S.get_area(), S.get_ch_area());
                        
                                        if (O.get_end_area() < min_score) min_score = O.get_end_area();
                                        if (O2.get_end_area() > max_score) max_score = O2.get_end_area();
                                    }
                            }
                        }
                    }
                    // here write results of each algo to file
                    file << "" 
                }
    // here write new line to file

            }
            closedir(d);
        }
            
    } catch (std::invalid_argument const &ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Usage: ./optimal_polygon -i <input_file> -o <output_file> -algorithm <\'local_search\' OR \'simulated_annealing\'> -L  < L parameter >  -max [maximal area polygonization]  -min [minimal area polygonization]  -threshold < double > | with \'local_search\' algorithm only> -annealing < \'local\' OR \'global\' OR \'subdivision\' > | with \'simulated_annealing\' algorithm only" << std::endl;
        return -1;
    } catch (std::exception const &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
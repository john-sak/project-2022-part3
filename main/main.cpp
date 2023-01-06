#include <iostream>
#include <string>

#include <arguments.hpp>
#include <polyline.hpp>
#include <optimization.hpp>

int main(int argc, char *argv[]) {

    // const std::string POLY_ALGO = "incremental";
    // const std::string EDDGE_SEL = "1";
    // const std::string INIT = "1a";

    // part 1
    const std::string poly_algos[2] = {"incremental", "convex_hull"};
    const std::string edge_sel[3] = {"1", "2", "3"};
    const std::string init[4] = {"1a", "1b", "2a", "2b"}; //incremental only
    // part 2
    const std::string opt_algos[2] = {"local_search", "simulated_annealing"};
    const std::string  annealing[3] = {"local", "global", "subdivision"}; //simulated annealing only


    try {
        // here new class for reading the folder first orrr just reading the folder and loop files
        while (file in folder) {
            // arg only for reading files
            arguments arg(argc, argv);
            // loop through  all combinations of part 1 and part 2 algorithms
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 4; k++) {
                        // create initial polygon
                        polyline S(arg.get_points(),poly_algos[i], edge_sel[j], init[k], arg.get_out_file());
                        for (int l = 0; l < 2; l++){
                            if ( l = 0) {
                                // optimize polygon area, write results in given out_file
                                optimization O(S.get_pl_points(),S.get_poly_line(), opt_algos[l], L, MIN_OR_MAX, THREASHOLD, arg.get_out_file(), S.get_area(), S.get_ch_area());

                            }
                            else
                                for ( int m = 0; m < 3; m++) {
                                    // optimize polygon area, write results in given out_file
                                    optimization O(S.get_pl_points(),S.get_poly_line(), opt_algos[l], L, MIN_OR_MAX, annealing[m], arg.get_out_file(), S.get_area(), S.get_ch_area());
                                }
                        }
                    }
                }

            }
            
            
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

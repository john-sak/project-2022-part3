#include <iostream>
#include <string>
#include <dirent.h>
#include <time.h>

#include <arguments.hpp>
#include <polyline.hpp>
#include <optimization.hpp>

#define DEFAULT_L "10"
#define DEFAULT_THRESHOLD "0.1"

int main(int argc, char *argv[]) {

    if (argc < 5) throw std::invalid_argument("Wrong arguments!");
    if (strcmp(argv[1], "-i") || strcmp(argv[3], "-o")) throw std::invalid_argument("Wrong arguments!");
    std::string path = std::string(argv[2]), out = std::string(argv[4]);

    int preprocess = 0;
    if (argc > 5) {
        if (argc != 6) throw std::invalid_argument("Wrong arguments!");
        if (strcmp(argv[5], "-preprocess")) throw std::invalid_argument("Wrong arguments!");
        preprocess = 1;
    }

    std::string L = DEFAULT_L;
    std::string THRESHOLD = DEFAULT_THRESHOLD;

    if (preprocess == 1) {
        // preprocess should find optimal values for L and THRESHOLD (?) and save them
        // on std::strings L and THRESHOLD
    }

    const std::string poly_algos[2] = {"incremental", "convex_hull"};
    const std::string edge_sel[3] = {"1", "2", "3"};
    const std::string init[4] = {"1a", "1b", "2a", "2b"}; //incremental only

    const std::string opt_algos[2] = {"local_search", "simulated_annealing"};
    const std::string annealing[3] = {"local", "global", "subdivision"}; //simulated annealing only no subdiv

    try {

        // open dir
        DIR *d;
        struct dirent *dir;
        if ((d = opendir(path.c_str())) == NULL) throw std::invalid_argument("Cannot open directory \'" + path + "\'");

        // write initial lines to file
        std::ofstream file(out);
        file << "      ||                  local_search                 ||              simulated_annealing              ||" << std::endl;
        file << "======||===============================================||===============================================||" << std::endl;
        file << "Size  || min score | max score | min bound | max bound || min score | max score | min bound | max bound ||" << std::endl;

        while ((dir = readdir(d)) != NULL) {
            if (!std::string(dir->d_name).compare("..") || !std::string(dir->d_name).compare(".")) continue;
            arguments arg(const_cast<char *> ((path + "/" + std::string(dir->d_name)).c_str()));
            // write size of points of file
            int size = arg.get_points().size();
            file << std::left << std::setw(6) << size << "|";

            // loop through algorithms


            // for local_search, simulated_annealing
            for (int l = 0; l < 2; l++) {

                double min_score = 0;
                double max_score = 0;
                double bound_min = 0;
                double bound_max = 1;
                // for incremental, convex_hull
                for (int i = 0; i < 2; i++) {
                    // for edge_selecion 1, 2, 3
                    for (int j = 0; j < 3; j++) {
                        // if incremental
                        if (i == 0) {
                            // for initialization 1a, 1b, 2a, 2b
                            for (int k = 0; k < 4; k++) {
                                //create initial polygon    
                                polyline S(arg.get_points(), poly_algos[i], edge_sel[j], init[k]);
                                time_t time_remain = S.get_time_remain();
                                if (time_remain <= 0 ) continue;
                                // if local_search
                                if (l == 0) {
                                    optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", THRESHOLD, S.get_area(), S.get_ch_area(),time_remain);
                                    if (O_min.get_time_remain() > 0) {
                                        min_score += O_min.get_end_area();
                                        if ((O_min.get_end_area() / S.get_ch_area()) > bound_min) bound_min = O_min.get_end_area() / S.get_ch_area();
                                    }
                                    else bound_min = 1;
                                    optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", THRESHOLD, S.get_area(), S.get_ch_area(),time_remain);
                                    if (O_max.get_time_remain() > 0) {
                                        max_score += O_max.get_end_area();
                                        if ((O_max.get_end_area()/ S.get_ch_area()) < bound_max) bound_max = O_max.get_end_area()/ S.get_ch_area();
                                    }
                                    else bound_max = 0;

                                // if simulated_annealing
                                } else {
                                    for (int m = 0; m < 2; m++) {
                                        optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", annealing[m], S.get_area(), S.get_ch_area(),time_remain);                                        
                                        if (O_min.get_time_remain() > 0) {
                                            min_score += O_min.get_end_area();
                                            if ((O_min.get_end_area() / S.get_ch_area()) > bound_min) bound_min = O_min.get_end_area() / S.get_ch_area();
                                        }
                                        else bound_min = 1;
                                        optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", annealing[m], S.get_area(), S.get_ch_area(),time_remain);
                                        if (O_max.get_time_remain() > 0) {
                                            max_score += O_max.get_end_area();
                                            if ((O_max.get_end_area()/ S.get_ch_area()) < bound_max) bound_max = O_max.get_end_area()/ S.get_ch_area();
                                        }
                                        else bound_max = 0;

                                    }
                                }
                            }
                        // if convex_hull
                        } else {
                            //create initial polygon
                            polyline S(arg.get_points(), poly_algos[i], edge_sel[j], "");
                            time_t time_remain = S.get_time_remain();
                            if (time_remain <= 0 ) continue;

                            // if local_search
                            if (l == 0) {
                                optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", THRESHOLD, S.get_area(), S.get_ch_area(),time_remain);                                
                                if (O_min.get_time_remain() > 0) {
                                    min_score += O_min.get_end_area();
                                    if ((O_min.get_end_area() / S.get_ch_area()) > bound_min) bound_min = O_min.get_end_area() / S.get_ch_area();
                                }
                                else bound_min = 1;
                                optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", THRESHOLD, S.get_area(), S.get_ch_area(),time_remain);
                                if (O_max.get_time_remain() > 0) {
                                    max_score += O_max.get_end_area();
                                    if ((O_max.get_end_area()/ S.get_ch_area()) < bound_max) bound_max = O_max.get_end_area()/ S.get_ch_area();
                                }
                                else bound_max = 0;

                            // if simulated_annealing
                            } else {
                                for (int m = 0; m < 2; m++) {                                    
                                    optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", annealing[m], S.get_area(), S.get_ch_area(),time_remain);
                                    if (O_min.get_time_remain() > 0) {
                                        min_score += O_min.get_end_area();
                                        if ((O_min.get_end_area() / S.get_ch_area()) > bound_min) bound_min = O_min.get_end_area() / S.get_ch_area();
                                    }
                                    else bound_min = 1;
                                    optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", annealing[m], S.get_area(), S.get_ch_area(),time_remain);
                                    if (O_max.get_time_remain() > 0) {
                                        max_score += O_max.get_end_area();
                                        if ((O_max.get_end_area()/ S.get_ch_area()) < bound_max) bound_max = O_max.get_end_area()/ S.get_ch_area();
                                    }
                                    else bound_max = 0;
                                    
                                }
                            }
                        }
                    }
                }
                // write results of current algorithm to output file
                
                file << std::left << "|" << std::setw(11) << min_score << "|" << std::setw(11) << max_score << "|" << std::setw(11) << bound_min << "|" << std::setw(11) << bound_max << "|";
                    
            }

            // write new line to file
            file << "|" << std::endl;
        }
        file.close();
        closedir(d);
    } catch (std::invalid_argument const &ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Usage: ./evaluate -i <point set path> -o <output file> -preprocess <optional>" << std::endl;
        return -1;
    } catch (std::exception const &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}

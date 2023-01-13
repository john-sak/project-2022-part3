#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <dirent.h>
#include <time.h>

#include <arguments.hpp>
#include <polyline.hpp>
#include <optimization.hpp>

#define DEFAULT_L "10"
#define DEFAULT_THRESHOLD "0.1"

typedef struct {
    float score[8];
} Scores;

int main(int argc, char *argv[]) {

    try {

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

        // open dir
        DIR *d;
        struct dirent *dir;
        if ((d = opendir(path.c_str())) == NULL) throw std::invalid_argument("Cannot open directory \'" + path + "\'");

        std::map<int, Scores> scores_per_size;

        while ((dir = readdir(d)) != NULL) {
            if (!std::string(dir->d_name).compare("..") || !std::string(dir->d_name).compare(".")) continue;
            arguments arg(const_cast<char *> ((path + "/" + std::string(dir->d_name)).c_str()));

            // write size of points of file
            int size = arg.get_points().size();
            Scores best_scores = {1, 0, 0, 1, 1, 0, 0, 1};

            time_t timer = 500 * size;

            // loop through algorithms

            // for local_search, simulated_annealing
            for (int l = 0; l < 2; l++) {

                // for incremental, convex_hull
                for (int i = 0; i < 2; i++) {

                    // for edge_selecion 1, 2, 3
                    for (int j = 0; j < 3; j++) {

                        // if incremental
                        if (i == 0) {

                            // for initialization 1a, 1b, 2a, 2b
                            for (int k = 0; k < 4; k++) {

                                //create initial polygon
                                try {
                                    polyline S(arg.get_points(), poly_algos[i], edge_sel[j], init[k], timer);

                                    time_t time_remain = S.get_time_remain();

                                    if (time_remain <= 0) continue;

                                    // if local_search
                                    if (l == 0) {

                                        float score;

                                        try {
                                            optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", THRESHOLD, S.get_area(), S.get_ch_area(), time_remain);

                                            score = O_min.get_time_remain() > 0 ? O_min.get_end_area() / S.get_ch_area() : 1;
                                            if (score < best_scores.score[0]) best_scores.score[0] = score;
                                            if (score > best_scores.score[2]) best_scores.score[2] = score;
                                        } catch (...) {
                                            // continue;
                                        }

                                        try {
                                            optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", THRESHOLD, S.get_area(), S.get_ch_area(), time_remain);

                                            score = O_max.get_time_remain() > 0 ? O_max.get_end_area() / S.get_ch_area() : 0;
                                            if (score > best_scores.score[1]) best_scores.score[1] = score;
                                            if (score < best_scores.score[3]) best_scores.score[3] = score;
                                        } catch (...) {
                                            // continue;
                                        }

                                    // if simulated_annealing
                                    } else {
                                        for (int m = 0; m < 2; m++) {

                                            float score;

                                            try {
                                                optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", annealing[m], S.get_area(), S.get_ch_area(), time_remain);

                                                score = O_min.get_time_remain() > 0 ? O_min.get_end_area() / S.get_ch_area() : 1;
                                                if (score < best_scores.score[4]) best_scores.score[4] = score;
                                                if (score > best_scores.score[6]) best_scores.score[6] = score;
                                            } catch (...) {
                                                // continue;
                                            }

                                            try {
                                                optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", annealing[m], S.get_area(), S.get_ch_area(), time_remain);

                                                score = O_max.get_time_remain() > 0 ? O_max.get_end_area() / S.get_ch_area() : 0;
                                                if (score > best_scores.score[5]) best_scores.score[5] = score;
                                                if (score < best_scores.score[7]) best_scores.score[7] = score;
                                            } catch (...) {
                                                // continue;
                                            }
                                        }
                                    }
                                } catch (...) {
                                    // continue;
                                }
                            }

                        // if convex_hull
                        } else {

                            //create initial polygon
                            try {
                                polyline S(arg.get_points(), poly_algos[i], edge_sel[j], "", timer);

                                time_t time_remain = S.get_time_remain();

                                if (time_remain <= 0) continue;

                                // if local_search
                                if (l == 0) {

                                    float score;

                                    try {
                                        optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", THRESHOLD, S.get_area(), S.get_ch_area(), time_remain);

                                        score = O_min.get_time_remain() > 0 ? O_min.get_end_area() / S.get_ch_area() : 1;
                                        if (score < best_scores.score[0]) best_scores.score[0] = score;
                                        if (score > best_scores.score[2]) best_scores.score[2] = score;
                                    } catch (...) {
                                        // continue;
                                    }

                                    try {
                                        optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", THRESHOLD, S.get_area(), S.get_ch_area(), time_remain);

                                        score = O_max.get_time_remain() > 0 ? O_max.get_end_area() / S.get_ch_area() : 0;
                                        if (score > best_scores.score[1]) best_scores.score[1] = score;
                                        if (score < best_scores.score[3]) best_scores.score[3] = score;
                                    } catch (...) {
                                        // continue;
                                    }

                                // if simulated_annealing
                                } else {
                                    for (int m = 0; m < 2; m++) {

                                        float score;

                                        try {
                                            optimization O_min(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "min", annealing[m], S.get_area(), S.get_ch_area(), time_remain);

                                            score = O_min.get_time_remain() > 0 ? O_min.get_end_area() / S.get_ch_area() : 1;
                                            if (score < best_scores.score[4]) best_scores.score[4] = score;
                                            if (score > best_scores.score[6]) best_scores.score[6] = score;
                                        } catch (...) {
                                            // continue;
                                        }

                                        try {
                                            optimization O_max(S.get_pl_points(), S.get_poly_line(), opt_algos[l], L, "max", annealing[m], S.get_area(), S.get_ch_area(), time_remain);

                                            score = O_max.get_time_remain() > 0 ? O_max.get_end_area() / S.get_ch_area() : 0;
                                            if (score > best_scores.score[5]) best_scores.score[5] = score;
                                            if (score < best_scores.score[7]) best_scores.score[7] = score;
                                        } catch (...) {
                                            // continue;
                                        }
                                    }
                                }
                            } catch (...) {
                                // continue;
                            }
                        }
                    }
                }
            }

            std::pair<std::map<int, Scores>::iterator, bool> ret = scores_per_size.insert(std::pair<int, Scores> (size, best_scores));

            if (ret.second == false) {
                ret.first->second.score[0] += best_scores.score[0];
                ret.first->second.score[2] += best_scores.score[2];
                ret.first->second.score[4] += best_scores.score[4];
                ret.first->second.score[6] += best_scores.score[6];

                ret.first->second.score[1] = std::max({ret.first->second.score[1], best_scores.score[1]});
                ret.first->second.score[3] = std::min({ret.first->second.score[3], best_scores.score[3]});
                ret.first->second.score[5] = std::max({ret.first->second.score[5], best_scores.score[5]});
                ret.first->second.score[7] = std::min({ret.first->second.score[7], best_scores.score[7]});
            }
        }
        closedir(d);

        // write results to file
        std::ofstream file(out);

        file << "      ||                  local_search                 ||              simulated_annealing              ||" << std::endl;
        file << "======||===============================================||===============================================||" << std::endl;
        file << " size || min score | max score | min bound | max bound || min score | max score | min bound | max bound ||" << std::endl;

        for (auto& it: scores_per_size) {
            file << std::left << std::setw(6) << it.first << "|";
            for (int i = 0; i < 8; i++) {
                file << std::left << "|" << std::setw(11) << it.second.score[i];
                if (i == 3) file << "|";
            }
            file << "||" << std::endl;
        }

        file.close();
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

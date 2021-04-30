#include "Sources\\.h\\matrix.h"
#include "Sources\\.h\\polynom.h"
#include "Sources\\.h\\gmres.h"
#include "Sources\\.h\\csr.h"
#include "Sources\\.h\\krylov.h"
#include "Sources\\.h\\back_gauss.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>

int to_int(std::string s) {
    int ans = 0;
    for (char i : s) {
        ans *= 10;
        ans += (i - '0');
    }
    return ans;
}

double to_float(std::string s, std::ofstream& fer) {
    int l_float = 0;
    bool flag = false;
    double ans = 0;
    for (char i : s) {
        if (i == '.' || i == ',') {
            if (flag) {
                fer << "WARNING! Data contains element, that is not number.\n";
                return 0;
            }
            flag = true;
        } else {
            if (i - '0' >= 0 && i - '0' <= 9) {
                ans *= 10;
                ans += i - '0';
                if (flag) {
                    l_float++;
                }
            } else {
                fer << "WARNING! Data contains element, that is not number.\n";
                return 0;
            }
        }
    }
    for (int i = 0; i < l_float; i++) {
        ans /= 10;
    }
    return ans;
}

int main() {
    std::ifstream fdata, fset;
    std::ofstream fout, flog, ftimelog, fer;
    fdata.open("data.txt");
    fset.open("settings.txt");
    fout.open("approx_result.txt");
    flog.open("Logs\\MAIN_log.txt");
    fer.open("Logs\\MAIN_errors_log.txt");
    ftimelog.open("Logs\\MAIN_time_log.txt");

    if(!fdata.is_open()) {
        fer << "FATAL ERROR! File data.txt is not found.\n";
        return 0;
    }
    if(!fdata.is_open()) {
        fer << "FATAL ERROR! File settings.txt is not found.\n";
        return 0;
    }

    std::string approximation_table[7] = {"polynom", "best_polynom", "ln", "pow", "exp", "hyper", "top"};
    std::map <std::string, std::string> settings = {{"title"       , ""    },
                                                    {"x_axis"      , ""    },
                                                    {"y_axis"      , ""    },
                                                    {"grid"        , "none"},
                                                    {"line_color"  , "blue"},
                                                    {"line_style"  , "-"   },
                                                    {"dot_color"   , "red" },
                                                    {"polynom"     , "0"   },
                                                    {"best_polynom", "0"   },
                                                    {"ln"          , "0"   },
                                                    {"pow"         , "0"   },
                                                    {"exp"         , "0"   },
                                                    {"hyper"       , "0"   },
                                                    {"top"         , "0"   }};
    std::map <std::string, std::string> :: iterator it = settings.begin();

    clock_t start = clock();
    clock_t curr = clock();
    while (true) {
        std::string curr;
        getline(fset, curr);
        std::string key = "", value = "";
        bool flag = false;
        for (char i : curr) {
            if (i == ' ') {
                flag = true;
            } else {
                if (flag) {
                    value += i;
                } else {
                    key += i;
                }
            }
        }
        if (fset.eof()) {
            break;
        }
        if (settings.find(key) != settings.end() || key == "all") {
            if (key == "polynom") {
                settings[key] = settings[key] + ' ' + value;
                continue;
            }
            if (key == "all") {
                settings["ln"] = "1";
                settings["pow"] = "1";
                settings["exp"] = "1";
                settings["hyper"] = "1";
                settings["top"] = "1";
                continue;
            }
            if (value == "") {
                settings[key] = "1";
            } else {
                settings[key] = value;
            }
        } else {
            if (key == "" || key == " ") {
                continue;
            }
            fer << "WARNING! Wrong input: " << key << " is not a flag." << '\n';
        }
    }
    curr = clock() - curr;
    ftimelog << "Settings reading                              " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
    flog << "Settings reading:                           successful\n";
    curr = clock();

    int n = 1, m = 0;
    std::string tmp;
    while (true) {
        getline(fdata, tmp);
        if (fdata.eof()) {
            break;
        }
        int tmp_m = 0;
        std::istringstream ist(tmp);
        while (ist >> tmp) {
            tmp_m++;
        }
        if (m < tmp_m) {
            m = tmp_m;
        }
        n++;
    }
    if (n != 2 && m != 2) {
        fer << "CRITICAL ERROR! Wrong data format.\n";
        return 0;
    }
    fdata.close();
    curr = clock() - curr;
    ftimelog << "Data reading                                  " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
    flog << "Data reading:                               successful\n";
    flog << "Data array dimentions are:                  " << n << ' ' << m << '\n';
    curr = clock();

    fdata.open("data.txt");
    double data_x[std::max(n, m)], data_y[std::max(n, m)];
    if (n == 2) {
        std::string tmp;
        getline(fdata, tmp);
        std::istringstream ist(tmp);
        for (int i = 0; i < m; i++) {
            ist >> tmp;
            data_x[i] = to_float(tmp, fer);
        }
        getline(fdata, tmp);
        std::istringstream ist2(tmp);
        for (int i = 0; i < m; i++) {
            ist2 >> tmp;
            data_y[i] = to_float(tmp, fer);
        }
    } else {
        for (int i = 0; i < std::max(n, m); i++) {
            std::string tmp;
            getline(fdata, tmp);
            std::istringstream ist(tmp);
            ist >> tmp;
            data_x[i] = to_float(tmp, fer);
            ist >> tmp;
            data_y[i] = to_float(tmp, fer);
        }
    }
    if (n < m) {
        std::swap(n, m);
    }
    curr = clock() - curr;
    ftimelog << "Data copying                                  " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
    flog << "Data copying:                               successful\n";
    curr = clock();
/*
    for (auto i : approximation_table) {
        std::cout << i << ' ' << settings[i] << '\n';
    }
*/
    for (i : approximation_table) {
        if (i == "polynom") {
            if (settings[i].size() != 1) {
                std::istringstream str_stream(settings[i]);
                std::string tmp;
                str_stream >> tmp;
                while (str_stream >> tmp) {
                    int m = to_int(tmp);
                    long double powers[m+1][m+1];
                    long double results_[m + 1][1];
                    for (int j = 0; j < m + 1; j++) {
                        results_[j][0] = 0;
                        for (int k = 0; k < m + 1; k++) {
                            powers[j][k] = 0;
                        }
                    }
                    for (int ind = 0; ind < n; ind++) {
                        for (int j = 0; j < m + 1; j++) {
                            for (int k = j; k < m + 1 + j; k++) {
                                powers[j][k - j] += pow(data_x[ind], k);
                            }
                        }
                    }

                    for (int j = 0; j < m + 1; j++) {
                        for (int ind = 0; ind < n; ind++) {
                            results_[j][0] += data_y[ind] * pow(data_x[ind], j);
                        }
                    }
                    //Matrix <long double> matrix(*powers, m + 1, m + 1);
                    std::vector<long double> results(m + 1)
                    for (int i = 0; i < m + 1; i++){
                        results[i] = results_[i][0];
                    }
                    std::vector<long double> ans = gmres(CSR<double> (m + 1, m + 1, to_tripl(powers, m + 1, m + 1)), results);
                    //std::cout << ans << '\n';
                    if (ans.size() != m + 1) {
                        fer << "CRITICAL ERROR! Something went wrong: approximation by polynom.\n";
                        flog << "Approximation by polynom:                     failed\n";
                        curr = clock() - curr;
                        ftimelog << "Approximation by polynom                        " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                        curr = clock();
                        continue;
                    }
                    fout << "y = " << ans[0];
                    for (int j = 1; j < m + 1; j++) {
                        if (ans[j] < 0) {
                            fout << " - " << fabs(ans[j]);
                        } else {
                            fout << " + " << ans[j];
                        }
                        fout << "*x^" << j;
                    }
                    fout << '\n';
                    curr = clock() - curr;
                    ftimelog << "Approximation by polynom " << m << "                    " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                    flog << "Approximation by polynom " << m << ":                 successful\n";
                    curr = clock();
                }
            }
            continue;
        }
        if (i == "pow" && settings[i] != "0") {
            double sum_ln_x = 0, sum_ln_y = 0, sum_ln_x_y = 0, sum_ln_x2 = 0, sum_ln_y2 = 0;
            bool flag = false;
            for (int j = 0; j < n; j++) {
                if (data_x[j] <= 0 || data_y[j] <= 0) {
                    fer << "CRITICAL ERROR! Can not build approximation by power when data contains negative number or 0\n";
                    flog << "Approximation by power:                     failed\n";
                    flag = true;
                    break;
                }
                double ln_y = log(fabs(data_y[j])), ln_x = log(fabs(data_x[j]));
                sum_ln_x += ln_x;
                sum_ln_y += ln_y;
                sum_ln_x_y += ln_x * ln_y;
                sum_ln_x2 += ln_x*ln_x;
                sum_ln_y2 += ln_y*ln_y;
            }
            if (flag) {
                curr = clock() - curr;
                ftimelog << "Approximation by power                        " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                curr = clock();
                continue;
            }
            double b = (n*sum_ln_x_y - sum_ln_x*sum_ln_y)/(n*sum_ln_x2 - sum_ln_x*sum_ln_x);
            double a = exp(sum_ln_y/n - b*sum_ln_x/n);
            fout << "y = " << a << " * x^" << b << '\n';
            curr = clock() - curr;
            ftimelog << "Approximation by power                        " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
            flog << "Approximation by power:                     successful\n";
            curr = clock();
            continue;
        }
        if (i == "top" && settings[i] != "0") {
            double sum_x = 0, sum_ln_y = 0, sum_x2 = 0, sum_x_ln_y = 0;
            bool flag = false;
            for (int j = 0; j < n; j++) {
                if (data_x[j] <= 0 || data_y[j] <= 0) {
                    fer << "CRITICAL ERROR! Can not build approximation by degree indicator when data contains negative number or 0\n";
                    flog << "Approximation by degree indicator:          failed\n";
                    flag = true;
                    break;
                }
                double ln_y = log(fabs(data_y[j])), x = data_x[j];
                sum_ln_y += ln_y;
                sum_x += x;
                sum_x2 += x*x;
                sum_x_ln_y += ln_y * x;
            }
            if (flag) {
                curr = clock() - curr;
                ftimelog << "Approximation by degree indicator             " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                curr = clock();
                continue;
            }
            double b = exp((n*sum_x_ln_y - sum_x * sum_ln_y)/(n*sum_x2 - sum_x * sum_x));
            double a = exp(sum_ln_y/n - log(fabs(b))*sum_x/n);
            fout << "y = " << a << " * " << b << "^x\n";
            curr = clock() - curr;
            ftimelog << "Approximation by degree indicator             " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
            flog << "Approximation by degree indicator:          successful\n";
            curr = clock();
            continue;
        }
        if (i == "ln" && settings[i] != "0") {
            double sum_y_lnx = 0, sum_lnx = 0, sum_y = 0, sum_lnx2 = 0;
            bool flag = false;
            for (int j = 0; j < n; j++) {
                if (data_x[j] <= 0) {
                    fer << "CRITICAL ERROR! Can not build approximation by log when data(x) contains negative number or 0\n";
                    flog << "Approximation by log:                       failed\n";
                    flag = true;
                    break;
                }
                double y = data_y[j], ln_x = log(data_x[j]);
                sum_y_lnx += y*ln_x;
                sum_lnx += ln_x;
                sum_y += y;
                sum_lnx2 += ln_x*ln_x;
            }
            if (flag) {
                curr = clock() - curr;
                ftimelog << "Approximation by log                          " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                curr = clock();
                continue;
            }
            double b = (n*sum_y_lnx - sum_lnx*sum_y)/(n*sum_lnx2 - sum_lnx*sum_lnx);
            double a = sum_y/n - b*sum_lnx/n;
            if (b < 0) {
                fout << "y = " << a << " - " << fabs(b) << "*(ln(x))\n";
            } else {
                fout << "y = " << a << " + " << b << "*(ln(x))\n";
            }
            curr = clock() - curr;
            ftimelog << "Approximation by log                          " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
            flog << "Approximation by log:                       successful\n";
            curr = clock();
            continue;
        }
        if (i == "exp" && settings[i] != "0") {
            double sum_x_lny = 0, sum_x = 0, sum_lny = 0, sum_x2 = 0;
            bool flag = false;
            for (int j = 0; j < n; j++) {
                if (data_y[j] <= 0) {
                    fer << "CRITICAL ERROR! Can not build approximation by exp when data(y) contains negative number or 0\n";
                    flog << "Approximation by exp:                       failed\n";
                    flag = true;
                    break;
                }
                double ln_y = log(data_y[j]), x = data_x[j];
                sum_x_lny += x*ln_y;
                sum_x += x;
                sum_x2 += x*x;
                sum_lny += ln_y;
            }
            if (flag) {
                curr = clock() - curr;
                ftimelog << "Approximation by exponent                     " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                curr = clock();
                continue;
            }
            double b = (n*sum_x_lny - sum_x*sum_lny)/(n*sum_x2 - sum_x*sum_x);
            double a = sum_lny/n - b*sum_x/n;
            if (b < 0) {
                fout << "y = exp(" << a << " - " << fabs(b) << "*x)\n";
            } else {
                fout << "y = exp(" << a << " + " << b << "*x)\n";
            }
            curr = clock() - curr;
            ftimelog << "Approximation by exponent                     " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
            flog << "Approximation by exp:                       successful\n";
            curr = clock();
            continue;
        }
        if (i == "hyper" && settings[i] != "0") {
            double sum_y_per_x = 0, sum_1_per_x = 0, sum_1_per_x2 = 0, sum_y = 0;
            bool flag = false;
            for (int j = 0; j < n; j++) {
                double y = data_y[j], x = data_x[j];
                if (x == 0) {
                    fer << "CRITICAL ERROR! Can not build approximation by hyperbole when data(x) contains 0\n";
                    flog << "Approximation by hyperbole:                 failed\n";
                    flag = true;
                    break;
                }
                sum_y_per_x += y/x;
                sum_1_per_x += 1/x;
                sum_1_per_x2 += 1/x/x;
                sum_y += y;
            }
            if (flag) {
                curr = clock() - curr;
                ftimelog << "Approximation by hyperbole                    " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
                curr = clock();
                continue;
            }
            double b = (n*sum_y_per_x - sum_1_per_x*sum_y)/(n*sum_1_per_x2 - sum_1_per_x*sum_1_per_x);
            double a = sum_y/n - b*sum_1_per_x/n;
            if (b < 0) {
                fout << "y = " << a << " - " << fabs(b) << "/x\n";
            } else {
                fout << "y = " << a << " + " << b << "/x\n";
            }
            curr = clock() - curr;
            ftimelog << "Approximation by hyperbole                    " << ((double)curr) / CLOCKS_PER_SEC << " sec\n";
            flog << "Approximation by hyperbole:                 successful\n";
            curr = clock();
            continue;
        }
    }

    ftimelog << "-----------------------------------------------------------\nAll programm                                  " << (double)((clock() - start)) / CLOCKS_PER_SEC << " sec\n";

    ftimelog.close();
    fdata.close();
    fset.close();
    flog.close();
    fout.close();
    fer.close();
}

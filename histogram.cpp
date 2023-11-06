
/*======================================================================================================================

  Yet another code to calculate and plot histogram.

  ACKNOWLEDGEMENTS:
    Alexey D. Kondorskiy, P.N.Lebedev Physical Institute of the Russian Academy of Science.
    E-mail: kondorskiy@lebedev.ru, kondorskiy@gmail.com.

======================================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include <vector>
#include <sys/stat.h>
#include <sstream>


// File name with data to input.
const std::string inp_file_name = "data.txt";

// Number of bins
const int NB = 11;


/*----------------------------------------------------------------------------------------------------------------------
  Convert int to string.
----------------------------------------------------------------------------------------------------------------------*/
std::string intToString(const int &i)
{
  std::stringstream ss;
  ss << i;
  return ss.str();
}


//**********************************************************************************************************************
int main(int argc, char **argv)
{
  // Read the data.
  std::vector<double> data;
  struct stat st;
  if (stat(inp_file_name.c_str(), &st) != 0) {
    std::cout << "Cant find file " << inp_file_name << std::endl;
    return 0;
  } else {
    std::ifstream fin(inp_file_name.c_str(), std::ios::in);
    while(!fin.eof()) {
      double tmp; fin >> tmp; if (!fin.eof()) data.push_back(tmp);
    }
    fin.close();
  }

  // Sort the data.
  std::sort(data.begin(), data.end());

  /* Test print.
  std::string test_file_name = "test-" + inp_file_name;
  std::ofstream fout_t(test_file_name.c_str(), std::ios::out);
  for (int i = 0; i < data.size(); ++i)
    fout_t << data[i] << std::endl;
  fout_t.close();	// */

  // Set bins.
  std::vector<double> x_min, x_max;
  std::vector<int> y_count;
  double h = (data[data.size() - 1] - data[0])/NB;
  for (int i = 0; i < NB; ++i) {
    double x = data[0] + i*h;
    x_min.push_back(x);
    if (i == (NB-1)) x_max.push_back(x + h*1.01); else x_max.push_back(x + h);
    y_count.push_back(0);
  }

  // Count.
  for (int i = 0; i < data.size(); ++i)
    for (int j = 0; j < x_min.size(); ++j )
      if ((x_min[j] <= data[i]) && (data[i] < x_max[j])) y_count[j]++;

  // Print histogram.
  std::string hist_file_name = "hist-" + inp_file_name;
  std::ofstream fout_h(hist_file_name.c_str(), std::ios::out);
  for (int j = 0; j < x_min.size(); ++j)
    fout_h << x_min[j] << " " << x_max[j] << " " << y_count[j] << std::endl;
  fout_h.close();   // */

  // Print histogram for plot.
  std::string plt_file_name = "plt-" + inp_file_name;
  std::ofstream fout_p(plt_file_name.c_str(), std::ios::out);
  int countmax = 0;
  for (int j = 0; j < x_min.size(); ++j) {
    fout_p << x_min[j] << " " << 0.0 << std::endl;
    fout_p << x_min[j] << " " << y_count[j] << std::endl;
    fout_p << x_max[j] << " " << y_count[j] << std::endl;
    fout_p << x_max[j] << " " << 0.0 << std::endl;
    if (countmax < y_count[j]) countmax = y_count[j];
  }
  fout_p.close();

  // Print location for plot.
  std::string loc_file_name = "loc-" + inp_file_name;
  std::ofstream fout_l(loc_file_name.c_str(), std::ios::out);
  for (int i = 0; i < data.size(); ++i)
    for (int j = 0; j < x_min.size(); ++j )
      if ((x_min[j] <= data[i]) && (data[i] < x_max[j]))
        fout_l << data[i] << " " << y_count[j] << std::endl;
  fout_l.close();

  // Plot with GNUplot.
  std::string plt_name = "plot.plt";
  std::ofstream fout_plt(plt_name.c_str(), std::ios::out);
  fout_plt << "set term pngcairo font \"arial,24\" size 1300,700 linewidth 3\n";
  fout_plt << "set size ratio 0.5\n";
  fout_plt << "set nokey\n";
  fout_plt << "set bmargin 6\n";
  fout_plt << "set lmargin 8\n";
  fout_plt << "set mxtics 2\n";
  fout_plt << "set mytics 2\n";
  fout_plt << "set tics out\n";
  fout_plt << "set grid xtics ytics mxtics mytics\n";
  fout_plt << "set style fill solid 0.5\n";
  fout_plt << "set style line 7 lc rgb \'red\'\n";
  fout_plt << "set xrange [" << intToString(x_min[0]*0.99) << ":" << intToString(x_max[x_max.size()-1]*1.01) << "]\n";
  fout_plt << "set yrange [0:" << (int)(countmax*1.2) << "]\n";
  fout_plt << "set output \"histogram.png\"\n";
  fout_plt << "plot \"" << plt_file_name << "\" with filledcurves fillcolor \"blue\" lw 1, \\\n";
  fout_plt << " \"" << loc_file_name << "\" with points ls 7 ps 1\n";
  fout_plt.close();

  std::string command = "C:\\Soft\\gnuplot\\bin\\gnuplot.exe " + plt_name;
  // command = "gnuplot " + plt_name;
  system(command.c_str());
  command = "rm " + plt_name + " " + plt_file_name + " " + loc_file_name;
  system(command.c_str());

  return 0;
};

//======================================================================================================================

/*************************************************************************

 Travelling Salesman Problem with Time Windows

 ---------------------------------------------------------------------

                       Copyright (c) 2009
             Manuel Lopez-Ibanez <manuel.lopez-ibanez@ulb.ac.be>
                  Christian Blum <cblum@lsi.upc.edu>

 This program is free software (software libre); you can redistribute
 it and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, you can obtain a copy of the GNU
 General Public License at: http://www.gnu.org/licenses/gpl.html

 ---------------------------------------------------------------------

  Compilation:

    g++ -o check_solution check_solution.cpp

*************************************************************************/

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <cassert>
#include <string>
#include <cstring>
using namespace std;

#define program_invocation_short_name "check_solution"

typedef double number_t;
#define STRING_NUMBER_IS "double"
#define NUMBER_T_MAX DBL_MAX
#define NUMBER_T_MIN DBL_MIN

class Solution {

public:

  static string instance;  
  static int n;   // number of customers
  static void LoadInstance (string filename);

  vector<int> permutation;

  int _constraint_violations;

  Solution (string filename);

  void print_one_line (FILE *stream=stdout) const;

  number_t makespan() const;
  void evaluate();

private:
  number_t _makespan;
  number_t _tourcost; // Sum of the traversal cost along the tour.

  // time-window start
  static vector<number_t> window_start;

  // time-window end
  static vector<number_t> window_end;

  // travel time/distance
  static vector<vector<number_t> > distance;

};

string Solution::instance;

// number of customers
int Solution::n = 0;

// time-window start
vector<number_t> Solution::window_start;

// time-window end
vector<number_t> Solution::window_end;

// travel time/distance
vector<vector<number_t> > Solution::distance;


Solution::Solution(string filename)
    : permutation (1,0), // Start at the depot.
      _constraint_violations (0),
      _makespan (0),
      _tourcost (0)
{
  permutation.reserve (n+1);
  
  ifstream indata;
  int node;

  indata.open (filename.c_str());
  if (!indata) { // file couldn't be opened
    cout << "error: file " << filename.c_str() << " could not be opened"
         << endl;
    exit (EXIT_FAILURE);
  }

  for (int i = 1; i < n; i++) {
    indata >> node;
    permutation.push_back (node);
  }

  permutation.push_back (0); // Finish at the depot;

  if (!indata) {
    cout << "error: file " << filename.c_str() 
         << " contains invalid solution" << endl;
    exit (EXIT_FAILURE);
  }

  indata.close();
}

void
Solution::LoadInstance (string filename)
{
  ifstream indata;
  number_t rtime;
  number_t ddate;

  instance = filename;
  indata.open (instance.c_str());
  if (!indata) { // file couldn't be opened
    cout << "error: file " << instance.c_str() << " could not be opened"
         << endl;
    exit (EXIT_FAILURE);
  }

  // Customer 0 is the depot.
  indata >> n;
  if (!indata || n <= 0) {
    cout << "error: invalid number of customers" << endl;
    exit (EXIT_FAILURE);
  }

  window_start.reserve(n);
  window_end.reserve(n);
  distance.reserve(n);

  for (int i = 0 ; i < n; i++) {
    distance.push_back(vector<number_t>(n,0));
    for (int j = 0; j < n; j++) {
      indata >> distance[i][j];
    }
  }

  if (!indata) {
    cout << "error: invalid time windows" << endl;
    exit (EXIT_FAILURE);
  }

  for (int i = 0; i < n; i++) {
    indata >> rtime >> ddate;
    window_start.push_back (rtime);
    window_end.push_back(ddate);
  }

  if (!indata) {
    cout << "error: invalid distance matrix" << endl;
    exit (EXIT_FAILURE);
  }

  indata.close();
}

void Solution::print_one_line (FILE *stream) const
{
  fprintf (stream, "makespan = %.2f\ttourcost = %.2f\tconstraint violations = %d\tpermutation =",
           double (makespan()), double(_tourcost), _constraint_violations);

  // Customers 0 and n+1 are always the depot (permutation[0] == 0,
  // permutation[n] == 0), so do not print them.
  for (int i = 1; i < int(permutation.size()) - 1; i++) {
    fprintf (stream, " %d", permutation[i]);
  }
  fprintf (stream, "\n");
}

number_t Solution::makespan() const
{
  return _makespan;
}

void Solution::evaluate()
{
  _makespan = 0;
  _tourcost = 0;
  int prev = 0; // starts at the depot
  int cviols = 0;

  if (int(permutation.size() - 1) != n) {
    printf ("invalid: (permutation.size() == %d) != (n == %d)\n",
            int(permutation.size() - 1), n);
    exit (EXIT_FAILURE);
  }

  for (int i = 1; i < n; i++) {
    int node = permutation[i];

    _tourcost += distance[prev][node];
    _makespan = max (_makespan + distance[prev][node], window_start[node]);
    
    if (_makespan > window_end[node]) {
      cviols++;
    }
    prev = node;
  }

  // finish at the depot
  _tourcost += distance[prev][0];

  _makespan = max (_makespan + distance[prev][0], window_start[0]);

  if (_makespan > window_end[0])
    cviols++;

  _constraint_violations = cviols;
}

static void version(void)
{
  printf ("%s", program_invocation_short_name);
  printf("\n\n"
"Copyright (C) 2009\n"
"Manuel Lopez-Ibanez (manuel.lopez-ibanez@ulb.ac.be) and\n"
"Christian Blum (cblum@lsi.upc.edu)\n"
"\n"
"This is free software, and you are welcome to redistribute it under certain\n"
"conditions.  See the GNU General Public License for details. There is NO   \n"
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
"\n"        );
}

static void usage(void)
{
  printf("\n"
         "Usage: %s INSTANCE_FILE SOLUTION_FILE\n\n", program_invocation_short_name);

    printf(
"Reads an instance file and a file with a permutation (from 1 to N, that is, not containing the depot) and evaluates the solution.\n"
"\n");
    version ();
}

int main(int argc, char **argv)
{
  if (argc != 3) {
    printf ("error: invalid number of parameters\n");
    usage();
    exit (EXIT_FAILURE);
  }
  string input_instance = argv[1];
  string input_solution = argv[2];

  Solution::LoadInstance (input_instance);
  Solution s(input_solution);
  s.evaluate();

  s.print_one_line();
  printf ("\n");

  exit (EXIT_SUCCESS);
}

/*
 * Name:
 * Date Submitted:
 * Lab Section:
 * Assignment Name:
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct point
{
    double x;
    double y;
};

// Hashing function to simplify closestPair() function code
// b is the dimension of the square array
// Using a helper function also allows me to easily change the formula
int spatialHash(double coord, int b) {
  return coord * b;
}

// Utilizes spatialHash() to create the table and return it to closestPair()
vector<vector<vector<point>>> createTable(string filename) {
  int fileSize, b;

  // Open the file
  ifstream inFile;
  inFile.open(filename);

  // Read in fileSize
  inFile >> fileSize;

  // Calculate b using fileSize (THIS MIGHT BE CHANGED LATER)
  b = sqrt(fileSize) / 2;

  // Initialize the vector
  vector<vector<vector<point>>> table(b, vector<vector<point>>(b));

  // Add points to table
  for(int i=0; i < fileSize; i++) {
    int tabx, taby;
    char space; // <- dummy variable because im bad at c++
    point current;

    // Read input
    if(inFile >> current.x >> space >> current.y) {
      // Hash x and y
      tabx = spatialHash(current.x, b);
      taby = spatialHash(current.y, b);

      // Add to table
      table[tabx][taby].push_back(current);
    } else {
      // Bad input was caught, uh oh
      puts("BAD INPUT FROM FILE! Is it formatted correctly?");
      puts("The correct format is '[x position] [y position]'");
    }
  }

  // Close file and return the created table
  inFile.close();
  return table;
}

// Helper function, determines the distance between points
double ptDistance(point one, point two) {
  return sqrt(pow((one.x - two.x), 2.0) + pow((one.y - two.y), 2.0));
}

/*Implement the following function
  Reads in a file specified by the parameter
  Format of file: #of points on first line
                  remaining (#of points) lines: x-value and y-value of point
                  one point per line
                  x-value and y-value are double-precision values and
                  bounded by the unit square 0.0 <= x,y < 1.0
  Should use "spatial hashing" where the #of cells scales with the #of points
  and find the distance between the closest pair of points which will be
  returned as a double type value

  NOTE FROM CHARLIE (the author): I split this function into a couple different
    functions to make it easier to read.

  My value for b is the square root of the number of points, divided by two.
    I chose this value because it would mean that there are approximately four
    values in each cell.
*/
double closestPair(string filename) {
  // Initialize distance to 100 since distances will always be smaller than that
  double dist = 100;
  double ndist = 0;
  
  // Create the table using helper function to ease readability
  vector<vector<vector<point>>> table = createTable(filename);

  // Loop through table, comparing each point to the points surrounding it
  // Outer table loop:
  for(int i = 0; i < table.size(); i++) {
    // Inner table loop:
    for(int j = 0; j < table[i].size(); j++) {
      // Bucket loop
      for(int k = 0; k < table[i][j].size(); k++) {
        // Assign current point
        point current = table[i][j][k];
        
        // Loop through and check bucket
        for(int b = 0; b < table[i][j].size(); b++) {
          ndist = ptDistance(current, table[i][j][b]);
          if(ndist < dist && ndist != 0) {
            dist = ndist;
          }
        }
        
        // Check U, if it exists
        if((j-1) >= 0) {
          for(int b = 0; b < table[i][j-1].size(); b++) {
            ndist = ptDistance(current, table[i][j-1][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check UR
        if((j-1) >= 0 && (i+1) < table.size()) {
          for(int b = 0; b < table[i+1][j-1].size(); b++) {
            ndist = ptDistance(current, table[i+1][j-1][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check R
        if((i+1) < table.size()) {
          for(int b = 0; b < table[i+1][j].size(); b++) {
            ndist = ptDistance(current, table[i+1][j][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check DR
        if((j+1) < table.size() && (i+1) < table.size()) {
          for(int b = 0; b < table[i+1][j+1].size(); b++) {
            ndist = ptDistance(current, table[i+1][j+1][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check D
        if((j+1) < table.size()) {
          for(int b = 0; b < table[i][j+1].size(); b++) {
            ndist = ptDistance(current, table[i][j+1][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check DL
        if((j+1) < table.size() && (i-1) >= 0) {
          for(int b = 0; b < table[i-1][j+1].size(); b++) {
           ndist = ptDistance(current, table[i-1][j+1][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check L
        if((i-1) >= 0) {
          for(int b = 0; b < table[i-1][j].size(); b++) {
            ndist = ptDistance(current, table[i-1][j][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }

        // Check UL
        if((j-1) >= 0 && (i-1) >= 0) {
          for(int b = 0; b < table[i-1][j-1].size(); b++) {
            ndist = ptDistance(current, table[i-1][j-1][b]);
            if(ndist < dist && ndist != 0) {
              dist = ndist;
            }
          }
        }
      }
    }
  }

  return dist;
}

int main()
{
    double min;
    string filename;
    cout << "File with list of points within unit square: ";
    cin >> filename;
    min = closestPair(filename);
    cout << setprecision(16);
    cout << "Distance between closest pair of points: " << min << endl;
    return 0;
}
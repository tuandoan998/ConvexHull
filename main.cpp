#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>


using namespace std;

struct Point{
  float x, y;
};

vector<Point> readInput(string fileName)
{
  ifstream inFile(fileName);
  if (!inFile.is_open())
	{
		cout << "genIntArray: Error opening input file.";
	}
	vector<Point> data;
	Point temp;
  int n;

  inFile >> n;

	for (int i=0;i<n;i++)
	{
		inFile >> temp.x >> temp.y;
		data.push_back(temp);
	}

	return data;
}


void writeOutput(vector<Point> arr, string fileName){
  ofstream outFile(fileName);
	if (!outFile.is_open())
	{
		cout << "genIntArray: Error opening output file.";
	}

  for (int i=0;i<arr.size();i++)
    outFile<<"("<<arr[i].x<<", " <<arr[i].y<<")"<<endl;

  outFile.close();
}


//return TRUE - Clockwise
//return FALSE - Counterclockwise
bool orientation(Point a, Point b, Point c){
  return ((b.x-a.x)*(b.y+a.y)+(c.x-b.x)*(c.y+b.y)+(a.x-c.x)*(a.y+c.y))>0;
}


void swap(Point &a, Point &b){
  Point tmp = a;
  a = b;
  b = tmp;
}

//sort theo x
void quickSort(vector<Point>& array, int left, int right){
  int i, j;
  Point pivot;
  if (left >= right)
    return;
  pivot = array[(left+right)/2];
  i=left;
  j=right;
  while(i<=j){
    while (array[i].x<pivot.x)
      i++;
    while (array[j].x>pivot.x)
      j--;
    if (i<=j){
      if (i<j)
        swap(array[i], array[j]);
      i++;
      j--;
    }
  }
  quickSort(array, left, j);
  quickSort(array, i, right);
}



vector<Point> subConvexHull(vector<Point> array, bool isUp){
  vector<Point> hull;
  hull.push_back(array[array.size()-1]);
  for (int i=0;i<array.size();i++)
    hull.push_back(array[i]);

  int i=0;
  while (i<hull.size()-2){
    cout << i << endl;
    cout <<  hull[i].x << ", " << hull[i].y<< endl;
    cout <<  hull[i+1].x << ", " << hull[i+1].y<< endl;
    cout <<  hull[i+2].x << ", " << hull[i+2].y<< endl;
    if (orientation(hull[i], hull[i+1], hull[i+2]) != isUp){
      hull.erase(hull.begin()+i+1);
      i-=2;
    }
    else if ((hull[i+2].x==array[array.size()-1].x)&&(hull[i+2].y==array[array.size()-1].y))
      break;
    i++;
    for (int j=0;j<hull.size();j++)
      cout << "("<< hull[j].x << ", " << hull[j].y<< ")  ";
    cout << endl;
  }
  hull.erase(hull.begin());
  for (int i=0;i<hull.size();i++)
    cout <<  hull[i].x << ", " << hull[i].y<< endl;
  cout << endl;
  return hull;
}

void splitPoint(vector<Point> arrPoint, vector<Point>& upArrPoint, vector<Point>& downArrPoint){
  upArrPoint.push_back(arrPoint[0]);
  downArrPoint.push_back(arrPoint[0]);
  for (int i=1;i<arrPoint.size()-1;i++){
    if (orientation(arrPoint[0], arrPoint[i], arrPoint[arrPoint.size()-1]))
      upArrPoint.push_back(arrPoint[i]);
    else
      downArrPoint.push_back(arrPoint[i]);
  }
  upArrPoint.push_back(arrPoint[arrPoint.size()-1]);
  downArrPoint.push_back(arrPoint[arrPoint.size()-1]);
}

vector<Point> convexHull(vector<Point> arrPoint){
  vector<Point> hull, upHull, downHull, upArrPoint, downArrPoint;

  //quickSort theo x
  quickSort(arrPoint, 0, arrPoint.size()-1);

  splitPoint(arrPoint, upArrPoint, downArrPoint);

  //tim nhung diem tren duong phia tren
  upHull = subConvexHull(upArrPoint, true);
  hull = upHull;

  //tim nhung diem tren duong phi duoi
  downHull = subConvexHull(downArrPoint, false);
  hull.insert(hull.end(), downHull.begin(), downHull.end());

  hull.erase(hull.begin());
  hull.pop_back();

  return hull;
}

int main(){
  vector<Point> arrPoint = readInput("input.txt");
  for (int i=0;i<arrPoint.size();i++)
    cout << arrPoint[i].x << ", " << arrPoint[i].y<< endl;
  cout << endl;
  vector<Point> res = convexHull(arrPoint);
  for (int i=0;i<res.size();i++)
    cout <<  res[i].x << ", " << res[i].y<< endl;
  cout << endl;
  writeOutput(res, "output.txt");
  return 0;
}

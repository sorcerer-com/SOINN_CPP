#pragma once

#include <map>
#include <vector>
#include <fstream>

using namespace std;

using VectorXd = vector < double > ;

inline VectorXd operator -(const VectorXd& a, const VectorXd& b)
{
	VectorXd result;
	for (int i = 0; i < (int)a.size(); i++)
		result.push_back(a[i] - b[i]);
	return result;
}

inline VectorXd operator *(double d, const VectorXd& a)
{
	VectorXd result;
	for (int i = 0; i < (int)a.size(); i++)
		result.push_back(a[i] * d);
	return result;
}

inline VectorXd operator +=(VectorXd a, const VectorXd& b)
{
	for (int i = 0; i < (int)a.size(); i++)
		a[i] += b[i];
	return a;
}

template <typename T>
inline void Write(ostream& ofile, const T& value)
{
	ofile.write((char*)&value, sizeof(value));
}

template <typename T>
inline void Write(ostream& ofile, const vector<T>& value)
{
	int size = (int)value.size();
	ofile.write((char*)&size, sizeof(size));
	for (int i = 0; i < size; i++)
		Write(ofile, value[i]);
}
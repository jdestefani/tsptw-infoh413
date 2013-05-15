/*

      TTTTTT   SSSSS  PPPPP	  TTTTTT  W         W
        TT    SS      PP  PP	TT	   W       W
        TT     SSSS   PPPPP		TT      W W W W
        TT        SS  PP		TT		 W W W
        TT    SSSSS   PP		TT		  W W

######################################################
########## Iterative improvement algorithms for ######
########## the TSP problem with Time Windows #########
######################################################

      Version: 1.0
      File:    NumericMatrix.h
      Author:  Jacopo De Stefani
      	  	   Michael Allwright <michael.allwright@upb.de>
      Purpose: Header file for user defined class to model time windows
      Check:   README and gpl.txt
*/

/***************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    email: jacopo.de.stefani@ulb.ac.be

***************************************************************************/

#ifndef MATRIX_H_
#define MATRIX_H_

#include <cassert>
#include <iostream>

template<class T>
class NumericMatrix{
	public:

	NumericMatrix(unsigned int rows, unsigned int columns):
		m_unRows(rows),
		m_unColumns(columns){
		m_TMatrix = new T[m_unRows*m_unColumns];
		for(unsigned int i=0; i < m_unRows*m_unColumns; i++){
			m_TMatrix[i] = 0;
		}
	}

	NumericMatrix(const T* lf_values) {
		Set(lf_values);
	}


	~NumericMatrix(){
		delete[] m_TMatrix;
	}

	inline T GetElement(unsigned int i,unsigned int j) const{
		assert(i < m_unRows && j < m_unColumns);
		return m_TMatrix[i*m_unColumns+j];
	}

	void SetElement(unsigned int i,unsigned int j, T element){
		assert(i < m_unRows && j < m_unColumns);
		m_TMatrix[i*m_unColumns+j] = element;
	}


	void Set(const T* lf_values) {
		for(unsigned int i = 0; i < m_unRows * m_unColumns; i++)
			m_TMatrix[i] = lf_values[i];
	}

	NumericMatrix<T>& operator=(const NumericMatrix<T>& c_matrix) {
		if(this != &c_matrix) {
			Set(c_matrix.m_TMatrix);
		}
		return *this;
	}

	inline double operator()(unsigned int i, unsigned int j) const {
		assert(i < m_unRows && j < m_unColumns);
		return m_TMatrix[i*m_unColumns+j];
	}

	inline double& operator()(unsigned int i, unsigned int j) {
		assert(i < m_unRows && j < m_unColumns);
		return m_TMatrix[i*m_unColumns+j];
	}


	bool operator==(const NumericMatrix<T>& c_matrix) const {
		for(unsigned int i = 0; i < m_unRows * m_unColumns; i++) {
			if(m_TMatrix[i] != c_matrix.m_TMatrix[i])
				return false;
		}
		return true;
	}

	bool operator!=(const NumericMatrix<T>& c_matrix) const {
		return !(*this == c_matrix);
	}

	NumericMatrix<T>& operator+=(const NumericMatrix<T>& c_matrix) {
		for(unsigned i = 0; i < m_unRows * m_unColumns; i++) {
			m_TMatrix[i] += c_matrix.m_TMatrix[i];
		}
		return *this;
	}

	NumericMatrix<T>& operator-=(const NumericMatrix<T>& c_matrix) {
		for(unsigned i = 0; i < m_unRows * m_unColumns; i++) {
			m_TMatrix[i] -= c_matrix.m_TMatrix[i];
		}
		return *this;
	}

	NumericMatrix<T>& operator*=(const NumericMatrix<T>& c_matrix) {
		T lfNewValues[m_unRows * m_unColumns];
		for(unsigned int i = 0; i < m_unRows; i++) {
			for(unsigned int j = 0; j < m_unColumns; j++) {
				lfNewValues[i * m_unColumns + j] = 0.0f;
				for(unsigned int k = 0; k < m_unColumns; k++) {
					lfNewValues[i * m_unColumns + j] += m_TMatrix[i * m_unColumns + k] * c_matrix.m_TMatrix[k * m_unColumns + j];
				}
			}
		}
		Set(lfNewValues);
		return *this;
	}

	NumericMatrix<T>& operator*=(double scalar) {
		for(unsigned i = 0; i < m_unRows * m_unColumns; i++) {
			m_TMatrix[i] *= scalar;
		}
		return *this;
	}

	NumericMatrix<T> operator+(const NumericMatrix<T>& c_matrix) const {
		NumericMatrix<T> cResult = (*this);
		cResult += c_matrix;
		return cResult;
	}

	NumericMatrix<T> operator-(const NumericMatrix<T>& c_matrix) const {
		NumericMatrix<T> cResult = (*this);
		cResult -= c_matrix;
		return cResult;
	}

	NumericMatrix<T> operator*(const NumericMatrix<T>& c_matrix) const {
		NumericMatrix<T> cResult = (*this);
		cResult -= c_matrix;
		return cResult;
	}

	NumericMatrix<T> operator*(double scalar) const {
		NumericMatrix<T> cResult = (*this);
		cResult *= scalar;
		return cResult;
	}

	friend std::ostream& operator<<(std::ostream& c_os,
			const NumericMatrix c_matrix) {

		/*std::ios_base::fmtflags unInitalFlags = c_os.flags();
			std::streamsize nInitalPrec = c_os.precision();

			c_os.setf(std::ios::fixed);
			c_os.precision(1);*/

			for(unsigned int i = 0; i < c_matrix.m_unRows; i++) {
				for(unsigned int j = 0; j < c_matrix.m_unColumns; j++) {
					c_os << c_matrix(i, j) << " ";
				}
				c_os << std::endl;
			}

			/*c_os.flags(unInitalFlags);
			c_os.precision(nInitalPrec);*/
			return c_os;

	}

	inline T GetMin() const{
		T min=m_TMatrix[0];
		for(unsigned int i = 0; i < m_unRows*m_unColumns; i++) {
			if(m_TMatrix[i] < min){
				min = m_TMatrix[i];
			}
		}
		return min;
	}

	inline T GetMax() const{
		T max=m_TMatrix[0];
		for(unsigned int i = 0; i < m_unRows*m_unColumns; i++) {
			if(m_TMatrix[i] > max){
				max = m_TMatrix[i];
			}
		}
		return max;
	}

	unsigned int GetColumns() const {
		return m_unColumns;
	}

	unsigned int GetRows() const {
		return m_unRows;
	}

	private:
		unsigned int m_unRows;
		unsigned int m_unColumns;
		T* m_TMatrix;

};

#endif /* MATRIX_H_ */

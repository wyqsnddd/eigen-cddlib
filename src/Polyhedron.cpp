// This file is part of eigen-cddlib.

// eigen-cddlib is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// eigen-cddlib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with eigen-cddlib.  If not, see
// <http://www.gnu.org/licenses/>.

#include "Polyhedron.h"

namespace Eigen {

std::atomic_int Polyhedron::counter(0);
std::mutex Polyhedron::mtx;

Polyhedron::Polyhedron()
    : matPtr_(nullptr)
    , polytope_(nullptr)
{
    if (counter == 0)
        dd_set_global_constants();
    counter++;
}

Polyhedron::~Polyhedron()
{
    counter--;

    if (matPtr_ != nullptr)
        dd_FreeMatrix(matPtr_);
    if (polytope_ != nullptr)
        dd_FreePolyhedra(polytope_);

    if (counter == 0)
        dd_free_global_constants();
}

void Polyhedron::setHrep(const Eigen::MatrixXd& A, const Eigen::VectorXd& b)
{
    std::unique_lock<std::mutex> lock(mtx);
    if (!hvrep(A, b, false))
        throw std::runtime_error("H-rep to V-rep conversion failed: " + stringFromError(err_));
}

void Polyhedron::setVrep(const Eigen::MatrixXd& A, const Eigen::VectorXd& b)
{
    std::unique_lock<std::mutex> lock(mtx);
    if (!hvrep(A, b, true))
        throw std::runtime_error("V-rep to H-rep conversion failed: " + stringFromError(err_));
}

std::pair<Eigen::MatrixXd, Eigen::VectorXd> Polyhedron::vrep() const
{
    std::unique_lock<std::mutex> lock(mtx);
    dd_MatrixPtr mat = dd_CopyGenerators(polytope_);
    return ddfMatrix2EigenMatrix(mat, true);
}

std::pair<Eigen::MatrixXd, Eigen::VectorXd> Polyhedron::hrep() const
{
    std::unique_lock<std::mutex> lock(mtx);
    dd_MatrixPtr mat = dd_CopyInequalities(polytope_);
    return ddfMatrix2EigenMatrix(mat, false);
}

void Polyhedron::printVrep() const
{
    std::unique_lock<std::mutex> lock(mtx);
    dd_MatrixPtr mat = dd_CopyGenerators(polytope_);
    dd_WriteMatrix(stdout, mat);
}

void Polyhedron::printHrep() const
{
    std::unique_lock<std::mutex> lock(mtx);
    dd_MatrixPtr mat = dd_CopyInequalities(polytope_);
    dd_WriteMatrix(stdout, mat);
}

void Polyhedron::setRays(const Eigen::MatrixXd& R)
{
    setVrep(R, Eigen::VectorXd::Zero(R.rows()));
}

void Polyhedron::setVertices(const Eigen::MatrixXd& V)
{
    setVrep(V, Eigen::VectorXd::Ones(V.rows()));
}

/**
 * Private functions
 */

bool Polyhedron::hvrep(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, bool isFromGenerators)
{
    Eigen::MatrixXd cMat = concatenateMatrix(A, b, isFromGenerators);
    return doubleDescription(cMat, isFromGenerators);
}

void Polyhedron::initializeMatrixPtr(Eigen::Index rows, Eigen::Index cols, bool isFromGenerators)
{
    if (matPtr_ != nullptr)
        dd_FreeMatrix(matPtr_);
    
    matPtr_ = dd_CreateMatrix(rows, cols);
    matPtr_->representation = (isFromGenerators ? dd_Generator : dd_Inequality);
}

bool Polyhedron::doubleDescription(const Eigen::MatrixXd& matrix, bool isFromGenerators)
{
    initializeMatrixPtr(matrix.rows(), matrix.cols(), isFromGenerators);

    for (auto row = 0; row < matrix.rows(); ++row)
        for (auto col = 0; col < matrix.cols(); ++col)
            matPtr_->matrix[row][col][0] = matrix(row, col);

    if (polytope_ != nullptr)
        dd_FreePolyhedra(polytope_);

    polytope_ = dd_DDMatrix2Poly(matPtr_, &err_);
    return (err_ == dd_NoError) ? true : false;
}

Eigen::MatrixXd Polyhedron::concatenateMatrix(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, bool isFromGenerators)
{
    double sign = (isFromGenerators ? 1 : -1);
    Eigen::MatrixXd mat(A.rows(), A.cols() + 1);
    mat.col(0) = b;
    mat.block(0, 1, A.rows(), A.cols()).noalias() = sign * A;
    return mat;
}

std::pair<Eigen::MatrixXd, Eigen::VectorXd> Polyhedron::ddfMatrix2EigenMatrix(const dd_MatrixPtr mat, bool isOuputVRep) const
{
    double sign = (isOuputVRep ? 1 : -1);
    auto rows = mat->rowsize;
    auto cols = mat->colsize;
    Eigen::MatrixXd mOut(rows, cols - 1);
    Eigen::VectorXd vOut(rows);
    for (auto row = 0; row < rows; ++row) {
        vOut(row) = mat->matrix[row][0][0];
        for (auto col = 1; col < cols; ++col)
            mOut(row, col - 1) = sign * mat->matrix[row][col][0];
    }

    return std::make_pair(mOut, vOut);
}

std::string Polyhedron::stringFromError(dd_ErrorType err)
{
  switch (err)
  {
    case dd_CannotHandleLinearity:
      return "dd_CannotHandleLinearity";
    case dd_ColIndexOutOfRange:
      return "dd_ColIndexOutOfRange";
    case dd_DimensionTooLarge:
      return "dd_DimensionTooLarge";
    case dd_EmptyHrepresentation:
      return "dd_EmptyHrepresentation";
    case dd_EmptyRepresentation:
      return "dd_EmptyRepresentation";
    case dd_EmptyVrepresentation:
      return "dd_EmptyVrepresentation";
    case dd_IFileNotFound:
      return "dd_IFileNotFound";
    case dd_ImproperInputFormat:
      return "dd_ImproperInputFormat";
    case dd_LPCycling:
      return "dd_LPCycling";
    case dd_NegativeMatrixSize:
      return "dd_NegativeMatrixSize";
    case dd_NoError:
      return "dd_NoError";
    case dd_NoLPObjective:
      return "dd_NoLPObjective";
    case dd_NoRealNumberSupport:
      return "dd_NoRealNumberSupport";
    case dd_NotAvailForH:
      return "dd_NotAvailForH";
    case dd_NotAvailForV:
      return "dd_NotAvailForV";
    case dd_NumericallyInconsistent:
      return "dd_NumericallyInconsistent";
    case dd_OFileNotOpen:
      return "dd_OFileNotOpen";
    case dd_RowIndexOutOfRange:
      return "dd_RowIndexOutOfRange";
    default:
      return "undefined error";
  }
}

} // namespace Eigen

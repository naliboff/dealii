// ---------------------------------------------------------------------
//
// Copyright (C) 2005 - 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// test LAPACKFullMatrix::add() by comparing to FullMatrix

#include "../tests.h"
#include "create_matrix.h"
#include <deal.II/lac/lapack_full_matrix.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/vector.h>

#include <iostream>


template <typename NumberType>
void
test(const unsigned int size)
{
  // Full matrix:
  FullMatrix<NumberType> A(size), B(size);
  create_random(A);
  create_random(B);

  // Lapack:
  LAPACKFullMatrix<NumberType> C(size), D(size);
  C = A;
  D = B;

  // do addition
  NumberType val = 0.1234;
  A.add(val,B);
  C.add(val,D);

  FullMatrix<NumberType> diff(size);
  diff = C;
  diff.add(-1., A);

  const NumberType error = diff.frobenius_norm();
  deallog << "difference: " << error << std::endl;
}


int main()
{
  const std::string logname = "output";
  std::ofstream logfile(logname.c_str());
  logfile.precision(3);
  deallog.attach(logfile);

  const std::vector<unsigned int> sizes = {{17,391}};
  for (const auto &s : sizes)
    {
      deallog << "size=" << s << std::endl;
      test<double>(s);
    }
}

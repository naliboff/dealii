// ---------------------------------------------------------------------
//
// Copyright (C) 2001 - 2017 by the deal.II authors
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

// Check extract used_vertices and find_closest_vertex when a mapping
// is present

#include "../tests.h"
#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/mapping_q_eulerian.h>
#include <deal.II/dofs/dof_handler.h>


template <int dim, int spacedim>
void test (const Point<spacedim> &p)
{
  deallog << "dim: " << dim << ", spacedim: "
          << spacedim << std::endl;

  Triangulation<dim,spacedim> tria;
  GridGenerator::hyper_cube(tria);
  tria.refine_global(1);

  // Vector fe
  FESystem<dim,spacedim> fe({FE_Q<dim, spacedim>(1)^spacedim});
  DoFHandler<dim,spacedim> dh(tria);
  dh.distribute_dofs(fe);

  // Displacement vector.
  Vector<double> disp(dh.n_dofs());
  for (unsigned int i=0; i<dh.n_dofs(); ++i)
    disp[i] = .5;

  // mapping
  MappingQEulerian<dim, Vector<double>, spacedim> mapping(1, dh, disp);

  auto m = GridTools::extract_used_vertices(tria, mapping);

  std::vector<bool> selected_vertices(tria.n_vertices(), false);
  selected_vertices[0] = true;
  selected_vertices[5] = true;

  for (auto &e: m)
    deallog << "Vertex: " << e.first << ": " << e.second << std::endl;

  auto i = GridTools::find_closest_vertex(mapping,tria,p,selected_vertices);
  deallog << "Closest vertex to " << p
          << ", v["<< i << "] :"
          << m[i] << std::endl;
};


int main ()
{
  initlog();
  test<2,2> (Point<2>(.2,.2));
  test<2,2> (Point<2>(.6,.9));
  return 0;
}


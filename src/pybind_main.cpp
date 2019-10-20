#include "pybind_main.h"

std::vector<std::pair<std::string, std::complex<double>>> simulate(const py::dict &options) {

  qflex::QflexInput input;

  // Check options for circuit
  switch(options.contains("circuit_filename") + options.contains("circuit")) {
    case 0:
    case 2:
      std::cerr << "ERROR: either 'circuit_filename' or 'circuit' must be specified" << std::endl;
      return {};
  }

  // Check options for ordering
  switch(options.contains("ordering_filename") + options.contains("ordering")) {
    case 0:
    case 2:
      std::cerr << "ERROR: either 'ordering_filename' or 'ordering' must be specified" << std::endl;
      return {};
  }

  // Check options for grid
  switch(options.contains("grid_filename") + options.contains("grid")) {
    case 0:
    case 2:
      std::cerr << "ERROR: either 'grid_filename' or 'grid' must be specified" << std::endl;
      return {};
  }

  // Temporary streams
  std::ifstream fs_circuit_data;
  std::ifstream fs_ordering_data;

  // Get circuit
  if(options.contains("circuit_filename")) {
    fs_circuit_data.open(options["circuit_filename"].cast<std::string>());
    if(not fs_circuit_data.good()) {
      std::cerr << "ERROR: cannot open file: " << options["circuit_filename"].cast<std::string>() << "." << std::endl;
      return {};
    }
    input.circuit_data = &fs_circuit_data;
  } else {
    std::cerr << "ERROR: not yet implemented." << std::endl;
    return {};
  }

  // Get ordering
  if(options.contains("ordering_filename")) {
    fs_ordering_data.open(options["ordering_filename"].cast<std::string>());
    if(not fs_ordering_data.good()) {
      std::cerr << "ERROR: cannot open file: " << options["ordering_filename"].cast<std::string>() << "." << std::endl;
      return {};
    }
    input.ordering_data = &fs_ordering_data;
  } else {
    std::cerr << "ERROR: not yet implemented." << std::endl;
    return {};
  }

  // Get grid
  if(options.contains("grid_filename")) {
    input.grid.load(options["grid_filename"].cast<std::string>());
  } else {
    std::cerr << "ERROR: not yet implemented." << std::endl;
    return {};
  }

  // Get auto_depth
  std::size_t auto_depth;
  if(options.contains("circuit_filename")) {
    auto_depth = qflex::compute_depth(std::ifstream(options["circuit_filename"].cast<std::string>()));
  } else {
    std::cerr << "ERROR: not yet implemented." << std::endl;
    return {};
  }

  // Get depth
  if(options.contains("depth")) {
    input.K = options["depth"].cast<int>();
    if(input.K != auto_depth) {
      std::cerr << "WARNING: user-provided depth (" << input.K << ") differs from the auto-computed depth (" << auto_depth << ")." << std::endl;
      return {};
    }
  } else input.K = auto_depth;

  // Setting initial and final circuit states.
  if(options.contains("initial_state"))
    input.initial_state = options["initial_state"].cast<std::string>();

  if(options.contains("final_state"))
    input.final_state = options["final_state"].cast<std::string>();

  // Evaluating circuit.
  std::vector<std::pair<std::string, std::complex<double>>> amplitudes =
      qflex::EvaluateCircuit(&input);

  return amplitudes;

}

#include <memory>
#include <fstream>
#include <map>

#include "context-clear.hpp"
#include "context-helib.hpp"
///#include "context-tfhe.hpp"


using namespace Sheep;

typedef std::chrono::duration<double, std::micro> DurationT;


template <typename PlaintextT>
std::unique_ptr<BaseContext<PlaintextT> >
make_context(std::string context_type, std::string context_params="") {
	if (context_type == "HElib") {
	  auto ctx =  std::make_unique<HElib::ContextHElib<PlaintextT> >();
	  if (context_params.length() > 0)
	    ctx->read_params_from_file(context_params);
	  return ctx;
	} else {
	  return std::make_unique<Clear::ContextClear<PlaintextT> >();
	}
}



template<typename T>
std::list<T> read_inputs_file(std::string filename) {
  std::list<T> inputs_list;
  std::ifstream inputstream(filename);	  
  if (inputstream.bad()) {
    std::cout<<"Empty or non-existent input file"<<std::endl;
  }
  
  /// loop over all lines in the input file 
  std::string line;
  while (std::getline(inputstream, line) ) {
    /// remove comments (lines starting with #) and empty lines
    int found= line.find_first_not_of(" \t");
    if( found != std::string::npos) {   
      if ( line[found] == '#') 
	continue;
      
      /// split up by whitespace
      std::string buffer;
      std::vector<std::string> tokens;
      std::stringstream ss(line);
      while (ss >> buffer) tokens.push_back(buffer);
      
      if (tokens.size() == 2) {   /// assume we have param_name param_value
	long input_val_long = stol(tokens[1]);
	T input_val = (T)(input_val_long);
	inputs_list.push_back(input_val);
      }	      
    }    
  } // end of loop over lines
  return inputs_list;
}

template <typename PlaintextT>
void print_outputs(std::list<PlaintextT> test_results, std::list<PlaintextT> control_results, DurationT duration) {
  std::cout<<std::endl<<"==============="<<std::endl;
  std::cout<<"=== RESULTS ==="<<std::endl<<std::endl;
  std::cout<<"Processing time: "<<std::to_string(duration.count())<<" microseconds."<<std::endl;
  if (test_results.size() != control_results.size()) {
    std::cout<<"Outputs have different size - something went wrong!"<<std::endl;
    return;
  }
  std::cout<<"Output values (test vs clear)"<<std::endl;
  auto test_iter = test_results.begin();
  auto ctrl_iter = control_results.begin();
  while (test_iter != test_results.end()) {
    std::cout<<"  test context : "<<std::to_string(*test_iter);
    std::cout<<"  clear context : "<<std::to_string(*ctrl_iter)<<std::endl;
    test_iter++;
    ctrl_iter++;
  }
  std::cout<<endl<<"==== END RESULTS ==="<<std::endl;
  
}

template <typename PlaintextT>
bool benchmark_run(std::unique_ptr<BaseContext<PlaintextT> > test_ctx,
		   std::unique_ptr<BaseContext<PlaintextT> > clear_ctx,
		   Circuit C,
		   std::string input_filename,
		   DurationT& duration)
{
	// read in inputs from input_filename
	std::list<PlaintextT> inputs = read_inputs_file<PlaintextT>(input_filename);
	
	std::list<PlaintextT> result_bench = test_ctx->eval_with_plaintexts(C, inputs, duration);
	std::cout<<"duration here in benchmark_run is "<<duration.count()<<std::endl;
	DurationT dummy;
	std::list<PlaintextT> result_clear = clear_ctx->eval_with_plaintexts(C, inputs, dummy);	


	print_outputs(result_bench, result_clear, duration);
	
	return true;
}





int
main(int argc, const char** argv) {

  
  if (argc < 5) {

    std::cout<<"Usage: <path_to>/custom_example circuit_file context_name input_type inputs_file [params_file]"<<std::endl;
    return -1;
  }

  /// read the circuit
  std::ifstream input_circuit(argv[1]);
  Circuit C;
  input_circuit >> C;
  std::cout << C;


  /// read the other input args
  std::string context_name = argv[2];  
  std::string input_type = argv[3];
  std::string inputs_file = argv[4];
  std::string parameter_file = "";
  if (argc == 6)
    parameter_file = argv[5];

  
  DurationT duration;
  /// run the benchmark
  bool isOK = false;
  if (input_type == "bool") {
    isOK = benchmark_run<bool>(make_context<bool>(context_name, parameter_file),
			       make_context<bool>("Clear"),
			       C, inputs_file, duration);
  } else if (input_type == "uint8_t") {
    isOK = benchmark_run<uint8_t>(make_context<uint8_t>(context_name, parameter_file),
				  make_context<uint8_t>("Clear"),
				  C, inputs_file, duration);    
  } else if (input_type == "int8_t") {
    isOK = benchmark_run<int8_t>(make_context<int8_t>(context_name, parameter_file),
				 make_context<int8_t>("Clear"),
				 C, inputs_file, duration);    
  } else if (input_type == "uint16_t") {
    isOK = benchmark_run<uint16_t>(make_context<uint16_t>(context_name, parameter_file),
				   make_context<uint16_t>("Clear"),
				   C, inputs_file, duration);    
  } else if (input_type == "int16_t") {
    isOK = benchmark_run<int16_t>(make_context<int16_t>(context_name, parameter_file),
				  make_context<int16_t>("Clear"),
				  C, inputs_file, duration);    
  } else if (input_type == "uint32_t") {
    isOK = benchmark_run<uint32_t>(make_context<uint32_t>(context_name, parameter_file),
				   make_context<uint32_t>("Clear"),
				   C, inputs_file, duration);    
  }  else if (input_type == "int32_t") {
    isOK = benchmark_run<int32_t>(make_context<int32_t>(context_name, parameter_file),
				  make_context<int32_t>("Clear"),
				  C, inputs_file, duration);    
  }  
  return isOK;
}
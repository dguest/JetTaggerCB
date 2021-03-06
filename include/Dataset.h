#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdio.h>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <TFile.h>
#include <TTree.h>

struct Numeric;

struct Reweighting
{
	std::vector<std::vector<double> > charm_correction { 0 };
    std::vector<std::vector<double> > bottom_correction { 0 };
};
/**
\details Provides a wrapper for a TFile while providing a generic branch setting interface.
*/
class Dataset
{
public:
	//----------------------------------------------------------------------------
	/**
	\details The ROOT file and the TTree to be used MUST be specified upon instantiation.
	\param root_file The filename of the TFile to be loaded.
	\param tree_name The name of the TTree holding the nTuple to be used.
	*/
	Dataset(std::string root_file = "", std::string tree_name = "");
	//----------------------------------------------------------------------------
	/**
	\details Default Destructor.
	*/
	~Dataset();
	//----------------------------------------------------------------------------
	/**
	\param name Name of the branch to set as an input variable to the Neural Network.
	\param type A string containing the numeric type of the elements of this branch. Can be one of "int", "double", "float".
	\return Returns a 1 if setting the branch was sucessful, 0 otherwise.
	*/
	bool set_input_branch(std::string name, std::string type);
	//----------------------------------------------------------------------------
	/**
	\param name Name of the branch to set as an output variable to the Neural Network.
	\param type A string containing the numeric type of the elements of this branch. Can be one of "int", "double", "float".
	\return Returns a 1 if setting the branch was sucessful, 0 otherwise.
	*/
	bool set_output_branch(std::string name, std::string type);

	//----------------------------------------------------------------------------
	/**
	\param name Name of the branch to set as an control variable to the Neural Network.
	\param type A string containing the numeric type of the elements of this branch. Can be one of "int", "double", "float".
	\return Returns a 1 if setting the branch was sucessful, 0 otherwise.
	*/
	bool set_control_branch(std::string name, std::string type);


	/**
	\param variable_names A std::vector<std::string> containing the variables of interest to return from the nTuple.
	\return The map with keys specified by variable_names.
	*/
	std::map<std::string, double> get_performance_map(std::vector<std::string> &variable_names);

	
	double get_value(std::string name);
	void operator[]( const int index );
	void at( const int index );
	std::vector<double> &input();
	std::vector<double> &output();
	std::vector<std::string> get_output_vars();
	std::vector<std::string> get_input_vars();

	/**
	\return Number of rows in the TTree.
	*/
	unsigned int num_entries();
	inline void set_pT_bins(std::vector<double> bins = {20, 30, 40, 50, 60, 75, 90, 110, 140, 200, 500})
	{
		m_pt_bins = bins;
		m_num_pt_bins = m_pt_bins.size() - 1;
	}
	inline void set_eta_bins(std::vector<double> bins = {0, 0.6, 1.2, 1.8, 2.5})
	{
		m_eta_bins = bins;
		m_num_eta_bins = m_eta_bins.size() - 1;
	}
	double get_physics_reweighting();
	void determine_reweighting(bool cdf = true, bool relative = true);
	
private:
	inline int get_cat_eta(double eta)
	{
		int n = m_eta_bins.size() - 1;
		eta = fabs(eta);
		for (int category = 0; category < n; ++category)
		{
			if ((eta >= m_eta_bins[category]) && (eta < m_eta_bins[category + 1]))
			{
				return category;
			}
		}
		if (eta > m_eta_bins.back())
		{
			return n;
		}
		else
		{
			return 0;
		}
	}
	inline int get_cat_pt(double pt)
	{
		int n = m_pt_bins.size() - 1;
		pt = fabs(pt);
		for (int category = 0; category < n; ++category)
		{
			if ((pt >= m_pt_bins[category]) && (pt < m_pt_bins[category + 1]))
			{
				return category;
			}
		}
		if (pt > m_pt_bins.back())
		{
			return n;
		}
		else
		{
			return 0;
		}
	}
	TFile *file;
	TTree *tree;
	bool fail;
	std::map<std::string, std::unique_ptr<Numeric>> variables;
	unsigned int n_entries, m_num_eta_bins, m_num_pt_bins;
	std::vector<std::string> input_vars, output_vars, control_vars;
	std::vector<double> m_input, m_output, m_pt_bins, m_eta_bins;
	Reweighting reweighting;
};

//----------------------------------------------------------------------------
//------------------ NON CLASS UTILITY-TYPE FUNCTIONS ------------------------
//----------------------------------------------------------------------------

struct Numeric
{
	double double_;
	float  float_;
	int    int_;
	bool   isDbl = false, 
	       isInt = false,
	       isFlt = false; 
};

inline double cast_as_double(Numeric number);
inline int cast_as_int(Numeric number);





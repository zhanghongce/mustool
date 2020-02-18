#ifndef REMUS_MASTER_H
#define REMUS_MASTER_H

#include "satSolvers/MSHandle.h"
#include "satSolvers/GlucoseHandle.h"
#include "satSolvers/CadicalHandle.h"
#include "Explorer.h"
#ifndef NOSMT
	#include "satSolvers/Z3Handle.h"
#endif
#ifndef NOLTL 
	#include "satSolvers/SpotHandle.h"
	#include "satSolvers/NuxmvHandle.h"
#endif
#include "types_h.h"
#include <set>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <ctime>
#include <chrono>	
#include <unordered_map>
#include <queue>

using namespace std;

class Master{
public:
	int bit;
	int dimension; //the number of constraints
	string algorithm; //MUS enumeration algorithm to be used
	int isValidExecutions;
	bool verbose; //TODO: make it int based 
	string output_file;
	bool validate_mus_c;
	float mus_approx;
	bool get_implies;
	bool criticals_rotation;
	bool mss_rotation;
	string domain;
	bool useMixedHeuristic;
	int hash;
	int unex_unsat;
	int unex_sat;
	int guessed;
	int rotated_msses;
	chrono::high_resolution_clock::time_point initial_time;
	vector<MUS> muses;
	vector<MSS> msses;
	Explorer* explorer;
	SatSolver* satSolver; 
	string sat_solver;
	std::vector<Formula> rotation_queue;
	bool conflicts_negation;
	int extended; //number of saved satisfiability checks via the grow extension
	Formula uni;
	Formula couni;

	Master(string filename, string alg, string ssolver);
	~Master();
	bool is_valid(Formula &f, bool core = false, bool grow = false);
	void block_down(Formula f);
	void block_up(Formula f);
	void mark_MUS(MUS& m, bool block = true);
	void mark_MSS(MSS m, bool block = true);
	void mark_MSS_executive(MSS m, bool block = true);
	MUS& shrink_formula(Formula& f, Formula crits = Formula());
	MSS grow_formula(Formula& f, Formula conflicts = Formula());
	vector<MSS> grow_formulas(Formula& f, Formula conflicts = Formula(), int limit = 1);
	void grow_combined(Formula &f, Formula conflicts = Formula());
	int grow_hitting_extension(Formula &mss, int c1);
	void grow_fixpoint(Formula &f);
	void write_mus_to_file(MUS& f);
	void write_mss_to_file(MSS& f);
	void validate_mus(Formula &f);
	void validate_mss(Formula &f);
	void enumerate();

	int rotateMSS(Formula mss);

	//reMUS algorithm functions
	int depthMUS;
	float dim_reduction;
	int current_depth;
	void find_all_muses_duality_based_remus(Formula subset, Formula crits, int depth);
	void extend_mus(Formula &top, Formula &mus);

	//unimus
	void unimus();
	void unimus_rotate_mus(int mid);
	vector<vector<pair<int,int>>> unimus_hitmap_pos;
	vector<vector<pair<int,int>>> unimus_hitmap_neg;
	void unimus_mark_mus(MUS &m);
	std::queue<int> unimus_rotation_queue;
	int unimus_rotated;
	int unimus_attempts;
	bool unimus_hitting_pair(int mid1, int mid2, int c1, int c2);
	std::vector<unordered_map<int, std::vector<int>>> unimus_map;
	void unimus_add_blocks(MUS &m1, int from, int to, vector<vector<int>> &blocks);

	//TOME algorithm functions
	void find_all_muses_tome();
	pair<Formula, Formula> local_mus(Formula bot, Formula top, int diff);

	//MARCO algorithm functions
	void marco_base();
};
#endif


#include "LivenessAnalysis.h"
#include <hash_map>
using namespace std;

bool same(hash_map<Instruction *, Variables> ins, hash_map<Instruction *, Variables> outs, Instructions instructions);

void livenessAnalysis(Instructions instructions)
{


	hash_map<Instruction *, Variables> outs;
	hash_map<Instruction *, Variables> ins;

	do {
		for (Instructions::iterator i = instructions.begin(); i != instructions.end(); ++i) {
			// prebacivanje unije svih succova u out[n]
			outs[*i] = (*i)->out;
			ins[*i] = (*i)->in;
			for (Instructions::iterator j = (*i)->succ.begin(); j !=(*i)->succ.end(); ++j) {
				for (Variables::iterator v = (*j)->in.begin(); v != (*j)->in.end(); ++v) {
					if (!variableExists(*v, (*i)->out)) {
						(*i)->out.push_back(*v);
					}
				}
			}
			
			//dodavanje u in[n]
			(*i)->in = (*i)->use;
			for (Variables::iterator j = (*i)->out.begin(); j != (*i)->out.end(); ++j) {
				if (!variableExists(*j, (*i)->in) 
					&& variableExists(*j,(*i)->out) 
					&& !variableExists(*j, (*i)->def)) {
						(*i)->in.push_back(*j);
				}
			}
		}
	} while(!same(ins, outs, instructions));

}

bool same(hash_map<Instruction *, Variables> ins, hash_map<Instruction *, Variables> outs, Instructions instructions) {

	for (Instructions::iterator i = instructions.begin(); i != instructions.end(); ++i) {
		ins[*i].sort();
		outs[*i].sort();
		(*i)->in.sort();
		(*i)->out.sort();

		if (ins[*i] != (*i)->in || outs[*i] != (*i)->out) {
			return false;
		}
	}
	return true;
}
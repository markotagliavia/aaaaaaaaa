#include "LivenessAnalysis.h"


void livenessAnalysis(Instructions instructions)
{	
	
	//deo osnovnog zadatka
	for(auto it = instructions.rbegin(); it != instructions.rend(); it++ )
	{
		(*it)->in.clear();
		(*it)->out.clear();
	}

	//in_ i out_ se moraju ovde deklarisati zbog do while
	Variables in_;		
	Variables out_;
	
	
	//promenljiva za do while petlju
	//bool state = false;

	//do 
	//{
		//state = false;
		
		//osnovni zadatak 
		for(auto it = instructions.rbegin(); it != instructions.rend(); it++ )
		{
			in_ = (*it)->in;		//kopiramo sadrzaj in liste iz instructions
			out_ = (*it)->out;		//koriparmo -||-   out  -||-
			
			//out[] <- U s succ in[s]    == sve liste in koje se nalaze u listi succ
			//prolazimo kroz elemente liste succ
			for(auto succ_it = (*it)->succ.begin(); succ_it!= (*it)->succ.end(); succ_it ++)
			{	
				//proalazimo kroz elemente liste in koja se nalazi u succ
				for(auto succ_in_it = (*succ_it)->in.begin(); succ_in_it != (*succ_it)->in.end(); succ_in_it ++)
					(*it)->out.push_back((*succ_in_it));  //dodajemo elemente in liste u out listu 	
			}
			
			//nakon dodavanja moguce je da se pojave duplikati nekih promenljivih, treba ih izbaciti
			(*it)->out.sort(); 		//prvo se lista soritra 
			(*it)->out.unique();		//pa se izbace duplikati jer unique radi samo sa sortiranom listom


			// in[n] <- use[n] U (out[n] - def[n])
			// (out[n] - def[n]) drugim recima: oni elementi iz out koji nisu u def

			// na in prvo dodajemo use							
			(*it)->in = (*it)->use;
			//prolazimo kroz sve elemente out liste 
			for(auto out_it = (*it)->out.begin(); out_it != (*it)->out.end(); out_it ++ )
			{
					//proveravamo da li se element out liste nalazi u def 
					//ako se ne nalazi treba dodati taj element out liste na in listu	
					if(!variableExists((*out_it), ((*it)->def)))
						(*it)->in.push_back((*out_it));
			
			}
			
			//opet sortiranje i izbacivanje elemenata koji se ponavljaju
			(*it)->in.sort();
			(*it)->in.unique();
			

			//DODATNI ZADATAK 
			//ovaj if i cela do while petlja 
			//proveravamo suprotan slucaj od onog u pseudokodu jer je u pseudokodu repeat until 
			// a ovde radimo sa while (repeat until nema u c++)
			// proveravamo da li se bar jedan od parova in_ in  i out_ out razlikiju 
			// ako se razlikiju state postaje true i ponovo ponavljamo petlju do while
			// ukoliko su jednaki state ostaje false i petlja se zavrsava :D 
		//	if((in_!=(*it)->in) || (out_!=(*it)->out))
		//	{
		//		state = true;
		//	}
			
				
		}

		
			

	//}while (state);
	
}

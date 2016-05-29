#include "ResourceAllocation.h"
#include "InterferenceGraph.h"
#include "Constants.h"
#include "Instruction.h"


bool doResourceAllocation(stack<Variable*>* simplificationStack, InterferenceGraph* ig)
{
	list<Regs> listaBoja;
		//pravimo listu boja
	listaBoja.push_back(reg3);		//smestamo boje (registre) u listu
	listaBoja.push_back(reg2);
	listaBoja.push_back(reg1);
	listaBoja.push_back(reg0);

	Variables listaObojenih;		// lista obojenih  

	simplificationStack->top()->assigment = listaBoja.back();	//prvom elementu steka dodeljujemo proizvoljnu boju
	listaObojenih.push_back(simplificationStack->top());		//smestamo element u listu obojenih
	simplificationStack->pop();					//skidamo smesteni element sa steka

	while(!simplificationStack->empty())				//dok stek nije prazan
	{
		list<Regs> boje = listaBoja;				//pomocna lista boja  
		int row = 0;		
		Variable* temp = simplificationStack->top();		//da koristimo temp umesto simplificationStack->top()
									//temp je trenutni element, onaj koji se boji
		row=temp->pos;						// row postaje pozicija tempa
		

		//prolazimo kroz listu obojenih, ako nadjemo da je u matrici smetnji trenutni element u smetnji sa nekim od objenih 
		//moramo izbaciti boju tog obojenog 
		for(auto it_obojenih = listaObojenih.begin(); it_obojenih!= listaObojenih.end(); it_obojenih++)
		{
			if((*ig).values[row][(*it_obojenih)->pos] == __INTERFERENCE__)
			{
				boje.remove((*it_obojenih)->assigment);
			}
		}

		//kada smo izbacili sve boje kojima su obojeni elementi koji su u smetnji proveravamo da li je lista pomocnih boja prazna
		if(!boje.empty())		//ako nije
		{		
			temp->assigment = boje.back();		//od preostalih boja uzimamo jednu (obicno onu sa vrha) u dodeljujemo je elemnetu
		}
		else 
			return false;				//ako je lista pomocnih boja prazna znaci da se graf ne moze 
								//obojiti pomocu moguceg broja boja
		
		simplificationStack->pop();			//skidamo element sa steka da bi presli na sledeci
		listaObojenih.push_back(temp);			//smestamo obojeni element u listu obojenih

	}

	return true;		//vraca true 
}


Instructions* removeMove(Instructions* instrs)
{
	for(auto it = instrs->begin(); it != instrs->end(); it++)
	{
		if((*it)->type == T_MOVE )
		{
			if(((*it)->src1->assigment == (*it)->dst->assigment))
			{
				it = instrs->erase(it);
				it--; //erase vraca iterator na element posle obrisanog u listi
			}					//zato ne treba it++
				
		}
	}
	return instrs;
}
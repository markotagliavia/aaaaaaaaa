#include "passI.h"

#include "assemblerLib.h"

using namespace std;


bool passI()
{
	DIRECTIVE_T directive;
	PARAM_STACK params;
	SECTION_T section = S_TEXT;
	SOURCE_LIST* sourceList;
	SOURCE_LIST::iterator sourceListIter;
	long memLocCounter;
	string sourceLine;
	long lineNum;
	string symbol;
	string msg;
	string executable;

	long numOfParams; //NOVO ----------------------------------------------------------------------------
	/******************************************************************************************
	* Add your variables here
	*******************************************************************************************/

	sourceList = getSourceList();   // get source code loaded in STL list, where each element is SOURCE_LINE, 
									// structure consisted of line number and source line string

	memLocCounter = 0; // memory locations counter start at 0

	for (sourceListIter = sourceList->begin(); sourceListIter != sourceList->end(); sourceListIter++)
	{
		// for each line from source code
		sourceLine = sourceListIter->sourceLine;	// get source line
		lineNum = sourceListIter->lineNumber;		// get line number

		getParams(params, sourceLine, lineNum);	// get params from current source line e.g.:
												// from line "add $t3, $t2, $0", extracted parametrs should be "$t3", "$t2", and "$0"
												// from line "value: .word 9", extracted parametrs should be "9"

		directive = getDirective(sourceLine, lineNum); // extract directive from source line (string following ".")

		if(getSymbol(symbol, sourceLine)) //CEO IF NOV ----------------------------------------------------------------------------kk
		{
			//proverava da li simbol postoji, ako postoji prijavi gresku, ako ne postoji doda ga u tabalu simbola
			if(symbolExists(symbol))
			{
				addError(lineNum, sourceLine, "Simbol vec postoji u tabeli simbola");
			}
			else
			{
				pushSymbol(symbol, memLocCounter, section);
			}
		}

		//u slucaju izvrsne instrukcije brojac se uvecava za 4  | ova provera moze da ide  i u case D_NONE: ali nije bitno gde je moze i ovde
		if(getExecutable(executable, sourceLine, lineNum, false)) //CEO IF NOV ----------------------------------------------------------------------------
			memLocCounter+=4;

		switch (directive)
		{
		case D_NONE:
			// directive not found in current line
			break;

		case D_ORG:
			// if the directive is .org
			if (checkEnoughParams(lineNum, sourceLine, params, 1)) // check if the line with directive is correct
			{
				changeSectionAndLocation(params.top(), section, memLocCounter); // current section is updated due to .org directive (data/text section)
			}
			break;

		case D_WORD:
			// .word directive => extract the value as first parameter, convert it to number and add to literal table
						
			
			//NOVO
			//UKOLIKO NEMA NIZOVA KORISTI OVAJ KOD 
			//pushLiteral(memLocCounter, constConv(params.top()));  //dodaje literal u tabelu literala 
			//memLocCounter+=4;													    //parametri: ima adresu memLocCounter i vrednost literala params.top()
			
			//******************************

			//UKOLIKO IMA NIZOVA ONDA SE KORISTI OVAJ KOD (ako u asemblerskom kodu u fact.s linije value = .word 9 10 11 
								  //jer je to niz ili ako bude trazio da se realizuje provera za niz)
								  //constConv od stringa pravi broj (jer je simbol string)
							          //ujedno je ovo za niz odgovor na pitanje ispod : Extra question ****
			numOfParams = params.size();
			for(long i=0; i<numOfParams; i++)
			{
				pushLiteral(memLocCounter, constConv(params.top()));
				memLocCounter+=4;
				params.pop();
			}
			//KRAJ NOVOG



			// Extra question: what if .word directive can have more than one parameter?
			break;

		case D_SPACE:
			// .space directive => just check if it is followed by a number (single parameter)
			checkEnoughParams(lineNum, sourceLine, params, 1);

						//NOVO
			//proverava za svaki .space koliko zauzima bajta, ukoliko zauzima neki broj bajta koji nije deljiv sa 4
			//onda ce zauzeti sledeci veci broj koji je deljiv sa 4. pr: .section 7 zauzima 7 bajta. sledeci veci broj koji je deljiv sa 4 je 8 
			//sto znaci da je .section 7 zauzeti 8 bajta i za toliko treba povecati memLocCounter 
			// tj memLocCounter += constConv(params.top()) + 1   ***poslednji if***  

			
			if(constConv(params.top())%4==0)
				memLocCounter += constConv(params.top());
			else if(constConv(params.top())%4==1)
				memLocCounter += constConv(params.top()) + 3;
			else if(constConv(params.top())%4==2)
				memLocCounter += constConv(params.top()) + 2;
			else if(constConv(params.top())%4==3)
				memLocCounter += constConv(params.top()) + 1;

			break;

		case D_GLOBL:
			// .globl directive => get the symbol as first parameter, check if exists in global symbols table and add it to the table
			if (checkEnoughParams(lineNum, sourceLine, params, 1))
			{
				if (globalSymbolExists(params.top()))
				{
					msg = "Global symbol already exist: " + params.top();
					addError(lineNum, sourceLine, msg);
				}
				else
				{
					pushGlobalSymbol(params.top(), memLocCounter);
				}
			}
			break;

		case D_EXTERN:
			// .extern directive => get the symbol as first parameter, check if exists in extern symbols table and add it to the table
			if (checkEnoughParams(lineNum, sourceLine, params, 1))
			{
				if (externSymbolExists(params.top()))
				{
					msg = "Extern symbol already exist: " + params.top();
					addError(lineNum, sourceLine, msg);
				}
				else
				{
					pushExternSymbol(params.top(), memLocCounter);
				}
			}
			break;

		default:
			// error!
			break;
		}
	}

	return !errorsFound();
}

#include "GameOfLife.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk.h>
#include <cilk_stub.h>
using namespace std;

GameOfLife::GameOfLife(unsigned int height, unsigned int width, InitModel model)
{
	m_height = height;
	m_width = width;

	m_buffer = new unsigned char [m_width * m_height];
	m_nextIter = new unsigned char [m_width * m_height];

	// fill buffer
	switch (model)
	{
	case PULSAR:
		{
			if (width > 15 && height > 15)
			{
				memset(m_buffer, 0x0, m_width * m_height * sizeof(unsigned char));
				m_buffer[2 * m_width + 4] = 1;
				m_buffer[2 * m_width + 5] = 1;
				m_buffer[2 * m_width + 6] = 1;

				m_buffer[2 * m_width + 10] = 1;
				m_buffer[2 * m_width + 11] = 1;
				m_buffer[2 * m_width + 12] = 1;

				m_buffer[4 * m_width + 2] = 1;
				m_buffer[5 * m_width + 2] = 1;
				m_buffer[6 * m_width + 2] = 1;

				m_buffer[4 * m_width + 7] = 1;
				m_buffer[5 * m_width + 7] = 1;
				m_buffer[6 * m_width + 7] = 1;

				m_buffer[4 * m_width + 9] = 1;
				m_buffer[5 * m_width + 9] = 1;
				m_buffer[6 * m_width + 9] = 1;

				m_buffer[4 * m_width + 14] = 1;
				m_buffer[5 * m_width + 14] = 1;
				m_buffer[6 * m_width + 14] = 1;

				m_buffer[7 * m_width + 4] = 1;
				m_buffer[7 * m_width + 5] = 1;
				m_buffer[7 * m_width + 6] = 1;

				m_buffer[7 * m_width + 10] = 1;
				m_buffer[7 * m_width + 11] = 1;
				m_buffer[7 * m_width + 12] = 1;

				m_buffer[9 * m_width + 4] = 1;
				m_buffer[9 * m_width + 5] = 1;
				m_buffer[9 * m_width + 6] = 1;

				m_buffer[9 * m_width + 10] = 1;
				m_buffer[9 * m_width + 11] = 1;
				m_buffer[9 * m_width + 12] = 1;

				m_buffer[10 * m_width + 2] = 1;
				m_buffer[11 * m_width + 2] = 1;
				m_buffer[12 * m_width + 2] = 1;

				m_buffer[10 * m_width + 7] = 1;
				m_buffer[11 * m_width + 7] = 1;
				m_buffer[12 * m_width + 7] = 1;

				m_buffer[10 * m_width + 9] = 1;
				m_buffer[11 * m_width + 9] = 1;
				m_buffer[12 * m_width + 9] = 1;

				m_buffer[10 * m_width + 14] = 1;
				m_buffer[11 * m_width + 14] = 1;
				m_buffer[12 * m_width + 14] = 1;

				m_buffer[14 * m_width + 4] = 1;
				m_buffer[14 * m_width + 5] = 1;
				m_buffer[14 * m_width + 6] = 1;

				m_buffer[14 * m_width + 10] = 1;
				m_buffer[14 * m_width + 11] = 1;
				m_buffer[14 * m_width + 12] = 1;

				break;
			}
			else
			{
				cout << "Board too small, using RANDOM." << endl;
			}
		}

	case RANDOM:
		{
			srand(time(NULL));
			for (unsigned int i = 0; i < m_height; i++)
			{
				for (unsigned int j = 0; j < m_width; j++)
				{
					m_buffer[i * m_width + j] = rand() % 2;
				}
			}
			break;
		}

	default:
		break;
	}
}


void GameOfLife::printIteration()
{
	for (int row = 0; row <= m_height; row++)
	{
		for (int col = 0; col <= m_width; col++)
		{
			char c;

			if (m_buffer[row * m_width + col] == 1)
				c = 0xb2;
			else
				c = 0x20;

			cout << c << " ";
		}
		cout << "\n";
	}
}


int GameOfLife::getNeighbourSum(unsigned int row, unsigned int col)
{
	int sum = 0;

	
	if(col == 0 && row == 0) 
	{
		if(m_buffer[col+1] == 1) sum++;
		if(m_buffer[m_width] == 1) sum++;
		if(m_buffer[m_width + 1] == 1) sum++;
	}
	else if(col == 0 && row != 0 && row != m_height-1)
	{
		if(m_buffer[m_width*(row-1)] == 1) sum++;
		if(m_buffer[m_width*(row-1) + 1] == 1) sum++;
		if(m_buffer[m_width*row + 1] == 1) sum++;
		if(m_buffer[m_width*(row+1)] == 1) sum++;
		if(m_buffer[m_width*(row+1) + 1] == 1) sum++;
	}
	else if(col == 0 && row == m_height-1)
	{
		if(m_buffer[m_width*(row-1)] == 1) sum++;
		if(m_buffer[m_width*(row-1) + 1] == 1) sum++;
		if(m_buffer[m_width*row + 1] == 1) sum++;
	}
	else if(col != 0 && row == m_height-1 && col != m_width-1)
	{
		if(m_buffer[m_width*(row-1)+(col-1)] == 1) sum++;
		if(m_buffer[m_width*(row-1)+col] == 1) sum++;
		if(m_buffer[m_width*(row-1)+col+1] == 1) sum++;
		if(m_buffer[m_width*row + (col-1)] == 1) sum++;
		if(m_buffer[m_width*row + (col+1)] == 1) sum++;
	}
	else if(row == m_height-1 && col == m_width-1)
	{
		if(m_buffer[m_width*(row-1)+(col-1)] == 1) sum++;
		if(m_buffer[m_width*(row-1)+col] == 1) sum++;
		if(m_buffer[m_width*row + (col-1)] == 1) sum++;
	}
	else if(row != m_height-1 && row != 0 && col == m_width-1 )
	{
		if(m_buffer[m_width*(row-1) + col] == 1) sum++;
		if(m_buffer[m_width*(row-1) + col - 1] == 1) sum++;
		if(m_buffer[m_width*row + m_width - 2] == 1) sum++;
		if(m_buffer[m_width*(row+1) + m_width - 2] == 1) sum++;
		if(m_buffer[m_width*(row+1) + m_width -1] == 1) sum++;
	}
	else if(row == 0 && col == m_width-1)
	{
		if(m_buffer[col - 1] == 1) sum++;
		if(m_buffer[col + m_width] == 1) sum++;
		if(m_buffer[col + m_width + 1] == 1) sum++;
	}
	else if(row == 0 && col != 0 && col != m_width-1)
	{
		if(m_buffer[col - 1] == 1) sum++;
		if(m_buffer[col + 1] == 1) sum++;
		if(m_buffer[m_width + col - 1] == 1) sum++;
		if(m_buffer[m_width + col] == 1) sum++;
		if(m_buffer[m_width + col + 1] == 1) sum++;
	}
	else
	{
		if(m_buffer[(row-1)*m_width+col-1] == 1) sum++;
		if(m_buffer[(row-1)*m_width+col] == 1) sum++;
		if(m_buffer[(row-1)*m_width+col+1] == 1) sum++;
		if(m_buffer[(row+1)*m_width+col-1] == 1) sum++;
		if(m_buffer[(row+1)*m_width+col] == 1) sum++;
		if(m_buffer[(row+1)*m_width+col+1] == 1) sum++;
		if(m_buffer[row*m_width+col-1] == 1) sum++;
		if(m_buffer[row*m_width+col+1] == 1) sum++;
	}
	


	return sum;
}


void GameOfLife::nextIterSerial()
{
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			// TODO: IMPLEMENT getNeighbourSum
			int neighbours = getNeighbourSum(row, col);
			//cout<<neighbours<<" ";
			if (m_buffer[row * m_width + col] == 1)
			{
				if ((neighbours == 2) || (neighbours == 3))
				{
					m_nextIter[row * m_width + col] = 1;
				}
				else
				{
					m_nextIter[row * m_width + col] = 0;
				}
			}
			else
			{
				if (neighbours == 3)
				{
					m_nextIter[row * m_width + col] = 1;
				}
				else
				{
					m_nextIter[row * m_width + col] = 0;
				}
			}
		}
	}

	// switch buffers
	unsigned char* temp = m_buffer;
	m_buffer = m_nextIter;
	m_nextIter = temp;
}


void GameOfLife::nextIter_Serial(unsigned int pocetak, unsigned int kraj)
{

	for (int row = pocetak; row < kraj; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			// TODO: IMPLEMENT getNeighbourSum
			int neighbours = getNeighbourSum(row, col);
			
			if (m_buffer[row * m_width + col] == 1)
			{
				if ((neighbours == 2) || (neighbours == 3))
				{
					m_nextIter[row * m_width + col] = 1;
				}
				else
				{
					m_nextIter[row * m_width + col] = 0;
				}
			}
			else
			{
				if (neighbours == 3)
				{
					m_nextIter[row * m_width + col] = 1;
				}
				else
				{
					m_nextIter[row * m_width + col] = 0;
				}
			}
		}
	}

	
}



void GameOfLife::Parallel(unsigned int pocetak, unsigned int kraj)
{
	if((kraj-pocetak) <= 4)
	{
		
		nextIter_Serial(pocetak,kraj);
	}
	else
	{
		 
		int korak = (kraj - pocetak)/2;
		
		cilk_spawn Parallel(pocetak, pocetak + korak);
		Parallel(pocetak + korak, kraj);
		cilk_sync;

		
		
	}
}

void GameOfLife::nextIterParallel()
{
	// TODO: PLACE CODE HERE
	Parallel(0,m_height);
	// switch buffers
		unsigned char* temp = m_buffer;
		m_buffer = m_nextIter;
		m_nextIter = temp;
	
}


GameOfLife::~GameOfLife()
{
	delete [] m_buffer;
	delete [] m_nextIter;
}

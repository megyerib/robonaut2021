#include "LineFilter.h"
#include <stdbool.h>
#include <stdlib.h>

#define TOLERANCE  1000

static void bubbleSort(int16_t arr[], int n);

LineInput FilterLines(LineInput meas[5], size_t recentIndex)
{
    bool votma[5][3] = {false};

    int16_t results[5];
    size_t resultNum = 0;

    // Végigmegyünk az utolsó 3 mérésen (i_meas)
    for (uint8_t im = recentIndex; im != (recentIndex+5-3)%5; im = (im+5-1)%5)
    {
    	// Végigmegyünk a mérés elemein... (i_line)
        for (uint8_t il = 0; il < meas[im].cnt; il++)
    	{
    		if (votma[im][il] == false) // ...de csak ha még nem vót
    		{
				int16_t end = meas[im].lines[il];
    			int16_t successor = end; // Ehhez keresünk predecessort
				int findings = 1;
				votma[im][il] = true; // Jelezzük, hogy vótmá.

				// Végigmegyünk az előző méréseken (i_prev)
                for (uint8_t ipm = (im+5-1)%5; ipm != recentIndex; ipm = (ipm+5-1)%5)
				{
					// Végigmegyünk az előző mérés elemein... (i_prevline)
                    for (uint8_t ipl = 0; ipl < meas[ipm].cnt; ipl++)
					{
						if (votma[ipm][ipl] == false) // ...de csak ha még nem vótak
						{
							int16_t predecessor = meas[ipm].lines[ipl];

							if (abs(successor - predecessor) <= TOLERANCE) // Ha elég közel van...
							{
								successor = predecessor; // Akkor ennek az elődeit keressük tovább
								votma[ipm][ipl] = true; // Jelezzük, hogy vótmá
								findings++; // Azt is, hogy még egy elemmel bővült a vonal
								break; // végül pedig kibreakelünk innen.
							}
						}
					}
				}

				// Megnézzük, hogy kiad-e egy vonalat (van legalább 3 eleme)
				if (findings >= 3)
				{
					results[resultNum] = end;
					resultNum++;
				}
    		}
    	}
    }

    // Rendezzük az eredményhalmazt
    bubbleSort(results, resultNum);

    // Előállítjuk a végeredményt
    LineInput ret = {0};

    if (resultNum <= 3)
    {
    	ret.cnt = resultNum;
        for (uint8_t i = 0; i < resultNum; i++)
    	{
    		ret.lines[i] = results[i];
    	}
    }
    else if (resultNum == 4)
    {
    	ret.cnt = 3;
		for (uint8_t i = 0; i < 3; i++) // Legkisebb 3
		{
			ret.lines[i] = results[i];
		}
    }
    else if (resultNum == 5)
	{
		ret.cnt = 3;
		for (uint8_t i = 0; i < 3; i++) // Középső 3
		{
			ret.lines[i] = results[i+1];
		}
	}

    return ret;
}

static void bubbleSort(int16_t arr[], int n)
{
    for (int i = 0; i < n-1; i++)
    {
		// Last i elements are already in place
		for (int j = 0; j < n-i-1; j++)
		{
			if (arr[j] > arr[j+1])
			{
				int16_t tmp = arr[j];
				arr[j]      = arr[j+1];
				arr[j+1]    = tmp;
			}
		}
    }
}

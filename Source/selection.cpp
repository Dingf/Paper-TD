#include "type.h"
#include "tower.h"
#include "selection.h"

Selection::Selection()
{
	Clear();
}

Selection::~Selection()
{
	Clear();
}

uint8 Selection::GetSize() const
{
	uint8 counter = 0;
	for (uint8 i = 0; i < SELECTION_SIZE; i++)
	{
		if (selection[i] != NULL)
		{
			counter++;
		}
	}
	return counter;
}

int32 Selection::GetIndex(Tower * tower) const
{
	for (uint8 i = 0; i < SELECTION_SIZE; i++)
	{
		if (selection[i] == tower)
		{
			return i;
		}
	}
	return NOT_FOUND;
}

bool Selection::Add(Tower * tower)
{
	if (GetIndex(tower) == NOT_FOUND)
	{
		for (uint8 i = 0; i < SELECTION_SIZE; i++)
		{
			if (selection[i] == NULL)
			{
				selection[i] = tower;
				return true;
			}
		}
	}
	return false;
}

bool Selection::Remove(Tower * tower)
{
	uint32 index = GetIndex(tower);

	if (index == NOT_FOUND)
	{
		return false;
	}
	else
	{
		return Remove(index);
	}
}

bool Selection::Remove(uint32 index)
{
	if (index >= SELECTION_SIZE)
	{
		return false;
	}
	else
	{
		for (uint32 i = index; i < SELECTION_SIZE - 1; i++)
		{
			selection[i] = selection[i+1];
		}
		selection[SELECTION_SIZE-1] = NULL;
		return true;
	}
}

void Selection::Clear()
{
	for (uint8 i = 0; i < SELECTION_SIZE; i++)
	{
		selection[i] = NULL;
	}
}

void Selection::Copy(const Selection& selection)
{
	Clear();
	for (uint8 i = 0; i < SELECTION_SIZE; i++)
	{
		Add(selection.GetTower(i));
	}
}

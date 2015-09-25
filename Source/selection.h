#ifndef INC_SELECTION_H
#define INC_SELECTION_H

#include "type.h"
#include "tower.h"

#define SELECTION_SIZE 12

struct Selection
{
	public:
		Selection();
		~Selection();

		uint8 GetSize() const;

		int32 GetIndex(Tower * tower) const;
		Tower * GetTower(uint32 index) const { return selection[index]; }

		bool Add(Tower * tower);
		bool Remove(Tower * tower);
		bool Remove(uint32 index);

		void Clear();

		void Copy(const Selection& selection);

	private:
		Tower * selection[SELECTION_SIZE];
};

#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "ContiguousBlock.h"

ContiguousBlock::ContiguousBlock(
	int32 block_type, int32 row_pos, int32 col_pos, int32 length, BlockDirection block_direction
)
:block_type(block_type), row_position(row_pos), col_position(col_pos), 
block_direction(block_direction), length(length),
cells_x(new int32[length]), cells_y(new int32[length])
{
	int r = 1, c = 1;
	if (block_direction == BlockDirection::TRtoBL)
		c = -1;

	for (int32 i = 0; i < length; i++) 
	{
		cells_y[i] = row_pos + (c * i);
		cells_x[i] = col_pos + (r * i);
	}
}

bool ContiguousBlock::begin(int32& r, int32& c) const
{
	idx = 0;
	if (idx < length)
	{
		r = cells_y[idx];
		c = cells_x[idx];
		return true;
	}
	return false;
}

bool ContiguousBlock::next(int32& r, int32& c) const
{
	if (idx + 1 < length)
	{
		++idx;
		r = cells_y[idx];
		c = cells_x[idx];
		return true;
	}
	return false;
}

bool ContiguousBlock::operator<(const ContiguousBlock& comp) const {
	if (this->row_position == comp.row_position)
	{
		if (this->col_position == comp.col_position)
		{
			return static_cast<int>(this->block_direction) < static_cast<int>(this->block_direction);
		}
		else
			return this->col_position < comp.col_position;
	}
	return this->row_position < comp.row_position;
}

bool ContiguousBlock::operator>(const ContiguousBlock& comp) const {
	if (this->row_position == comp.row_position)
	{
		if (this->col_position == comp.col_position)
		{
			return static_cast<int>(this->block_direction) > static_cast<int>(this->block_direction);
		}
		else
			return this->col_position > comp.col_position;
	}
	return this->row_position > comp.row_position;
}

ContiguousBlock::~ContiguousBlock()
{
	delete[] cells_x;
	delete[] cells_y;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <utility>
/**
 * 
 */

enum class BlockDirection {
	TtoB=0, LtoR, TLtoBR, TRtoBL
};

class SHINEFORYOURLIGHT_API ContiguousBlock
{
public:
	int32 block_type;
	int32 row_position;
	int32 col_position;
	BlockDirection block_direction;
	int32 length;

	ContiguousBlock(
		int32 block_type, int32 row_pos, int32 col_pos,
		int32 length, BlockDirection block_direction = BlockDirection::TtoB
	);
	~ContiguousBlock();

	bool begin(int32& r, int32& c) const;
	bool next(int32& r, int32& c) const;
	bool operator<(const ContiguousBlock& comp) const;
	bool operator>(const ContiguousBlock& comp) const;
private:
	mutable int32 idx;
	int32* cells_x;
	int32* cells_y;
};

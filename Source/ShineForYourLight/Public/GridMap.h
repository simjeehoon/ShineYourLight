// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContiguousBlock.h"


#define TEMP_BIT (1<<31)
/**
 * Puzzle의 맵을 체크하는 클래스
 */
class SHINEFORYOURLIGHT_API GridMap
{
private:
	TArray<TArray<int32> > grid_map;
	int32 width, height;
public:
	GridMap(int32 width, int32 height);
	~GridMap();

	// 너비 반환
	int32 GetWidth() { return width; }

	// 높이 반환
	int32 GetHeight() { return height; }
	
	// 셀 정보 반환
	int32 GetCell(int32 row_num, int32 col_num);

	// 셀 채우기
	void SetCell(int32 row_num, int32 col_num, int32 value, bool isTemporary=false);

	// 임시 셀 지우기
	void RemoveTemporaryCell();

	// 임시 셀 확정시키기
	void SetTemporaryToConfirmed();

	// 연속된 블록 검사
	TArray<ContiguousBlock> CheckContiguous();

	void PushDown(const TArray<ContiguousBlock>& blocks);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContiguousBlock.h"


#define TEMP_BIT (1<<31)
/**
 * Puzzle�� ���� üũ�ϴ� Ŭ����
 */
class SHINEFORYOURLIGHT_API GridMap
{
private:
	TArray<TArray<int32> > grid_map;
	int32 width, height;
public:
	GridMap(int32 width, int32 height);
	~GridMap();

	// �ʺ� ��ȯ
	int32 GetWidth() { return width; }

	// ���� ��ȯ
	int32 GetHeight() { return height; }
	
	// �� ���� ��ȯ
	int32 GetCell(int32 row_num, int32 col_num);

	// �� ä���
	void SetCell(int32 row_num, int32 col_num, int32 value, bool isTemporary=false);

	// �ӽ� �� �����
	void RemoveTemporaryCell();

	// �ӽ� �� Ȯ����Ű��
	void SetTemporaryToConfirmed();

	// ���ӵ� ��� �˻�
	TArray<ContiguousBlock> CheckContiguous();

	void PushDown(const TArray<ContiguousBlock>& blocks);
};

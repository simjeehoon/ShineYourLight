// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMap.h"

GridMap::GridMap(int width, int height):width(width), height(height)
{
	grid_map.Init(TArray<int32>(0, width), height);
}

GridMap::~GridMap()
{
}

int32 GridMap::GetCell(int32 row_num, int32 col_num)
{
	//세로 범위를 벗어난 경우 0 반환
	if (row_num < 0 || row_num >= GetHeight())
	{
		return 0;
	}

	//가로 범위를 벗어난 경우 0 반환
	if (col_num < 0 || col_num >= GetWidth())
	{
		return 0;
	}

	return grid_map[row_num][col_num] & ~TEMP_BIT;
}

void GridMap::SetCell(int32 row_num, int32 col_num, int32 value, bool isTemporary)
{
	//세로 범위를 벗어난 경우 무시
	if (row_num < 0 || row_num >= GetHeight())
	{
		return;
	}

	//가로 범위를 벗어난 경우 무시
	if (col_num < 0 || col_num >= GetWidth())
	{
		return;
	}

	if (isTemporary)
		value |= TEMP_BIT;
	grid_map[row_num][col_num] = value;
}

void GridMap::RemoveTemporaryCell()
{
	for (int32 r = GetHeight() - 1; r >= 0; --r)
	{
		for (int32 c = GetWidth() - 1; c >= 0; --c)
		{
			if (grid_map[r][c] & TEMP_BIT)
				grid_map[r][c] = 0;
		}
	}
}

void GridMap::SetTemporaryToConfirmed()
{
	for (int32 r = GetHeight() - 1; r >= 0; --r)
	{
		for (int32 c = GetWidth() - 1; c >= 0; --c)
		{
			grid_map[r][c] &= ~TEMP_BIT;
		}
	}
}

TArray<ContiguousBlock> GridMap::CheckContiguous()
{
	TArray<ContiguousBlock> result;
	int32 prev = 0;
	int32 count = 0;

	int32 r;
	int32 c;
	int32 i;

	// 연속된 가로 탐색
	for (r = 0; r < GetHeight(); ++r)
	{
		for (c = 0; c < GetWidth(); ++c)
		{
			if (grid_map[r][c])
			{
				if (grid_map[r][c] == prev) // 연속된 가로 발견
				{
					++count;
				}
				else // 가로 불연속
				{
					if (count >= 2) // 가로 길이 충족
					{
						++count;
						result.Emplace(prev, r, c - count, count, BlockDirection::LtoR);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // 현재 셀로 재설정
		}
		if (count >= 2)
		{
			++count;
			result.Emplace(prev, r, c - count, count, BlockDirection::LtoR);
		}
		prev = 0; // 이전셀 재설정
		count = 0;
	}

	// 연속된 세로 탐색
	for (c = 0; c < GetWidth(); ++c)
	{
		for (r = 0; r < GetHeight(); ++r)
		{
			if (grid_map[r][c])
			{
				if (grid_map[r][c] == prev) // 연속된 세로 발견
				{
					++count;
				}
				else // 세로 불연속
				{
					if (count >= 2) // 세로 길이 충족
					{
						++count;
						result.Emplace(prev, r - count, c, count, BlockDirection::TtoB);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // 현재 셀로 재설정
		}
		if (count >= 2)
		{
			++count;
			result.Emplace(prev, r - count, c, count, BlockDirection::TtoB);
		}
		prev = 0; // 이전셀 재설정
		count = 0;
	}

	
	r = 0;
	c = GetWidth() - 1;
	// 연속된 TLtoBR 탐색
	while (1)
	{
		// 시작 위치 지정
		if (c >= 0) // 왼쪽으로 이동
		{
			--c;
		}
		else if (r < GetHeight()) // 아래로 이동
		{
			c = 0;
			++r;
		}
		else
			break;

		// 대각선으로 탐색
		i = 0;
		while (c + i < GetWidth() && r + i < GetHeight())
		{
			if (grid_map[r + i][c + i])
			{
				if (grid_map[r + i][c + i] == prev) // 연속된 대각 블록 발견
				{
					++count;
				}
				else // 불연속
				{
					if (count >= 2) // 연속 길이 충족
					{
						++count;
						result.Emplace(prev, r + i - count, c + i - count, count, BlockDirection::TLtoBR);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // 현재 셀로 재설정

			++i;
		}
	
		//대각선 탐색 종료
		if (count >= 2) // 연속 길이 충족
		{
			++count;
			result.Emplace(prev, r + i - count, c + i - count, count, BlockDirection::TLtoBR);
		}
		prev = 0; // 이전셀 재설정
		count = 0;
	}

	r = 0;
	c = 0;
	// 연속된 TRtoBL 탐색
	while (1)
	{
		// 시작 위치 지정
		if (c < GetWidth()) // 오른쪽으로 이동
		{
			++c;
		}
		else if (r < GetHeight()) // 아래로 이동
		{
			c = GetWidth() - 1;
			++r;
		}
		else
			break;

		// 대각선으로 탐색
		i = 0;
		while (c - i >= 0 && r + i < GetHeight())
		{
			if (grid_map[r + i][c - i])
			{
				if (grid_map[r + i][c - i] == prev) // 연속된 대각 블록 발견
				{
					++count;
				}
				else // 불연속
				{
					if (count >= 2) // 연속 길이 충족
					{
						++count;
						result.Emplace(prev, r + i - count, c - i + count, 
							count, BlockDirection::TRtoBL);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // 현재 셀로 재설정

			++i;
		}

		//대각선 탐색 종료
		if (count >= 2) // 연속 길이 충족
		{
			++count;
			result.Emplace(prev, r + i - count, c - i + count, count, BlockDirection::TRtoBL);
		}
		prev = 0; // 이전셀 재설정
		count = 0;
	}

	result.Sort();
	return result;
}

void GridMap::PushDown(const TArray<ContiguousBlock>& blocks)
{
	int32 i;
	int32 cell;
	const ContiguousBlock* prev = nullptr;
	for (const ContiguousBlock& block : blocks)
	{
		i = 0;
		if (block.block_direction == BlockDirection::TtoB) // 세로 모양
		{
			//블록 위부분 데이터 얻기
			while (1)
			{
				cell = GetCell(block.row_position - 1 - i, block.col_position);
				if (!cell)
					break;
				//블록부분부터 데이터 덮어쓰기
				SetCell(block.row_position + block.length - 1 - i, block.col_position,
					cell
				);
				++i;
			}
			while (GetCell(block.row_position + block.length - 1 - i, block.col_position))
			{
				//남은 위치 0으로 채우기
				SetCell(block.row_position + block.length - 1 - i, block.col_position,
					0
				);
				++i;
			}
		}
		else // 세로가 아닌 모양
		{
			int32 c = 0;
			if (prev != nullptr &&
				prev->row_position == block.row_position &&
				prev->col_position == block.col_position) // 이전에 시작 위치가 중첩될경우
			{
				c = 1; // 첫 셀은 제외한다.
			}
			if (block.block_direction == BlockDirection::LtoR)
			{
				//가로길이만큼 반복
				for (; c < block.length; c++)
				{
					//블록 위부분 데이터 얻기
					while (1)
					{
						cell = GetCell(block.row_position - 1 - i, block.col_position + c);
						if (!cell)
							break;
						//블록부분부터 데이터 덮어쓰기
						SetCell(block.row_position - i, block.col_position + c,
							cell
						);
						++i;
					}
					while (GetCell(block.row_position - i, block.col_position + c))
					{
						//남은 위치 0으로 채우기
						SetCell(block.row_position - i, block.col_position + c,
							0
						);
						++i;
					}
				}
			}
			else if (block.block_direction == BlockDirection::TLtoBR)
			{
				//길이만큼 반복
				for (; c < block.length; c++)
				{
					//블록 위부분 데이터 얻기
					while (1)
					{
						cell = GetCell(block.row_position - 1 - i + c, block.col_position + c);
						if (!cell)
							break;
						//블록부분부터 데이터 덮어쓰기
						SetCell(block.row_position - i + c, block.col_position + c,
							cell
						);
						++i;
					}
					while (GetCell(block.row_position - i + c, block.col_position + c))
					{
						//남은 위치 0으로 채우기
						SetCell(block.row_position - i + c, block.col_position + c,
							0
						);
						++i;
					}

				}
			}
			else if (block.block_direction == BlockDirection::TRtoBL)
			{
				//길이만큼 반복 왼쪽 방향으로 진행하므로 col_pos는 c만큼 감소한다.
				for (; c < block.length; c++)
				{
					//블록 위부분 데이터 얻기
					while (1)
					{
						cell = GetCell(block.row_position - 1 - i + c, block.col_position - c);
						if (!cell)
							break;
						//블록부분부터 데이터 덮어쓰기
						SetCell(block.row_position - i + c, block.col_position - c,
							cell
						);
						++i;
					}
					while (GetCell(block.row_position - i + c, block.col_position - c))
					{
						//남은 위치 0으로 채우기
						SetCell(block.row_position - i + c, block.col_position - c,
							0
						);
						++i;
					}
				}
			}
		}
		prev = &block;
	}
}

